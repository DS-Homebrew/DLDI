/*******************************************************
	Modified by Rudolph (鼓碾)

	2006/11/29
		* CRC is read with SD_ReadData. 
	2006/11/22
		* Added write routine support

 Original is EZ4FATLIB code offered by EZTEAM.
 Hardware Routines for SD card using a EZ4 series.

********************************************************/

//#if !defined (_SD_OPERA_H__)
#ifndef _SD_OPERA_H__
#define _SD_OPERA_H__

//#include "Nandop.h"

#include "common.h"

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef BOOL
typedef bool BOOL ;
#endif


/*******************
typedef struct
{
	unsigned char 	reseved1 : 6 ; 
	unsigned char 	CSD_STRUCTURE : 2 ;		//2字节 csd 结构
	
	unsigned char 	TAAC ;					//data read accessed time 1
	unsigned char 	NSAC ;					//access time 2 in clk cycles 
	unsigned char 	TRAN_SPEED ;				//max data transfer rate
	unsigned char  	CCC_H : 8 ;				//card command class
	
	unsigned char 	READ_BL_LEN : 4 ;			//max read data block len
	unsigned char  	CCC_L: 4 ;				//card command class
	
	unsigned char 	C_SIZE_H2 : 2 ;			//device size hight [11-10]
	unsigned char 	reseved2 : 2 ;				
	unsigned	char		DSR_IMP : 1 ;				//DSR implement
	unsigned char		READ_BLK_MISALIGN : 1 ;	//read block misalignment
	unsigned char 	WRITE_BLK_MISALIGN : 1 ; 	//write block misalignment 
	unsigned char 	READ_BL_PARTIAL : 1 ; 		//partial block data read allowed 
	
	unsigned char 	C_SIZE_H1  ;				//device size hight [9-2]
	
	unsigned char		VDD_R_CURR_MAX	: 3 ;		//max read current VDD max
	unsigned char		VDD_R_CURR_MIN	: 3 ;		//max read current VDD min
	unsigned char 	C_SIZE_L : 2  ;				//device size hight [1-0]

	unsigned char 	C_SIZE_MULT_H : 2 ;		//device size multiplier
	unsigned char 	VDD_W_CURR_MAX : 3  ; 	//max wirte current VDD max
	unsigned char 	VDD_W_CURR_MIN : 3  ; 	//max wirte current VDD min	
	
	unsigned char 	SECTOR_SIZE_H : 6 ;		//erase sector size
	unsigned char 	ERASE_BLK_EN : 1 ;			//erase single block enable
	unsigned char 	C_SIZE_MULT_L : 1 ;		
	
	unsigned char 	WP_GRP_SIZE : 7 ;			//write protect group size
	unsigned char 	SECTOR_SIZE_L : 1 ;	
	
	unsigned char		WRITE_BL_LEN_H : 2 ;		// max. write data block length
	unsigned char 	R2W_FACTOR : 3 ;			//write speed factor
	unsigned char 	reserved3 : 2 ;
	unsigned char 	WP_GRP_ENABLE : 1 ;		//write protect group enable
	
	unsigned char 	reserved4 : 5 ;
	unsigned char		WRITE_BL_PARTIA : 1 ;		//partial blocks for write allowed
	unsigned char		WRITE_BL_LEN_L : 2 ;
	
	unsigned char		reserved5 : 2 ;				
	unsigned char		FILE_FORMAT : 2 ;			//File format
	unsigned char		TMP_WRITE_PROTECT : 1 ; 	//temporary write protection
	unsigned char 	PERM_WRITE_PROTECT : 1 ;	//permanent write protection
	unsigned char		COPY : 1 ;				//copy flag (OTP)
	unsigned char 	FILE_FORMAT_GRP : 1 ; 		//File format group 
	
	unsigned char 	ALLONE : 1 ;				// not used, always’1’
	unsigned char		CRC : 7 ;					//CRC
}SD_CSDREAL ;

typedef enum _CSD_TAAC_TIMEUNIT{
	One_ns =0,
	Ten_ns,
	Tundred_ns,
	One_us,
	Ten_us,
	Tundred_us,
	One_ms,
	Ten_ms
}CSD_TAAC_TIMEUNIT;

typedef struct _CSD_TAAC{
	CSD_TAAC_TIMEUNIT	TimeUnit; 
	BYTE				fTimeValue;
}CSD_TAAC;

typedef enum _CSD_TRAN_SPEEDUNIT{
	Tundred_kbits_persec=0,
	One_Mbits_persec,
	Ten_Mbits_persec,
	Tundred_Mbits_persec
}CSD_TRAN_SPEEDUNIT;

typedef struct _CSD_TRAN_SPEED{
	CSD_TRAN_SPEEDUNIT	transfer_rate;
	BYTE				fTimeValue;
}CSD_TRAN_SPEED;

typedef enum _CSD_FILE_FORMA{
	HD_LIKE,			//Hard disk-like file system with partition table
	DOS_FAT,			//DOS FAT(floppy-like) with sector only(no partition table);
	UFF,				//Universal File Format
	Unknow				//Others/Unkonw
}CSD_FILE_FORMAT;

typedef struct
{
	BYTE	byManufacturerID;
	WORD	wOemID;
	char		szProductName[6];
	BYTE    	byProductReision;
	DWORD	dwProductSn;
	WORD	wData;
	BYTE	byCRC;
}SD_CID;

typedef struct
{
	BYTE 				CSDStruct;				//CSD Struct Version
	CSD_TAAC			Taac;
	BYTE				byNsac;
	CSD_TRAN_SPEED		TranSpeed;				
	WORD				wCCC;
	BYTE				bRead_Bl_Len;				//最大的读数据块的大小
	BOOL				bRead_Bl_Partial;			//局部的块读允许,最小的块大小是one byte
	BOOL				bWrite_Blk_Misalign;
	BOOL				bRead_Blk_Misalign;
	BOOL				bDsr_Imp;
	WORD				wC_Size;
	BYTE				byVdd_R_Curr_Min;
	BYTE				byVdd_R_Curr_Max;
	BYTE				byVdd_W_Curr_Min;
	BYTE				byVdd_W_Curr_Max;
	WORD				wC_Size_Mult;
	BOOL				bErase_Blk_En;
	BYTE				bySector_Size;
	BYTE				byWp_Grp_Size;
	BOOL				bWp_Grp_Enable;
	BYTE				byR2w_Factor;
	BYTE				byWrite_Bl_Len;
	BOOL				bWrite_Bl_Partial;
	BYTE				byFile_Format_Grp;
	BOOL				bCopy;
	BYTE				byPerm_Write_Protect;
	BYTE				byTmp_Write_Protect;
	CSD_FILE_FORMAT		File_Format;
}SD_CSD;

**************/


