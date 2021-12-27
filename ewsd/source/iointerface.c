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

// ------------

#include <nds.h>
#include <stdio.h>
#include "common.h"
#include "sdbasic.h"
#include "crc.h"
#include "misc.h"
#include "io_ewsd.h"

extern char testCode[8];

// shared RCA
u32 SdCardRCA;

unsigned char response[SD_BLOCKSIZE];
unsigned char argument[4];

//-----------------------------------------------------------

void spiWaitNotBusy ();


//============================================================
// Write a byte to SD card.
//============================================================

void sdmWrite(unsigned char  n)
{
	unsigned char i;

	for(i=8;i;i--)
	{
		*(vu16 *)(__ADDR_SD_CMD) =	(n&0x80) >> 7;
		n<<=1;
	}
}



unsigned char sdmRead(u32 clock)
{

	unsigned char n,i;

	n = 0;

	for(i=clock;i;i--)
	{
		n<<=1;
		if( SDB_SDM_GetResponse() /*SD_DO*/ )
			n|=1;

	}

	return n;
}




unsigned char sdmWaitStartBit()
{
	int i;
	unsigned char ret = 1;
	unsigned char m = 1;		//


	i = 0;

	do {

		i++;
		m = SDB_SDM_GetResponse();

	} while ( (m == 1) &&
		(i < MAXWAITCLOCK) );

	if (i<MAXWAITCLOCK)
		ret = 0;


	return ret;
}



//================================================================


unsigned int LSwapINT16(unsigned short dData1,unsigned short dData2)
{
    unsigned int  dData;
    dData = ((dData2<<8)&0xff00)|(dData1&0x00ff);
	return dData;
}

unsigned long LSwapINT32(unsigned long dData1,unsigned long dData2,unsigned long dData3,unsigned long dData4)
{
    unsigned long  dData;
    dData = ((dData4<<24)&0xff000000)|((dData3<<16)&0xff0000)|((dData2<<8)&0xff00)|(dData1&0xff);
	return dData;
}

void sdDelay(char number)
{
	char i; /* Null for now */

	for (i=0; i<number; i++)
	{

		/* Clock out an idle byte (0xFF) */
		sdmRead(8);
	}
}



void sdPackArg(unsigned char *argument, u32 value)
{
	argument[3] = (unsigned char)(value >> 24);
	argument[2] = (unsigned char)(value >> 16);
	argument[1] = (unsigned char)(value >> 8);
	argument[0] = (unsigned char)(value);
}



/*
	return
	1: command success and response returned
	0: fail

*/

void sdmWriteCmd( unsigned char cmd,
					  unsigned char *argument)
{
	int i;
	int j = 1;
	u8 crc7;

	/* All data is sent MSB first, and MSb first */
	/* Send the header/command */
	/*	Format:
	cmd[7:6] : 01
	cmd[5:0] : command */

	u8 crc_buff[5];
	crc_buff[0] = (cmd & 0x3F) | 0x40;


	sdmWrite(crc_buff[0]);


	for (i=3; i>=0; i--)
	{
		crc_buff[j] = argument[i];
		j++;
		sdmWrite(argument[i]);
	}
	/* This is the CRC. It only matters what we put here for the first
	command. Otherwise, the CRC is ignored for SPI mode unless we
	enable CRC checking. */

	crc7 = sdCRC7L(crc_buff, 5, 0);

	// append a end bit "1" and make a byte
	crc7 = crc7 * 2 + 1;

	sdmWrite(crc7);
}

