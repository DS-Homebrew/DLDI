/*
	gmtf.c -- DLDI driver for the Datel Game'n'Music card
	
 Copyright (c) 2007 Michael "Chishm" Chisholm
	
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

//---------------------------------------------------------------------------------
// The following were taken from libnds
#define	REG_CARD_DATA_RD	(*(vu32*)0x04100010)
#define REG_AUXSPICNT	(*(vu16*)0x040001A0)
#define REG_AUXSPICNTH	(*(vu8*)0x040001A1)
#define REG_AUXSPIDATA	(*(vu8*)0x040001A2)
#define REG_ROMCTRL		(*(vu32*)0x040001A4)
#define REG_CARD_COMMAND	((vu8*)0x040001A8)
#define CARD_SPICNTH_ENABLE  (1 << 7) // In byte 1, i.e. 0x8000
#define CARD_SPICNTH_IRQ     (1 << 6) // In byte 1, i.e. 0x4000
#define CARD_BUSY         (1<<31)           // when reading, still expecting incomming data?
#define CARD_ENABLE     BIT(15)
#define CARD_SPI_ENABLE BIT(13)
#define CARD_SPI_BUSY   BIT(7)
#define CARD_SPI_HOLD   BIT(6)
//---------------------------------------------------------------------------------

#define SD_COMMAND_TIMEOUT 0xFFF
#define SD_WRITE_TIMEOUT 0xFFFF
#define CARD_CR2_SETTINGS 0xA0586000

#define STOP_TRANSMISSION 12
#define READ_SINGLE_BLOCK 17
#define READ_MULTIPLE_BLOCK 18
#define WRITE_SINGLE_BLOCK 24
#define SD_WRITE_OK 0x05

static inline u8 transferSpiByte (u8 send)
{
	REG_SPIDATA = send;
	while (REG_AUXSPICNT & CARD_SPI_BUSY);
	return REG_SPIDATA;
}

static inline u8 getSpiByte (void) 
{
	REG_SPIDATA = 0xFF;
	while (REG_AUXSPICNT & CARD_SPI_BUSY);
	return REG_SPIDATA;
}

void openSpi (void) 
{
	volatile u32 temp;
	
	REG_AUXSPICNTH = CARD_SPICNTH_ENABLE | CARD_SPICNTH_IRQ;
	REG_CARD_COMMAND[0] = 0xF2;
	REG_CARD_COMMAND[1] = 0x00;
	REG_CARD_COMMAND[2] = 0x00;
	REG_CARD_COMMAND[3] = 0x00;
	REG_CARD_COMMAND[4] = 0x00;
	REG_CARD_COMMAND[5] = 0xCC;			// 0xCC == enable microSD ?
	REG_CARD_COMMAND[6] = 0x00;
	REG_CARD_COMMAND[7] = 0x00;
	REG_ROMCTRL = CARD_CR2_SETTINGS;

	while (REG_ROMCTRL & CARD_BUSY) {
		temp = REG_CARD_DATA_RD;
	}

	REG_AUXSPICNT = CARD_ENABLE | CARD_SPI_ENABLE | CARD_SPI_HOLD;
}

void closeSpi (void)
{
	volatile u32 temp;

	REG_AUXSPICNTH = CARD_SPICNTH_ENABLE | CARD_SPICNTH_IRQ;
	REG_CARD_COMMAND[0] = 0xF2;
	REG_CARD_COMMAND[1] = 0x00;
	REG_CARD_COMMAND[2] = 0x00;
	REG_CARD_COMMAND[3] = 0x00;
	REG_CARD_COMMAND[4] = 0x00;
	REG_CARD_COMMAND[5] = 0xC8;			// 0xCC == disable microSD ?
	REG_CARD_COMMAND[6] = 0x00;
	REG_CARD_COMMAND[7] = 0x00;
	REG_ROMCTRL = CARD_CR2_SETTINGS;

	while (REG_ROMCTRL & CARD_BUSY) {
		temp = REG_CARD_DATA_RD;
	}

	REG_AUXSPICNT = CARD_ENABLE | CARD_SPI_ENABLE | CARD_SPI_HOLD;
	REG_SPIDATA = 0xFF;
	
	while (REG_AUXSPICNT & CARD_SPI_BUSY) {};
}
	
u8 sendCommand (u8 command, u32 argument)
{
	u8 commandData[6];
	int timeout;
	u8 spiByte;
	int i;

	commandData[0] = command | 0x40; 
	commandData[1] = (u8)(argument >> 24);
	commandData[2] = (u8)(argument >> 16);
	commandData[3] = (u8)(argument >>  8);
	commandData[4] = (u8)(argument >>  0);
	commandData[5] = 0x95;	// Fake CRC, 0x95 is only important for SD CMD 0
	
	// Send read sector command
	for (i = 0; i < 6; i++) {
		REG_SPIDATA = commandData[i];
		while (REG_AUXSPICNT & CARD_SPI_BUSY);
	}

	// Wait for a response
	timeout = SD_COMMAND_TIMEOUT;
	do {
		spiByte = getSpiByte();
	} while (spiByte == 0xFF && --timeout > 0);
	
	return spiByte;
}

bool sdRead (u32 sector, u8* dest)
{
	u8 spiByte;
	int timeout;
	int i;
	volatile u32 temp;
	
	if (sendCommand (READ_SINGLE_BLOCK, sector * BYTES_PER_READ) != 0x00) {
		return false;
	}

	// Wait for data start token
	timeout = SD_COMMAND_TIMEOUT;
	do {
		spiByte = getSpiByte ();
	} while (spiByte == 0xFF && --timeout > 0);

	if (spiByte != 0xFE) {
		return false;
	}
	
	for (i = BYTES_PER_READ; i > 0; i--) {
		*dest++ = getSpiByte();
	}

	// Clean up CRC
	temp = getSpiByte();
	temp = getSpiByte();
	
	// Send 8 clocks for SD card
	getSpiByte ();
	
	return true;
}

bool sdReadMultiple (u32 sector, u8* dest, u32 numSectors)
{
	u8 spiByte;
	int timeout;
	int i;
	volatile u32 temp;
	
	if (sendCommand (READ_MULTIPLE_BLOCK, sector * BYTES_PER_READ) != 0x00) {
		return false;
	}

	while (numSectors > 0) {
		// Wait for data start token
		timeout = SD_COMMAND_TIMEOUT;
		do {
			spiByte = getSpiByte ();
		} while (spiByte == 0xFF && --timeout > 0);
	
		if (spiByte != 0xFE) {
			return false;
		}
		
		for (i = BYTES_PER_READ; i > 0; i--) {
			*dest++ = getSpiByte();
		}
	
		// Clean up CRC
		temp = getSpiByte();
		temp = getSpiByte();
		
		-- numSectors;
	}
	
	spiByte = sendCommand (STOP_TRANSMISSION, 0);

	// Wait for card to write data
	timeout = SD_COMMAND_TIMEOUT;
	while (spiByte == 0 && --timeout > 0) {
		spiByte = getSpiByte();
	}
	if (timeout == 0) {
		return false;
	}

	// Send 8 clocks for SD card
	getSpiByte ();
	
	return true;
}


bool sdWrite (u32 sector, u8* src)
{
	int i;
	int timeout;
	
	if (sendCommand (WRITE_SINGLE_BLOCK, sector * BYTES_PER_READ) != 0) {
		return false;
	}
	
	// Send start token
	transferSpiByte (0xFE);
	
	// Send data
	for (i = BYTES_PER_READ; i > 0; i--) {
		REG_SPIDATA = *src++;
		while (REG_AUXSPICNT & CARD_SPI_BUSY);
	}

	// Send fake CRC
	transferSpiByte (0xFF);
	transferSpiByte (0xFF);
	
	// Get data response
	if ((getSpiByte() & 0x0F) != SD_WRITE_OK) {
		return false;
	}
	
	// Wait for card to write data
	timeout = SD_WRITE_TIMEOUT;
	while (getSpiByte() == 0 && --timeout > 0);
	
	if (timeout == 0) {
		return false;
	}
	
	// Send 8 clocks for SD card
	getSpiByte ();

	return true;
}

/*-----------------------------------------------------------------
startUp
Initialize the interface, geting it into an idle, ready state
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
bool startup(void) {
	openSpi ();
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
bool readSectors (u32 sector, u32 numSectors, void* buffer) {
	u8* data = (u8*)buffer;
	
	if (numSectors == 1) {
		return sdRead (sector, data);
	} else {
		return sdReadMultiple (sector, data, numSectors);
	}
}



/*-----------------------------------------------------------------
writeSectors
Write "numSectors" 512-byte sized sectors from "buffer" to the card, 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool writeSectors (u32 sector, u32 numSectors, void* buffer) {
	u8* data = (u8*)buffer;
	
	while (numSectors > 0) {
		if (!sdWrite (sector, data)) {
			return false;
		}
		sector ++;
		data += BYTES_PER_READ;
		numSectors --;
	}
	
	return true;
}

/*-----------------------------------------------------------------
shutdown
shutdown the card, performing any needed cleanup operations
-----------------------------------------------------------------*/
bool shutdown(void) {
	closeSpi ();
	return true;
}
