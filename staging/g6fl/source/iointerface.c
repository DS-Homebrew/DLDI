/*
	iointerface.c for G6 flash card

Written by Viruseb (viruseb@hotmail.com)
	Many thanks to Puyo for his help in the reading code and ecc generation
	and Theli for remote debbuging.

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
 
If you wish to understand how the code works, I encourage you reading 
the datasheet of K9K4G08U0M nand flash device from Samsung before.

Just some figures to keep in mind :
	1 page  = 4 sectors + 64byte
	1 block = 64 pages = 256 sectors
	1 4G device = 4096 blocks

The spare 64byte in page are use :
	- to store the ECC. There is 3bytes ecc per 256bytes (bytes 8...15+numsector*16).
	- to store lookuptable values (bytes 4..7).
	
04/12/06 : Version 0.10
	Just freshly written. Not tested on real G6L thought. 
	Extreme caution have to be taken when testing WriteBlockFunction 
	since it may brick your G6L or more likely corrupt your data 
	and formating can be necessary.
	
05/12/06 : Version 0.11
	Thank to Theli, a lot of stupid mistakes removed, a lot of debugging done.
	Reading code checked against Puyo's code. 
	Device and FAT table is recognised by Fat_InitFiles()
	Known issues : DMA read (G6_ReadDMA) is malfunctionning
				Strange things append when trying to read more than 1 sectors at a time
				Have to check LookUpTable values against Puyo's LookUpTable they seems differents after 512values 
	
19/12/06 : Version 0.12
	Reading code ok
	
20/12/06 : Version 0.13
	Some reading bugs corrected
	
07/01/07 : Version 0.14
	Writing code finaly working. Need some optimizations.
	
10/01/07 : Version 0.15
	Code cleaning. Need to add DMA R/W and use of cache programming in later version. 
	
03/02/07 : Version 0.16
	Unaligned R/W supported.
	Write code rewritten, using cache programming now.

04/03/07 : Version 0.17
	VerifyBlockFunc corrected (no more in use now)
	
23/03/07 : Version 0.18
	a bug corrected in make_ecc_256
	
25/03/07 : Version 0.19
	Improved writing speed
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

/*****************************************************************************/
/*						G6 IO FUNCTIONS					    */
/*****************************************************************************/
// G6 global variables

// LookUpTable : Corespondance between logical block number and physical block number
// The flash device is divided in 512-block region that I call 'superblock'
// In each superblock 500 blocks can be used the 12 reminding form a fifo. 
// Each time you write something a new block is taken from the fifo, data copied from the old block to the new block,
// your data is overwritten into the new block and the old block is placed in the fifo. 
// (That's why your 4G device is 500Mo and not 512Mo) 
// This can prevent writing again and again on the same block and thus increase flash life
#define NUMMAXSUPERBLOCK 16 // 16 for a 8G device
u16 LookUpTable[500*NUMMAXSUPERBLOCK];
u16 FreeFifo[12*NUMMAXSUPERBLOCK];
u16 NbFree[NUMMAXSUPERBLOCK];
u16 CurFree[NUMMAXSUPERBLOCK];

// NandFlashID : Give us the G6 type inside NDS (1G,2G...)
// 8 = 1G
// 9 = 2G
// 10 = 4G
// 11 = 8G
u16 NandFlashID;
u8  FlashIDTable[4] = {0xF1,0xDA,0xDC,0xD3};

// Some definitions
#define DMA3_CR_L   (*(vu16*)0x040000DC)
#define DMA3_CR_H	(*(vu16*)0x040000DE)

// G6 adresses and commands
#define WriteWordData(data) 	(*(volatile u16*)0x09F04468) = data		// Data to be written in flash
#define ReadWordData() 			(*(volatile u16*)0x09F02468)			// Data read from flash
#define ReadStatus() 			((*(volatile u16*)0x09F0330E)&0xC000)	// Status line of the flash
#define WriteByteAddress(adr) 	(*(volatile u16*)0x09F04044) = (adr)&0xFF	// Write an adresse into the flash
#define WriteByteCommand(cmd) 	(*(volatile u16*)0x09F04000) = (cmd)&0xFF	// Write a flash command

// Helping marco
#define ReverseU32(val) ((val<<24) | ((val<<8)&0xFF0000) | ((val>>8)&0xFF00) | (val>>24))


/*-----------------------------------------------------------------
fakememset
dldi doesn't allow us to use every function we want
-----------------------------------------------------------------*/
void fakememset(void* _buffer,u8 data,u32 nb8b)
{
	u16 dat16 = data | (data<<8);
	u16* buffer = (u16*)_buffer;
	u32 i;
	
	for (i=0;i<(nb8b>>1);i++)
		buffer[i] = dat16;
}

