#ifndef IO_MARTSD_H_
#define IO_MARTSD_H_

#include "nds.h"
//#include "gba_nds_fat/disc_io.h"
//extern "C" {


//crc32???
uint16      cal_crc_CCITT(unsigned char *ptr,unsigned char len);
//crc7
uint8       cal_crc_730(unsigned char *ptr,unsigned char len);
//crc16
uint16      SD_cal_crc16(unsigned char *ptr,unsigned int len,unsigned char *buf2);

uint32  SD_SendCommand (int type , unsigned int param );
bool    SD_R16Response(unsigned char *ppbuf);
bool    SD_R2Response(unsigned char *ppbuf);
bool    SD_R3Response(unsigned char *ppbuf);
bool    SD_R7Response(unsigned char *ppbuf,int wait);
bool    SD_ReadResponse(unsigned char *ppbuf,int len);

bool    SD_ReadSingleBlock(unsigned int address , unsigned char *ppbuf, int len);

bool    SD_WriteSingleBlock(unsigned int address , unsigned char *ppbuf, int len);

bool    SD_initial();

bool    IsSDHC();

//LPIO_INTERFACE MartSD_GetInterface(void) ;

//}
#endif /*IO_MARTSD_H_*/
