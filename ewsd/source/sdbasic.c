#include <nds.h>


#include "sdbasic.h"
#include "misc.h"

// SPI mode basic functions for Ewin new serials.

#define ADDR_SD_DATA			0xB00000
#define ADDR_SD_CLK				0xC00000

//--------------------------------------------------------------------------

void TFC_Command19(BYTE a1, BYTE a2, BYTE a3, BYTE a4, BYTE a5);
WORD SDT_Command1ARead(LONG addr);


// the low 4 bits
inline u32 SDB_SDM_GetDataOut()
{
	u32 ret;
	
	ret = *(vu16 *)(__ADDR_SD_DATA);
	ret = *(vu16 *)(__ADDR_SD_DATA);
	return  ret;
}

// the low 4 bits
void SDB_SDM_SetDataOut(BYTE hbyte)
{
	*(vu16 *)(__ADDR_SD_DATA) = hbyte & 0x0F;
	
}


u16 SDB_SDM_CheckCard()
{
	u16 ret;

	ret = *(vu16 *)(__ADDR_SD_CMD);

	ret = ret & 0x0F;
	return  ret;
}

inline u32 SDB_SDM_GetResponse()  // bit 4
{
	u32 ret;

	ret = *(vu16 *)(__ADDR_SD_CMD);
	ret = ret & 0x01;
	return  ret;
}
//-----------------------------------------------------------------------


// init Ewin card internal registers SPI_MODE or SD_MODE
void SDT_SetFlashCSConfig(BYTE mode);

/////////////////////////////////////////////////////////////////


// set controller to sd mode 
#define FLASH_DISABLE 3


void SDB_SetEwinSD()
{
	/*
		bit 0, bit 1          flashcs 00 = nor, 01 = NOR, 11 = sdmode 
		bit 2					srambank 
		bit 3
		bit 4
	*/

	SDT_SetFlashCSConfig(FLASH_DISABLE); // go to SD mode

}

void SDB_InitEwinSD()
{

		SDB_SetEwinSD();

}