/*-----------------------------------------------------------------
make_ecc_256
Caclulate 3 bytes ECC per 256bytes.
This function is taken from samsung_new_ecc_for_256byte.c from 
Samsung. 
u8 * ecc buf IN : location of the calculated ECC
u16* databuf IN : data
-----------------------------------------------------------------*/
u32 make_ecc_256(u16* data16)
{
	
    u32	i;
	u32	tmp;
	u32	uiparity = 0;
	u32	parityCol, ecc = 0;
	u32	parityCol4321 = 0, parityCol4343 = 0, parityCol4242 = 0, parityColTot = 0;
	u32* data32 = (u32*)(data16);
	u8*  data8  = (u8*)(data16);
	u32	Xorbit=0;

	for( i = 0; i < 8; i++)
	{
		// data16 32b aligned  ?
		if (((u32)data16&3)==0) {
			parityCol = *data32++; 
			tmp = *data32++; parityCol ^= tmp; parityCol4242 ^= tmp;
			tmp = *data32++; parityCol ^= tmp; parityCol4343 ^= tmp;
			tmp = *data32++; parityCol ^= tmp; parityCol4343 ^= tmp; parityCol4242 ^= tmp;
			tmp = *data32++; parityCol ^= tmp; parityCol4321 ^= tmp;
			tmp = *data32++; parityCol ^= tmp; parityCol4242 ^= tmp; parityCol4321 ^= tmp;
			tmp = *data32++; parityCol ^= tmp; parityCol4343 ^= tmp; parityCol4321 ^= tmp;
			tmp = *data32++; parityCol ^= tmp; parityCol4242 ^= tmp; parityCol4343 ^= tmp; parityCol4321 ^= tmp;
		} else if (((u32)data16&1)==0) { // data16 16b aligned ?
			parityCol = data16[0] + (data16[1]<<8); 
			data16 += 2;
			tmp = data16[0] + (data16[1]<<8); parityCol ^= tmp; parityCol4242 ^= tmp;
			data16 += 2;
			tmp = data16[0] + (data16[1]<<8); parityCol ^= tmp; parityCol4343 ^= tmp;
			data16 += 2;
			tmp = data16[0] + (data16[1]<<8); parityCol ^= tmp; parityCol4343 ^= tmp; parityCol4242 ^= tmp;
			data16 += 2;
			tmp = data16[0] + (data16[1]<<8); parityCol ^= tmp; parityCol4321 ^= tmp;
			data16 += 2;
			tmp = data16[0] + (data16[1]<<8); parityCol ^= tmp; parityCol4242 ^= tmp; parityCol4321 ^= tmp;
			data16 += 2;
			tmp = data16[0] + (data16[1]<<8); parityCol ^= tmp; parityCol4343 ^= tmp; parityCol4321 ^= tmp;
			data16 += 2;
			tmp = data16[0] + (data16[1]<<8); parityCol ^= tmp; parityCol4242 ^= tmp; parityCol4343 ^= tmp; parityCol4321 ^= tmp;
			data16 += 2;
		} else { //data16 is 8bit aligned
			parityCol = data8[0] + (data8[1]<<8) + (data8[2]<<16) + (data8[3]<<24); 
			data8 += 4;
			tmp = data8[0] + (data8[1]<<8) + (data8[2]<<16) + (data8[3]<<24);  parityCol ^= tmp; parityCol4242 ^= tmp;
			data8 += 4;
			tmp = data8[0] + (data8[1]<<8) + (data8[2]<<16) + (data8[3]<<24);  parityCol ^= tmp; parityCol4343 ^= tmp;
			data8 += 4;
			tmp = data8[0] + (data8[1]<<8) + (data8[2]<<16) + (data8[3]<<24);  parityCol ^= tmp; parityCol4343 ^= tmp; parityCol4242 ^= tmp;
			data8 += 4;
			tmp = data8[0] + (data8[1]<<8) + (data8[2]<<16) + (data8[3]<<24);  parityCol ^= tmp; parityCol4321 ^= tmp;
			data8 += 4;
			tmp = data8[0] + (data8[1]<<8) + (data8[2]<<16) + (data8[3]<<24);  parityCol ^= tmp; parityCol4242 ^= tmp; parityCol4321 ^= tmp;
			data8 += 4;
			tmp = data8[0] + (data8[1]<<8) + (data8[2]<<16) + (data8[3]<<24);  parityCol ^= tmp; parityCol4343 ^= tmp; parityCol4321 ^= tmp;
			data8 += 4;
			tmp = data8[0] + (data8[1]<<8) + (data8[2]<<16) + (data8[3]<<24);  parityCol ^= tmp; parityCol4242 ^= tmp; parityCol4343 ^= tmp; parityCol4321 ^= tmp;
			data8 += 4;
		}
		

		parityColTot ^= parityCol;

		tmp = (parityCol >> 16) ^ parityCol;
		tmp = (tmp >> 8) ^ tmp;
		tmp = (tmp >> 4) ^ tmp;
		tmp = ((tmp >> 2) ^ tmp) & 0x03;
		if ((tmp == 0x01) || (tmp == 0x02))
		{
			uiparity ^= i;
			Xorbit ^= 0x01;
		}
	}

	tmp = (parityCol4321 >> 16) ^ parityCol4321;
	tmp = (tmp << 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x200;	// p128

	tmp = (parityCol4343 >> 16) ^ parityCol4343;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x80;	// p64

	tmp = (parityCol4242 >> 16) ^ parityCol4242;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x20;	// p32

	tmp = parityColTot & 0xFFFF0000;
	tmp = tmp >> 16;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x08;	// p16

	tmp = parityColTot & 0xFF00FF00;
	tmp = (tmp >> 16) ^ tmp;
	tmp = (tmp >> 8);
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp >> 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x02;	// p8

	tmp = parityColTot & 0xF0F0F0F0 ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= ((tmp << 1) ^ tmp) & 0x800000;	// p4

	tmp = parityColTot & 0xCCCCCCCC ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp << 4) ^ tmp;
	tmp = (tmp >> 2);
	ecc |= ((tmp << 1) ^ tmp) & 0x200000;	// p2

	tmp = parityColTot & 0xAAAAAAAA ;
	tmp = (tmp << 16) ^ tmp;
	tmp = (tmp >> 8) ^ tmp;
	tmp = (tmp >> 4) ^ tmp;
	tmp = (tmp << 2) ^ tmp;
	ecc |= (tmp & 0x80000);	// p1

	ecc |= (uiparity & 0x01) <<11;	// parit256_1
	ecc |= (uiparity & 0x02) <<12;	// parit512_1
	ecc |= (uiparity & 0x04) <<13;	// parit1024_1

	if (Xorbit)
	{
		ecc |= (ecc ^ 0x00A8AAAA)>>1;
	}
	else
	{
		ecc |= (ecc >> 1);
	}

	ecc = ~ecc;
	return ecc;
}


