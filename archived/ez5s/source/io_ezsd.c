/*
	This software is completely free. No warranty is provided.
	If you use it, please give me credit and email me about your
	project at chishm@hotmail.com

	See gba_nds_fat.txt for help and license details.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_EZSD.h"
#include <nds.h>

#define CARD_DATA CARD_CR2

uint32 status=0,flags = 0x00586000;

/********************************************************************************/
void		OpenWrite()
{
}

void		CloseWrite()
{
}

void  SetNandControl(uint16  control)
{
}
/********************************************************************************/

u32 SDadd ; //此全局变量为沟通的SD地址


//Function:	CRC:X^16+X^12+X^5+1
unsigned int cal_crc_CCITT(unsigned char *ptr,unsigned char len)
{
	unsigned char i;
	unsigned int  crc=0;
	while(len--!=0)
	{
		for(i=0x80;i!=0;i/=2)
		{
			if((crc & 0x8000)!=0)
			{
				crc*=2;
				crc^=0x1021;
			}		
			else
			{
				crc*=2;
			}
			if((*ptr & i)!=0)
				crc ^= 0x1021;
		}
		ptr++;
	}
	return crc;
}

void  SD_SendCommand (int type , unsigned int param )
{
	uint8 command[8];
	DWORD temp;
	command[0]= (param)&0xFF;
	command[1]= (param>>8)&0xFF;
	command[2]= (param>>16)&0xFF;
	command[3]= (param>>24)&0xFF;
	command[4]= 0x40 | (type&0xFF);
	command[5]= 0x00;
	command[6]= 0xFA; 
	command[7]= 0xB8;


	cardWriteCommand(command);
  	flags = 0x00586000;
  	flags &= (~0x07000000);
	flags |= 0xa7000000 ;
	flags &= 0xFFFFE000 ;
	CARD_CR2 = flags ;
	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));
  	temp = CARD_DATA_RD;
}

bool SD_R16Response(unsigned char *ppbuf, int wait)
{
	return SD_ReadResponse(ppbuf,6,0x10000);
}

bool SD_R2Response(unsigned char *ppbuf, int wait)
{
  	return SD_ReadResponse(ppbuf,17,0x10000);
}

bool SD_R3Response(unsigned char *ppbuf, int wait)
{
	bool ret ;
	ret = SD_ReadResponse(ppbuf,6,0x10000);
	if(ret)
	{
		if((ppbuf[0]!=0x3F)&&(ppbuf[5]!=0xFF))
			return false ;
		return true ;
	}
	return false ;
}
bool SD_ReadSingleBlock(unsigned int address , unsigned char *ppbuf, int len)
{
	SD_SendCommand(17,address); // single block  read , parm = address
	SD_ReadData(ppbuf,len,0x100000);
	return true ;

}

bool SD_ReadMultiBlock(unsigned int address , unsigned char *ppbuf, int len)
{
	int i;
	for(i=0;i<len;i+=0x200)
	{
		SD_ReadSingleBlock(address+i,ppbuf+i,0x200);
	}
	return true;	
}

bool SD_ReadResponse(unsigned char *ppbuf,int len,int wait)
{
	//int i ;
	uint32 temp,temp1;
	uint8 command[8];
	uint8 *p;
	p = (uint8 *)(&temp);
	
	//等待起始标志位置
	REG_EXMEMCNT &= ~0x0800;
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x01;
	command[6]= 0xFA;
	command[7]= 0xB8;
	do{
		cardWriteCommand(command);
		flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;		
	}while(temp & 0x000000FF);
	ppbuf[0] = p[1];
	ppbuf[1] = p[2];
	ppbuf[2] = p[3];
	
	
	if(len==6)
	{
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= 0x00;
		command[3]= 0x00;
		command[4]= 0x00;
		command[5]= 0x02;
		command[6]= 0xFA;
		command[7]= 0xB8;
		cardWriteCommand(command);
		flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;		
		ppbuf[3] = p[0];
		ppbuf[4] = p[1];
		ppbuf[5] = p[2];
		ppbuf[6] = p[3];
	}
	else if(len==17)
	{	
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= 0x00;
		command[3]= 0x00;
		command[4]= 0x00;
		command[5]= 0x02;
		command[6]= 0xFA;
		command[7]= 0xB8;
		cardWriteCommand(command);
		flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;
		ppbuf[3] = p[0];
		ppbuf[4] = p[1];
		ppbuf[5] = p[2];
		ppbuf[6] = p[3];
		
		
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= 0x00;
		command[3]= 0x00;
		command[4]= 0x00;
		command[5]= 0x03;
		command[6]= 0xFA;
		command[7]= 0xB8;
		cardWriteCommand(command);
		flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;
		ppbuf[7] = p[0];
		ppbuf[8] = p[1];
		ppbuf[9] = p[2];
		ppbuf[10] = p[3];
		
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= 0x00;
		command[3]= 0x00;
		command[4]= 0x00;
		command[5]= 0x04;
		command[6]= 0xFA;
		command[7]= 0xB8;
		cardWriteCommand(command);
		flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;
		ppbuf[11] = p[0];
		ppbuf[12] = p[1];
		ppbuf[13] = p[2];
		ppbuf[14] = p[3];
		
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= 0x00;
		command[3]= 0x00;
		command[4]= 0x00;
		command[5]= 0x05;
		command[6]= 0xFA;
		command[7]= 0xB8;
		cardWriteCommand(command);
		flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;
		ppbuf[15] = p[0];
		ppbuf[16] = p[1];
	}	
	return true;
}