int sdmSendCommand( unsigned char cmd,
					  unsigned char response_type,
					  unsigned char *response,
					  unsigned char *argument)
{
	int i;
	int response_length;
	unsigned char tmp;

	int j = 1;


	sdmWriteCmd( cmd,
		argument );

	if (response_type == Rx) // no response
	{
		sdDelay(2);  // drive 8 clocks
		return 1;
	}

	// wait start bit

	/* Wait for a response. A response can be recognized by the
	start bit (a zero) */

	j=0;
	if (sdmWaitStartBit() == 0 )
	{

		// waitbit (0) found, read next 7bits and make the first byte
		tmp = sdmRead(7);
		response[j] = tmp;  // the first byte
		j++;

		response_length = 48 / 8 - 1;  // next 40 bits r1
		switch (response_type)
		{
		case R1:
		case R1B:

			response_length = 48 / 8 - 1;
			break;
		case R2:

			response_length = 136 / 8 - 1;
			break;

		case R3:
		case R6:
			response_length = 48 / 8  - 1;
			break;


		default:
			break;
		}

		for (i = 0; i<response_length; i++ )
		{
			tmp = sdmRead(8);
			response[j] = tmp;  // the first byte
			j++;

		}



	}
	else
	{
		// start bit not found!
		return 0; // command fails
	};


	/* If the response is a "busy" type (R1B), then there¡¯s some
	* special handling that needs to be done. The card will
	* output a continuous stream of zeros, so the end of the BUSY
	* state is signaled by any nonzero response. The bus idles
	* high.
	*/

	i=0;
	if (response_type == R1B)
	{

		do
		{
			i++;
			tmp = sdmRead(8);

		}
		/* This should never time out, unless SDI is grounded.
		* Don¡¯t bother forcing a timeout condition here. */
		while (tmp != 0xFF);

	}

	sdDelay(2);   // 8 clocks


	return 1;
}



int sdmInitialize()
{
  return 1;
}

extern void sdmFastReadData512(u16 *dst);


// C code
void sdmFastReadData512C(u16 *dst)
{
	u32 loopcounter = SD_BLOCKSIZE/sizeof(u16);
	u32 r0, r1;
	u32 i;

	// test code
	for (i=0; i<loopcounter; i++)
	{

		r0 = *(vu32 *)(__ADDR_SD_DATA);
		r1 = *(vu32 *)(__ADDR_SD_DATA);

		dst[i] = r0 >> 16;

	}

}


// after initialize
// in tran mode, buswidth = 4
// note the block address parameter
// block size = SD_BLOCKSIZE [512B]

int sdmReadBlock( u32 blockaddr,
	unsigned char *data)
{
	unsigned long int i = 0;
	unsigned char tmp;

	/* Adjust the block address to a linear address */
	blockaddr <<= SD_BLOCKSIZE_NBITS;


	/* Pack the address */
	sdPackArg(argument, blockaddr);


	// try response first
	//sdmSendCommand( CMD17, CMD17_R, response, argument);
	/* Need to add size checking */
	sdmWriteCmd( CMD17,  argument);

	do
	{
		tmp =  *(vu16 *)(__ADDR_SD_DATA);
		tmp &= 0xF; // 4 bits


	} while (tmp != 0 && i < SD_INIT_TRY) ;

	if (i == SD_INIT_TRY)
		return SD_ERROR;

	u16 *pfast16 = (u16 *)data;



//------------------------------------------------------
	// test code
	sdmFastReadData512(pfast16);


	// send more clocks
	sdDelay(2);   // 8 clocks

	return SD_SUCCESS;


}
/*
	03 2d 06 08 be0 e9 17 43 0f 55 aa
*/

//--------------------------------------------------------

