/*******************************************************
	Modified by Rudolph (鼓碾)

	2006/11/29
		* CRC is checked only when there is 
		   a possibility that reading fails.
	2006/11/28
		* Changed in error checking of Read routine.
		* Speed up of an initialization routine.
	2006/11/24
		* Improvement in speed of Write routine
		* Improvement in reliability of reading data
	2006/11/20
		* Added write routine support

 Original is EZ4FATLIB code offered by EZTEAM.
 Hardware Routines for SD card using a EZ4 series.

********************************************************/

#include <nds.h>
#include "stdio.h"
#include "string.h"

#include "sdopera.h"


u32 SDadd;


void  SD_SendCommand(int type , unsigned int param)
{
	unsigned char ppbuf[8];
	unsigned char crc = 0 ;

	ppbuf[0] = 0x40 | (type&0xFF) ;
	ppbuf[1] = (param>>24)&0xFF ;
	ppbuf[2] = (param>>16)&0xFF ;
	ppbuf[3] = (param>>8)&0xFF ;
	ppbuf[4] = (param)&0xFF ;
	crc = crc_730(ppbuf,5);
	ppbuf[5] = (crc<<1)|0x1 ;
	SD_WriteBufferToLine(ppbuf,6);
}

bool	SD_SendCommandRS(int type, unsigned int param)
{
	u8	p[16];
	int	rtry;
	bool	ret;

	rtry = 0x1000;
	do {
		SD_SendCommand(type, param);
		ret = SD_ReadResponse(p, 6, 0x100);
		if(ret && (p[0] != type))
			ret = false;
		rtry--;
	} while(rtry && (!ret));

	if(rtry)	return true;
	return false;
}

bool	SD_SendCommandRS3(int type, unsigned char *ppbuf, unsigned int param)
{
	int	rtry;
	bool	ret;

	rtry = 0x1000;
	do {
		SD_SendCommand(type, param);
		ret = SD_ReadResponse(ppbuf, 6, 0x1000);
		if(ret && ((ppbuf[0] != 0x3F) || (ppbuf[5] != 0xFF)))
			ret = false;
		rtry--;
	} while(rtry && (!ret));

	if(rtry)	return true;
	return false;
}


bool 	SD_R16Response(unsigned char *ppbuf)
{
	return SD_ReadResponse(ppbuf, 6, 0x1000);
}

bool 	SD_R2Response(unsigned char *ppbuf)
{
	return SD_ReadResponse(ppbuf, 17, 0x1000);
}

void	SD_ReadLoop(int lp)
{
	int i, k;

	for(i=0;i<lp;i++)
		k = *(vu16*)0x9FFFF40 ;
}


bool 	SD_ReadSingleBlock(unsigned int address , unsigned char *ppbuf, int len)
{
	bool	ret;
	u8	rcrc[12];
	u8	crc[8];

	ret = false;
	do {
		SD_SendCommand(17, address);
		if(SD_ReadData(ppbuf, 512, rcrc, 0x200)) {
			ret = true;
			if(((rcrc[9]&0x0F) != 0x0F) || (rcrc[9] != rcrc[11]) || ((rcrc[9]&rcrc[11]) == 0xFF)) {
				SD_cal_crc16(ppbuf, 512, crc);
				if((*(vu32*)(crc) != *(vu32*)(rcrc)) || (*(vu32*)(crc+4) != *(vu32*)(rcrc+4)))
					ret = false;
			}
		}
		SD_ReadLoop(8);
	} while(!ret);

	return ret;
}


bool	SD_WriteSingleBlock(unsigned int address , unsigned char *ppbuf, int len)
{
	bool	ret, wr;
	u8	pbuf[528];
	int	i;
	u16	k[16];

	memcpy(pbuf, ppbuf, 512);
	SD_cal_crc16(pbuf, 512, pbuf + 512);

	wr = false;
	do {
		ret = SD_SendCommandRS(24, address);
		if(ret) {
			SD_WriteData(pbuf, 520, 0x1000);
			for(i = 0; i < 10; i++)
				k[i] = *(vu16*)0x9fea000;
			if(!(k[3] & 0x100) && (k[4] & 0x100) && !(k[5] & 0x100))
				wr = true;
		}
	} while(ret && (!wr));

	SD_SendCommandRS(13,(SDadd<<16));

	return ret;
}


void	 SD_Enable()
{
	WAIT_CR &= ~0x80;
	*(vu16*)0x9fe0000 = 0xd200;
	*(vu16*)0x8000000 = 0x1500;
	*(vu16*)0x8020000 = 0xd200;
	*(vu16*)0x8040000 = 0x1500;
	*(vu16*)0x9400000 = 1;
	*(vu16*)0x9C40000 = 0x1500;
	*(vu16*)0x9fc0000 = 0x1500;
}

void	SD_Disable()
{
	*(vu16*)0x9fe0000 = 0xd200;
	*(vu16*)0x8000000 = 0x1500;
	*(vu16*)0x8020000 = 0xd200;
	*(vu16*)0x8040000 = 0x1500;  
	*(vu16*)0x9400000 = 0;
	*(vu16*)0x9C40000 = 0xd200;
	*(vu16*)0x9fc0000 = 0x1500;
  	WAIT_CR |= 0x80;
}

bool SD_initial()
{
	bool  ret ;
	unsigned char pres[32] ;

	SD_ReadLoop(147);
	SD_SendCommand(0,0);
	SD_ReadLoop(8);
	
	SD_SendCommand(55,0);			// 指示下一个脕E�媲app脕E丒
	ret = SD_R16Response(pres);
	if((!ret) || pres[0] != 55)
		return false; 

	SD_SendCommandRS3(41, pres, 0x0);		//紒E獾缪狗段�
	do
	{
		ret = SD_SendCommandRS(55,0);
		if(ret)
			ret = SD_SendCommandRS3(41, pres, 0xFC0000);
	} while((!ret) || (pres[1] != 0x80)) ;

	SD_SendCommand(2,0);			//Get CID 
	ret = SD_R2Response(pres);

	do
	{
		SD_SendCommand(3,0);		//进葋Stand by 状态，并得到sd卡状态
		ret = SD_R16Response(pres);
	} while((!ret) || ((pres[3] & 0x1E) != 0x6)); 	//stand by state
	
	SDadd = pres[1] * 0x100 + pres[2] ;

	ret = SD_SendCommandRS(7,SDadd<<16);	//select card

	ret = SD_SendCommandRS(55,SDadd<<16); 	//app command

	ret = SD_SendCommandRS(6,2);		//00, 1 bit , 10  4 bit 

	ret = SD_SendCommandRS(16,0x200) ;	//设定一个block为512大小

	return ret ;
}

