/*
	iointerface.c
	
 Copyright (c) 2006 Michael "Chishm" Chisholm
	
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


// When compiling for NDS, make sure NDS is defined
#ifndef NDS
 #if defined ARM9 || defined ARM7
  #define NDS
 #endif
#endif

#ifdef NDS
 #include <nds/ndstypes.h>
#else
 #include "gba_types.h"
#endif

#define BYTES_PER_READ 512

#ifndef NULL
 #define NULL 0
#endif
#include <nds.h>

#include <nds/card.h>


#define ROMCTRL_FLAG 0xA0180010
#define ROMCTRL_FLAGW 0xE0180010

u32 cardcommand_r4(u8 cmd,u32 address,u32 data) 
{
    u32 returndata = 0;
	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
	REG_CARD_COMMAND[0] = cmd;
	REG_CARD_COMMAND[1] = (address >> 24) & 0xff;
	REG_CARD_COMMAND[2] = (address >> 16) & 0xff;
	REG_CARD_COMMAND[3] = (address >> 8) & 0xff;
	REG_CARD_COMMAND[4] = address & 0xff;
	REG_CARD_COMMAND[5] = (data>>16) & 0xff;
	REG_CARD_COMMAND[6] = (data>>8) & 0xff;
	REG_CARD_COMMAND[7] = data & 0xff;
	REG_ROMCTRL = ROMCTRL_FLAG | 0x07000000;
	do{
		if (REG_ROMCTRL&CARD_DATA_READY) 
			returndata=REG_CARD_DATA_RD;
	}while (REG_ROMCTRL&CARD_BUSY);
	return returndata;
}
int wait_ack(void)
{
    u32 temp;
    while(1)
    {
        temp=cardcommand_r4(0xe0,0,0);
        if (((temp>>21)&1) ==1)
        {
            break;
        }
    }
    return 0;
}
int wait_data(void)
{
    u32 temp;
    while(1)
    {
        temp=cardcommand_r4(0xe0,0,0);
        if (((temp>>28)&1) ==1)
        {
            break;
        }
    }
    return 0;
}

void cmd_begin()
{
    cardcommand_r4(0xe1,0,0);
}

void cardcommand_r512(u8 cmd,u32 address,u32 buf) 
{
    u32 returndata;
    cmd_begin();

	REG_CARD_COMMAND[0] = cmd;
	REG_CARD_COMMAND[1] = (address >> 24) & 0xff;
	REG_CARD_COMMAND[2] = (address >> 16) & 0xff;
	REG_CARD_COMMAND[3] = (address >> 8) & 0xff;
	REG_CARD_COMMAND[4] = address & 0xff;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 2;
	REG_CARD_COMMAND[7] = 0;
	REG_ROMCTRL = ROMCTRL_FLAG;
	do{
		if (REG_ROMCTRL&CARD_DATA_READY) 
			returndata=REG_CARD_DATA_RD;
	}while (REG_ROMCTRL&CARD_BUSY);
    wait_data();

	REG_CARD_COMMAND[0] = 0xe8;
	REG_CARD_COMMAND[1] = 0;
	REG_CARD_COMMAND[2] = 0;
	REG_CARD_COMMAND[3] = 0;
	REG_CARD_COMMAND[4] = 0;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;
    REG_ROMCTRL =ROMCTRL_FLAG | 0x01000000;
    if ((buf & 3) != 0)
    {
        do {
            if (REG_ROMCTRL & CARD_DATA_READY) {
                u32 tempdata=REG_CARD_DATA_RD;
                *(u8*)buf++ = (tempdata>>0) & 0xff;
                *(u8*)buf++ = (tempdata>>8) & 0xff;
                *(u8*)buf++ = (tempdata>>16) & 0xff;
                *(u8*)buf++ = (tempdata>>24) & 0xff;
                }
        } while (REG_ROMCTRL & CARD_BUSY);
    }
    else
    {
        do {
            if (REG_ROMCTRL & CARD_DATA_READY) {
                *(u32*)buf=REG_CARD_DATA_RD;
                buf += 4;
                }
        } while (REG_ROMCTRL & CARD_BUSY);

    }
  

}


void cardcommand_w512(u8 cmd,u32 address,u32 buf) 
{
    u32 returndata;
    cmd_begin();

	REG_CARD_COMMAND[0] = 0xe9;
	REG_CARD_COMMAND[1] = 0;
	REG_CARD_COMMAND[2] = 0;
	REG_CARD_COMMAND[3] = 0;
	REG_CARD_COMMAND[4] = 0;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;

    REG_ROMCTRL =ROMCTRL_FLAGW | 0x01000000;
    if ((buf & 3) != 0)
    {
        do {
            if (REG_ROMCTRL & CARD_DATA_READY) {
                    u32 tempdata = ((u8*)buf)[0] | (((u8*)buf)[1] << 8) | (((u8*)buf)[2] << 16) | (((u8*)buf)[3] << 24);
                    REG_CARD_DATA_RD= tempdata;
                    buf+=4;
                }
        } while (REG_ROMCTRL & CARD_BUSY);
    }
    else
    {
        do {
            if (REG_ROMCTRL & CARD_DATA_READY) {
                    REG_CARD_DATA_RD= *(u32*)buf;
                    buf+=4;
                }
        } while (REG_ROMCTRL & CARD_BUSY);

    }

	REG_CARD_COMMAND[0] = cmd;
	REG_CARD_COMMAND[1] = (address >> 24) & 0xff;
	REG_CARD_COMMAND[2] = (address >> 16) & 0xff;
	REG_CARD_COMMAND[3] = (address >> 8) & 0xff;
	REG_CARD_COMMAND[4] = address & 0xff;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 2;
	REG_CARD_COMMAND[7] = 0;
	REG_ROMCTRL = ROMCTRL_FLAG;
	do{
		if (REG_ROMCTRL&CARD_DATA_READY) 
			returndata=REG_CARD_DATA_RD;
	}while (REG_ROMCTRL&CARD_BUSY);

    wait_ack();

}

bool readSectors (u32 sector, u32 numSectors, void* buffer) {
    int i;
    for (i=0;i<numSectors ;i++ )
    {
        cardcommand_r512(0x50,sector+i,((u32)buffer)+(i<<9));
    }
	return true;
}
bool writeSectors (u32 sector, u32 numSectors, void* buffer) {
    int i;
    for (i=0;i<numSectors ;i++ )
    {
        cardcommand_w512(0x51,sector+i,((u32)buffer)+(i<<9));
    }
	return true;
}


bool startup(void) {
	return true;
}

/*-----------------------------------------------------------------
isInserted
Is a card inserted?
return true if a card is inserted and usable
-----------------------------------------------------------------*/
bool isInserted (void) {
	return true;
}


/*-----------------------------------------------------------------
clearStatus
Reset the card, clearing any status errors
return  true if the card is idle and ready
-----------------------------------------------------------------*/
bool clearStatus (void) {
	return true;
}

/*-----------------------------------------------------------------
shutdown
shutdown the card, performing any needed cleanup operations
-----------------------------------------------------------------*/
bool shutdown(void) {
	return true;
}