int sdmWriteBlock( u32 blockaddr,
	unsigned char *data)
{
	unsigned long int i = 0;


	/* Adjust the block address to a linear address */
	blockaddr <<= SD_BLOCKSIZE_NBITS;


	/* Pack the address */
	sdPackArg(argument, blockaddr);


	// write single block
	sdmSendCommand( CMD24, CMD24_R, response, argument);

		// start bit
//	SDB_SDM_SetDataOut(0);
	*(vu16 *)(__ADDR_SD_DATA) = 0;


	for (i = 0; i<SD_BLOCKSIZE; i++)	// write one block 512 byte
	{
//		SDB_SDM_SetDataOut(data[i]>>4);
//		SDB_SDM_SetDataOut(	data[i]);

		*(vu16 *)(__ADDR_SD_DATA) = data[i]>>4;
		*(vu16 *)(__ADDR_SD_DATA) = data[i];


	}

	// write crc16
	{

		u32 k;
		u16 cal_crc[5]; // 4 dataline
		u8* pcrc = (u8*)cal_crc;

		sdCRC16X(data , SD_BLOCKSIZE, (u8*)cal_crc);

		for (k=0; k<8; k++)
		{

//			SDB_SDM_SetDataOut(	pcrc[k] >> 4);
//			SDB_SDM_SetDataOut(	pcrc[k]);
			*(vu16 *)(__ADDR_SD_DATA) = pcrc[k] >> 4;
			*(vu16 *)(__ADDR_SD_DATA) = pcrc[k];
		}

		// end bit
//		SDB_SDM_SetDataOut(0xf);
		*(vu16 *)(__ADDR_SD_DATA) = 0xf;

		// Get CRC status

		BYTE t = 1;
		do {
//			t = SDB_SDM_GetDataOut() & 1; // dat0
			t = *(vu16 *)(__ADDR_SD_DATA);
			t &= 1;

		} while (t);   //wait start bit, t=0 break
/*
		t = SDB_SDM_GetDataOut();
		t = SDB_SDM_GetDataOut();
		t = SDB_SDM_GetDataOut(); // 3bit status

		t = SDB_SDM_GetDataOut(); // e
		t = SDB_SDM_GetDataOut(); // s
*/

		t = *(vu16 *)(__ADDR_SD_DATA);
		t = *(vu16 *)(__ADDR_SD_DATA);
		t = *(vu16 *)(__ADDR_SD_DATA);
		t = *(vu16 *)(__ADDR_SD_DATA);
		t = *(vu16 *)(__ADDR_SD_DATA);

		// program busy
		t = 0;
		do {
			// t = SDB_SDM_GetDataOut() & 1; // dat0
			t = *(vu16 *)(__ADDR_SD_DATA);
			t &= 1;

		} while (t == 0);   //check busy bit, t=1 break

	}

	// more read clocks
	sdDelay(2);   // 8 clocks

	return SD_SUCCESS;

}

//------------------------------------------------------
// io interface
//------------------------------------------------------

#define MAXTRANS_SECTORS 1

/*-----------------------------------------------------------------
startUp
Initialize the interface, geting it into an idle, ready state
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
extern bool isInserted (void);
bool startup(void) {
	u32 ok = 0;
	u32 j;

	SDB_SetEwinSD();
	for (j=0; j<SD_INIT_TRY && ok != 1; j++)
	{
		ok = sdmInitialize();
	}

	if (ok == 0 )
		return false;



	return isInserted();
}

/*-----------------------------------------------------------------
isInserted
Is a card inserted?
return true if a card is inserted and usable
-----------------------------------------------------------------*/
bool isInserted (void) {
	u8 checkCode = SDB_SDM_CheckCard();

// support SD/miniSD/microSD
//	if (checkCode & 4)
//		return false;

	if (checkCode & 2)
		return false;

	return true;
}


/*-----------------------------------------------------------------
clearStatus
Reset the card, clearing any status errors
return  true if the card is idle and ready
-----------------------------------------------------------------*/
bool clearStatus (void) {
	return false;
}


/*-----------------------------------------------------------------
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool readSectors (u32 sector, u32 numSectors, void* buffer) {
	u8 *p = (u8*)buffer;

	if (numSectors == 0)
		numSectors = MAXTRANS_SECTORS;
	while (numSectors--)
	{
		if (sdmReadBlock( sector, p) == SD_ERROR)
			return false;
		sector++;
		p += SD_BLOCKSIZE;
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
	u8 *p = (u8*)buffer;

	if (numSectors == 0)
		numSectors = MAXTRANS_SECTORS;
	while (numSectors--)
	{
		if (sdmWriteBlock( sector, p) == SD_ERROR)
			return false;
		sector++;
		p += SD_BLOCKSIZE;
	}

	return true;
}

/*-----------------------------------------------------------------
shutdown
shutdown the card, performing any needed cleanup operations
-----------------------------------------------------------------*/
bool shutdown(void) {
	return clearStatus() ;
}

