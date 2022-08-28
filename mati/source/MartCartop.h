#ifndef MARTCARTOP_H_
#define MARTCARTOP_H_

#include <nds/card.h>
#include <nds/registers_alt.h>

#include "nds.h"

#define WAIT_CR  (*(vuint16*)0x04000204)
#define REG_WRAMCNT (*(vuint8*)0x04000247)
#define FAT_at_SRAM  0x80
#define FLASH_FAT_AT_ADR1  0x100000
#define FLASH_FAT_AT_ADR2  0x180000

/////////////////////////
//#define dsCardi_WriteSram        Mart_WriteSram
#define SetFattableBase          Mart_SetFattableBase
#define dsCardi_SectorPerClus    Mart_SectorPerClus
#define SetCardUseSDHC            Mart_SetCardUseSDHC

#define Enable_Arm9DS            Mart_Arm9Access
#define Enable_Arm7DS            Mart_Arm7Access
#define dsCardi_ReadSram		 Mart_ReadSram4byte
#define dsCardi_Read4ByteMode	 Mart_Read4BYTE	
#define dsCardi_SetRomOP		 Mart_SetRomOP

        void        Mart_Arm7Access(void);
        void        Mart_Arm9Access(void);

        void        Mart_SetRomOP(uint8 * command);
        
        uint32      Mart_Read4BYTE(uint8 * command);//,uint16 wait = 0);
        void 		Mart_Read0BYTE(uint8 * command,uint16 wait);

        uint32      Mart_Read512BYTE(uint8 * command,uint8* buf);//,uint32 wait=0);
        
        //void        cardWriteCommand(uint8 * command);

///////////////////////////////////////////////////////////////////      
        uint32  	Mart_ReadSram4byte(uint32 address);
        uint32  	Mart_WriteSram(uint32 address,uint16 data);
        uint32      dsCardi_WriteSram(uint32 address,uint16 data);
        
        uint32 		Mart_SectorPerClus(uint8 SecNO);
        uint32 		Mart_SetFattableBase(uint8 base);
        uint32 		Mart_SetCardUseSDHC(uint8 base);

        uint32   	Mart_ReadFirmwareVer();       
        uint32 		Mart_ReadFlashCommand(uint32 address);
        uint32  	Mart_ReadFlash(uint32 address, uint8* pbuf);
        uint32  	Mart_SetHeaderCleanSRAMRead(u32 start ,u32 size);
        uint32   	Mart_DisableSpiWrite(void);
        uint32   	Mart_EnableSpiWrite(void);
        //3 in 1
        #define FlashBase		0x08000000

        #define	FlashLoaderOffSet 	0x00040000 //留硜EMbit 空间存放3in1  loader
        #define TotalGbaGameAddr  0x00030000  //存储rom 个数
        #define GotoGBAFirstAddr  0x00030005
        #define GbaGameInfoAddr	0x0030010  // 存储rom 信息
        #define GbaGameFilenameAddr 0x3a000 //存储rom 别脕E

        #define _GoldenEnable	0x0A000400
        #define _GoldenSaver	0x0A000200
        #define _RamPageReturn  0x0A000410
        #define _PSramName  0x0A000500
        #define _PSramSaveSize  0x0A000450
        #define _SingleName  0x0A000600
        #define _SingleSaveSize  0x0A000460

        #define _PSramExist  0x0A000470
        #define _SingleExist  0x0A000480

        void		OpenNorWrite();
        void		CloseNorWrite();
        void 		SetRompage(u16 page);
        void 		SetRampage(u16 page);
        void 		SetSerialMode();
        uint32   	ReadNorFlashID();
        void 		chip_reset();
        void 		Block_Erase(u32 blockAdd);
        void 		ReadNorFlash(u8* pBuf,u32 address,u16 len);
        void 		WriteNorFlash(u32 address,u8 *buffer,u32 size);
        void 		WriteSram(uint32 address, u8* data , uint32 size );
        void 		ReadSram(uint32 address, u8* data , uint32 size );
        void  		OpenRamWrite();
        void 		CloseRamWrite();
        void 		SetShake(u16 data);
        void 		resetgbasavemode();
        void 		WritePSram(u8* address, u8* data , uint32 length );
        void        ReadPSram(u8* address, u8* data , uint32 length );
        
#endif /*MARTCARTOP_H_*/