void SD_ReadData(unsigned char *ppbuf, int len,int wait)
{
	uint32 i ;
	uint32 temp=0;
	uint32 temp1;
	uint8 command[8];
	uint32 target = 512;
	unsigned char *p;
	p = (unsigned char *)(&temp);

	//等待起始标志位置
	REG_EXMEMCNT &= ~0x0800;
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x01;
	command[6]= 0xFA;
	command[7]= 0xB8;
	do{
		cardWriteCommand(command);
		flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;		
	}while(temp & 0x000000FF);
	//读512 Byte数据
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x00;
	command[6]= 0xF7;
	command[7]= 0xB8;
	cardWriteCommand(command);
	flags = 0x00586000;
  	flags &= (~0x00000000);
	flags |= 0xa0000000 ;
	flags &= 0xFFFFE000 ;
	flags |= 0x01000000 ;
	CARD_CR2 = flags ;
	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));
	i=0;
	
  	do {
		// Read data if available
    		if (CARD_CR2 & CARD_DATA_READY) {
		  		if (i< target) 
				{
      				temp = CARD_DATA_RD;
      				temp1= CARD_DATA;
      				ppbuf[i] = *p;
      				ppbuf[i+1] = *(p+1);
      				ppbuf[i+2] = *(p+2);
      				ppbuf[i+3] = *(p+3);
				// *((uint32 *)(&ppbuf[i])) = CARD_DATA_RD;
      			}
      			i+=4;
    		}
  	} while (CARD_CR2 & CARD_BUSY);
	

	/*
	for(i=0;i<512;i+=4)
	{
		temp = CARD_DATA_RD;
      	temp1= CARD_DATA;
      	ppbuf[i] = *p;
      	ppbuf[i+1] = *(p+1);
      	ppbuf[i+2] = *(p+2);
      	ppbuf[i+3] = *(p+3);
	}
	*/
}