/*-----------------------------------------------------------------
SelectOperation
?? 
u16 op IN : Operation to select
-----------------------------------------------------------------*/
u16 static SelectOperation(u16 op)
{
	u16 i;
	
	i = *(volatile u16*)0x09000000;
	i = *(volatile u16*)0x09FFFFE0;
	
	i = *(volatile u16*)0x09FFFFEC;
	i = *(volatile u16*)0x09FFFFEC;
	i = *(volatile u16*)0x09FFFFEC;
	
	i = *(volatile u16*)0x09FFFFFC;
	i = *(volatile u16*)0x09FFFFFC;
	i = *(volatile u16*)0x09FFFFFC;
	
	i = *(volatile u16*)0x09FFFF4A;
	i = *(volatile u16*)0x09FFFF4A;
	i = *(volatile u16*)0x09FFFF4A;

	i = *(volatile u16*)(0x09200000 + (op<<1));
	i = *(volatile u16*)0x09FFFFF0;
	i = *(volatile u16*)0x09FFFFE8;

	return 1;
}

/*-----------------------------------------------------------------
WriteProtectEnable
Enable the write protect of the flash device
-----------------------------------------------------------------*/
void static inline WriteProtectEnable()
{
	(*(volatile u16*)0x080000BC) = 0x5A;
	(*(volatile u16*)0x09F40040) = 0x6D;
	(*(volatile u16*)0x09F40042) = 0x7C;
	(*(volatile u16*)0x09F05244) = 0x01;
}

/*-----------------------------------------------------------------
WriteProtectDisable
Disable the write protect of the flash device
-----------------------------------------------------------------*/
void static inline WriteProtectDisable()
{
	(*(volatile u16*)0x080000BC) = 0x5A;
	(*(volatile u16*)0x09F40040) = 0x6D;
	(*(volatile u16*)0x09F40042) = 0x7C;
	(*(volatile u16*)0x09F05244) = 0x00;
}

/*-----------------------------------------------------------------
ChipEnable
?? 
-----------------------------------------------------------------*/
void static inline ChipEnable()
{
	(*(volatile u16*)0x080000BC) = 0x5A;
	(*(volatile u16*)0x09F40020) = 0x6D;
	(*(volatile u16*)0x09F40022) = 0x7C;
	(*(volatile u16*)0x09F05222) = 0x00;
}

/*-----------------------------------------------------------------
ChipEnable
?? 
-----------------------------------------------------------------*/
void static inline ChipDisable()
{
	(*(volatile u16*)0x080000BC) = 0x5A;
	(*(volatile u16*)0x09F40020) = 0x6D;
	(*(volatile u16*)0x09F40022) = 0x7C;
	(*(volatile u16*)0x09F05222) = 0x01;
}

/*-----------------------------------------------------------------
SelectRAM
?? Enable the RAM chip inside the G6
bool return OUT : always true
-----------------------------------------------------------------*/
bool static inline SelectRAM()
{
	return SelectOperation(6);
}

/*-----------------------------------------------------------------
SelectRAM
?? Disable the RAM chip inside the G6
bool return OUT : always true
-----------------------------------------------------------------*/
bool static inline DisableRAM()
{
	WriteProtectDisable();
	ChipDisable();
	SelectOperation(2);
	return true;
}

/*-----------------------------------------------------------------
SelectNand
?? Enable the nand flashchip inside the G6
bool return OUT : always true
-----------------------------------------------------------------*/
bool static inline SelectNand()
{
	SelectOperation(3);
	ChipEnable();
	return true;
}

/*-----------------------------------------------------------------
Write32bAdr
Write the adresse
u32 adr IN : addresse to write
-----------------------------------------------------------------*/
void Write32bAdr(u32 adr) 
{
	WriteByteAddress(0);					
	WriteByteAddress(adr);		// first byte	
	WriteByteAddress(adr>>8); 	// second byte	
	WriteByteAddress(adr>>16);	// third byte	
	// If flash is big there is a 5th adress send
	if (NandFlashID > 8) 
		WriteByteAddress(adr>>24);
}

/*-----------------------------------------------------------------
SectorToPhyAddr
Return the physical adresse of the sector
u32 sector IN: Sector number
u16* lookupindex IN : Index in LookUpTable of the sector
u32	return OUT : Physical adress of the sector divided by 256
					bit 0 = 0
					bit 1..2 = s0..1 (sector number in the page)
					bit 3..7 = 0 (DC in flash datashet)
					bit 8..13 = s2..7 (page number in the block)
					bit 14... = physical block number
-----------------------------------------------------------------*/
u32 static inline SectorToPhyAddr(u32 sector, u16 *lookupindex)
{
	u16 secblocknb;
	
	*lookupindex = (sector>>8);
	secblocknb = LookUpTable[*lookupindex];
	
	// Form the physical adresse 
	return ((sector&3)<<1) + (secblocknb<<12) + ((sector<<6)&0x3F00);
}

/*-----------------------------------------------------------------
TestInvalidBlock
Basicaly we test if numblk have more than one bit set 
between the 16ith to the 23ith bit 
u32 numblock IN : Block number to be tested
bool return OUT : 1=ok 0=ko
-----------------------------------------------------------------*/
bool static inline TestInvalidBlock(u32 numblk)
{
	u8 j;
	u32 testnumblk;
	
	j = 0;
	for (testnumblk=0x10000;testnumblk==0x1000000;testnumblk = testnumblk*2) {	
		if ((numblk & testnumblk)==0)
			j++;
	}
	
	if (j > 1)
		return false;
	else
		return true;
}

/*-----------------------------------------------------------------
ReadRBStatus
Wait for the status line of the flash
bool return OUT : 1=ok 0=timeout
-----------------------------------------------------------------*/
bool static inline ReadRBStatus()
{
	int i,timeout;
	
	// A little delay.
	for (i=0;i<10;i++);
	
	// Test the RB line of the flash device for about xxms (have to calculate)
	timeout = 0x07A120;
	for (i=0;i < timeout;i++) 
		if (ReadStatus() == 0xC000)
			return true;
	return false;
}

