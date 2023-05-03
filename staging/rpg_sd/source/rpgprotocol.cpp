/*
    rpgprotocol.cpp
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

#include "rpgprotocol.h"
#include "dbgtool.h"

#define PAGESIZE_0      (0<<24)
#define PAGESIZE_4      (7<<24)
#define PAGESIZE_512    (1<<24)
#define PAGESIZE_1K     (2<<24)
#define PAGESIZE_2K     (3<<24)
#define PAGESIZE_4K     (4<<24)

void __attribute__((naked)) ioRpgDelay(u32 us)
{
    __asm("subs r0, #1");
    __asm("bgt ioRpgDelay");
    __asm("bx lr");
}

void ioRpgSendCommand( u32 command[2], u32 pageSize, u32 latency, void * buffer )
{
  REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;

  for( u32 i=0; i<2; ++i )
  {
    CARD_COMMAND[i*4+0] = command[i]>>24;
    CARD_COMMAND[i*4+1] = command[i]>>16;
    CARD_COMMAND[i*4+2] = command[i]>>8;
    CARD_COMMAND[i*4+3] = command[i]>>0;
  }

  pageSize -= pageSize & 3; // align to 4 byte
  u32 pageParam = PAGESIZE_4K;
  u32 transferLength = 4096;
  // make zero read and 4 byte read a little special for timing optimization(and 512 too)
  if( 0 == pageSize ) {
    transferLength = 0;
    pageParam = PAGESIZE_0;
  } else if( 4 == pageSize ) {
    transferLength = 4;
    pageParam = PAGESIZE_4;
  } else if( 512 == pageSize ) {
    transferLength = 512;
    pageParam = PAGESIZE_512;
  }

  // go
  REG_ROMCTRL = 0;
  REG_ROMCTRL = KEY_PARAM | CARD_ACTIVATE | CARD_nRESET | pageParam | latency;

  u8 * pbuf = (u8 *)buffer;
  u32 * pbuf32 = (u32 * )buffer;
  bool useBuf = ( NULL != pbuf );
  bool useBuf32 = (useBuf && (0 == (3 & ((unsigned int)buffer))));

  u32 count = 0;
  u32 cardCtrl = REG_ROMCTRL;
  while( (cardCtrl & CARD_BUSY) && count < pageSize)
  {
    cardCtrl = REG_ROMCTRL;
    if( cardCtrl & CARD_DATA_READY  ) {
      u32 data = CARD_DATA_RD;
      if( useBuf32 && count < pageSize) {
        *pbuf32++ = data;
      }
      else if ( useBuf && count < pageSize ) {
        pbuf[0] = (unsigned char) (data >>  0);
        pbuf[1] = (unsigned char) (data >>  8);
        pbuf[2] = (unsigned char) (data >> 16);
        pbuf[3] = (unsigned char) (data >> 24);
        pbuf += sizeof (unsigned int);
      }
      count += 4;
    }
  }

  // if read is not finished, ds will not pull ROM CS to high, we pull it high manually
  if( count != transferLength ) {
    // MUST wait for next data ready,
    // if ds pull ROM CS to high during 4 byte data transfer, something will mess up
    // so we have to wait next data ready
    do { cardCtrl = REG_ROMCTRL; } while(!(cardCtrl & CARD_DATA_READY));
    // and this tiny delay is necessary
    ioRpgDelay(33);
    // pull ROM CS high
    REG_ROMCTRL = 0;
    REG_ROMCTRL = KEY_PARAM | CARD_ACTIVATE | CARD_nRESET | 0 | 0x0000;
  }
  // wait rom cs high
  do { cardCtrl = REG_ROMCTRL; } while( cardCtrl & CARD_BUSY );
}

bool ioRpgWaitCmdBusy( bool forceWait )
{
  bool timeout = true;
  while( timeout && forceWait ) {
    REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
    for( u32 i=0; i<8; ++i )
      CARD_COMMAND[i] = 0xB8;
    // go
    REG_ROMCTRL = 0;
    REG_ROMCTRL = KEY_PARAM | CARD_ACTIVATE | CARD_nRESET | (6<<24) | 0;

    u32 count = 0;
    u32 transferLength = 256 << 6;
    u32 data = 0;
    u32 cardCtrl = REG_ROMCTRL;

    while( cardCtrl & CARD_BUSY )
    {
      cardCtrl = REG_ROMCTRL;
      if( cardCtrl & CARD_DATA_READY  ) {
        data = CARD_DATA_RD;
        count += 4;
        if( 0x00000fc2 == data ) {
          timeout = false;
          break;
        }
      }
      ioRpgDelay( 16 ); // 1us
    }

    if( count != transferLength ) {// if read is not finished, ds will not pull ROM CS to high, we pull it high manually
      do { cardCtrl = REG_ROMCTRL; } while(!(cardCtrl & CARD_DATA_READY));
      ioRpgDelay( 33 );
      REG_ROMCTRL = 0;
      REG_ROMCTRL = KEY_PARAM | CARD_ACTIVATE | CARD_nRESET | 0 | 0x0000;
    }

    // wait rom cs high
    do { cardCtrl = REG_ROMCTRL; } while( cardCtrl & CARD_BUSY );

    if( timeout ) {
      dbg_printf("ioRpgWaitCmdBusy time out, " );
      dbg_printf("(%08x)\n", data );
    }
  }
  return !timeout;
}