bool SD_GetCSDStruct(unsigned char * ppbuf , int len , SD_CSD *pCsd)
{
	//const float fay[]={0 , 1.0 , 1.2 , 1.3 , 1.5 , 2.0 , 2.5 , 3.0 , 3.5 , 4.0 , 4.5 , 5.0 , 5.5 , 6.0 , 7.0 , 8.0 };
	if(!ppbuf) return false;
	if(len!=17) return false ;
	SD_CSDREAL *preal ;
	preal = (SD_CSDREAL *)&ppbuf[1] ;
	pCsd->CSDStruct = preal->CSD_STRUCTURE ;
	pCsd->Taac.TimeUnit = (CSD_TAAC_TIMEUNIT)((preal->TAAC)&0x7);
	pCsd->Taac.fTimeValue = (preal->TAAC&0x7F)>>3 ; 
	pCsd->byNsac = preal->NSAC ;
	pCsd->TranSpeed.transfer_rate = (CSD_TRAN_SPEEDUNIT)((preal->TRAN_SPEED)&0x7) ; 
	pCsd->TranSpeed.fTimeValue = (preal->TRAN_SPEED&0x7F)>>3 ;
	pCsd->wCCC = (preal->CCC_H)*0x10 +  (preal->CCC_L) ;
	pCsd->bRead_Bl_Len = preal->READ_BL_LEN ;
	pCsd->bRead_Bl_Partial = preal->READ_BL_PARTIAL ;
	pCsd->bWrite_Blk_Misalign = preal->WRITE_BLK_MISALIGN ;
	pCsd->bRead_Blk_Misalign  = preal->READ_BLK_MISALIGN ;
	pCsd->bDsr_Imp = preal->DSR_IMP ;
	pCsd->wC_Size = ((preal->C_SIZE_H2)<<10)+((preal->C_SIZE_H1)<<2)+preal->C_SIZE_L ;
	pCsd->byVdd_R_Curr_Min = preal->VDD_R_CURR_MIN ;
	pCsd->byVdd_R_Curr_Max = preal->VDD_R_CURR_MAX ;
	pCsd->byVdd_W_Curr_Min = preal->VDD_W_CURR_MIN;
	pCsd->byVdd_W_Curr_Max = preal->VDD_W_CURR_MAX ;
	BYTE s = ((preal->C_SIZE_MULT_H)<<2) + preal->C_SIZE_MULT_L ;
	pCsd->wC_Size_Mult = 1<<(s+2) ;
	pCsd->bErase_Blk_En = preal->ERASE_BLK_EN ;
	pCsd->bySector_Size = preal->SECTOR_SIZE_H*2 + preal->SECTOR_SIZE_L;
	pCsd->byWp_Grp_Size = preal->WP_GRP_SIZE ;
	pCsd->bWp_Grp_Enable = preal->WP_GRP_ENABLE ;
	pCsd->byR2w_Factor = preal->R2W_FACTOR ;
	pCsd->byWrite_Bl_Len = preal->WRITE_BL_LEN_H * 4 + preal->WRITE_BL_LEN_L ;
	pCsd->bWrite_Bl_Partial = preal->WRITE_BL_PARTIA ;
	pCsd->byFile_Format_Grp = preal->FILE_FORMAT_GRP ;
	pCsd->bCopy = preal->COPY ;
	pCsd->byPerm_Write_Protect = preal->PERM_WRITE_PROTECT ;
	pCsd->byTmp_Write_Protect = preal->TMP_WRITE_PROTECT ;
	pCsd->File_Format = (CSD_FILE_FORMAT)preal->FILE_FORMAT ;
	return true ;
}

