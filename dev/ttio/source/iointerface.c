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

#include "common.h"

void cardWriteCommand(const u8 *command) {
	int index;

	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;

	for (index = 0; index < 8; index++) {
		REG_CARD_COMMAND[7-index] = command[index];
	}
}

void cardPolledTransfer(u32 flags, u32 *destination, u32 length, const u8 *command) {
	u32 data;
	cardWriteCommand(command);
	REG_ROMCTRL = flags;
	u32 * target = destination + length;
	do {
		// Read data if available
		if (REG_ROMCTRL & CARD_DATA_READY) {
			data=REG_CARD_DATA_RD;
			if (NULL != destination && destination < target)
				*destination++ = data;
		}
	} while (REG_ROMCTRL & CARD_BUSY);
}
/*
u32 sdmode_sdhc(void) {
	u8 command[8];
	u32 ret;
	u32 address=0x7fa00-0x20;
	command[0] = 0xb0;
	command[1] = 0;
	command[2] = 0;
	command[3] = address & 0xff;
	command[4] = (address >> 8) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[6] = (address >> 24) & 0xff;
	command[7] = 0x70;
	cardPolledTransfer(0xa7180000, &ret, 1, command);
	return ret;
}
*/
u32 sd_command(u8 cmd)
{
	REG_CARD_COMMAND[0] = cmd;
	REG_ROMCTRL = 0xA7180000;
	while (!(REG_ROMCTRL & CARD_DATA_READY));
	return REG_CARD_DATA_RD;
}

void sd_readpage(unsigned int addr,unsigned int dst)
{
	u32 data;
	
	REG_CARD_COMMAND[4] = (u8)(addr);
	REG_CARD_COMMAND[3] = (u8)(addr >> 8);
	REG_CARD_COMMAND[2] = (u8)(addr >> 16);
	REG_CARD_COMMAND[1] = (u8)(addr >> 24);
	sd_command(0x53);
	
	while(sd_command(0x80));
	
	REG_CARD_COMMAND[0] = 0x81;
	REG_ROMCTRL=0xA1180000;
	do {
		if (REG_ROMCTRL & CARD_DATA_READY) {
			data=REG_CARD_DATA_RD;
			((uint8*)dst)[0] = data & 0xff;
			((uint8*)dst)[1] = (data >> 8) & 0xff;
			((uint8*)dst)[2] = (data >> 16) & 0xff;
			((uint8*)dst)[3] = (data >> 24) & 0xff;
			dst+=4;
		}
	} while (REG_ROMCTRL & CARD_BUSY);
}

void sd_writepage(unsigned int addr,unsigned int dst)
{
	REG_CARD_COMMAND[7] = 0;
	REG_CARD_COMMAND[6] = 1;
	REG_CARD_COMMAND[5] = 24;
	REG_CARD_COMMAND[4] = (u8)(addr);
	REG_CARD_COMMAND[3] = (u8)(addr>> 8);
	REG_CARD_COMMAND[2] = (u8)(addr >> 16);
	REG_CARD_COMMAND[1] = (u8)(addr >> 24);
	sd_command(0x51);
	
	while(sd_command(0x50));
	sd_command(0x52);
	
	REG_CARD_COMMAND[0] = 0x82;
	REG_ROMCTRL = 0xE1180000;
	do {
		if (REG_ROMCTRL & CARD_DATA_READY) {
			REG_CARD_DATA_RD= ((uint8*)dst)[0] | (((uint8*)dst)[1] << 8) | (((uint8*)dst)[2] << 16) | (((uint8*)dst)[3] << 24);
			dst+=4;
		}
	} while (REG_ROMCTRL & CARD_BUSY);
	
	while(sd_command(0x50));
	sd_command(0x56);
	while(sd_command(0x50));
}

/*-----------------------------------------------------------------
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool readSectors (u32 sector, u32 numSectors, void* buffer) {
	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
    int i;
	if(sdmode_sdhc)
    {
		for (i=0;i<numSectors ;i++ )
		{
            sd_readpage(sector+i,((u32)buffer)+(i<<9));
		}
    }
	else
    {
		for (i=0;i<numSectors ;i++ )
		{
		    sd_readpage((sector+i)<<9,((u32)buffer)+(i<<9));
		}
    }
	return true;
}

/*-----------------------------------------------------------------
writeSectors
Write "numSectors" 512-byte sized sectors from "buffer" to the card, 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool writeSectors (u32 sector, u32 numSectors, void* buffer) {
	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
    int i;
	if(sdmode_sdhc)
    {
		for (i=0;i<numSectors ;i++ )
		{
		    sd_writepage(sector+i,((u32)buffer)+(i<<9));
        }
    }
	else
    {
		for (i=0;i<numSectors ;i++ )
		{
		    sd_writepage((sector+i)<<9,((u32)buffer)+(i<<9));
        }
    }
	return true;
}

/*-----------------------------------------------------------------
startUp
Initialize the interface, geting it into an idle, ready state
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
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