/*-----------------------------------------------------------------
ReadNandFlashStatus
Wait for the end of the command and read its status
int return OUT : 	0 = ok
					1 = command failed
					2 = flash write protected
					-1= command timeout
-----------------------------------------------------------------*/
int static inline ReadNandFlashStatus()
{
	u16 val;
	
	// Wait for the completion of the command
	if (!ReadRBStatus())
		return -1;

	// Send the Read status command to the flash
	WriteByteCommand(0x70);
	// Get the response of the read status command
	val = ReadWordData() & 0xFF;
	// Reset command
	WriteByteCommand(0xFF);
	
	// Test for different status value
	// check if the command is finish and the flash is not write protected
	if (val == 0xE0)
		return 0;
	// Check for the pass/fail bit (bit0)
	if ((val&1)==1)
		return 1;
	// ckeck if it's write protected	
	if (val == 0x60)
		return 2;
	else
		return val&0xFF;
	
	return 0;	
}

/*-----------------------------------------------------------------
DMAReadWordData
Read data from flash. As the name of the functions don't explain,
we don't use the DMA but rather direct transfer.
u16* buffer IN : buffer 
u16 nbword IN : Number of word (16b) to be transferred
-----------------------------------------------------------------*/
void static DMAReadWordData(u16 buffer[],u32 nbword)
{
	u32 i; 
	u16 word;
	u8* buf_8 = (u8*)buffer;
	
	// adresse buffer is 16bit aligned  ?
	if (((u32)buffer&1)!=0) { 
		if (nbword >= 1)
			for (i=0;i<nbword;i++) {
				word = ReadWordData();
				buf_8[2*i] = word;
				buf_8[2*i+1] = word>>8;
			}
	} else {
		// Copy all word to the buffer
		for (i=0;i<nbword;i++)
			buffer[i] = ReadWordData();
	}
}

/*-----------------------------------------------------------------
DMAWriteWordData
Write data to flash. 
u16* buffer IN : buffer 
u16 nbword IN : Number of word (16b) to be transferred
-----------------------------------------------------------------*/
void static DMAWriteWordData(u16 buffer[],u32 nbword)
{
	u16 word;
	u8* buf_8 = (u8*)buffer;
	u32 i;
	
	// adresse buffer is 16bit aligned  ?
	if (((u32)buffer&1)!=0) { 
		// Align buffer to 16bit
		if (nbword != 0) 
			for (i=0;i<nbword;i++) {
				word = buf_8[2*i] + (buf_8[2*i+1]<<8);
				WriteWordData(word);	
			}
	} else {
		for (i=0;i<nbword;i++) 
			WriteWordData(buffer[i]);	
	}
}

/*-----------------------------------------------------------------
CopyBackCommand
Copy a single page using the copy back features of the flash
u32 dest IN : Adresse of the destination page in the flash >>8
u32 sourc IN : Adresse of the source page in the flash >>8
bool return OUT : true=ok false=ko
-----------------------------------------------------------------*/
bool CopyBackCommand(u32 dest,u32 source)
{
	// Read for Copy-back 1st cycle
	WriteByteCommand(0x00);
	// Send the addresse
	Write32bAdr(source);
	// Read for Copy-back 2nd cycle
	WriteByteCommand(0x35);
	// Wait for the end of the command
	if (!ReadRBStatus())
		return false;
		
	// Copy-Back Programm 1st cycle
	WriteByteCommand(0x85);
	// Send the addresse
	Write32bAdr(dest);
	
	return true;
}

/*-----------------------------------------------------------------
ReadCommand
Do a flash read command.
adr begin at A8
u32 adr IN : flash adresse without the first byte
u16* buffer IN : buffer 
u16 nbword IN : number of word to be transferred (<2112)
bool return OUT : true=ok false=ko 
-----------------------------------------------------------------*/
bool ReadCommand(u32 adr,u16 buffer[],u16 nbword)
{
	// Read Command 1st cycle
	WriteByteCommand(0x00);
	// Then write the adresse to be read
	Write32bAdr(adr);
	// Read command 2nd cycle
	WriteByteCommand(0x30);
	// Wait for the completion of the command
	if (!ReadRBStatus())
		return false;
	// Finish by transfering the buffer
	DMAReadWordData(buffer,nbword);
	
	return true;
}

/*-----------------------------------------------------------------
ReadWordFunction
Read data from flash and put them in a buffer 
u64 adr IN : full flash adresse
				bit 0..12  : colunm adress in a page
				bit 12..15 : don't care (see flash datasheet)
				bit 16..21 : page number in a block
				bit 22..34 : block number in the device
u16* buffer IN : buffer 
u16 nbword IN : number of word to be transferred (<2112)
bool return OUT : true=ok false=ko 
-----------------------------------------------------------------*/
bool ReadWordFunction(u64 adr,u16 buffer[],u16 nbword)
{
	// Read Command 1st cycle
	WriteByteCommand(0x00);
	// Then write the adresse to be read
	WriteByteAddress(adr);
	WriteByteAddress(adr>>8);
	WriteByteAddress(adr>>16);
	WriteByteAddress(adr>>24);
	if (NandFlashID > 8) 
		WriteByteAddress(adr>>32);
	// Read command 2nd cycle
	WriteByteCommand(0x30);
	// Wait for the completion of the command
	if (!ReadRBStatus())
		return false;
	// Transfer the data to the destination
	DMAReadWordData(buffer,nbword);
	
	return true;
}

