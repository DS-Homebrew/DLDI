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

#ifndef NDS
#error "This is a driver for DS"
#endif

#include <nds/system.h>

#define BYTES_PER_READ 512

#ifndef NULL
 #define NULL 0
#endif

#define PAGESIZE_0			(0<<24)
#define PAGESIZE_4			(7<<24)
#define PAGESIZE_512		(1<<24)

#define CLOCK_4MHz			(0<<0)
#define SPI_ROM				(0<<13)

// the following defines are from libnds
// Card bus
#define	REG_CARD_DATA_RD	(*(vu32*)0x04100010)

#define REG_AUXSPICNT	(*(vu16*)0x040001A0)
#define REG_AUXSPICNTH	(*(vu8*)0x040001A1)
#define REG_AUXSPIDATA	(*(vu8*)0x040001A2)
#define REG_ROMCTRL		(*(vu32*)0x040001A4)

#define REG_CARD_COMMAND	((vu8*)0x040001A8)

#define CARD_ACTIVATE     (1<<31)           // when writing, get the ball rolling
#define CARD_nRESET       (1<<29)           // value on the /reset pin (1 = high out, not a reset state, 0 = low out = in reset)

// 3 bits in b10..b8 indicate something
// read bits
#define CARD_BUSY         (1<<31)           // when reading, still expecting incomming data?
#define CARD_DATA_READY   (1<<23)           // when reading, CARD_DATA_RD or CARD_DATA has another word of data and is good to go

#define CARD_ENABLE			(1<<15)

/*
Define last zone that was read or written
*/
static int dsxLastZone = -1;

static unsigned char dsxBuffer[BYTES_PER_READ];

/*-----------------------------------------------------------------
wait_msecs
Wait for a certain amount of milliseconds, uses vertical scanlines to synchronize.
A hardware timer may be used aswell, if necessary.
-----------------------------------------------------------------*/
void dsxWaitMs(unsigned int requestTime)
{
	unsigned int lastLine = REG_VCOUNT;
	unsigned int newLine;
	unsigned int elapsedTime = 0; // in ms
	unsigned int elapsedLines = 0; // in lines


	while(elapsedTime < requestTime)
	{
		int diffLine;
		newLine = REG_VCOUNT;

		diffLine = newLine - lastLine;
		if (diffLine < 0)
			diffLine = 263+diffLine;

		elapsedLines += diffLine;

		//does this correctly optimize?
		elapsedTime = elapsedLines/16; // 16 lines = 1ms

		lastLine = newLine;
	}
}


void dsxSendCommand(unsigned int command[2], unsigned int pageSize, unsigned int latency, unsigned char *buf)
{
	int i;
	unsigned int *buf32 = (unsigned int*)buf;
	unsigned int *bufend32 = (unsigned int*)( (unsigned int)buf + pageSize );
	unsigned char *bufend = buf + pageSize;
	unsigned int ctrl, data;
	bool useBuf, useBuf32;

	useBuf = (0 != buf);
	useBuf32 = (useBuf && (0 != (3 & ((unsigned int)buf))));

	for(i=0; i<2; i++)
	{
		REG_CARD_COMMAND[i*4+0] = command[i]>>24;
		REG_CARD_COMMAND[i*4+1] = command[i]>>16;
		REG_CARD_COMMAND[i*4+2] = command[i]>>8;
		REG_CARD_COMMAND[i*4+3] = command[i]>>0;
	}


	switch(pageSize)
	{
		case 0:
			pageSize = PAGESIZE_0;
		break;

		case 4:
			pageSize = PAGESIZE_4;
		break;

		case 512:
			pageSize = PAGESIZE_512;
		break;	
	}

	REG_ROMCTRL = CARD_ACTIVATE | CARD_nRESET | pageSize | ((*(unsigned int*)0x027ffe60) & ~0x07001FFF) | latency;

	do
	{
		ctrl = REG_ROMCTRL;

		if (ctrl & CARD_DATA_READY)
		{
			data = REG_CARD_DATA_RD;

			if (useBuf32)
			{
				if (buf32 < bufend32)
					*buf32++ = data;
			} 
			else if (useBuf)
			{
				if (buf < bufend)
				{
					buf[0] = (unsigned char) (data >>  0);
					buf[1] = (unsigned char) (data >>  8);
					buf[2] = (unsigned char) (data >> 16);
					buf[3] = (unsigned char) (data >> 24);
					buf += sizeof (unsigned int);
				}
			}
		}
	} while( ctrl & CARD_ACTIVATE );	
}

//utility function to poll for write operation finish
void dsxPoll(void)
{
	unsigned int command[2];
	const unsigned int writeResultSize = 4;
	unsigned int writeResult;


	command[0] = 0x02000000;
	command[1] = 0;
	do
	{
		dsxSendCommand(command, writeResultSize, 0x800, (unsigned char*)&writeResult);
	}
	while( (writeResult) != 0);
}

