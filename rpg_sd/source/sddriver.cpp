/*
    sddriver.cpp
    Copyright (C) 2007 Acekard, www.acekard.com
    Copyright (C) 2007-2009 somebody
    Copyright (C) 2009 yellow wood goblin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <nds.h>
#include "sddriver.h"
#include "rpgprotocol.h"
#include "dbgtool.h"


static u16 SDCardAddr = 0;
static bool _isSD20 = false;
static bool _isSDHC = false;
static u32 _size=0;

static void composeResponse1( void * data, void * response ) {
  const u8 * pdata = (u8 *)data;
  u8 * pResponse = (u8 *)response;
  *(u32 *)pResponse = 0;

  u32 bitCount = 39;
  for( int i = 0; i < 4; ++i ) {
    for( int j = 0; j < 8; ++j ) {
      u8 bit = (pdata[bitCount--] & 0x80) ? 1 : 0;
      //dbg_printf( "%s", bit ? "1" : "0" );
      pResponse[i] |= bit << j;
    }
  }
}

static void composeResponse2( void * data, void * response ) {
  const u8 * pdata = (u8 *)data;
  u8 * pResponse = (u8 *)response;
  *(u64 *)response = *(((u64 *)response) + 1) = 0;

  u32 bitCount = 135;
  for( int i = 0; i < 16; ++i ) {
    for( int j = 0; j < 8; ++j ) {
      u8 bit = (pdata[bitCount--] & 0x80) ? 1 : 0;
      pResponse[i] |= bit << j;
    }
  }
}

static void sendSDCommandR0( u32 * cmd )
{
  ioRpgSendCommand( cmd, 0, 80, NULL );
}

static void sendSDCommandR1( u32 * cmd, u8 * response )
{
  ALIGN(4) u8 tempResponse[64];tempResponse[0] = 0;
  ioRpgSendCommand( cmd, 48, 40, tempResponse + 1 );

  composeResponse1( tempResponse, response );
}

static void sendSDCommandR2( u32 * cmd, u8 * response )
{
  ALIGN(4) u8 tempResponse[160];tempResponse[0] = 0;
  ioRpgSendCommand( cmd, 136, 40, tempResponse + 1 );
  composeResponse2( tempResponse, response );
}

static void waitSDState( u8 state )
{
  u32 ready = 0;
  u32 cmd[2] = { 0xC0000000, 0x00000000 };
  do {
    ioRpgSendCommand( cmd, 4, 0, &ready );
    ready &= 0x0f;
  } while( ready != state );

}

static u32 calcSDSize(u8* data)
{
  u32 result=0;
  u32 type=data[15]>>6;
  switch(type)
  {
    case 0:
      {
        u32 block_len=data[10]&0xf;
        block_len=1<<block_len;
        u32 mult=(data[5]>>7)|((data[6]&3)<<1);
        mult=1<<(mult+2);
        result=data[9]&3;
        result=(result<<8)|data[8];
        result=(result<<2)|(data[7]>>6);
        result=(result+1)*mult*block_len/512;
      }
      break;
    case 1:
      result=data[8]&0x3f;
      result=(result<<8)|data[7];
      result=(result<<8)|data[6];
      result=(result+1)*1024;
      break;
  }
  return result;
}

//===============================================================//

bool sddInitSD()
{
  // reset sd card
  {
    u32 cmd[2] = { 0xD5000000, 0x00000000 };
    sendSDCommandR0( cmd );
  }

  // CMD8 for SDHC initialization
  {
    ALIGN(4) u8 response[4];
    u32 cmd[2] = { 0xD5010008, 0x000001AA };
    sendSDCommandR1( cmd, response );
    _isSD20 = (*(u32 *)response == cmd[1]);
    dbg_printf("cmd %d response %08x\n", 0x08, *(u32 *)response );
  }


  u32 retry = 0;

  while( retry < 10000 ) // about 1 second
  {
    // ACMD41
    {
      ALIGN(4) u8 response[4];
      u32 cmd[2] = { 0xD5010037, 0x00000000 };
      sendSDCommandR1( cmd, response );
      //dbg_printf("cmd %d response %08x\n", 0x37, *(u32 *)response );
    }

    {
      ALIGN(4) u8 response[4];
      u32 cmd[2] = { 0xD5010029, 0x00ff8000 };
      if( _isSD20 )
        cmd[1] |= 0x40000000;
      sendSDCommandR1( cmd, response );
      //dbg_printf("cmd %d response %08x\n", 0x29, *(u32 *)response );
      if( !(response[3] & 0x80) ) {
        ioRpgDelay( 1666 );
        retry++;
      } else {
        _isSDHC = ( *(u32 *)response & 0x40000000 );
        break;
      }
    }
  }
  dbg_printf("retry %d times\n", retry );

  // CMD2
  {
    ALIGN(4) u8 response[16];
    u32 cmd[2] = { 0xD5010002, 0x00000000 };
    sendSDCommandR2( cmd, response );
    dbg_printf("cmd 02 response %08x\n", *(u32 *)response );
  }

  // CMD3
  {
    ALIGN(4) u8 response[4];
    u32 cmd[2] = { 0xD5010003, 0x00000000 };
    sendSDCommandR1( cmd, response );
    dbg_printf("cmd 03 response %08x\n", *(u32 *)response );
    SDCardAddr = *(u32 *)response >> 16;
  }

  // CMD9
  {
    ALIGN(4) u8 response[16];
    u32 cmd[2] = { 0xD5010009, 0x00000000 | (SDCardAddr << 16) };
    sendSDCommandR2( cmd, response );
    dbg_printf("cmd 09 response\n%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x \n",response[0],response[1],response[2],response[3],response[4],response[5],response[6],response[7],response[8],response[9],response[10],response[11],response[12],response[13],response[14],response[15]);
    _size=calcSDSize(response);
  }

  // CMD7
  {
    ALIGN(4) u8 response[4];
    u32 cmd[2] = { 0xD5010007, 0x00000000 | (SDCardAddr << 16) };
    sendSDCommandR1( cmd, response );
    dbg_printf("cmd 07 response %08x\n", *(u32 *)response );
  }

  // ACMD6
  {
    ALIGN(4) u8 response[4];
    u32 cmd[2] = { 0xD5010037, 0x00000000 | (SDCardAddr << 16) };
    sendSDCommandR1( cmd, response );
    dbg_printf("cmd %d response %08x\n", 0x37, *(u32 *)response );
  }

  {
    ALIGN(4) u8 response[4];
    u32 cmd[2] = { 0xD5010006, 0x00000002 };
    sendSDCommandR1( cmd, response );
    dbg_printf("cmd 06 response %08x\n", *(u32 *)response );
  }

  // CMD13
  {
    ALIGN(4) u8 response[4];
    u32 cmd[2] = { 0xD501000D, 0x00000000 | (SDCardAddr << 16) };
    sendSDCommandR1( cmd, response );
    u8 state = (response[1] >> 1);  // response bit 9 - 12
    if( 4 == state ) {
      dbg_printf("sd init ok\n");
      //wait_press_b();
      if( _isSDHC ) {
        u32 isSDHCcmd[2] = { 0xC1010000, 0x00000000 };
        ioRpgSendCommand( isSDHCcmd, 0, 0, NULL );
      }
      return true;
    }
    else {
      dbg_printf("sd init fail\n" );
      //wait_press_b();
      return false;
    }

    dbg_printf(" the last response is\n%08x\n", *(u32 *)response );
  }
}

void sddReadBlocks( u32 addr, u32 blockCount, void * buffer )
{
  u8 * pBuffer = (u8 *)buffer;

  u32 address = _isSDHC ? addr : (addr << 9);
  u32 sdCommand[2] = { 0xD5030011, address }; // input read address here
  sdCommand[1] = address;
  if( 1 == blockCount ) {
    sdCommand[0]=0xD5030011;
    sendSDCommandR0( sdCommand );
    ioRpgWaitCmdBusy( true );

    sdCommand[0] = 0xB7000000;
    sdCommand[1] = 0x00000000 | 0x00130000; // address dont care here

    ioRpgSendCommand( sdCommand, 512, 4, pBuffer );
    ioRpgDelay( 16 * 5 );                                // wait for sd crc auto complete
  } else {
    u32 lastSector=_size-1;
    ALIGN(4) u8 response[4];
    sdCommand[0]=0xD5040012;
    sendSDCommandR0( sdCommand );
    ioRpgWaitCmdBusy( true );

    sdCommand[0] = 0xB7000000;
    sdCommand[1] = 0x00000000 | 0x00130000; // address dont care here

    while( blockCount ) {
      ioRpgSendCommand( sdCommand, 512, 4, pBuffer );
      waitSDState( 0x07 );
      pBuffer += 512;
      blockCount--;
      if((++addr)==lastSector) break;
    }

    sdCommand[0] = 0xD501000C;
    sdCommand[1] = 0x00000000;
    sendSDCommandR1( sdCommand, response );
    if(blockCount)
    {
      while(blockCount)
      {
        sddReadBlocks(addr++,1,pBuffer);
        pBuffer+=512;
        blockCount--;
      }
    }
    //dbg_printf("cmd %d response %08x\n", 0x0C, *(u32 *)response );
  }
}

void sddWriteBlocks( u32 addr, u32 blockCount, const void * buffer )
{
  const u8 * pBuffer = (u8 *)buffer;

  u32 address = _isSDHC ? addr : (addr << 9);
  if( 1 == blockCount ) {
    u32 sdWriteBlock[2] = { 0xD5050018, address };
    sendSDCommandR0( sdWriteBlock );
    ioRpgPushData( pBuffer, 512 );
    waitSDState( 0x00 );
    pBuffer += 512;
  } else {
    u32 sdWriteBlocks[2] = { 0xD5060019, address };

    while( blockCount-- ) {
      sendSDCommandR0( sdWriteBlocks );
      ioRpgPushData( pBuffer, 512 );
      waitSDState( 0x0e );
      pBuffer += 512;
    }

    ALIGN(4) u8 response[4];
    u32 sdStopTransmission[2] = { 0xD501000C, 0x00000000 };
    sendSDCommandR1( sdStopTransmission, response );
    //dbg_printf("cmd %d response %08x\n", 0x0C, *(u32 *)response );
    waitSDState( 0x00 );
  }
}