/*-----------------------------------------------------------------
WriteWordFunction
Write data to flash
u64 adr IN : full flash adresse
u16* buffer IN : buffer 
u16 nbword IN : number of word to be transferred (<2112)
bool return OUT : true=ok false=ko 
-----------------------------------------------------------------*/
bool WriteWordFunction(u64 adr,u16 buffer[],u16 nbword)
{
	// Write Command 1st cycle
	WriteByteCommand(0x80);
	// End the adresse
	WriteByteAddress(adr);
	WriteByteAddress(adr>>8);
	WriteByteAddress(adr>>16);
	WriteByteAddress(adr>>24);
	if (NandFlashID > 8) 
		WriteByteAddress(adr>>32);
	// Write the data
	DMAWriteWordData(buffer,nbword);
	// Write Command 2nd cycle
	WriteByteCommand(0x10);
	
	// Wait for the completion of the command
	if (ReadNandFlashStatus() == 0)
		return true;
	else {
		WriteByteCommand(0xFF);
		return false;
	}
}

/*-----------------------------------------------------------------
EraseBlockFunc
Perform an Block Erase command on a block pointed by blockadr
u32 blockadr IN : blockadr is (blocknumber<<14)
bool return OUT : true=ok false=ko
-----------------------------------------------------------------*/
bool EraseBlockFunc(u32 blockadr)
{
	int retval;
	
	// First block is reserved so we can't erase it.
	if (blockadr < 0x4000)
		return false;
		
	// Block erase command 1st cycle
	WriteByteCommand(0x60);
	// Then write the adresse of the block to be erased
	WriteByteAddress(blockadr>>8);
	WriteByteAddress(blockadr>>16);
	if (NandFlashID > 8) 
		WriteByteAddress(blockadr>>24);
	// Block erase command 2nd cycle
	WriteByteCommand(0xD0);	
	
	retval = ReadNandFlashStatus();
	// Reset the nand flash
	WriteByteCommand(0xFF);
	 // Ex_ReadNandFlashStatus return 0 to say it's ok
	if (retval == 0)		
		return true;
	else
		return false;
}

/*-----------------------------------------------------------------
LabelFailBlock
Label the block with defective tag
u32 blockadr IN : blockadr is block number <<14
-----------------------------------------------------------------*/
void LabelFailBlock(u32 blockadr)
{
	int numpage;
	u16 buffer[32];
	
	fakememset(buffer,0,sizeof(buffer));
	// clear the spare buffer of all page in the block
	// seems to be the way to recognize an invalid block
	for (numpage=0;numpage<64;numpage++) 
		WriteWordFunction(2048+numpage*65536+blockadr*256,buffer,32);
		
}

/*-----------------------------------------------------------------
GetNextFreeBlock
Return the next free block in the superblock
u16 superblock IN : superblock number
u16 return OUT : free blockadress
-----------------------------------------------------------------*/
u16 GetNextFreeBlock(u16 superblock)
{	
	// Incremente the current free block pointer
	if (NbFree[superblock] == 0)
		return 0;
	CurFree[superblock]++;
	if (CurFree[superblock] > NbFree[superblock]-1)
		CurFree[superblock] = 0;
		
	return FreeFifo[superblock*12+CurFree[superblock]];
}

/*-----------------------------------------------------------------
ReplaceFreeBlock
Intervert the old and the free block
u16 superblock IN : superblock number
u16 oldblockindex IN : index in LookUpTable of the old block
-----------------------------------------------------------------*/
void ReplaceFreeBlock(u16 superblock,u16 oldblockindex)
{
	u16 tempblock;
	
	tempblock = LookUpTable[oldblockindex];
	LookUpTable[oldblockindex] = FreeFifo[superblock*12+CurFree[superblock]];
	FreeFifo[superblock*12+CurFree[superblock]] = tempblock;
	
}

/*-----------------------------------------------------------------
RemoveCurrentFreeBlock
Remove the current free block in the fifo
u16 superblock IN : superblock number
-----------------------------------------------------------------*/
void RemoveCurrentFreeBlock(u16 superblock)
{
	u16 k;
	
	for (k=CurFree[superblock];k<NbFree[superblock]-1;k++)
		FreeFifo[superblock*12+k] = FreeFifo[superblock*12+k+1];
	if (NbFree[superblock] != 0)
		NbFree[superblock]--;
}

/*-----------------------------------------------------------------
CopyBackPages
Copy a number of pages from a block to another usign copy back
u32 desteblock IN : dest block addresse
u32 sourceblock IN : source block addresse
u16 pagestart IN : Starting page
u16 nbpages IN : Number of pages to write
bool return OUT : status
-----------------------------------------------------------------*/
bool CopyBackPages(u32 destblock,u32 sourceblock,u16 pagestart,u16 nbpages)
{
	u16 i;
	
	for (i=pagestart;i<pagestart+nbpages;i++) {
		if (!CopyBackCommand(destblock+(i<<8),sourceblock+(i<<8)))
			return false;
		// Copy back program 2nd cycle = finish the write of the page
		WriteByteCommand(0x10);
		// Check if the command went well
		if (ReadNandFlashStatus() != 0) 
			if (ReadNandFlashStatus() != 0) 
				return false;		
	}
		
	return true;
}

