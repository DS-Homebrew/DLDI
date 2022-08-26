/* EZ5 DLDI driver */

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "io_ez5.h"
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
extern unsigned char cal_crc_730(unsigned char *ptr,unsigned char len);
extern void startshow(void);


//static bool  bSDHC =false;
static bool  bSDHC =false;
static uint32 SDadd ;//¥À»´æ÷±‰¡øŒ™πµÕ®µƒSDµÿ÷∑
unsigned int ss;

static unsigned short CRC16Table[ 256 ] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
};

unsigned short cal_crc_CCITT(unsigned char *buff,unsigned char size){
	unsigned short crc16=0;
	for(;size;size--){
		crc16 = CRC16Table[ (crc16 >> 8) ^ *buff++ ] ^ (crc16 << 8);
	}
	return crc16;
}

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

//------------------------------------------------------
void inline Enable_Arm9DS()
{
    WAIT_CR &= ~0x0880;
}

void inline Enable_Arm7DS()
{
    WAIT_CR |= 0x0880;
}

//---------------------------------------------------------------------------------
void dsCardi_SetRomOP(uint8 * command) 
{
    uint32 status ,index;
    do
    {
        status = CARD_CR2 ;
    }while(status&0x80000000);
       
    
    CARD_CR1H = CARD_CR1_IRQ|CARD_CR1_ENABLE ;

    for (index = 0; index < 8; index++) {
        CARD_COMMAND[7-index] = command[index];
    }
}
//---------------------------------------------------------
void    cardWriteCommand(uint8 * command)
{
    int index;

    CARD_CR1H = CARD_CR1_ENABLE | CARD_CR1_IRQ;

    for (index = 0; index < 8; index++) {
        CARD_COMMAND[7-index] = command[index];
    }
}
//------------------------------------------------------------------
uint32      dsCardi_Read4ByteMode(uint8 * command)
{
    uint32 status=0;
    Enable_Arm9DS();
    dsCardi_SetRomOP(command);

//…Ë÷√øÿ÷∆ºƒ¥Ê∆ÅEx40001A4
    CARD_CR2 = 0xA7586000 ;

    do{
        status = CARD_CR2; //0x40001a4
    }while(!(status & 0x800000));

    uint32 DD = CARD_DATA_RD ;
    Enable_Arm7DS();
    return DD ;

}
//--------------------------------------------------------------
uint32  SD_SendCommand (int type , unsigned int param )
{
    uint8 command[8];
    command[0]= (param)&0xFF;
    command[1]= (param>>8)&0xFF;
    command[2]= (param>>16)&0xFF;
    command[3]= (param>>24)&0xFF;
    command[4]= 0x40 | (type&0xFF);
    command[5]= 0x00;
    command[6]= 0xFA; 
    command[7]= 0xB8;
    return dsCardi_Read4ByteMode(command);
}
//---------------------------------------------

bool SD_ReadResponse(unsigned char *ppbuf,int len)
{
    uint8 command[8];
    uint32 status ;
    int counterFA01=0;
    uint8 *p = (uint8 *)(&status);
    
    //µ»¥˝∆ º±ÅEæŒª÷√
    WAIT_CR &= ~0x0800;
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= 0x00;
    command[4]= 0x00;
    command[5]= 0x01;
    command[6]= 0xFA;
    command[7]= 0xB8;
    
    do
    {
        status = dsCardi_Read4ByteMode(command);
        counterFA01++;
        if(counterFA01>99)   ///////modify 2008-12-16
            return false;
    }while(status & 0x000000FF);
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
        status = dsCardi_Read4ByteMode(command);
        
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
        status = dsCardi_Read4ByteMode(command);
        
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
        status = dsCardi_Read4ByteMode(command);
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
        status = dsCardi_Read4ByteMode(command);
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
        status = dsCardi_Read4ByteMode(command);
        ppbuf[15] = p[0];
        ppbuf[16] = p[1];
    }   
    return true;
}

bool SD_R16Response(unsigned char *ppbuf)
{
    return SD_ReadResponse(ppbuf,6);
}

//------------------------------------------
bool SD_WaitOK()
{
    //’‚∂Œ¥˙¬ÅE≠°≠√≤À∆Œﬁ”√
    uint32 status ;
    WAIT_CR &= ~0x0800;
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
        status = dsCardi_Read4ByteMode(command);
    }while(status & 0x000000FF);
    return true;
}

