/*
  io_mmcf.c based on

	io_mpcf.c based on

	compact_flash.c
	By chishm (Michael Chisholm)

	Hardware Routines for reading a compact flash card
	using the GBA Movie Player

	CF routines modified with help from Darkfader

	This software is completely free. No warranty is provided.
	If you use it, please give me credit and email me about your
	project at chishm@hotmail.com

	See gba_nds_fat.txt for help and license details.
*/
// When compiling for NDS, make sure NDS is defined
#ifndef NDS
	#if defined ARM9 || defined ARM7
		#define NDS
	#endif
#endif

#ifdef NDS
	#include <nds/ndstypes.h>
	#include <nds/system.h>
#else
	#include "gba_types.h"
#endif


#ifndef NULL
	#define NULL 0
#endif

#define BYTES_PER_READ 512

//---------------------------------------------------------------
// DMA
#ifdef _IO_USE_DMA
	#ifndef NDS
		#include "gba_dma.h"
	#else
		#include <nds/dma.h>
		#ifdef ARM9
			#include <nds/arm9/cache.h>
		#endif
	#endif
#endif

#define REG_EXMEMCNT (*(vu16*)0x04000204)

//---------------------------------------------------------------
// CF Addresses
#define CF_STATUS        *(vu16*)(0x080E0000)
#define CF_FEATURES      *(vu16*)(0x08020000)
#define CF_SECTOR_COUNT  *(vu16*)(0x08040000)
#define CF_SECTOR_NO     *(vu16*)(0x08060000)
#define CF_CYLINDER_LOW  *(vu16*)(0x08080000)
#define CF_CYLINDER_HIGH *(vu16*)(0x080A0000)
#define CF_SEL_HEAD      *(vu16*)(0x080C0000)
#define CF_COMMAND       *(vu16*)(0x080E0000)

#define CF_DATA			(vu16*)0x09000000		// Pointer to buffer of CF data transered from card
#define CARD_TIMEOUT	0x00989680				// Updated due to suggestion from SaTa, otherwise card will timeout sometimes on a write

//---------------------------------------------------------------
// CF Commands
#define CF_STS_40	(u8)0x40
#define CF_STS_BUSY	(u8)0x80

#ifdef _IO_USEFASTCNT
inline u16 setFastCNT(u16 originData) {
	//  2-3   32-pin GBA Slot ROM 1st Access Time (0-3 = 10, 8, 6, 18 cycles)
	//    4     32-pin GBA Slot ROM 2nd Access Time (0-1 = 6, 4 cycles)
    const u16 mask = ~(7<<2);//~ 000011100, clear bit 2-3 + 4
    const u16 setVal = ((2) << 2) | (1<<4);
    return (originData & mask) | setVal;
}
#endif

static bool CF_Block_Ready(void) {
	u32 i = 0;
	
	while ((CF_STATUS & CF_STS_BUSY) && (i < CARD_TIMEOUT)) {
		i++;
		while ((!(CF_STATUS & CF_STS_40)) && (i < CARD_TIMEOUT)) { i++; }
	} 
	
	if (i >= CARD_TIMEOUT)return false;
			
	return true;
}


static bool CF_Set_Features(u32 feature) {
	if (!CF_Block_Ready())return false;
	
	CF_FEATURES = feature;
	CF_SECTOR_COUNT = 0x00;  // config???
	CF_SEL_HEAD = 0x00;
	CF_COMMAND = 0xEF;
	
	return true;
}



/*-----------------------------------------------------------------
MMCF_IsInserted
Is a compact flash card inserted?
bool return OUT:  true if a CF card is inserted
-----------------------------------------------------------------*/
bool MMCF_IsInserted(void) {
	if (!CF_Set_Features(0xAA))return false;
	return true;
}


/*-----------------------------------------------------------------
MMCF_ClearStatus
Tries to make the CF card go back to idle mode
bool return OUT:  true if a CF card is idle
-----------------------------------------------------------------*/
bool MMCF_ClearStatus(void) { return CF_Block_Ready(); }


bool ReadSectors (u32 sector, int numSecs, u16* buff) {
	int i;
#ifdef _IO_ALLOW_UNALIGNED
	u8 *buff_u8 = (u8*)buff;
	int temp;
#endif

#if (defined _IO_USE_DMA) && (defined NDS) && (defined ARM9)
	DC_FlushRange(buffer, numSecs * BYTES_PER_READ);
#endif

	if (!CF_Block_Ready())return false;
	
	CF_SECTOR_COUNT = (numSecs == 256) ? 0 : numSecs;
	CF_SECTOR_NO = sector;
	CF_CYLINDER_LOW = sector >> 8;
	CF_CYLINDER_HIGH = sector >> 16;
	CF_SEL_HEAD = ((sector >> 24) & 0x0F) | 0xE0;
	CF_COMMAND = 0x20; // read sectors

	while (numSecs--)	{
		
		if (!CF_Block_Ready())return false;
#ifdef _IO_USE_DMA
	#ifdef NDS
		DMA3_SRC = (u32)CF_DATA;
		DMA3_DEST = (u32)buff;
		DMA3_CR = 256 | DMA_COPY_HALFWORDS | DMA_SRC_FIX;
	#else
		DMA3COPY (CF_DATA, buff, 256 | DMA16 | DMA_ENABLE | DMA_SRC_FIXED);
	#endif
		buff += BYTES_PER_READ / 2;
#elif defined _IO_ALLOW_UNALIGNED
		i=256;
		if ((u32)buff_u8 & 1) {
			while(i--) {
				// if (!CF_Block_Ready())return false;
				temp = *CF_DATA;
				*buff_u8++ = temp & 0xFF;
				*buff_u8++ = temp >> 8;
			}
		} else {
			while(i--)*buff++ = *CF_DATA;
		}
#else
		i=256;
		while(i--)*buff++ = *CF_DATA;
#endif
	}

#if (defined _IO_USE_DMA) && (defined NDS)
	// Wait for end of transfer before returning
	while(DMA3_CR & DMA_BUSY);
#endif

	return true;
}


