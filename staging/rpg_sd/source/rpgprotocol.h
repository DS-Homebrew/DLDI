/*
    rpgprotocol.h
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

#ifndef _RPGPROTOCOL_H_
#define _RPGPROTOCOL_H_

#include <nds.h>
#ifdef __cplusplus
extern "C" {
#endif

enum MAP_TABLE_NAME
{
  MTN_MAP_A     = 0,
  MTN_MAP_B     = 1,
  MTN_NAND_OFFSET1  = 2,
  MTN_SAVE_TABLE    = 3
};

enum DEVICE_NAME
{
  DN_DEFAULT  = 0,
  DN_NAND   = 1,
  DN_SRAM   = 2,
  DN_SD   = 3
};

enum SRAM_ALLOC {
  SRAM_NANDLUT_START = 0x000000,
  SRAM_SAVETABLE_START = 0x010000,
  SRAM_FAT_START = 0x020000,
  SRAM_LOADING_FILENAME = 0x0A0000,
  SRAM_DISKBUFFER_START = 0x0FF000,
  SRAM_SAVEDATA_START = 0x100000,
};
#define KEY_PARAM           0x00406000
//#define KEY_PARAM           0x00000000


void ioRpgSendCommand( u32 command[2], u32 pageSize, u32 latency, void * buffer );

bool ioRpgWaitCmdBusy( bool forceWait );

void ioRpgSetDeviceStatus( u8 deviceSelection, u8 tables, u32 table0BlockSize, u32 table1BlockSize );

void ioRpgSetMapTableAddress( u32 tableName, u32 tableInRamAddress );

void ioRpgReadSram( u32 address, void * buffer, u32 length );

void ioRpgWriteSram( u32 address, const void * data, u32 length );

void ioRpgReadNand( u32 address, void * buffer, u32 length );

void ioRpgReadNandRedundant( u32 address, void * buffer );

void ioRpgWriteNand( u32 address, const void * data, u32 length );

void ioRpgEraseNand( u32 blockAddr );

void ioRpgPageCopyNandToNand( u32 src, u32 dest );

void ioRpgPageCopyNandInternal( u32 src, u32 dest );

void ioRpgReadSDBlock( u32 address, void * buffer, u32 blockCount );

void ioRpgWriteSDBlock( u32 address, const void * data );

void ioRpgAsignSaveBlocks( u32 tableAddress, u32 nandAddress[64] );

void ioRpgDelay( u32 us );

static inline void ioRpgMemCopy( void * dest, const void * src, u32 length )
{
  if( 0 == ((u32)dest & 0x03) && 0 == ((u32)src & 0x03) ) {
    u32 * pSrc32 = (u32 *)src;
    u32 * pDest32 = (u32 *)dest;
    length >>= 2;

    while( length-- > 0 ) {
      *pDest32++ = *pSrc32++;
    }
  } else {
    u8 * pSrc8 = (u8 *)src;
    u8 * pDest8 = (u8 *)dest;
    while( length-- > 0 ) {
      *pDest8++ = *pSrc8++;
    }
  }
}

#define CARD_COMMAND32   ((vuint32*)0x040001A8)
static inline void ioRpgPushData( const void * data, u16 length )
{
  if( 0 == ((u32)data & 0x03) ) {
    u32 * pData32 = (u32 *)data;
    while( length ) {
      CARD_COMMAND32[0] = *pData32++;
      CARD_COMMAND32[1] = *pData32++;
      length -= 8;
      REG_ROMCTRL = KEY_PARAM | CARD_ACTIVATE | CARD_nRESET;
      while( REG_ROMCTRL & CARD_BUSY ) {}
    }
  } else {
    u8 * pData8 = (u8 *)data;
    while( length ) {
      CARD_COMMAND[0] = *pData8++;
      CARD_COMMAND[1] = *pData8++;
      CARD_COMMAND[2] = *pData8++;
      CARD_COMMAND[3] = *pData8++;
      CARD_COMMAND[4] = *pData8++;
      CARD_COMMAND[5] = *pData8++;
      CARD_COMMAND[6] = *pData8++;
      CARD_COMMAND[7] = *pData8++;
      length -= 8;
      REG_ROMCTRL = KEY_PARAM | CARD_ACTIVATE | CARD_nRESET;
      while( REG_ROMCTRL & CARD_BUSY ) {}
    }
  }
}

#ifdef __cplusplus
}
#endif

#endif//_RPGPROTOCOL_H_