/*-----------------------------------------------------------------
OverWriteSectors
Copy back a pages and overwrite some sectors in ot
u32 desteblock IN : dest block addresse
u32 sourceblock IN : source block addresse
u16 pagestart IN : Starting page
u16 secstart IN : Starting sector
u16 nbsec IN : Number of sectors to write
bool return OUT : status
-----------------------------------------------------------------*/
bool OverWriteSectors(u32 destblock,u32 sourceblock,u16 pagestart,u16 secstart,u16 nbsec,u16* buffer)
{
	u16 secnum;
	u32 eccbuffer[16];
	u32 ecc1,ecc2;
	
	// Read the spare buffer
	if(!ReadCommand(sourceblock+(pagestart<<8)+8,(u16*)eccbuffer,32))
		return false;	
	// Copy back the pages
	if (!CopyBackCommand(destblock+(pagestart<<8),sourceblock+(pagestart<<8)))
		return false;
	
	for (secnum=0;secnum<nbsec;secnum++) {
		// Replace the sector in the dest page
		WriteByteCommand(0x85);
		// adresse
		WriteByteAddress(0);
		WriteByteAddress(2*(secnum+secstart));
		DMAWriteWordData(&buffer[secnum*256],256);
		// do the ECC stuff
		ecc1 = make_ecc_256(&buffer[secnum*256]);
		ecc2 = make_ecc_256(&buffer[128+secnum*256]);
		eccbuffer[3+4*(secnum+secstart)] = (eccbuffer[3+4*(secnum+secstart)]&0xFF) | (ecc1<<8);
		eccbuffer[2+4*(secnum+secstart)] = (eccbuffer[2+4*(secnum+secstart)]&0xFF000000) | ((ecc2<<8)>>8);
	}
	// Finish by copying the eccbuffer
	// Copy back program
	WriteByteCommand(0x85);
	// adresse
	WriteByteAddress(0);
	WriteByteAddress(8); // addresse of the spares bytes 2048-2211
	DMAWriteWordData((u16*)eccbuffer,32);
	// Copy back program 2nd cycle = finish the write of the page
	WriteByteCommand(0x10);
	// Check if the command went well
	if (ReadNandFlashStatus() != 0) 
		if (ReadNandFlashStatus() != 0) 
			return false;
			
	return true;
}

/*-----------------------------------------------------------------
CacheProgramSectors
Copy back a pages and overwrite some sectors in ot
u32 desteblock IN : dest block addresse
u32 secnum IN : source block addresse
u16 pagestart IN : Starting page
u16 nbpages IN : Number of pages to write
bool return OUT : status
-----------------------------------------------------------------*/
bool CacheProgramSectors(u32 destblock,u16 secnum,u16 pagestart,u16 nbpages,u16* buffer)
{
	u16 i,valblock,page,sector;
	u32 p;
	u32 eccbuffer[16];
	u32 ecc1,ecc2;
	
	// Prepare the eccbuffer 
	// Calc the parity
	p = 0xFFFFFFFF;
	for (i=0;i<9;i++) 
		if ((secnum&(1<<i))!=0)
			p = ~p;
	valblock = 0x1000 | (secnum<<1) | (p&1);
	valblock = (valblock<<8) | (valblock>>8); // Reverse
	// add block<->sector corespondance in eccbuffer
	for (i=0;i<4;i++) {
		eccbuffer[0+4*i] = 0XFFFFFFFF;
		eccbuffer[1+4*i] = 0XFFFF | (valblock<<16);
		eccbuffer[2+4*i] = 0XFFFFFF | (valblock<<24);
		eccbuffer[3+4*i] = 0XFFFFFF00 | (valblock>>8);
	}
	for (page=0;page<nbpages;page++) {
		// Start the write program
		WriteByteCommand(0x80);
		Write32bAdr(destblock+(pagestart+page)*256);
		// Write the data
		for (sector=0;sector<4;sector++) {
			DMAWriteWordData(&buffer[page*1024+sector*256],256);
			// do the ECC stuff
			ecc1 = make_ecc_256(&buffer[page*1024+sector*256]);
			ecc2 = make_ecc_256(&buffer[page*1024+128+sector*256]);
			eccbuffer[3+4*sector] = (eccbuffer[3+4*sector]&0xFF) | (ecc1<<8);
			eccbuffer[2+4*sector] = (eccbuffer[2+4*sector]&0xFF000000) | ((ecc2<<8)>>8);
		}
		DMAWriteWordData((u16*)eccbuffer,32);
		if (page == nbpages-1) { 
			WriteByteCommand(0x10);	// Last page finish the job
			// Wait for completion
			if (ReadNandFlashStatus() != 0) 
				if (ReadNandFlashStatus() != 0) 
					return false;		
		} else {
			WriteByteCommand(0x15); // If not ask for cache program		
			if (!ReadRBStatus())
				return false;
		}	
	}
	
	return true;
}

