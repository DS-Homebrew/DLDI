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


// Card bus
#define	REG_CARD_DATA_RD	(*(vu32*)0x04100010)

#define REG_AUXSPICNTH	(*(vu8*)0x040001A1)
#define REG_ROMCTRL		(*(vu32*)0x040001A4)

#define REG_CARD_COMMAND	((vu8*)0x040001A8)

#define CARD_CR1_ENABLE  0x80  // in byte 1, i.e. 0x8000
#define CARD_CR1_IRQ     0x40  // in byte 1, i.e. 0x4000

// 3 bits in b10..b8 indicate something
// read bits
#define CARD_BUSY         (1<<31)           // when reading, still expecting incomming data?
#define CARD_DATA_READY   (1<<23)           // when reading, REG_CARD_DATA_RD or CARD_DATA has another word of data and is good to go


#define SECTOR_SIZE		0x200	 // 512-byte sized sectors


void SCDS_sd_command(u8 cmd,u32 arg)
{
	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
	REG_CARD_COMMAND[0] = 0x33;
	REG_CARD_COMMAND[1] = 0;
	REG_CARD_COMMAND[2] = cmd|0x40;
	REG_CARD_COMMAND[3] = (arg>>24)&0xFF;
	REG_CARD_COMMAND[4] = (arg>>16)&0xFF;
	REG_CARD_COMMAND[5] = (arg>>8)&0xFF;
	REG_CARD_COMMAND[6] = (arg>>0)&0xFF;
	REG_CARD_COMMAND[7] = 0x01;
	REG_ROMCTRL = 0xA7180000;
	while (!(REG_ROMCTRL & CARD_DATA_READY));
	do{
		REG_CARD_COMMAND[0] = 0x38;
		REG_ROMCTRL = 0xA7180000;
		while (!(REG_ROMCTRL & CARD_DATA_READY)) ;
	}while(REG_CARD_DATA_RD);
}

void SCDS_get_resp()
{
	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
	REG_CARD_COMMAND[0] = 0x30;
	REG_CARD_COMMAND[1] = (48+16);
	REG_CARD_COMMAND[2] = 0;
	REG_CARD_COMMAND[3] = 0;
	REG_CARD_COMMAND[4] = 0;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;
	REG_ROMCTRL = 0xA7180000;
	while (!(REG_ROMCTRL & CARD_DATA_READY));
	do{
		REG_CARD_COMMAND[0] = 0x38;
		REG_ROMCTRL = 0xA7180000;
		while (!(REG_ROMCTRL & CARD_DATA_READY)) ;
	}while(REG_CARD_DATA_RD);
}

void SCDS_readsector(u32 addr,u32 buff)
{
	SCDS_sd_command(17,addr); 
	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
	REG_CARD_COMMAND[0] = 0x34;
	REG_CARD_COMMAND[1] = 0;
	REG_CARD_COMMAND[2] = 0;
	REG_CARD_COMMAND[3] = 0;
	REG_CARD_COMMAND[4] = 0;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;
	REG_ROMCTRL = 0xA7180000;
	while (!(REG_ROMCTRL & CARD_DATA_READY));
	do{
		REG_CARD_COMMAND[0] = 0x38;
		REG_ROMCTRL = 0xA7180000;
		while (!(REG_ROMCTRL & CARD_DATA_READY)) ;
	}while(REG_CARD_DATA_RD);

    REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
	REG_CARD_COMMAND[0] = 0x36;
	REG_CARD_COMMAND[1] = 0;
	REG_CARD_COMMAND[2] = 0;
	REG_CARD_COMMAND[3] = 0;
	REG_CARD_COMMAND[4] = 0;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;
	REG_ROMCTRL = 0xA1180000;
	do {
		if (REG_ROMCTRL & CARD_DATA_READY) {
			*(vu32*)buff=REG_CARD_DATA_RD;
			buff+=4;
		}
	} while (REG_ROMCTRL & CARD_BUSY);
}

void SCDS_writesector(u32 addr,u32 buff)
{
	SCDS_sd_command(24,addr); 
	SCDS_get_resp();
	u16 i;
	for(i=0;i<SECTOR_SIZE;i+=4)
	{
		REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;
		REG_CARD_COMMAND[0] = 0x37;
		REG_CARD_COMMAND[1] = (i >> 8) & 0xff;
		REG_CARD_COMMAND[2] = i & 0xff;
		REG_CARD_COMMAND[3] = ((uint8*)buff)[0];
		REG_CARD_COMMAND[4] = ((uint8*)buff)[1];
		REG_CARD_COMMAND[5] = ((uint8*)buff)[2];
		REG_CARD_COMMAND[6] = ((uint8*)buff)[3];
		REG_CARD_COMMAND[7] = 0;
		REG_ROMCTRL = 0xA7180000;
		while (!(REG_ROMCTRL & CARD_DATA_READY)) ;
		REG_CARD_DATA_RD;
		buff+=4;
	}
	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ; 
	REG_CARD_COMMAND[0] = 0x35;
	REG_CARD_COMMAND[1] = 0;
	REG_CARD_COMMAND[2] = 0;
	REG_CARD_COMMAND[3] = 0;
	REG_CARD_COMMAND[4] = 0;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;
	REG_ROMCTRL = 0xA7180000;
	while (!(REG_ROMCTRL & CARD_DATA_READY));
	do{
		REG_CARD_COMMAND[0] = 0x38;
		REG_ROMCTRL = 0xA7180000;
		while (!(REG_ROMCTRL & CARD_DATA_READY)) ;
	}while(REG_CARD_DATA_RD);
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
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool readSectors (u32 sector, u32 numSectors, void* buffer)
{
    sector<<=9;				//SECTOR_SIZE=512
	u32 dest=(u32)buffer;
	
    for(;numSectors;numSectors--) {
        SCDS_readsector(sector,dest);
		sector+=SECTOR_SIZE;
		dest+=SECTOR_SIZE;
    }
    return true;
}

/*-----------------------------------------------------------------
writeSectors
Write "numSectors" 512-byte sized sectors from "buffer" to the card, 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool writeSectors (u32 sector, u32 numSectors, void* buffer)
{
    sector<<=9;				//SECTOR_SIZE=512
	u32 data=(u32)buffer;
	
    for(;numSectors;numSectors--) {
        SCDS_writesector(sector,data);
		sector+=SECTOR_SIZE;
		data+=SECTOR_SIZE;
    }
    return true;
}

/*-----------------------------------------------------------------
shutdown
shutdown the card, performing any needed cleanup operations
-----------------------------------------------------------------*/
bool shutdown(void) {
	return true;
}