bool WriteSectors(u32 sector, int numSecs, u16* buff) {
		
	int i;
#ifdef _IO_ALLOW_UNALIGNED
	u8 *buff_u8 = (u8*)buff;
	int temp;
#endif
	
#if defined _IO_USE_DMA && defined NDS && defined ARM9
	DC_FlushRange(buffer, numSecs * BYTES_PER_READ);
#endif

	if (!CF_Block_Ready())return false;
	
	CF_SECTOR_COUNT = (numSecs == 256) ? 0 : numSecs;
	CF_SECTOR_NO = sector;
	CF_CYLINDER_LOW = sector >> 8;
	CF_CYLINDER_HIGH = sector >> 16;
	CF_SEL_HEAD = ((sector >> 24) & 0x0F) | 0xE0;
	CF_COMMAND = 0x30; // write sectors
	
	while (numSecs--) {
		
		if (!CF_Block_Ready())return false;

#ifdef _IO_USE_DMA
	#ifdef NDS
		DMA3_SRC = (u32)buff;
		DMA3_DEST = (u32)CF_DATA;
		DMA3_CR = 256 | DMA_COPY_HALFWORDS | DMA_DST_FIX;
	#else
		DMA3COPY(buff, CF_DATA, 256 | DMA16 | DMA_ENABLE | DMA_DST_FIXED);
	#endif
		buff += BYTES_PER_READ / 2;
#elif defined _IO_ALLOW_UNALIGNED
		i=256;
		if ((u32)buff_u8 & 1) {
			while(i--) {
				// if (!CF_Block_Ready())return false;
				temp = *buff_u8++;
				temp |= *buff_u8++ << 8;
				*CF_DATA = temp;
			}
		} else {
			while(i--)*CF_DATA = *buff++;
		}
#else
		i=256;
		while(i--)*CF_DATA = *buff++;
#endif
	}
#if defined _IO_USE_DMA && defined NDS
	// Wait for end of transfer before returning
	while(DMA3_CR & DMA_BUSY);
#endif
	return true;
}


/*-----------------------------------------------------------------
MMCF_ReadSectors
Read 512 byte sector numbered "sector" into "buffer"
u32 sector IN: address of first 512 byte sector on CF card to read
u32 numSecs IN: number of 512 byte sectors to read
void* buffer OUT: pointer to 512 byte buffer to store data in
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool MMCF_ReadSectors(u32 sector, u32 numSecs, void* buffer) {
	bool Result = false;
#ifdef _IO_USEFASTCNT
	u16 originMemStat = REG_EXMEMCNT;
	REG_EXMEMCNT = setFastCNT(originMemStat);
#endif
	while (numSecs > 0) {
		int sector_count = (numSecs > 256) ? 256 : numSecs;
		Result = ReadSectors(sector, sector_count, (u16*)buffer);
		sector += sector_count;
		numSecs -= sector_count;
		buffer += (sector_count * BYTES_PER_READ);
	}
#ifdef _IO_USEFASTCNT
	REG_EXMEMCNT = originMemStat;
#endif	
	return Result;
}

/*-----------------------------------------------------------------
MMCF_WriteSectors
Write 512 byte sector numbered "sector" from "buffer"
u32 sector OUT: address of 512 byte sector on CF card to write
u32 numSecs OUT: number of 512 byte sectors to write
void* buffer IN: pointer to 512 byte buffer to write data to
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool MMCF_WriteSectors(u32 sector, u32 numSecs, void* buffer) {
	bool Result = false;
#ifdef _IO_USEFASTCNT
	u16 originMemStat = REG_EXMEMCNT;
	REG_EXMEMCNT = setFastCNT(originMemStat);
#endif
	while (numSecs > 0) {
		int sector_count = (numSecs > 256) ? 256 : numSecs;
		Result = WriteSectors(sector, sector_count, (u16*)buffer);
		sector += sector_count;
		numSecs -= sector_count;
		buffer += (sector_count * BYTES_PER_READ);
	}
#ifdef _IO_USEFASTCNT
	REG_EXMEMCNT = originMemStat;
#endif	
	return Result;
}
/*-----------------------------------------------------------------
MMCF_Shutdown
unload the GBAMP CF interface
-----------------------------------------------------------------*/
bool MMCF_Shutdown(void) { return CF_Block_Ready(); }

/*-----------------------------------------------------------------
MMCF_StartUp
initializes the CF interface, returns true if successful,
otherwise returns false
-----------------------------------------------------------------*/
bool MMCF_StartUp(void) { return MMCF_IsInserted(); }