//utility function to force a zone switch
void dsxZoneSwitch(unsigned int lba)
{
	unsigned int newZone;
	unsigned int newZoneTmp;
	unsigned int command[2];

	newZoneTmp = lba / 256;
	newZone = 0;

	//Calculate the new zone without using a divide...
	//forces the compiler to keep the code size small enough
	while(newZoneTmp >= 1000)
	{
		newZoneTmp -= 1000;
		newZone++;
	}

	// If switching zones
	if (newZone != dsxLastZone)
	{
		command[0] = 0xBF000000 | (lba>>8);
		command[1] = (lba<<24);	

		// Tell hardware to switch zones, by simply issueing a read
		dsxSendCommand(command, BYTES_PER_READ, 0xFFF, dsxBuffer);

		// Wait a moment for it to finish
		dsxPoll();
	}

	dsxLastZone = newZone;
}




/*-----------------------------------------------------------------
startUp
Initialize the interface, geting it into an idle, ready state
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
bool dsxStartup(void) {
	return true;
}

/*-----------------------------------------------------------------
isInserted
Is a card inserted?
return true if a card is inserted and usable
-----------------------------------------------------------------*/
bool dsxIsInserted (void) {
	//we should probably be doing a check here!
	return true;
}


/*-----------------------------------------------------------------
clearStatus
Reset the card, clearing any status errors
return  true if the card is idle and ready
-----------------------------------------------------------------*/
bool dsxClearStatus (void) {
	return true;
}


/*-----------------------------------------------------------------
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool dsxReadSectors (u32 sector, u32 numSectors, void* buffer) {
	unsigned char *buf8 = (unsigned char *)buffer;
	unsigned int j;
	unsigned int command[2];
	unsigned int lba = sector + 0x6000;

	REG_AUXSPICNT = CARD_ENABLE | CLOCK_4MHz | SPI_ROM;

	dsxPoll();

	for(j=0; j<numSectors; j++)
	{
		//put us in the right zone
		//NOTE: reads which cross zones are invalid.
		dsxZoneSwitch(lba);

		command[0] = 0xBF000000 | (lba>>8);
		command[1] = (lba<<24);

		dsxSendCommand(command, BYTES_PER_READ, 0x8f8, buf8);

		lba++;
		buf8 += BYTES_PER_READ;
	}

	return true;
}



/*-----------------------------------------------------------------
writeSectors
Write "numSectors" 512-byte sized sectors from "buffer" to the card, 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool dsxWriteSectors (u32 sector, u32 numSectors, void* buffer) {

	unsigned char *buf = (unsigned char*)buffer;
	unsigned int i, j;
	unsigned int command[2];
	const unsigned int writeResultSize = 4;
	unsigned int writeResult;
	unsigned int lba = sector + 0x6000;

	REG_AUXSPICNT = CARD_ENABLE | CLOCK_4MHz | SPI_ROM;

	//Don't interrupt the card if he's busy.. he gets grumpy.
	dsxPoll();


	for(j=0; j<numSectors; j++)
	{
		// Check if we are switching zones
		//Don't issue a write across a zone.
		//it's not possible to begin with
		//But... if you do find a way to do it... don't.
		dsxZoneSwitch(lba);

		//Reset fpga ram address
		command[0] = 0x03000000;
		command[1] = 0;

		//Go ahead, send it just once, see what happens. ;-)
		dsxSendCommand(command, 0, 0, (unsigned char*)&writeResult);
		dsxSendCommand(command, 0, 0, (unsigned char*)&writeResult);

		for(i=0; i<BYTES_PER_READ/4; i++)
		{
			command[0] = 0x04000000 | buf[1] | (buf[2] << 8) | (buf[3] << 16);
			command[1] = buf[0] << 24;
			buf += sizeof (unsigned int);
			dsxSendCommand(command, 0, 0, (unsigned char*)&writeResult);
		}

		// Commit page
		command[0] = 0x05000000 | (lba >> 8);
		command[1] = (lba << 24);
		dsxSendCommand(command, writeResultSize, 0xFFF, (unsigned char*)&writeResult);

		//Wait for it to actually write the data.
		dsxPoll();

		lba++;
	}

	// Finalize
	command[0] = 0xBC000000;
	command[1] = 0;
	dsxSendCommand(command, writeResultSize, 0xFFF, (unsigned char*)&writeResult);	

	//Now wait for it to ACTUALLY actually write the data.
	dsxPoll();

	//Now wait some more.
	//
	//If you're trying to optimize, and see this innocent looking, seemingly
	//senseless wait, and decide to remove it, or even lower it, you'll regret it.
	//
	//Seriously.
	//
	//I'm not kidding around here.
	//
	//Just leave it alone.
	//
	//
	dsxWaitMs(10);

	return true;
}

/*-----------------------------------------------------------------
shutdown
shutdown the card, performing any needed cleanup operations
-----------------------------------------------------------------*/
bool dsxShutdown(void) {

	//bye bye.
	return true;
}
