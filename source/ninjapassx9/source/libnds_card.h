/*---------------------------------------------------------------------------------

    This file is a part of libnds (card.h)

	Copyright (C) 2005
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/

#pragma once

#ifndef NULL
#define NULL 0
#endif


#include <nds/ndstypes.h>

// Card bus
#define	REG_CARD_DATA_RD	(*(vu32*)0x04100010)

#define REG_AUXSPICNTH	(*(vu8*)0x040001A1)
#define REG_ROMCTRL		(*(vu32*)0x040001A4)

#define REG_CARD_COMMAND	((vu8*)0x040001A8)

// 3 bits in b10..b8 indicate something
// read bits
#define CARD_BUSY         (1<<31)           // when reading, still expecting incomming data?
#define CARD_DATA_READY   (1<<23)           // when reading, CARD_DATA_RD or CARD_DATA has another word of data and is good to go

//REG_AUXSPICNT
#define CARD_SPICNTH_ENABLE  (1<<7)  // in byte 1, i.e. 0x8000
#define CARD_SPICNTH_IRQ     (1<<6)  // in byte 1, i.e. 0x4000

#ifdef __cplusplus
extern "C" {
#endif

void cardWriteCommand(const u8 *command);
void cardPolledTransfer(u32 flags, u32 *destination, u32 length, const u8 *command);

#ifdef __cplusplus
}
#endif
