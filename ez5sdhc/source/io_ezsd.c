/* EZ5 plus SDHC DLDI driver */

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "io_EZSD.h"
//#include <nds.h>

#define CARD_DATA CARD_CR2
#define WAIT_CR  (*(vuint16*)0x4000204)
#define CARD_CR1H      (*(vuint8*)0x040001A1)
#define CARD_CR2       (*(vuint32*)0x040001A4)
#define CARD_COMMAND   ((vuint8*)0x040001A8)
#define CARD_DATA_RD   (*(vuint32*)0x04100010)

#define CARD_CR1_ENABLE  0x80  // in byte 1, i.e. 0x8000
#define CARD_CR1_IRQ     0x40  // in byte 1, i.e. 0x4000
#define CARD_BUSY       (1<<31)  // when reading, still expecting incomming data?
#define CARD_DATA_READY (1<<23)  // when reading, CARD_DATA_RD or CARD_DATA has another word of data and is good to go

uint32 status=0,flags = 0x00586000;
extern void SD_cal_crc16(unsigned char*p1,unsigned int s,unsigned char*p2);

u32 SDadd; //此全局变量为沟通的SD地址
// some variables statically allocated by the build rather than built onto stack space, for unaligned writes - cory1492
__attribute__((aligned(32))) static u8 pbuf[528];
__attribute__((aligned(32))) static u8 crcbuf[16];
static uint8 command[8];

// let's not link to external lib when we can help it....
static inline void cardWriteCommand(void)
{
	int index;
	WAIT_CR &= ~0x0800;

	CARD_CR1H = CARD_CR1_ENABLE | CARD_CR1_IRQ;

	for (index = 0; index < 8; index++)
	{
		CARD_COMMAND[7-index] = command[index];
	}
}

// let's not link to external lib when we can help it.... this is all that is stopping LDFLAGS from allowing -nostdlib, and it's only used on unaligned writes
// adapted from the solaris implementation
static inline void* __memcpy(void* as1, const void* as2, unsigned int n)
{
	u8* s1 = (u8*)as1;
	const u8* s2 = (u8*)as2;

	while (n-- > 0)
		*s1++ = *s2++;
	return(as1);
}

void delay(int times)
{
	int i;
	for( i=0;i<times;i++)
	{
		i++;
		i--;
	}
}

static inline void SD_SendCommand (int type , unsigned int param )
{
	DWORD temp;
	command[0]= (param)&0xFF;
	command[1]= (param>>8)&0xFF;
	command[2]= (param>>16)&0xFF;
	command[3]= (param>>24)&0xFF;
	command[4]= 0x40 | (type&0xFF);
	command[5]= 0x00;
	command[6]= 0xFA;
	command[7]= 0xB8;
	cardWriteCommand();
	CARD_CR2 = 0xA7586000;

	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));

	temp = CARD_DATA_RD;
	delay(0x200);
}

bool SD_ReadSingleBlock(unsigned int address , unsigned char *ppbuf, int len)
{
	int wait = 0x40000;
	uint32 i = 0;
	uint32 temp = 0;
	uint32 target = 512;
	unsigned char *p;
	p = (unsigned char *)(&temp);	

	SD_SendCommand(17,address); // single block  read , parm = address
	//等待起始标志位置
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x01;
	command[6]= 0xFA;
	command[7]= 0xB8;

	do{
		cardWriteCommand();
		CARD_CR2 = 0xA7586000;

		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));

		temp = CARD_DATA_RD;
		wait --;
		if(wait<0)
		{
			while(1); // this could be the problem with some microSD cards??
		}
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
	cardWriteCommand();
	CARD_CR2 = 0xA1586000;

	// test if ppbuf is 4byte-aligned - ps2aich's code from ez5s driver
	if ((((uint32) ppbuf) & 0x3) )
	{
		// ppbuf is non-aligned
		do {
			// Read data if available
			if (CARD_CR2 & CARD_DATA_READY) {
				if (i< target)
				{
					temp = CARD_DATA_RD;
					ppbuf[i] = *p;
					ppbuf[i+1] = *(p+1);
					ppbuf[i+2] = *(p+2);
					ppbuf[i+3] = *(p+3);
				}
				i+=4;
			}
		} while (CARD_CR2 & CARD_BUSY);
	}
	else
	{
		// ppbuf is aligned
		do {
			// Read data if available
			if (CARD_CR2 & CARD_DATA_READY) {
				if (i< target)
				{
					*((uint32 *)(&ppbuf[i])) = CARD_DATA_RD;
				}
				i+=4;
			}
		} while (CARD_CR2 & CARD_BUSY);
	}	
	return true;
}