///-------------------------------------------------
void SD_WriteData(unsigned char *ppbuf, int len,int wait)
{
    uint32 status;
    //BYTE ii;
    WAIT_CR &= ~0x0800;
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
    status = dsCardi_Read4ByteMode(command);

    WAIT_CR &= ~0x0800;
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
        CARD_CR2 = 0xA0586000 ;
        status = CARD_CR2; //0x40001a4
    }
    do
    {
        WAIT_CR &= ~0x0800;
        command[0]= 0x00;
        command[1]= 0x00;
        command[2]= 0x00;
        command[3]= 0x00;
        command[4]= 0x00;
        command[5]= 0x00;
        command[6]= 0xF8;
        command[7]= 0xB8;
        status = dsCardi_Read4ByteMode(command);
    }while(status & 0x00000001);

//∂¡CRC◊¥Ã¨
    WAIT_CR &= ~0x0800;
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= 0x00;
    command[4]= 0x00;
    command[5]= 0x00;
    command[6]= 0xF8;
    command[7]= 0xB8;
    status = dsCardi_Read4ByteMode(command);
    
    do
    {
        WAIT_CR &= ~0x0800;
        command[0]= 0x00;
        command[1]= 0x00;
        command[2]= 0x00;
        command[3]= 0x00;
        command[4]= 0x00;
        command[5]= 0x00;
        command[6]= 0xF8;
        command[7]= 0xB8;
        status = dsCardi_Read4ByteMode(command);
    }while((status & 0x00000001)!=0x01);
}

