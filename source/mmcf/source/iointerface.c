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

//---------------------------------------------------------------
// CF Addresses
#define REG_CF_STS		((vu16*)0x080E0000)	// Status of the CF Card / Device control
#define REG_CF_CMD		((vu16*)0x080E0000)	// Commands sent to control chip and status return
#define REG_CF_ERR		((vu16*)0x08020000)	// Errors / Features

#define REG_CF_SEC		((vu16*)0x08040000)	// Number of sector to transfer
#define REG_CF_LBA1		((vu16*)0x08060000)	// 1st byte of sector address
#define REG_CF_LBA2		((vu16*)0x08080000)	// 2nd byte of sector address
#define REG_CF_LBA3		((vu16*)0x080A0000)	// 3rd byte of sector address
#define REG_CF_LBA4		((vu16*)0x080C0000)	// last nibble of sector address | 0xE0

#define REG_CF_DATA		((vu16*)0x09000000)	// Pointer to buffer of CF data transered from card

// CF Card status
#define CF_STS_INSERTED		0x50
#define CF_STS_REMOVED		0x00
#define CF_STS_READY		0x58

#define CF_STS_DRQ			0x08
#define CF_STS_BUSY			0x80

// CF Card commands
#define CF_CMD_LBA			0xE0
#define CF_CMD_READ			0x20
#define CF_CMD_WRITE		0x30

#define CF_CARD_TIMEOUT	10000000

/*-----------------------------------------------------------------
startUp
Initializes the CF interface
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
bool startup(void) {
	// See if there is a read/write register
	u16 temp = *REG_CF_LBA1;
	*REG_CF_LBA1 = (~temp & 0xFF);
	temp = (~temp & 0xFF);
	if (!(*REG_CF_LBA1 == temp))return false;
	// Make sure it is 8 bit
	*REG_CF_LBA1 = 0xAA55;
	if (*REG_CF_LBA1 == 0xAA55)return false;
	return true;
}

/*-----------------------------------------------------------------
isInserted
Is a compact flash card inserted?
bool return OUT:  true if a CF card is inserted
-----------------------------------------------------------------*/
bool isInserted (void) {
	// Change register, then check if value did change
	*REG_CF_STS = CF_STS_INSERTED;
	return (*REG_CF_STS & CF_STS_INSERTED);
}