bool SD_ReadMultiBlock(unsigned int address , unsigned char *ppbuf, int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		SD_ReadSingleBlock((address+i),ppbuf+i*0x200,0x200);
	}
	return true;	
}

/*
// return and ppbuf data was always discarded so it's been omitted, only used in a couple places in the driver - cory1492
static inline void SD_ReadResponse(void)
{
	uint32 temp;
	uint8 *p;
//	unsigned char ppbuf[8];

	int wait = 0x10000; // original driver provided support for variable wait periods - cory1492
	int counterFA01=0;
	p = (uint8 *)(&temp);
	
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x01;
	command[6]= 0xFA;
	command[7]= 0xB8;

	do{
		cardWriteCommand();
		CARD_CR2 = 0xa7586000;

		do
		{
			status = CARD_CR2; //0x40001a4
			wait --;
			if(wait<0)
				return;
		}while(!(status & 0x800000));

		temp = CARD_DATA_RD;
		counterFA01++;
		if(counterFA01>4)
			return;
	}while(temp & 0x000000FF);

	ppbuf[0] = p[1];
	ppbuf[1] = p[2];
	ppbuf[2] = p[3];
	
	// len is always 6, original driver provided support for 6 and 17
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x02;
	command[6]= 0xFA;
	command[7]= 0xB8;
	cardWriteCommand();
	CARD_CR2 = 0xA7586000;

	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));

	temp = CARD_DATA_RD;
	ppbuf[3] = p[0];
	ppbuf[4] = p[1];
	ppbuf[5] = p[2];
	ppbuf[6] = p[3];
	return;
}
*/
// this is only used once, in the function directly below, response is always discarded - cory1492
static inline void SD_WaitOK()
{
	//等待起始标志位置
	uint32 temp,temp1;
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x01;
	command[6]= 0xFA;
	command[7]= 0xB8;
	
	do{
		cardWriteCommand();
		flags = 0x00586000;
		flags &= (~0x07000000);
		flags |= 0xa7000000;
		flags &= 0xFFFFE000;
		CARD_CR2 = flags;
		
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;	
	}while(temp & 0x000000FF);

	return;
}

