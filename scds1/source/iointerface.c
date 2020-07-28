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
 #include <nds/jtypes.h>
#else
 #include "gba_types.h"
#endif

#define BYTES_PER_READ 512

#ifndef NULL
 #define NULL 0
#endif


// Card bus
#define CARD_CR1       (*(vu16*)0x040001A0)
#define CARD_CR1H      (*(vu8*)0x040001A1)
#define CARD_EEPDATA   (*(vu8*)0x040001A2)
#define CARD_CR2       (*(vu32*)0x040001A4)
#define CARD_COMMAND   ((vu8*)0x040001A8)

#define CARD_DATA_RD   (*(vu32*)0x04100010)

#define CARD_1B0       (*(vu32*)0x040001B0)  
#define CARD_1B4       (*(vu32*)0x040001B4)
#define CARD_1B8       (*(vu16*)0x040001B8)
#define CARD_1BA       (*(vu16*)0x040001BA)


#define CARD_CR1_ENABLE  0x80  // in byte 1, i.e. 0x8000
#define CARD_CR1_IRQ     0x40  // in byte 1, i.e. 0x4000


// CARD_CR2 register:

#define CARD_ACTIVATE   (1<<31)  // when writing, get the ball rolling
// 1<<30
#define CARD_nRESET     (1<<29)  // value on the /reset pin (1 = high out, not a reset state, 0 = low out = in reset)
#define CARD_28         (1<<28)  // when writing
#define CARD_27         (1<<27)  // when writing
#define CARD_26         (1<<26)
#define CARD_22         (1<<22)
#define CARD_19         (1<<19)
#define CARD_ENCRYPTED  (1<<14)  // when writing, this command should be encrypted
#define CARD_13         (1<<13)  // when writing
#define CARD_4          (1<<4)   // when writing

// 3 bits in b10..b8 indicate something
// read bits
#define CARD_BUSY       (1<<31)  // when reading, still expecting incomming data?
#define CARD_DATA_READY (1<<23)  // when reading, CARD_DATA_RD or CARD_DATA has another word of data and is good to go


#define SECTOR_SIZE		0x200	 // 512-byte sized sectors


void SCDS_sd_command(u8 cmd,u32 arg)
{
	CARD_CR1H = 0xC0;
	CARD_COMMAND[0] = 0x33;
	CARD_COMMAND[1] = 0;
	CARD_COMMAND[2] = cmd|0x40;
	CARD_COMMAND[3] = (arg>>24)&0xFF;
	CARD_COMMAND[4] = (arg>>16)&0xFF;
	CARD_COMMAND[5] = (arg>>8)&0xFF;
	CARD_COMMAND[6] = (arg>>0)&0xFF;
	CARD_COMMAND[7] = 0x01;
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY));
	do{
		CARD_COMMAND[0] = 0x38;
		CARD_CR2 = 0xA7180000;
		while (!(CARD_CR2 & CARD_DATA_READY)) ;
	}while(CARD_DATA_RD);
}

void SCDS_get_resp()
{
	CARD_CR1H = 0xC0;
	CARD_COMMAND[0] = 0x30;
	CARD_COMMAND[1] = (48+16);
	CARD_COMMAND[2] = 0;
	CARD_COMMAND[3] = 0;
	CARD_COMMAND[4] = 0;
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 0;
	CARD_COMMAND[7] = 0;
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY));
	do{
		CARD_COMMAND[0] = 0x38;
		CARD_CR2 = 0xA7180000;
		while (!(CARD_CR2 & CARD_DATA_READY)) ;
	}while(CARD_DATA_RD);
}

void SCDS_readsector(u32 addr,u32 buff)
{
	SCDS_sd_command(17,addr); 
	CARD_CR1H = 0xC0;
	CARD_COMMAND[0] = 0x34;
	CARD_COMMAND[1] = 0;
	CARD_COMMAND[2] = 0;
	CARD_COMMAND[3] = 0;
	CARD_COMMAND[4] = 0;
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 0;
	CARD_COMMAND[7] = 0;
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY));
	do{
		CARD_COMMAND[0] = 0x38;
		CARD_CR2 = 0xA7180000;
		while (!(CARD_CR2 & CARD_DATA_READY)) ;
	}while(CARD_DATA_RD);

    CARD_CR1H = 0xC0;
	CARD_COMMAND[0] = 0x36;
	CARD_COMMAND[1] = 0;
	CARD_COMMAND[2] = 0;
	CARD_COMMAND[3] = 0;
	CARD_COMMAND[4] = 0;
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 0;
	CARD_COMMAND[7] = 0;
	CARD_CR2 = 0xA1180000;
	do {
		if (CARD_CR2 & CARD_DATA_READY) {
			*(vu32*)buff=CARD_DATA_RD;
			buff+=4;
		}
	} while (CARD_CR2 & CARD_BUSY);
}

void SCDS_writesector(u32 addr,u32 buff)
{
	SCDS_sd_command(24,addr); 
	SCDS_get_resp();
	u16 i;
	for(i=0;i<SECTOR_SIZE;i+=4)
	{
		CARD_CR1H = 0xC0;
		CARD_COMMAND[0] = 0x37;
		CARD_COMMAND[1] = (i >> 8) & 0xff;
		CARD_COMMAND[2] = i & 0xff;
		CARD_COMMAND[3] = ((uint8*)buff)[0];
		CARD_COMMAND[4] = ((uint8*)buff)[1];
		CARD_COMMAND[5] = ((uint8*)buff)[2];
		CARD_COMMAND[6] = ((uint8*)buff)[3];
		CARD_COMMAND[7] = 0;
		CARD_CR2 = 0xA7180000;
		while (!(CARD_CR2 & CARD_DATA_READY)) ;
		CARD_DATA_RD;
		buff+=4;
	}
	CARD_CR1H = 0xC0; 
	CARD_COMMAND[0] = 0x35;
	CARD_COMMAND[1] = 0;
	CARD_COMMAND[2] = 0;
	CARD_COMMAND[3] = 0;
	CARD_COMMAND[4] = 0;
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 0;
	CARD_COMMAND[7] = 0;
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY));
	do{
		CARD_COMMAND[0] = 0x38;
		CARD_CR2 = 0xA7180000;
		while (!(CARD_CR2 & CARD_DATA_READY)) ;
	}while(CARD_DATA_RD);
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
