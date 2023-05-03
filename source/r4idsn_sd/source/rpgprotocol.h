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

#define KEY_PARAM           0x00406000

void ioRpgSendCommand(u32 command[2],u32 pageSize,u32 latency,void* buffer);
bool ioRpgWaitCmdBusy(bool forceWait);
void ioRpgDelay(u32 us);

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
      REG_CARD_COMMAND[0] = *pData8++;
      REG_CARD_COMMAND[1] = *pData8++;
      REG_CARD_COMMAND[2] = *pData8++;
      REG_CARD_COMMAND[3] = *pData8++;
      REG_CARD_COMMAND[4] = *pData8++;
      REG_CARD_COMMAND[5] = *pData8++;
      REG_CARD_COMMAND[6] = *pData8++;
      REG_CARD_COMMAND[7] = *pData8++;
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
