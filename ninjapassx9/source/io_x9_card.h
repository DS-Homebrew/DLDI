/*
io_x9_card.h

Hardware routines for interfacing with SD cards on a Ninjapass X9

Copyright (C) 2007 by CJ Bell
siegebell at gmail dot com


This software is provided 'as-is', without any express or implied warranty.  In
no event will the author be held liable for any damages arising from the use of
this software.  Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not claim
	that you wrote the original software. If you use this software in a
	product, an acknowledgment in the product documentation would be
	appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.

*/

#ifndef _io_x9_card_h__
#define _io_x9_card_h__

// Shared data just to save space
static uint32 x9Buffer[0x200];
static uint8  x9Command[8];

static inline __attribute__((always_inline)) void X9CardIO(uint8 cmd, uint8 arg1, uint32 arg2, uint32 flags, uint32* buffer, uint32 words)
{
	x9Command[7] = cmd;
	x9Command[6] = (arg2>>24)&0xFF;
	x9Command[5] = (arg2>>16)&0xFF;
	x9Command[4] = (arg2>>8)&0xFF;
	x9Command[3] = (arg2)&0xFF;
	x9Command[2] = 0;
	x9Command[1] = 0;
	x9Command[0] = arg1;

	cardPolledTransfer(flags, buffer, words, x9Command);
}


static inline __attribute__((always_inline)) void cardIO_1()
{
	x9Command[7] = 0x58;
	x9Command[6] = 0;
	x9Command[5] = 0;
	x9Command[4] = 0;
	x9Command[3] = 0;
	x9Command[2] = 0;
	x9Command[1] = 2;
	x9Command[0] = 0;

	cardPolledTransfer(0xA0586000, x9Buffer, 0, x9Command);
}

static inline __attribute__((always_inline)) void cardIO_2(uint32 arg)
{
	X9CardIO(0x5D, 0, arg, 0xA0586000, x9Buffer, 0);
}

static inline __attribute__((always_inline)) void cardIO_3(uint8 arg0)
{
	x9Command[7] = 0x58;
	x9Command[6] = 0;
	x9Command[5] = 0;
	x9Command[4] = 0;
	x9Command[3] = 0;
	x9Command[2] = 0;
	x9Command[1] = 2 | arg0;
	x9Command[0] = 0;

	// uses 512 bytes of x9Buffer
	cardPolledTransfer(0xA0586000, x9Buffer, 0, x9Command);
}

static inline __attribute__((always_inline)) void cardIO_4(uint32 address)
{
	X9CardIO(0x5C, 0, address, 0xA0586000, x9Buffer, 0);
}


#ifdef __cplusplus
extern "C"{
#endif

//extern void X9CardPolledWrite(uint32 flags, const uint32* buffer, const uint8* command);
extern void X9CardIO(uint8 cmd, uint8 arg1, uint32 arg2, uint32 flags, uint32* buffer, uint32 words);
extern void X9CardWriteData(uint8 arg1, uint32 arg2, const void* buffer);
extern void cardPolledTransfer(uint32 flags, uint32* dest,uint32 length, const uint8* command);

#ifdef __cplusplus
}
#endif


#endif // INCLUDED_io_x9_card_h
