/*
io_x9_card.c

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

#include <nds/ndstypes.h>
#include "io_x9_card.dldi.h"
#include "io_x9sd.dldi.h"

#include "tonccpy.h"

//#define DO_DEBUG(statements) do { statements; } while(0)               
#define DO_DEBUG(statements)

// Hmm... what do the flags mean?
//0xC1586000 write data
//0xA0586000 io 1 2 3 4
//0xA1586000 send command, expecting command-response, repeating a command w/o data
//0xB15863FA ?? just after (sending write-data, then write-command)
//0xA3586000 reading data -- expecting data-stream after command

void cardWriteCommand(const uint8* command)
{
	while(REG_ROMCTRL & CARD_BUSY){	
		if(REG_ROMCTRL & CARD_DATA_READY){
			break;
		}
	}
	
	//enable only if disabled previously
	if(!(REG_AUXSPICNTH & CARD_CR1_ENABLE)){
		REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
	}
	
	int index;
	for (index = 0; index < 8; index++) {
		REG_CARD_COMMAND[7-index] = command[index];
	}
}

/* Writes data to the X9 until "not ready"
   In practice, will transfer 512 bytes
   
   NOTE: buffer must be aligned to a 4 byte boundary
*/
void X9CardPolledWrite(uint32 flags, const uint32* buffer, const uint8* command)
{
    cardWriteCommand(command);
    REG_ROMCTRL = flags;
    do
    {
        if(REG_ROMCTRL & CARD_DATA_READY)
		REG_CARD_DATA_RD = *buffer++;
		
    } 
	while(REG_ROMCTRL & CARD_BUSY);
}

void X9CardWriteData(uint8 arg1, uint32 arg2, const void* buffer)
{
    x9Command[7] = 0x64;
    x9Command[6] = (arg2>>24)&0xFF;
    x9Command[5] = (arg2>>16)&0xFF;
    x9Command[4] = (arg2>>8)&0xFF;
    x9Command[3] = (arg2)&0xFF;
    x9Command[2] = 0;
    x9Command[1] = 0;
    x9Command[0] = arg1;
	
	if(((int)(buffer) % 4) == 0)
		X9CardPolledWrite(0xC1586000, (uint32*)buffer, x9Command);
	else
	{
	    tonccpy((uint8*)x9Buffer, (const uint8*)buffer, 512);
        X9CardPolledWrite(0xC1586000, (uint32*)x9Buffer, x9Command);
	}
}

void cardPolledTransfer(uint32 flags, uint32* dest,uint32 length, const uint8* command)
{
    cardWriteCommand(command);
    //ori
	//CARD_CR2 = flags;
	REG_ROMCTRL = flags;
	
    uint32* end = dest + length;
    do
    {
		//ori
        //if(CARD_CR2 & CARD_DATA_READY)
        if (REG_ROMCTRL & CARD_DATA_READY)
		{
            u32 value = REG_CARD_DATA_RD;
            if(dest < end)
                *dest = value;
            ++dest;                                                                               
        }

    } 
	//ori
	//while(CARD_CR2 & CARD_BUSY);
	while(REG_ROMCTRL & CARD_BUSY);
	
}