/*-----------------------------------------------------------------
WriteBlockFunction
Write a complete block. The functions is quite big but in a nutshell
it picks a free block in the free block fifo. 
Copy the data from the source block to the free block.
Overwrite the free block with the user provieded sectors.
Update the LookUpTable.
The function write as many sector until the end of the block and return.
u32 startsec IN : Starting secotr number
u16 numsecs IN : Number of sector to write 
u16* buffer IN : buffer containing the sectors
int return OUT : error code or number of sector written
					-3 the sector is in the first block of the flash (reserved area)
					-4 the sector is outside the flash
					-5 No free block found, consider buying a new flash !
					>=0 Number of sector actually written
-----------------------------------------------------------------*/
int WriteBlockFunc(u32 startsec,u16 numsecs,u16* buffer)
{
	u16 nbsecwritten,nbsec,nbpage,startpage;
	u16 nbtry,startseclookupindex,startsecsuperblock;
	u32 freeblockadr,destpageadr,sourcepageadr,adrblocktoerase;
	
	// Calculate the physical adresse of the sector
	startseclookupindex = startsec>>8;
	// in which superblock the sector is ?
	startsecsuperblock = LookUpTable[startseclookupindex]>>11;
	// The block number cannot be equal to zero
	if (LookUpTable[startseclookupindex]==0) 
		return -3;
	// phy adresse cannot be greater than 2^31
	if (LookUpTable[startseclookupindex] > (1<<15))
		return -4;
		
	while (1==1) {
		// We take the first free block in the free block queue = rotating the block and increase flash life
		freeblockadr = GetNextFreeBlock(startsecsuperblock)<<12;
		// No free block found ?, this is bad meaning that your flash is really really broken
		if (freeblockadr == 0)
			return -5;
			
		// Try to write all the sectors
		nbtry=0;
		do {
			destpageadr = freeblockadr;
			sourcepageadr = (LookUpTable[startseclookupindex]<<12);  // We begin at the beginning of the block
			nbsecwritten  = 0;
			// Copy back the pages until the first sector
			nbpage = ((startsec&0xFF)>>2);
			startpage = nbpage;
			if (nbpage != 0)
				if (!CopyBackPages(destpageadr,sourcepageadr,0,nbpage))
					goto errorwriting;
			// Overwrite the sector in startpage
			nbsec = 4 - (startsec&0x3);
			if (nbsec > numsecs) 
				nbsec = numsecs;
			if ((nbsec != 0)&&(nbsec!=4)) {
				if (!OverWriteSectors(destpageadr,sourcepageadr,startpage,(startsec&0x3),nbsec,buffer))
					goto errorwriting;
				startpage++;
				nbsecwritten += nbsec;
				numsecs -= nbsec;
			}
			// Cache progamming for writing entiere pages
			nbpage = (numsecs>>2); // 4 sectors per pages
			if (nbpage+startpage > 64) // Too many page to write ?
				nbpage = 64-startpage;
			if (nbpage != 0) {
				if (!CacheProgramSectors(destpageadr,startseclookupindex-500*startsecsuperblock,startpage,nbpage,&buffer[256*nbsecwritten]))
					goto errorwriting;
				startpage += nbpage;
				nbsecwritten += 4*nbpage;
			}
			// Overwrite the last sectors
			nbsec = numsecs&3;
			if ((nbsec != 0)&&(startpage != 64)) { 
				if (!OverWriteSectors(destpageadr,sourcepageadr,startpage,0,nbsec,&buffer[256*nbsecwritten]))
					goto errorwriting;
				startpage++;
				nbsecwritten += nbsec;
			}
			// Finish by copy back the remaining pages
			if (startpage != 64) 
				if (!CopyBackPages(destpageadr,sourcepageadr,startpage,64-startpage))
					goto errorwriting;
			// if ok then we erase the old block and update the lookuptable
			adrblocktoerase = LookUpTable[startseclookupindex]<<12;
			// Put the free block in the lookuptable replacing the old block
			ReplaceFreeBlock(startsecsuperblock,startseclookupindex);
			// We want to erase the old block
			if (!EraseBlockFunc(adrblocktoerase)) 
				if (!EraseBlockFunc(adrblocktoerase)) {
					// after 2 try we consider the block to be defective
					LabelFailBlock(adrblocktoerase);
					RemoveCurrentFreeBlock(startsecsuperblock);	
				}	
			// we have finish we're happy
			// This is the normal endpoint when everything went well
			return nbsecwritten;		
errorwriting:
			// If we are here it's something went wrong
			nbtry++;
			// Try to clear the block and try again the writing
			while ((EraseBlockFunc(freeblockadr) != 1)&&(nbtry<=2))
				nbtry++;
		} while (nbtry <= 2);
		
		// ok no chance the free block is dead
		// Label the block to be of no use anymore
		LabelFailBlock(freeblockadr);
		// Remove the dead free block of the fifo
		RemoveCurrentFreeBlock(startsecsuperblock);
	}
	
	// Should never go there
	return -6;
}

/*-----------------------------------------------------------------
CheckNandInserted
Return true is there is a G6 inside the NDS.
Update the NandFlashID as well
bool return OUT : true=ok false=ko
-----------------------------------------------------------------*/
bool CheckNandInserted()
{
	u16 id1,id2,i;
	
	// Start with unknown device
	NandFlashID = 0;
	// Reset the nand flash
	WriteByteCommand(0xFF);
	// And wait for the completion of the command
	if (!ReadRBStatus())
		return 0;
	// Read ID command
	WriteByteCommand(0x90);
	WriteByteAddress(0);
	id1 = ReadWordData();
	id2 = ReadWordData();
	// Check for the device marker code and ID data
	if ((id2&0xFF00) != 0x1500)
		return false;
	if ((id1&0xFF) != 0xEC)
		return false;
	// Search the device by device code
	for (i=8;i<12;i++)
		if (id1>>8 == FlashIDTable[i-8]) {
			NandFlashID = i;
			return true;	
		}
	
	return false;	
}

/*-----------------------------------------------------------------
BuildLookUpTableRoutine
Aux function to make the lookup table
u32 return OUT : Number of block 
-----------------------------------------------------------------*/
u32 BuildLookUpRoutine(u64 adr,u16 superblock)
{
	u64 adrval;
	u32 val,adr32;
	u16 i,j,offset1,counter;
	u16 testbit,nbonebit;
	
	counter = 0;
	offset1 = 0;
	for (i=0;i<512;i++) {
		// Point the second u32 in the extra 64byte of the first page of the current block
		adrval = adr + (u64)0x400000*(u64)i + (u64)2052;
		adr32 = (u32)(adrval>>20);
		// Read the sector number
		ReadWordFunction(adrval,(u16*)&val,2);
		val = ReverseU32(val);
		if (TestInvalidBlock(val)) {
			val = (val<<16)>>16; // we need only the first 16bit
			//  Free blocks are filled with 0xFF
			if (val==0xFFFF) {
				if (offset1 <= 11) {
					FreeFifo[superblock*12+offset1] = adr32;
					NbFree[superblock]++;
				}
				offset1++;
			} else {
				// Blocks filled with 0 are failed
				if ((val != 0)&&((val&0x1000)!=0)) {
					// Check the parity of val, should be even
					testbit = 1;
					nbonebit = 0;
					for (j=0;j<16;j++) {
						if ((val&testbit) == testbit)
							nbonebit++;
						testbit = testbit<<1;
					}
					if ((nbonebit&1) == 0) {
						counter++;
						LookUpTable[(superblock*500)+((val>>1)&0x1FF)] = adr32; 
					}
				} 
			}
		}
	}
	return counter;
}