#ifdef __cplusplus
extern "C" {
#endif

void	SD_WriteBufferToLine(unsigned char* pBuf , int lenght );
bool	SD_ReadResponse(unsigned char* pBuf , int lenght , int timeout );
bool	SD_ReadData(unsigned char* pBuf, int lenght, unsigned char* pcrc, int timeout);
void	SD_WriteData(unsigned char* pBuf , int lenght , int timeout );
unsigned char crc_730(unsigned char *ptr, unsigned char len) ;
void	SD_cal_crc16(unsigned char *ptr, unsigned short lenght, unsigned char *crcptr) ;

bool	SD_initial();
void	SD_Enable() ;
void	SD_Disable() ;
bool 	SD_ReadSingleBlock(unsigned int address , unsigned char *ppbuf, int len);
bool	SD_WriteSingleBlock(unsigned int address , unsigned char *ppbuf, int len);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
unsigned int cal_crc_CCITT(unsigned char *ptr,unsigned char len);
void  	SD_SendCommand(int type , unsigned int param);
bool	SD_SendCommandRS(int type, unsigned int param);
bool 	SD_R16Response(unsigned char *ppbuf=0, int wait = 0 ) ;
bool 	SD_R2Response(unsigned char *ppbuf=0, int wait = 0 ) ;
bool	SD_R3Response(unsigned char *ppbuf=0, int wait = 0 ) ;

bool	SD_WaitDataline(int loop) ;
bool	SD_WriteWaitDataline(int loop);
#endif

#endif 