bool		SD_GetCIDStruct(unsigned char * ppbuf , int len , SD_CID *pCid)
{
	BYTE *p;
	if(!ppbuf)	return false ;
	if(len != 17) return false ;
	memset(pCid,0,sizeof(SD_CID));
	p = &ppbuf[0];
	pCid->byManufacturerID = p[0];
	pCid->wOemID = *((WORD *) &p[1]);
	memcpy(pCid->szProductName,&p[3],5);
	pCid->byProductReision = p[8];
	pCid->dwProductSn = *((DWORD *) &p[9]);
	pCid->wData = *((WORD *) &p[13]);
	pCid->byCRC = (p[15] & 0xFE);
	//this is just a test 
	return true;
	
}
void SD_ReadLoop(int lp)
{
	uint32 i ;
	uint32 temp,temp1;
	uint32 target = 0x80;
	REG_EXMEMCNT &= ~0x0800;
	uint8 command[8];
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x00;
	command[6]= 0xF8;
	command[7]= 0xB8;
	cardWriteCommand(command);
	flags = 0x00586000;
  	flags &= (~0x00000000);
	flags |= 0xa0000000 ;
	flags &= 0xFFFFE000 ;
	flags |= 0x01000000 ;
	CARD_CR2 = flags ;
	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));
  	i=0;
  	do {
    		// Read data if available
    		if (CARD_CR2 & CARD_DATA_READY) {
      			if (i< target) 
      			{
      				temp = CARD_DATA_RD;
      				temp1= CARD_DATA;
      			}
      			i++;
    		}
  	} while (CARD_CR2 & CARD_BUSY);
  	temp1= CARD_DATA;

}
bool SD_WaitOK()
{
	//等待起始标志位置
	uint32 temp,temp1;
	REG_EXMEMCNT &= ~0x0800;
	uint8 command[8];
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x01;
	command[6]= 0xFA;
	command[7]= 0xB8;
	do{
		cardWriteCommand(command);
		flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;	
	}while(temp & 0x000000FF);
	return true;
}
bool	SD_WriteSingleBlock(unsigned int address , unsigned char *ppbuf, int len)
{
	unsigned short w1,w2,w3,w4 ;
	unsigned short b1,b2,b3,b4 ;
	unsigned short rw1,rw2,rw3,rw4 ;
	unsigned char ptmp[144];
	unsigned char pbuf[1024+64] ;
	register unsigned int i ;
	
	memcpy(pbuf,ppbuf,512);
	for(i=0;i<512;i+=4)
		ptmp[i>>2] = (pbuf[i]&0x80) + ((pbuf[i+1]&0x80)>>2)+ ((pbuf[i+2]&0x80)>>4) + ((pbuf[i+3]&0x80)>>6) 
				+ ((pbuf[i]&0x8)<<3) + ((pbuf[i+1]&0x8)<<1)+ ((pbuf[i+2]&0x8)>>1) + ((pbuf[i+3]&0x8)>>3) ;
	b1 = cal_crc_CCITT(ptmp,128);
	for(i=0;i<512;i+=4)
		ptmp[i>>2] = ((pbuf[i]&0x40)<<1) + ((pbuf[i+1]&0x40)>>1)+ ((pbuf[i+2]&0x40)>>3) + ((pbuf[i+3]&0x40)>>5) 
				+ ((pbuf[i]&0x4)<<4) + ((pbuf[i+1]&0x4)<<2)+ ((pbuf[i+2]&0x4)) + ((pbuf[i+3]&0x4)>>2) ;
	b2 = cal_crc_CCITT(ptmp,128);
	for(i=0;i<512;i+=4)
		ptmp[i>>2] =  ((pbuf[i]&0x20)<<2) + ((pbuf[i+1]&0x20))+ ((pbuf[i+2]&0x20)>>2) + ((pbuf[i+3]&0x20)>>4) 
				+ ((pbuf[i]&0x2)<<5) + ((pbuf[i+1]&0x2)<<3)+ ((pbuf[i+2]&0x2)<<1) + ((pbuf[i+3]&0x2)>>1) ;
	b3 = cal_crc_CCITT(ptmp,128);
	for(i=0;i<512;i+=4)
		ptmp[i>>2] = ((pbuf[i]&0x10)<<3) + ((pbuf[i+1]&0x10)<<1)+ ((pbuf[i+2]&0x10)>>1) + ((pbuf[i+3]&0x10)>>3) 
				+ ((pbuf[i]&0x1)<<6) + ((pbuf[i+1]&0x1)<<4)+ ((pbuf[i+2]&0x1)<<2) + ((pbuf[i+3]&0x1)) ;
	b4 = cal_crc_CCITT(ptmp,128);
	w1=b1>>8 ;
	w2=b2>>8 ;
	w3=b3>>8 ;
	w4=b4>>8 ;	
	rw1 = ((w1&0x80))+((w2&0x80)>>1)+((w3&0x80)>>2)+((w4&0x80)>>3)
		+((w1&0x40)>>3)+((w2&0x40)>>4)+((w3&0x40)>>5)+((w4&0x40)>>6) ;
	rw2 = ((w1&0x20)<<2)+((w2&0x20)<<1)+((w3&0x20))+((w4&0x20)>>1)
		+((w1&0x10)>>1)+((w2&0x10)>>2)+((w3&0x10)>>3)+((w4&0x10)>>4) ;
	rw3 = ((w1&0x8)<<4)+((w2&0x8)<<3)+((w3&0x8)<<2)+((w4&0x8)<<1)
		+((w1&0x4)<<1)+((w2&0x4))+((w3&0x4)>>1)+((w4&0x4)>>2) ;
 	rw4 = ((w1&0x2)<<6)+((w2&0x2)<<5)+((w3&0x2)<<4)+((w4&0x2)<<3)
		+((w1&0x1)<<3)+((w2&0x1)<<2)+((w3&0x1)<<1)+((w4&0x1)) ;
	pbuf[512] = rw1 ;
	pbuf[513] = rw2 ;
	pbuf[514] = rw3 ;
	pbuf[515] = rw4 ;
	w1=b1 ;
	w2=b2 ;
	w3=b3 ;
	w4=b4 ;	
	rw1 = ((w1&0x80))+((w2&0x80)>>1)+((w3&0x80)>>2)+((w4&0x80)>>3)
		+((w1&0x40)>>3)+((w2&0x40)>>4)+((w3&0x40)>>5)+((w4&0x40)>>6) ;
	rw2 = ((w1&0x20)<<2)+((w2&0x20)<<1)+((w3&0x20))+((w4&0x20)>>1)
		+((w1&0x10)>>1)+((w2&0x10)>>2)+((w3&0x10)>>3)+((w4&0x10)>>4) ;
	rw3 = ((w1&0x8)<<4)+((w2&0x8)<<3)+((w3&0x8)<<2)+((w4&0x8)<<1)
		+((w1&0x4)<<1)+((w2&0x4))+((w3&0x4)>>1)+((w4&0x4)>>2) ;
 	rw4 = ((w1&0x2)<<6)+((w2&0x2)<<5)+((w3&0x2)<<4)+((w4&0x2)<<3)
		+((w1&0x1)<<3)+((w2&0x1)<<2)+((w3&0x1)<<1)+((w4&0x1)) ;
	//应该做四次循环
	pbuf[516] = rw1 ;
	pbuf[517] = rw2 ;
	pbuf[518] = rw3 ;
	pbuf[519] = rw4 ;

//	ppbuf[512] = pbuf[512];
//	ppbuf[513] = pbuf[513];
//	ppbuf[514] = pbuf[514];
//	ppbuf[515] = pbuf[515];
//	ppbuf[516] = pbuf[516];
//	ppbuf[517] = pbuf[517];
//	ppbuf[518] = pbuf[518];
//	ppbuf[519] = pbuf[519];

//	unsigned char p[20];
	SD_SendCommand(24,address);
	SD_WaitOK();
	SD_WriteData(pbuf,520,0x10000);
	//SD_ReadLoop(8);
	return true ;
}