/*-----------------------------------------------------------------
BuildLookUpTable
Build the lookup table
	numphyblock = [numlogblock%500+512*(numlogblock div 500)]>>2
bool return OUT : true=ok false=ko
-----------------------------------------------------------------*/
bool BuildLookUpTable()
{
	u64 adr;
	u32 retval,stopval;
	u16 i,j;
	
	// Check if NandFlashID is valid
	if ((NandFlashID-8) > 3)
		return false;
	// fill the tables with 0
	fakememset(LookUpTable,0,sizeof(LookUpTable));
	fakememset(FreeFifo,0,sizeof(FreeFifo));
	fakememset(NbFree,0,sizeof(NbFree));
	fakememset(CurFree,0xFF,sizeof(CurFree));
	
	// The LookUpTable is generated 512block by 512block
	// stopval is the number of superblock in the device
	if (NandFlashID == 7) {
		stopval = 1;
	} else {
		stopval = 1<<(NandFlashID-7);
		// Just to be sure I guess
		if (stopval == 0) {
			WriteByteCommand(0xFF);
			return true;
		}	
	}
	
	adr = 0;
	for (i=0;i<stopval;i++) {
		j = 5;
		// We try five time to build it
		do {
			// Reset the nand flash
			WriteByteCommand(0xFF);
			// Build 512 corespondances of the lookup table
			retval = BuildLookUpRoutine(adr,i);
			j--;
		} while ((j != 0)&&(retval < 500));
		adr += 0x80000000; // Increment adr to point to the beginning of the next 512block starting adresse = 2^31
	} 
	
	// Reset the nand flash
	WriteByteCommand(0xFF);
	return true;	
}

/*****************************************************************************/
/*						DLDI INTERFACE					    */
/*****************************************************************************/

/*-----------------------------------------------------------------
startUp
Initialize the interface, geting it into an idle, ready state
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
bool startup(void) {
	bool retval;
	
	SelectNand();
	retval = CheckNandInserted();
	if (retval == 1)
	{
		SelectNand();
		retval = BuildLookUpTable();
	} 
	DisableRAM();
	
	return retval;
}

/*-----------------------------------------------------------------
isInserted
Is a card inserted?
return true if a card is inserted and usable
-----------------------------------------------------------------*/
bool isInserted (void) {
	bool retval;
	
	SelectNand();
	retval = CheckNandInserted();
	DisableRAM();
	
	return retval;
}


/*-----------------------------------------------------------------
clearStatus
Reset the card, clearing any status errors
return  true if the card is idle and ready
-----------------------------------------------------------------*/
bool clearStatus (void) {
	bool retval;
	
	SelectNand();
	retval = ReadRBStatus();
	DisableRAM();
	
	return retval;
}


/*-----------------------------------------------------------------
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool readSectors (u32 sector, u32 numSectors, void* _buffer) {
	u16 temp;
	u32 nbsecread,nbsectoread;
	u32 secphyadr;
	u16 *buffer = (u16 *)_buffer;
	 
	SelectNand();
	
	// Verification of the buffer pointer
	if ((u32)buffer == 0)	{
		WriteProtectDisable();
		ChipDisable();
		SelectOperation(2);
		return true;
	}
	
	// do till we have read numsecs sectors
	nbsecread = 0;
	do {
		// Calculate the number of sectors to read
		secphyadr = SectorToPhyAddr(sector,&temp);
		
		// Calculate the numbers of sector to read
		nbsectoread = 4 - ((secphyadr>>1)&3);
		if (nbsecread + nbsectoread > numSectors)
			nbsectoread = numSectors - nbsecread;
				
		// Perform a read flash command
		WriteByteCommand(0);
		Write32bAdr(secphyadr);
		WriteByteCommand(0x30);
		
		// Wait for the completion of the command
		while (((*(volatile u16*)0x09F0330E) & 0x8000) != 0x8000);
		// Read the data and write them into the buffer
		DMAReadWordData(buffer,nbsectoread*256);

		buffer += nbsectoread<<8; // Incremente the buffer for the next read
		sector += nbsectoread;	   // sector point to the next logical sector to read
		nbsecread += nbsectoread;
		
		// reset the flash
		WriteByteCommand(0xFF);
	} while(numSectors > nbsecread);
	
	DisableRAM();
	return true;
}



/*-----------------------------------------------------------------
writeSectors
Write "numSectors" 512-byte sized sectors from "buffer" to the card, 
starting at "sector".
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool writeSectors (u32 sector, u32 numSectors, void* _buffer) {
	bool retval;
	u16 numsectowrite;
	int nbsecwrite;
	u16 *buffer = (u16 *)_buffer;
	
	SelectNand();
	WriteProtectEnable();
	
	// 0 stand for 256
	if (numSectors!=0)
		numsectowrite = numSectors;
	else
		numsectowrite = 256;
	
	retval = true;
	do {
		nbsecwrite = WriteBlockFunc(sector,numsectowrite,buffer);
		buffer += nbsecwrite*256;
		sector += nbsecwrite;
		if (nbsecwrite < 0) {
			retval = false;
			break;
		}
		numsectowrite -= nbsecwrite;
	} while (numsectowrite != 0);
	
	//  Why 2 times ?
	WriteProtectDisable();
	DisableRAM();
	
	return retval;
}

/*-----------------------------------------------------------------
shutdown
shutdown the card, performing any needed cleanup operations
-----------------------------------------------------------------*/
bool shutdown(void) {
	bool retval;
	
	// Why the hell do we have to do everything 2 times ??
	SelectNand();
	SelectNand();
	if (!ReadRBStatus())
		retval = false;
	else 
		retval = true;
	DisableRAM();
	DisableRAM();
	
	/*// Licks DLDI with reset: (this is PUYO's code)
	SelectOperation(0);
	*((vu32*)0x027FFFF8)= 0x080000C0;*/

	return retval;
}
