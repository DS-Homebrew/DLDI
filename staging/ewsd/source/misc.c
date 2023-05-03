#include <nds.h>
#include <ctype.h>

#include "sdbasic.h"
#include "misc.h"


u32 gReadWriteDelay = DELAY_465K;

inline void rwDelay(u32 t)
{

	int c;
	int d;


	for (c = 0; c <t; c++)
		for (d = 0; d <t; d++);

}


char *strupr(char *s)
{
  char *p = s;

  while (*p)
  {
    *p = (char) toupper(*p);
    p++;
  }

  return s;
}


#define MAP_ADDR(a0, a1, a2) *(vu16 *)(0x08000000 + ( a2 << 16 | a1 << 8 | a0 ) * 2)


// Command Code 19 00 00 FE 00 15


inline void TFC_Command19(BYTE a1, BYTE a2, BYTE a3, BYTE a4, BYTE a5)
{
	MAP_ADDR(a1, a2, a3) = a5 << 8 | a4;
//	rwDelay(gReadWriteDelay);
}


inline void TFC_Command19_A(LONG addr, WORD d)
{
	*(vu16 *)(addr * 2 + 0x08000000) = d;
//	rwDelay(gReadWriteDelay);
}

inline WORD SDT_Command1ARead(LONG addr)
{
	vu16 *p;
	p = (vu16 *)(addr * 2 + 0x08000000);
//	rwDelay(gReadWriteDelay);
	return *p;


}




/*****************************************************
Set Flash CS config register.

	000    nor
	001	  NOR

	010    all disabled
	111    sd mode

0 0 sram bank select
******************************************************/

#define ADDR_FLASH_CS_CONFIG		0x84

void SDT_SetFlashCSConfig(BYTE mode)
{

	vu16* p;
	p = (vu16*)0x09FE0000;
	p[0] = 0xd200;

	p = (vu16*)0x08000000;
	p[0] = 0x1500;

//////////////////////////////////////////////////////////////

	//`define ADDR_FLASH_CS_CONFIG		  	8'h84

	p = (vu16*)0x09080000;
	p[0] = mode;

//////////////////////////////////////////////////////////////


	p = (vu16*)0x08020000;
	p[0] = 0xd200;



}



//---------------------------------------------------------------------------------