/*-----------------------------------------------------------------
readSectors
Read 512 byte sector numbered "sector" into "buffer"
u32 sector IN: address of first 512 byte sector on CF card to read
u32 numSectors IN: number of 512 byte sectors to read,
 1 to 256 sectors can be read
void* buffer OUT: pointer to 512 byte buffer to store data in
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool readSectors (u32 sector, u32 numSectors, void* buffer) {
	int i;

	u16 *buff = (u16*)buffer;
	u8 *buff_u8 = (u8*)buffer;
	int temp;

	// Wait until CF card is finished previous commands
	i=0;
	while ((*REG_CF_CMD & CF_STS_BUSY) && (i < CF_CARD_TIMEOUT)) { i++; }
	
	// Wait until card is ready for commands
	i = 0;
	while ((!(*REG_CF_STS & CF_STS_INSERTED)) && (i < CF_CARD_TIMEOUT)) { i++; }
	if (i >= CF_CARD_TIMEOUT)return false;
	
	// Set number of sectors to read
	*REG_CF_SEC = (numSectors < 256 ? numSectors : 0);	// Read a maximum of 256 sectors, 0 means 256
	
	// Set read sector
	*REG_CF_LBA1 = sector & 0xFF;						// 1st byte of sector number
	*REG_CF_LBA2 = (sector >> 8) & 0xFF;					// 2nd byte of sector number
	*REG_CF_LBA3 = (sector >> 16) & 0xFF;				// 3rd byte of sector number
	*REG_CF_LBA4 = ((sector >> 24) & 0x0F) | CF_CMD_LBA;	// last nibble of sector number
	
	// Set command to read
	*REG_CF_CMD = CF_CMD_READ;
	
	
	while (numSectors--) {
		// Wait until card is ready for reading
		i = 0;
		while (((*REG_CF_STS & 0xff)!= CF_STS_READY) && (i < CF_CARD_TIMEOUT)) { i++; }
		if (i >= CF_CARD_TIMEOUT)return false;
		// Read data
		i=256;
		if ((u32)buff_u8 & 0x01) {
			while(i--) {
				temp = *REG_CF_DATA;
				*buff_u8++ = temp & 0xFF;
				*buff_u8++ = temp >> 8;
			}
		} else {
			while(i--)*buff++ = *REG_CF_DATA; 
		}
	}
	return true;
}


/*-----------------------------------------------------------------
writeSectors
Write 512 byte sector numbered "sector" from "buffer"
u32 sector IN: address of 512 byte sector on CF card to read
u32 numSectors IN: number of 512 byte sectors to read,
 1 to 256 sectors can be read
void* buffer IN: pointer to 512 byte buffer to read data from
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool writeSectors (u32 sector, u32 numSectors, void* buffer) {
	int i;

	u16 *buff = (u16*)buffer;
	u8 *buff_u8 = (u8*)buffer;
	int temp;
	
	// Wait until CF card is finished previous commands
	i=0;
	while ((*REG_CF_CMD & CF_STS_BUSY) && (i < CF_CARD_TIMEOUT)) { i++; }
	
	// Wait until card is ready for commands
	i = 0;
	while ((!(*REG_CF_STS & CF_STS_INSERTED)) && (i < CF_CARD_TIMEOUT)) { i++; }
	if (i >= CF_CARD_TIMEOUT)return false;
	
	// Set number of sectors to write
	*REG_CF_SEC = (numSectors < 256 ? numSectors : 0);	// Write a maximum of 256 sectors, 0 means 256	
	
	// Set write sector
	*REG_CF_LBA1 = sector & 0xFF;							// 1st byte of sector number
	*REG_CF_LBA2 = (sector >> 8) & 0xFF;					// 2nd byte of sector number
	*REG_CF_LBA3 = (sector >> 16) & 0xFF;					// 3rd byte of sector number
	*REG_CF_LBA4 = ((sector >> 24) & 0x0F )| CF_CMD_LBA;	// last nibble of sector number
	
	// Set command to write
	*REG_CF_CMD = CF_CMD_WRITE;
	
	while (numSectors--) {
		// Wait until card is ready for writing
		i = 0;
		while (((*REG_CF_STS & 0xff) != CF_STS_READY) && (i < CF_CARD_TIMEOUT)) { i++; }
		if (i >= CF_CARD_TIMEOUT)return false;
		
		// Write data
		i=256;
		if ((u32)buff_u8 & 0x01) {
			while(i--) {
				temp = *buff_u8++;
				temp |= *buff_u8++ << 8;
				*REG_CF_DATA = temp;
			}
		} else {
			while(i--)*REG_CF_DATA = *buff++; 
		}
	}
	
	return true;
}


/*-----------------------------------------------------------------
clearStatus
Tries to make the CF card go back to idle mode
bool return OUT:  true if a CF card is idle
-----------------------------------------------------------------*/
bool clearStatus (void) {
	int i;
	
	// Wait until CF card is finished previous commands
	i=0;
	while ((*REG_CF_CMD & CF_STS_BUSY) && (i < CF_CARD_TIMEOUT)) { i++; }
	
	// Wait until card is ready for commands
	i = 0;
	while ((!(*REG_CF_STS & CF_STS_INSERTED)) && (i < CF_CARD_TIMEOUT)) { i++; }
	if (i >= CF_CARD_TIMEOUT)return false;

	return true;
}

/*-----------------------------------------------------------------
shutdown
shutdown the CF interface
-----------------------------------------------------------------*/
bool shutdown(void) { return clearStatus(); }

