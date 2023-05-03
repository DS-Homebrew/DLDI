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

#define	sdmode_sdhc		(*(vu32*)0x023FEE60)	//FIXME: DLDI could not alloc global memory ??

/*-----------------------------------------------------------------
startUp
Initialize the interface, geting it into an idle, ready state
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
void sd_readpage(unsigned int addr,unsigned int dst)
{
	CARD_COMMAND[0] = 0x53;
	CARD_COMMAND[1] = (u8)(addr >> 24);
	CARD_COMMAND[2] = (u8)(addr >> 16);
	CARD_COMMAND[3] = (u8)(addr >> 8);
	CARD_COMMAND[4] = (u8)(addr);
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY)) ;
	CARD_DATA_RD;

    do{
        CARD_COMMAND[0] = 0x80;
        CARD_CR2 = 0xA7180000;
        while (!(CARD_CR2 & CARD_DATA_READY));
    }while(CARD_DATA_RD);

    CARD_COMMAND[0] = 0x81;
    CARD_CR2=0xA1180000;
    do {
        if (CARD_CR2 & CARD_DATA_READY) {
            *(vu32*)dst=CARD_DATA_RD;
            dst+=4;
        }
    } while (CARD_CR2 & CARD_BUSY);
}
void sd_setmode(u8 mode,u8 cmd,u32 arg)
{
	CARD_COMMAND[0] = 0x51;
	CARD_COMMAND[1] = (u8)(arg >> 24);
	CARD_COMMAND[2] = (u8)(arg >> 16);
	CARD_COMMAND[3] = (u8)(arg >> 8);
	CARD_COMMAND[4] = (u8)(arg);
	CARD_COMMAND[5] = cmd;
	CARD_COMMAND[6] = mode;
	CARD_COMMAND[7] = 0;
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY)) ;
	CARD_DATA_RD;
}
u32 sd_isbusy()
{
	CARD_COMMAND[0] = 0x50;
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY)) ;
	return CARD_DATA_RD;
}
u32 sd_getresp()
{
	CARD_COMMAND[0] = 0x52;
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY)) ;
	return CARD_DATA_RD;
}

u32 sd_cmd_r1(u8 cmd,u32 arg)//4
{
	sd_setmode(1,cmd,arg);
	while(sd_isbusy());
	return sd_getresp();
}

void sd_writepage(unsigned int addr,unsigned int dst)
{
    sd_cmd_r1(24,addr);
    CARD_COMMAND[0] = 0x82;
    CARD_CR2 = 0xE1180000;
    do {
        if (CARD_CR2 & CARD_DATA_READY) {
                CARD_DATA_RD=*(vu32*)dst;
                dst+=4;
            }
    } while (CARD_CR2 & CARD_BUSY);
	while(sd_isbusy());
    CARD_COMMAND[0] = 0x56;
    CARD_CR2 = 0xA7180000;
    while (!(CARD_CR2 & CARD_DATA_READY));
    CARD_DATA_RD;
	while(sd_isbusy());

}


/*-----------------------------------------------------------------
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool readSectors (u32 sector, u32 numSectors, void* buffer) {
	CARD_CR1H = CARD_CR1_ENABLE | CARD_CR1_IRQ;
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
	CARD_CR1H = CARD_CR1_ENABLE | CARD_CR1_IRQ;
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
u32 cardcommand_r4(u8 cmd,u32 address) 
{
	CARD_CR1H = CARD_CR1_ENABLE | CARD_CR1_IRQ;

	CARD_COMMAND[0] = cmd;
	CARD_COMMAND[1] = (address >> 24) & 0xff;
	CARD_COMMAND[2] = (address >> 16) & 0xff;
	CARD_COMMAND[3] = (address >> 8) & 0xff;
	CARD_COMMAND[4] = address & 0xff;
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 0;
	CARD_COMMAND[7] = 0;
	CARD_CR2 = 0xA7180000;
	while (!(CARD_CR2 & CARD_DATA_READY)) ;
	return CARD_DATA_RD;
}


bool startup(void) {
    sdmode_sdhc=cardcommand_r4(0x70,(0x7fa00-0x20));
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