void SD_WriteData(unsigned char *ppbuf, int len,int wait)
{
	uint32 temp,temp1;
	//BYTE ii;
	REG_EXMEMCNT &= ~0x0800;
	uint8 command[8];
	int i=0;
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0xF0;
	command[3]= 0xFF;
	command[4]= 0xFF;
	command[5]= 0xFF;
	command[6]= 0xF6; 
	command[7]= 0xB8;
  	cardWriteCommand(command);
  	flags = 0x00586000;
  	flags &= (~0x07000000);
	flags |= 0xa7000000 ;
	flags &= 0xFFFFE000 ;
	CARD_CR2 = flags ;
	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));
  	temp = CARD_DATA_RD;
  	temp1= CARD_DATA;

	for(i=0;i<len;i+=2)
	{
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= ppbuf[i+1] | 0xF0;
		command[3]= (ppbuf[i+1]>>4) | 0xF0;
		command[4]= ppbuf[i] | 0xF0;
		command[5]= (ppbuf[i]>>4) | 0xF0;
		command[6]= 0xF6; 
		command[7]= 0xB8;
	  	cardWriteCommand(command);
	  	flags = 0x00586000;
	  	flags &= (~0x07000000);
		flags |= 0xa7000000 ;
		flags &= 0xFFFFE000 ;
		CARD_CR2 = flags ;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
	  	temp = CARD_DATA_RD;
	  	temp1= CARD_DATA;
	}
	do
	{
			REG_EXMEMCNT &= ~0x0800;
			command[0]= 0x00;
			command[1]= 0x00;
			command[2]= 0x00;
			command[3]= 0x00;
			command[4]= 0x00;
			command[5]= 0x00;
			command[6]= 0xF8;
			command[7]= 0xB8;
			cardWriteCommand(command);
			flags = 0x00586000;
		  	flags &= (~0x07000000);
			flags |= 0xa7000000 ;
			flags &= 0xFFFFE000 ;
			CARD_CR2 = flags ;
			do
			{
				status = CARD_CR2; //0x40001a4
			}while(!(status & 0x800000));
	  		temp = CARD_DATA_RD;
	  		temp1= CARD_DATA;
	}while(temp & 0x00000001);

//读CRC状态
	REG_EXMEMCNT &= ~0x0800;
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x00;
	command[6]= 0xF8;
	command[7]= 0xB8;
	cardWriteCommand(command);
	flags = 0x00586000;
	flags &= (~0x07000000);
	flags |= 0xa7000000 ;
	flags &= 0xFFFFE000 ;
	CARD_CR2 = flags ;
	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));
	temp = CARD_DATA_RD;
	temp1= CARD_DATA;
	
	do
	{
			REG_EXMEMCNT &= ~0x0800;
			command[0]= 0x00;
			command[1]= 0x00;
			command[2]= 0x00;
			command[3]= 0x00;
			command[4]= 0x00;
			command[5]= 0x00;
			command[6]= 0xF8;
			command[7]= 0xB8;
			cardWriteCommand(command);
			flags = 0x00586000;
		  	flags &= (~0x07000000);
			flags |= 0xa7000000 ;
			flags &= 0xFFFFE000 ;
			CARD_CR2 = flags ;
			do
			{
				status = CARD_CR2; //0x40001a4
			}while(!(status & 0x800000));
	  		temp = CARD_DATA_RD;
	  		temp1= CARD_DATA;
	}while((temp & 0x00000001)!=0x01);
}

