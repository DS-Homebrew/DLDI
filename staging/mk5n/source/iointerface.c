/*----------------------------------------------------------------------------
 *      by NEOTEAM 2007
 *----------------------------------------------------------------------------
 *      http://www.neoflash.com
 *----------------------------------------------------------------------------*/
 
#include <nds/ndstypes.h>

#include "tonccpy.h"

#ifndef NULL
	#define NULL 0
#endif

// Card bus
#define	REG_CARD_DATA_RD	(*(vu32*)0x04100010)

#define REG_AUXSPICNT	(*(vu16*)0x040001A0)
#define REG_AUXSPICNTH	(*(vu8*)0x040001A1)
#define REG_AUXSPIDATA	(*(vu8*)0x040001A2)
#define REG_ROMCTRL		(*(vu32*)0x040001A4)

#define REG_CARD_COMMAND	((vu8*)0x040001A8)

#define CARD_SPICNTH_ENABLE  (1<<7)  // in byte 1, i.e. 0x8000
#define CARD_SPICNTH_IRQ     (1<<6)  // in byte 1, i.e. 0x4000

#define	nrio_fattab		((u8*)0x023FEE00)				//FIXME: DLDI could not alloc global memory ??
#define	nrio_buf		((u8*)(0x023FEE00-0x800))
#define CARD_CR2_D2		(*(vu32*)(0x023FEE00-0x800-8))
#define nrio_fattabaddr (*(vu32*)(0x023FEE00-0x800-4))
#define nrio_bufaddr	(*(vu32*)(0x023FEE00-0x800-12))

#define CARD_HEADER_CARDCONTROL13  (*(vu32*)0x02FFFE60)

u32 CalcCARD_CR2_D2()
{
	u32 da,db,dc,ncr2;
	da=CARD_HEADER_CARDCONTROL13;
	db=CARD_HEADER_CARDCONTROL13;
	da=da&0x00001FFF;
	db=db&0x003F0000;
	db=db>>16;
	dc=da+db;
	dc=(dc/2)-1;
	ncr2=CARD_HEADER_CARDCONTROL13;
	ncr2&=~0x003F1FFF;
	ncr2|=dc;
	return ncr2;
}

void cardreadpage(unsigned int addr,unsigned int dst,unsigned char cmd,unsigned int card_cr2)
{
	REG_AUXSPICNTH = CARD_SPICNTH_ENABLE | CARD_SPICNTH_IRQ;
	REG_CARD_COMMAND[0] = cmd;
	REG_CARD_COMMAND[1] = (addr >> 24);
	REG_CARD_COMMAND[2] = (addr >> 16);
	REG_CARD_COMMAND[3] = (addr >> 8);
	REG_CARD_COMMAND[4] = addr;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;
	
	
	REG_ROMCTRL = card_cr2;
	do {
		// Read data if available
		if (REG_ROMCTRL & 0x800000) {
			*(volatile unsigned int*)dst=REG_CARD_DATA_RD;
			dst+=4;
		}
	} while (REG_ROMCTRL & 0x80000000);
}

__inline
void cardreadpage_B7(unsigned int addr,unsigned int dst)
{
	cardreadpage(addr,dst,0xB7,*(vu32*)0x027FFE60);
}

__inline
void cardreadpage_D2(unsigned int addr,unsigned int dst)
{
	cardreadpage(addr,dst,0xD2,CARD_CR2_D2);
}

u32 nrio_addrlog2phy(u32 addr)
{
	u32 indexaddr,subaddr;
	indexaddr=(addr/16384)*4+0x02000000;
	subaddr=addr%16384;
	if(indexaddr!=nrio_fattabaddr)
	{
		nrio_fattabaddr=indexaddr;
		cardreadpage_B7((indexaddr/512)*512,(u32)nrio_fattab);
	}
	indexaddr=*(vu32*)(nrio_fattab+(indexaddr%512));
	if(indexaddr==0xFFFFFFFF)return indexaddr;
	return indexaddr+subaddr;
}

void _nrio_readSector (u32 sector, void* buffer)
{
	u32 phyaddr;
	sector<<=9;

	if((sector/0x800)!=nrio_bufaddr)
	{
		phyaddr=nrio_addrlog2phy((sector/0x800)*0x800);
		if(phyaddr!=0xFFFFFFFF)
		{
			cardreadpage_D2(phyaddr,(u32)nrio_buf);
		}
		else
		{
			toncset((u8*)buffer, 0, 512);
		}
		nrio_bufaddr=(sector/0x800);
	}
	tonccpy((void*)(nrio_buf+sector%0x800), (void*)buffer, 128);
}


bool startup(void)
{
	CARD_CR2_D2=CalcCARD_CR2_D2();
	CARD_CR2_D2=(u32)((CARD_CR2_D2)&(~0x07000000))|(3<<24);
	nrio_fattabaddr=0xFFFFFFFF;
	nrio_bufaddr=0xFFFFFFFF;
	return true;
}

bool isInserted(void)
{
	return true;
}

bool readSectors(u32 sector, u32 numSecs, void* buffer)
{
	u32 *u32_buffer = (u32*)buffer, i;
	for (i = 0; i < numSecs; i++) {
		_nrio_readSector(sector,u32_buffer);
		sector++;
		u32_buffer += 128;
	}
	return true;
}

bool writeSectors(u32 sector, u32 numSecs, void* buffer)
{
	return false;
}

bool clearStatus(void)
{
	return true;
}

bool shutdown(void)
{
	return true;
}
