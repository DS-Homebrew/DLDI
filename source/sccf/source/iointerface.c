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

#include "io_cf_common.h"

//---------------------------------------------------------------
// CF Addresses
#define REG_SCCF_STS		((vu16*)0x098C0000)	// Status of the CF Card / Device control
#define REG_SCCF_CMD		((vu16*)0x090E0000)	// Commands sent to control chip and status return
#define REG_SCCF_ERR		((vu16*)0x09020000)	// Errors / Features

#define REG_SCCF_SEC		((vu16*)0x09040000)	// Number of sector to transfer
#define REG_SCCF_LBA1		((vu16*)0x09060000)	// 1st byte of sector address
#define REG_SCCF_LBA2		((vu16*)0x09080000)	// 2nd byte of sector address
#define REG_SCCF_LBA3		((vu16*)0x090A0000)	// 3rd byte of sector address
#define REG_SCCF_LBA4		((vu16*)0x090C0000)	// last nibble of sector address | 0xE0

#define REG_SCCF_DATA		((vu16*)0x09000000)	// Pointer to buffer of CF data transered from card

static const CF_REGISTERS _SCCF_Registers = {
	REG_SCCF_DATA,
	REG_SCCF_STS,
	REG_SCCF_CMD,
	REG_SCCF_ERR,
	REG_SCCF_SEC,
	REG_SCCF_LBA1,
	REG_SCCF_LBA2,
	REG_SCCF_LBA3,
	REG_SCCF_LBA4
};

/*-----------------------------------------------------------------
changeMode (was SC_Unlock)
Added by MightyMax
Modified by Chishm
Modified again by loopy
1=ram(readonly), 5=ram, 3=SD interface?
-----------------------------------------------------------------*/
#define SC_MODE_RAM 0x5
#define SC_MODE_MEDIA 0x3 
#define SC_MODE_RAM_RO 0x1
void changeMode(u8 mode) {
	vu16 *unlockAddress = (vu16*)0x09FFFFFE;
	*unlockAddress = 0xA55A ;
	*unlockAddress = 0xA55A ;
	*unlockAddress = mode ;
	*unlockAddress = mode ;
} 


/*-----------------------------------------------------------------
startUp
Initializes the CF interface
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
bool startup(void) {
	changeMode (SC_MODE_MEDIA);
	return _CF_startup(&_SCCF_Registers);
}

/*-----------------------------------------------------------------
isInserted
Is a compact flash card inserted?
bool return OUT:  true if a CF card is inserted
-----------------------------------------------------------------*/
bool isInserted (void) {
	return _CF_isInserted();
}


/*-----------------------------------------------------------------
clearStatus
Tries to make the CF card go back to idle mode
bool return OUT:  true if a CF card is idle
-----------------------------------------------------------------*/
bool clearStatus (void) {
	return _CF_clearStatus();
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
	return _CF_readSectors(sector, numSectors, buffer);
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
	return _CF_writeSectors(sector, numSectors, buffer);
}

/*-----------------------------------------------------------------
shutdown
shutdown the CF interface
-----------------------------------------------------------------*/
bool shutdown(void) {
	return _CF_shutdown();
}