bool SD_initial()
{
	bool  ret ;
	unsigned char pres[40] ;
	SD_CID	cid ;	
	SD_CSD csd ;
	
	SD_ReadLoop(147);
	SD_SendCommand(0,0);
	SD_ReadLoop(147);
	
//	for(counter=0;counter<4000;counter++)

	SD_SendCommand(55,0);   // 指示下一个命令是app命令
	ret = SD_R16Response(pres,0);	
	if(ret)
	{
		SD_SendCommand(41,0x0); //检测电压范围
		ret = SD_R16Response(pres,0);
		do
		{
			SD_SendCommand(55,0);
			ret = SD_R16Response(pres,0);
			SD_SendCommand(41,0xFC0000);
			ret = SD_R3Response(pres,0);
		}while(pres[1]!=0x80) ;
	}
	if(ret)
	{
		SD_SendCommand(2,0);	//Get CID 
		ret = SD_R2Response(pres,0);
		SD_GetCIDStruct(pres,17,&cid);
	}
	if(ret)
	{
		do
		{
			SD_SendCommand(3,0);		//进入stand by 状态，并得到sd卡状态
			ret = SD_R16Response(pres,0);
		}while((pres[3]&0x1E) != 0x6); //stand by state
	}
	if(ret)
	{
		SDadd = pres[1]*0x100 + pres[2] ;
		SD_SendCommand(9,(SDadd<<16));//send_csd
		ret = SD_R2Response(pres,0);
		SD_GetCSDStruct(pres,17,&csd);
	}
	if(ret)
	{
		SD_SendCommand(7,SDadd<<16);	//select card
		ret = SD_R16Response(pres,0);	
	}
	if(ret)
	{
		SD_SendCommand(55,SDadd<<16); //app command
		ret = SD_R16Response(pres,0);
		SD_SendCommand(6,2);		//00, 1 bit , 10  4 bit 
		ret = SD_R16Response(pres,0);
	}
	if(ret)
	{
		SD_SendCommand(16,0x200) ;	//设定一个block为512大小
		ret = SD_R16Response(pres,0);
	}
	return true;
}


//======================================================
bool EZSD_read1sector(u32 sectorn,u32 TAddr)
{
	
	return true;
	
} 
//==================================================


//======================================================
bool EZSD_write1sector(u32 sectorn,u32 TAddr)
{
	return true;
} 
//==================================================

/*-----------------------------------------------------------------
EZSD_IsInserted
Is a compact flash card inserted?
bool return OUT:  true if a CF card is inserted
-----------------------------------------------------------------*/
bool EZSD_IsInserted (void) 
{
	return true;
}


/*-----------------------------------------------------------------
EZSD_ClearStatus
Tries to make the CF card go back to idle mode
bool return OUT:  true if a CF card is idle
-----------------------------------------------------------------*/
bool EZSD_ClearStatus (void) 
{
	return true;
}


/*-----------------------------------------------------------------
EZSD_ReadSectors
Read 512 byte sector numbered "sector" into "buffer"
u32 sector IN: address of first 512 byte sector on CF card to read
u8 numSecs IN: number of 512 byte sectors to read,
 1 to 256 sectors can be read, 0 = 256
void* buffer OUT: pointer to 512 byte buffer to store data in
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool EZSD_ReadSectors (u32 sector, u8 numSecs, void* buffer)
{
	SD_ReadMultiBlock(sector*0x200,buffer,numSecs*0x200);
	return true;
}

/*-----------------------------------------------------------------
EZSD_WriteSectors
Write 512 byte sector numbered "sector" from "buffer"
u32 sector IN: address of 512 byte sector on CF card to read
u8 numSecs IN: number of 512 byte sectors to read,
 1 to 256 sectors can be read, 0 = 256
void* buffer IN: pointer to 512 byte buffer to read data from
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool EZSD_WriteSectors (u32 sector, u8 numSecs, void* buffer)
{
	int i;
	for(i=0;i<numSecs;i++)
	{
		SD_WriteSingleBlock( (i+sector)*512,buffer+i*512,0x200);
	}
	return true;
}

/*-----------------------------------------------------------------
EZ_Unlock
Returns true if EZ was unlocked, false if failed
Added by MightyMax
-----------------------------------------------------------------*/
bool EZSD_Shutdown(void) {
	return true;
} ;

bool EZSD_StartUp(void) {
	//init sd 
	return SD_initial();
} ;
