/*----------------------------------------------------------------------------
 *      by NEOTEAM 2007
 *----------------------------------------------------------------------------
 *      http://www.neoflash.com
 *----------------------------------------------------------------------------*/
 
#include "common.h"

#include <nds/card.h>

#define	nrio_fattab		((u8*)0x023FEE00)				//FIXME: DLDI could not alloc global memory ??
#define	nrio_buf		((u8*)(0x023FEE00-0x800))
#define CARD_CR2_D2		(*(vu32*)(0x023FEE00-0x800-8))
#define nrio_fattabaddr (*(vu32*)(0x023FEE00-0x800-4))
#define nrio_bufaddr	(*(vu32*)(0x023FEE00-0x800-12))


u32 CalcCARD_CR2_D2()
{
	u32 da,db,dc,ncr2;
	da=*(vu32*)0x027FFE60;
	db=*(vu32*)0x027FFE60;
	da=da&0x00001FFF;
	db=db&0x003F0000;
	db=db>>16;
	dc=da+db;
	dc=(dc/2)-1;
	ncr2=*(vu32*)0x027FFE60;
	ncr2&=~0x003F1FFF;
	ncr2|=dc;
	return ncr2;
}

void cardreadpage(unsigned int addr,unsigned int dst,unsigned char cmd,unsigned int card_cr2)
{
	*(volatile unsigned char*)0x40001A1 = 0xC0;
	*(volatile unsigned char*)0x40001A8 = cmd;
	*(volatile unsigned char*)0x40001A9 = (addr >> 24);
	*(volatile unsigned char*)0x40001AA = (addr >> 16);
	*(volatile unsigned char*)0x40001AB = (addr >> 8);
	*(volatile unsigned char*)0x40001AC = addr;
	*(volatile unsigned char*)0x40001AD = 0;
	*(volatile unsigned char*)0x40001AE = 0;
	*(volatile unsigned char*)0x40001AF = 0;
	
	
	*(volatile unsigned int*)0x40001A4 = card_cr2;
	do {
		// Read data if available
		if ((*(volatile unsigned int*)0x40001A4) & 0x800000) {
			*(volatile unsigned int*)dst=*(volatile unsigned int*)0x4100010;
			dst+=4;
		}
	} while ((*(volatile unsigned int*)0x40001A4) & 0x80000000);
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
			u32 i;
			u32*pbuf=(u32*)buffer;
			for(i=0;i<512;i++)*pbuf++=0;
		}
		nrio_bufaddr=(sector/0x800);
	}
	swiFastCopy((void*)(nrio_buf+sector%0x800),(void*)buffer,128);
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
