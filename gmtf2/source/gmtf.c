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

#ifndef NDS
#error "This is a DS only card"
#endif
 
#include <nds/jtypes.h>

#define BYTES_PER_SECTOR 0x200

#ifndef NULL
 #define NULL 0
#endif

//---------------------------------------------------------------------------------
// The following were taken from libnds
#define CARD_CR1       (*(vuint16*)0x040001A0)
#define CARD_CR1H      (*(vuint8*)0x040001A1)
#define CARD_EEPDATA   (*(vuint8*)0x040001A2)
#define CARD_CR2       (*(vuint32*)0x040001A4)
#define CARD_COMMAND   ((vuint8*)0x040001A8)
#define CARD_DATA_RD   (*(vuint32*)0x04100010)
#define CARD_CR1_ENABLE  0x80  // in byte 1, i.e. 0x8000
#define CARD_CR1_IRQ     0x40  // in byte 1, i.e. 0x4000
#define CARD_BUSY       (1<<31)  // when reading, still expecting incomming data?
#define CARD_SPI_BUSY 0x80
#define CARD_CR1_EN	0x8000
#define	CARD_CR1_SPI_EN	0x2000
#define	CARD_CR1_SPI_HOLD	0x40
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
	CARD_EEPDATA = send;
	while (CARD_CR1 & CARD_SPI_BUSY);
	return CARD_EEPDATA;
}

static inline u8 getSpiByte (void) 
{
	CARD_EEPDATA = 0xFF;
	while (CARD_CR1 & CARD_SPI_BUSY);
	return CARD_EEPDATA;
}

void openSpi (void) 
{
	volatile u32 temp;
	
	CARD_CR1H = CARD_CR1_ENABLE | CARD_CR1_IRQ;
	CARD_COMMAND[0] = 0xF2;
	CARD_COMMAND[1] = 0x00;
	CARD_COMMAND[2] = 0x00;
	CARD_COMMAND[3] = 0x00;
	CARD_COMMAND[4] = 0x00;
	CARD_COMMAND[5] = 0xCC;			// 0xCC == enable microSD ?
	CARD_COMMAND[6] = 0x00;
	CARD_COMMAND[7] = 0x00;
	CARD_CR2 = CARD_CR2_SETTINGS;

	while (CARD_CR2 & CARD_BUSY) {
		temp = CARD_DATA_RD;
	}

	CARD_CR1 = CARD_CR1_EN | CARD_CR1_SPI_EN | CARD_CR1_SPI_HOLD;
}

void closeSpi (void)
{
	volatile u32 temp;

	CARD_CR1H = CARD_CR1_ENABLE | CARD_CR1_IRQ;
	CARD_COMMAND[0] = 0xF2;
	CARD_COMMAND[1] = 0x00;
	CARD_COMMAND[2] = 0x00;
	CARD_COMMAND[3] = 0x00;
	CARD_COMMAND[4] = 0x00;
	CARD_COMMAND[5] = 0xC8;			// 0xCC == disable microSD ?
	CARD_COMMAND[6] = 0x00;
	CARD_COMMAND[7] = 0x00;
	CARD_CR2 = CARD_CR2_SETTINGS;

	while (CARD_CR2 & CARD_BUSY) {
		temp = CARD_DATA_RD;
	}

	CARD_CR1 = CARD_CR1_EN | CARD_CR1_SPI_EN | CARD_CR1_SPI_HOLD;
	CARD_EEPDATA = 0xFF;
	
	while (CARD_CR1 & CARD_SPI_BUSY) {};
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
		CARD_EEPDATA = commandData[i];
		while (CARD_CR1 & CARD_SPI_BUSY);
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
	
	if (sendCommand (READ_SINGLE_BLOCK, sector * BYTES_PER_SECTOR) != 0x00) {
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
	
	for (i = BYTES_PER_SECTOR; i > 0; i--) {
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
	
	if (sendCommand (READ_MULTIPLE_BLOCK, sector * BYTES_PER_SECTOR) != 0x00) {
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
		
		for (i = BYTES_PER_SECTOR; i > 0; i--) {
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
	
	if (sendCommand (WRITE_SINGLE_BLOCK, sector * BYTES_PER_SECTOR) != 0) {
		return false;
	}
	
	// Send start token
	transferSpiByte (0xFE);
	
	// Send data
	for (i = BYTES_PER_SECTOR; i > 0; i--) {
		CARD_EEPDATA = *src++;
		while (CARD_CR1 & CARD_SPI_BUSY);
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
		data += BYTES_PER_SECTOR;
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