//-------------------------------------------------------------------------------
bool    SD_WriteSingleBlock(unsigned int address , unsigned char *ppbuf, int len)
{
    unsigned char pres[40] ;

    u8  pbuf[520] __attribute__ ((aligned (4)));
    __memcpy(pbuf, ppbuf, 512);
	{
		unsigned char w1,w2,w3,w4 ;
		unsigned short b1,b2,b3,b4 ;
		//unsigned short rw1,rw2,rw3,rw4 ;
		unsigned char ptmp[128];
		register unsigned int i ;

	for(i=0;i<512;i+=4)
		ptmp[i>>2] = (pbuf[i]&0x80) | ((pbuf[i+1]&0x80)>>2)| ((pbuf[i+2]&0x80)>>4) | ((pbuf[i+3]&0x80)>>6) 
				+ ((pbuf[i]&0x8)<<3) | ((pbuf[i+1]&0x8)<<1)| ((pbuf[i+2]&0x8)>>1) | ((pbuf[i+3]&0x8)>>3) ;
	b1 = cal_crc_CCITT(ptmp,128);
	for(i=0;i<512;i+=4)
		ptmp[i>>2] = ((pbuf[i]&0x40)<<1) | ((pbuf[i+1]&0x40)>>1)| ((pbuf[i+2]&0x40)>>3) | ((pbuf[i+3]&0x40)>>5) 
				| ((pbuf[i]&0x4)<<4) | ((pbuf[i+1]&0x4)<<2)| ((pbuf[i+2]&0x4)) | ((pbuf[i+3]&0x4)>>2) ;
	b2 = cal_crc_CCITT(ptmp,128);
	for(i=0;i<512;i+=4)
		ptmp[i>>2] =  ((pbuf[i]&0x20)<<2) | ((pbuf[i+1]&0x20))| ((pbuf[i+2]&0x20)>>2) | ((pbuf[i+3]&0x20)>>4) 
				| ((pbuf[i]&0x2)<<5) | ((pbuf[i+1]&0x2)<<3)| ((pbuf[i+2]&0x2)<<1) | ((pbuf[i+3]&0x2)>>1) ;
	b3 = cal_crc_CCITT(ptmp,128);
	for(i=0;i<512;i+=4)
		ptmp[i>>2] = ((pbuf[i]&0x10)<<3) | ((pbuf[i+1]&0x10)<<1)| ((pbuf[i+2]&0x10)>>1) | ((pbuf[i+3]&0x10)>>3) 
				| ((pbuf[i]&0x1)<<6) | ((pbuf[i+1]&0x1)<<4)| ((pbuf[i+2]&0x1)<<2) | ((pbuf[i+3]&0x1)) ;
	b4 = cal_crc_CCITT(ptmp,128);
	w1=b1>>8 ;
	w2=b2>>8 ;
	w3=b3>>8 ;
	w4=b4>>8 ;
	pbuf[512] = ((w1&0x80))|((w2&0x80)>>1)|((w3&0x80)>>2)|((w4&0x80)>>3)
		|((w1&0x40)>>3)|((w2&0x40)>>4)|((w3&0x40)>>5)|((w4&0x40)>>6) ;
	pbuf[513] = ((w1&0x20)<<2)|((w2&0x20)<<1)|((w3&0x20))|((w4&0x20)>>1)
		|((w1&0x10)>>1)|((w2&0x10)>>2)|((w3&0x10)>>3)|((w4&0x10)>>4) ;
	pbuf[514] = ((w1&0x8)<<4)|((w2&0x8)<<3)|((w3&0x8)<<2)|((w4&0x8)<<1)
		|((w1&0x4)<<1)|((w2&0x4))|((w3&0x4)>>1)|((w4&0x4)>>2) ;
 	pbuf[515] = ((w1&0x2)<<6)|((w2&0x2)<<5)|((w3&0x2)<<4)|((w4&0x2)<<3)
		|((w1&0x1)<<3)|((w2&0x1)<<2)|((w3&0x1)<<1)|((w4&0x1)) ;
	w1=b1 ;
	w2=b2 ;
	w3=b3 ;
	w4=b4 ;	
	pbuf[516] = ((w1&0x80))|((w2&0x80)>>1)|((w3&0x80)>>2)|((w4&0x80)>>3)
		|((w1&0x40)>>3)|((w2&0x40)>>4)|((w3&0x40)>>5)|((w4&0x40)>>6) ;
	pbuf[517] = ((w1&0x20)<<2)|((w2&0x20)<<1)|((w3&0x20))|((w4&0x20)>>1)
		|((w1&0x10)>>1)|((w2&0x10)>>2)|((w3&0x10)>>3)|((w4&0x10)>>4) ;
	pbuf[518] = ((w1&0x8)<<4)|((w2&0x8)<<3)|((w3&0x8)<<2)|((w4&0x8)<<1)
		|((w1&0x4)<<1)|((w2&0x4))|((w3&0x4)>>1)|((w4&0x4)>>2) ;
 	pbuf[519] = ((w1&0x2)<<6)|((w2&0x2)<<5)|((w3&0x2)<<4)|((w4&0x2)<<3)
		|((w1&0x1)<<3)|((w2&0x1)<<2)|((w3&0x1)<<1)|((w4&0x1)) ;
	}

    SD_SendCommand(24,address); 
    SD_R16Response(pres); 

    //’‚¿ÅE«ÀÊ±„º”“ª∏ˆ∂¡£¨Ω‚æˆ ±º‰Œ ÃÅE
    uint8 command[8];   
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= 0x00;
    command[4]= 0x00;
    command[5]= 0x01;
    command[6]= 0xFA;
    command[7]= 0xB8;   
    dsCardi_Read4ByteMode(command);
    
    WAIT_CR &= ~0x0800;
    SD_WriteData(pbuf,520,0x10000);
    SD_WaitOK();

    return true ;
}
//--------------------------------------------------------------------
bool SD_ReadData(unsigned char *ppbuf, int len,int wait)
{
    uint32 i=0;
    uint8 command[8];
    uint32 target = 512;
    uint32 status ;
 
    //µ»¥˝∆ º±ÅEæŒª÷√
    
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= 0x00;
    command[4]= 0x00;
    command[5]= 0x01;
    command[6]= 0xFA;
    command[7]= 0xB8;
    do{
        status = dsCardi_Read4ByteMode(command);
        wait -- ;
        if(wait<0)
        {
            WAIT_CR &= 0x880 ;
            return  false;
        }
    }while(status & 0x000000FF);
    //∂¡512 Byte ˝æ›
    WAIT_CR &= ~0x0800;
  
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= 0x00;
    command[4]= 0x00;
    command[5]= 0x00;
    command[6]= 0xF7;
    command[7]= 0xB8;
    dsCardi_SetRomOP(command);
    CARD_CR2 = 0xA1586000 ;
    if((uint32)ppbuf&0x3)
    {
        uint32 temp ;
        uint8* p = (uint8*)&temp ;
        i = 0;
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
        } while (CARD_CR2 & 0x80000000);
    }
    else
    {
        i = 0;
        do {
        // Read data if available
            if (CARD_CR2 & CARD_DATA_READY) {
                if (i< target) 
                {
                    *((uint32 *)(&ppbuf[i])) = CARD_DATA_RD;
                }
                i+=4;
            }
        } while(CARD_CR2 & 0x80000000);
    }
    
    return true ;
}
//---------------------------------------------------------------------------
bool SD_ReadSingleBlock(unsigned int address , unsigned char *ppbuf, int len)
{
 
    ss = address ;
    SD_SendCommand(17,address); // single block  read , parm = address
    SD_ReadData(ppbuf,len,0x10000); 
    return true ;
}
//----------------------------------------------------------------------
bool SD_ReadMultiBlock(uint32 address , unsigned char *ppbuf, int len)
{
    int i;
    for(i=0;i<len;i++)
    {
        if(bSDHC==true)
            SD_ReadSingleBlock((address+i),ppbuf+i*0x200,0x200);
        else
            SD_ReadSingleBlock((address+i)*0x200,ppbuf+i*0x200,0x200);
    }
    return true;    
}
////////////////////////////////////////////////////////////////
/*uint32 dsCardi_SetModifyMode()
{
    uint8 command[8];
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= 0x00;
    command[4]= 0x5a;
    command[5]= 0xa5;
    command[6]= 0xff;
    command[7]= 0xB9;
    return dsCardi_Read4ByteMode(command);
}

uint32 dsCardi_ModifyReturn()
{
    uint8 command[8];
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= 0x00;
    command[4]= 0xaa;
    command[5]= 0x55;
    command[6]= 0xff;
    command[7]= 0xB9;
    return dsCardi_Read4ByteMode(command);
}
bool  WaitCmd_return00()
{
	  uint32 temp=0;
    uint32 loop= 8;
    uint8 command[8];
    //µ»¥˝∆ º±ÅEæŒª÷√
    WAIT_CR &= ~0x0800;
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= 0x00;
    command[4]= 0x00;
    command[5]= 0x01;
    command[6]= 0xFA;
    command[7]= 0xB8;
    do
    {
        temp = dsCardi_Read4ByteMode(command);
        loop --;
    }while(loop);   
    return true;
}
bool SD_R2Response(unsigned char *ppbuf)
{
    return SD_ReadResponse(ppbuf,17);
}
bool SD_R3Response(unsigned char *ppbuf)
{
    bool ret ;
    ret = SD_ReadResponse(ppbuf,6);
    if(ret)
    {
        if((ppbuf[0]!=0x3F)&&(ppbuf[5]!=0xFF))
            return false ;
        return true ;
    }
    else
    {
        dsCardi_SetModifyMode();
        dsCardi_ModifyReturn();
    }
    return false ;
}
void SD_ReadLoop()
{
	  uint32 temp;
    uint32 i;
    uint32 target = 0x80;
    WAIT_CR &= ~0x0800;
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
    CARD_CR2 = 0xA1586000 ;
    i=0;
    do {
        // Read data if available
        if (CARD_CR2 & CARD_DATA_READY) {
            if (i< target) 
            {
                temp = CARD_DATA_RD;
            }
            i++;
        }
    } while (CARD_CR2 & 0x80000000);
}
*/
uint32 dsCardi_ReadSram(uint32 address) 
{
	address += 0x80000;
    uint8 command[8];
    command[0]= 0x00;
    command[1]= 0x00;
    command[2]= 0x00;
    command[3]= address;
    command[4]= address>>8;
    command[5]= address>>16;
    command[6]= 0x01;
    command[7]= 0xB7;
    return dsCardi_Read4ByteMode(command);
}
bool SD_initial()
{

	if(dsCardi_ReadSram(0x7dfE0) == 1) bSDHC= true;	
	else bSDHC = false;
		
	return true;

}

	
//-----------------------------------------------------------------
bool EZ5_StartUp(void)
{
	return SD_initial();
};
//-----------------------------------------------------------------
bool EZ5_IsInserted (void)
{
	return true;
}
//-----------------------------------------------------------------
bool EZ5_ReadSectors (u32 sector, u8 numSecs, void* buffer)
{
	      //startshow();
    SD_ReadMultiBlock(sector,(u8*)buffer,numSecs);
    return true;
}
//---------------------------------------------------------------
bool EZ5_WriteSectors (u32 sector, u8 numSecs, void* buffer)
{
    int i;
    for(i=0;i<numSecs;i++)
    {
        if(bSDHC==true)
            SD_WriteSingleBlock( (i+sector),(u8*)buffer+i*512,0x200);
        else
            SD_WriteSingleBlock( (i+sector)*512,(u8*)buffer+i*512,0x200);
    }
    return true;
   
}
//-----------------------------------------------------------------
bool EZ5_ClearStatus (void)
{
	return true;
}
//-----------------------------------------------------
bool EZ5_Shutdown(void)
{
	return true;
};
 