// wait was never used... - cory1492
static inline void SD_WriteData(unsigned char *ppbuf)
{
	uint32 temp,temp1;
	int i=0;
	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0xF0;
	command[3]= 0xFF;
	command[4]= 0xFF;
	command[5]= 0xFF;
	command[6]= 0xF6;
	command[7]= 0xB8;
	cardWriteCommand();
	flags = 0x00586000;
	flags &= (~0x07000000);
	flags |= 0xa7000000;
	flags &= 0xFFFFE000;
	CARD_CR2 = flags;

	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));

	temp = CARD_DATA_RD;
	temp1= CARD_DATA;

	// write block data
	for(i=0;i<512;i+=2)
	{
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= ppbuf[i+1] | 0xF0;
		command[3]= (ppbuf[i+1]>>4) | 0xF0;
		command[4]= ppbuf[i] | 0xF0;
		command[5]= (ppbuf[i]>>4) | 0xF0;
		command[6]= 0xF6;
		command[7]= 0xB8;
		cardWriteCommand();
		flags = 0x00586000;
		flags &= (~0x07000000);
		flags |= 0xa7000000;
		flags &= 0xFFFFE000;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;
	}
	// write CRC data
	for(i=0;i<8;i+=2)
	{
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= crcbuf[i+1] | 0xF0;
		command[3]= (crcbuf[i+1]>>4) | 0xF0;
		command[4]= crcbuf[i] | 0xF0;
		command[5]= (crcbuf[i]>>4) | 0xF0;
		command[6]= 0xF6; 
		command[7]= 0xB8;
	  	cardWriteCommand();
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
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= 0x00;
		command[3]= 0x00;
		command[4]= 0x00;
		command[5]= 0x00;
		command[6]= 0xF8;
		command[7]= 0xB8;
		cardWriteCommand();
		flags = 0x00586000;
		flags &= (~0x07000000);
		flags |= 0xa7000000;
		flags &= 0xFFFFE000;
		CARD_CR2 = flags;

		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));

		temp = CARD_DATA_RD;
		temp1= CARD_DATA;
	}while(temp & 0x00000001);

	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x00;
	command[6]= 0xF8;
	command[7]= 0xB8;
	cardWriteCommand();
	flags = 0x00586000;
	flags &= (~0x07000000);
	flags |= 0xa7000000;
	flags &= 0xFFFFE000;
	CARD_CR2 = flags;

	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));
	temp = CARD_DATA_RD;
	temp1= CARD_DATA;
	
	do
	{
		command[0]= 0x00;
		command[1]= 0x00;
		command[2]= 0x00;
		command[3]= 0x00;
		command[4]= 0x00;
		command[5]= 0x00;
		command[6]= 0xF8;
		command[7]= 0xB8;
		cardWriteCommand();
		flags = 0x00586000;
		flags &= (~0x07000000);
		flags |= 0xa7000000;
		flags &= 0xFFFFE000;
		CARD_CR2 = flags;
		do
		{
			status = CARD_CR2; //0x40001a4
		}while(!(status & 0x800000));
		temp = CARD_DATA_RD;
		temp1= CARD_DATA;
	}while((temp & 0x00000001)!=0x01);
}

bool SD_WriteSingleBlock(unsigned int address , unsigned char *ppbuf, int len)
{
	uint32 status;
	unsigned char* tbuf;
	// test if ppbuf is 4byte-aligned
	if ((((uint32) ppbuf) & 0x3) )
	{
		// is unaligned
		__memcpy(pbuf, ppbuf, 512);
		tbuf = pbuf;
	}
	else // is aligned
	{
		tbuf = ppbuf;
	}
	SD_cal_crc16(tbuf, 512, crcbuf);

	SD_SendCommand(24,address);	
	SD_WaitOK();//SD_ReadResponse();//SD_R16Response(pres,0);

	command[0]= 0x00;
	command[1]= 0x00;
	command[2]= 0x00;
	command[3]= 0x00;
	command[4]= 0x00;
	command[5]= 0x01;
	command[6]= 0xFA;
	command[7]= 0xB8;
	cardWriteCommand();
	CARD_CR2 = 0xa7586000;

	do
	{
		status = CARD_CR2; //0x40001a4
	}while(!(status & 0x800000));
	status = CARD_DATA_RD;
	
	SD_WriteData(tbuf);
	SD_WaitOK();
	return true;
}

bool SD_initial()
{
	return true;
}

bool EZSD_read1sector(u32 sectorn,u32 TAddr)
{
	return true;
}

bool EZSD_write1sector(u32 sectorn,u32 TAddr)
{
	return true;
}

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
	SD_ReadMultiBlock(sector,(u8*)buffer,numSecs);
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
		SD_WriteSingleBlock((i+sector),(u8*)buffer+i*512,0x200);
	}
	return true;
}

/*-----------------------------------------------------------------
EZ_Unlock
Returns true if EZ was unlocked, false if failed
Added by MightyMax
-----------------------------------------------------------------*/
bool EZSD_Shutdown(void)
{
	return true;
};

bool EZSD_StartUp(void)
{
	return SD_initial();
};

