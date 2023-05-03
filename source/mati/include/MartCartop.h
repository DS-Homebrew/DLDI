#ifndef MARTCARTOP_H_
#define MARTCARTOP_H_

#include <nds/ndstypes.h>

#define WAIT_CR  (*(vuint16*)0x4000204)
#define REG_WRAMCNT (*(vu8*)0x04000247)
#define REG_AUXSPICNTH      (*(vuint8*)0x040001A1)
#define REG_ROMCTRL       (*(vuint32*)0x040001A4)
#define REG_CARD_COMMAND   ((vuint8*)0x040001A8)
#define REG_CARD_DATA_RD   (*(vuint32*)0x04100010)

#define CARD_CR1_ENABLE  0x80  // in byte 1, i.e. 0x8000
#define CARD_CR1_IRQ     0x40  // in byte 1, i.e. 0x4000
#define CARD_BUSY       (1<<31)  // when reading, still expecting incomming data?
#define CARD_DATA_READY (1<<23)  // when reading, REG_CARD_DATA_RD has another word of data and is good to go

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

        void        Mart_SetRomOP(u8 * command);
        
        u32      Mart_Read4BYTE(u8 * command);//,u16 wait = 0);
        void 		Mart_Read0BYTE(u8 * command,u16 wait);

        u32      Mart_Read512BYTE(u8 * command,u8* buf);//,u32 wait=0);
        
        //void        cardWriteCommand(u8 * command);

///////////////////////////////////////////////////////////////////      
        u32  	Mart_ReadSram4byte(u32 address);
        u32  	Mart_WriteSram(u32 address,u16 data);
        u32      dsCardi_WriteSram(u32 address,u16 data);
        
        u32 		Mart_SectorPerClus(u8 SecNO);
        u32 		Mart_SetFattableBase(u8 base);
        u32 		Mart_SetCardUseSDHC(u8 base);

        u32   	Mart_ReadFirmwareVer();       
        u32 		Mart_ReadFlashCommand(u32 address);
        u32  	Mart_ReadFlash(u32 address, u8* pbuf);
        u32  	Mart_SetHeaderCleanSRAMRead(u32 start ,u32 size);
        u32   	Mart_DisableSpiWrite(void);
        u32   	Mart_EnableSpiWrite(void);
        //3 in 1
        #define FlashBase		0x08000000

        #define	FlashLoaderOffSet 	0x00040000 //����EMbit �ռ���3in1  loader
        #define TotalGbaGameAddr  0x00030000  //�洢rom ����
        #define GotoGBAFirstAddr  0x00030005
        #define GbaGameInfoAddr	0x0030010  // �洢rom ��Ϣ
        #define GbaGameFilenameAddr 0x3a000 //�洢rom ��ÁE

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
        u32   	ReadNorFlashID();
        void 		chip_reset();
        void 		Block_Erase(u32 blockAdd);
        void 		ReadNorFlash(u8* pBuf,u32 address,u16 len);
        void 		WriteNorFlash(u32 address,u8 *buffer,u32 size);
        void 		WriteSram(u32 address, u8* data , u32 size );
        void 		ReadSram(u32 address, u8* data , u32 size );
        void  		OpenRamWrite();
        void 		CloseRamWrite();
        void 		SetShake(u16 data);
        void 		resetgbasavemode();
        void 		WritePSram(u8* address, u8* data , u32 length );
        void        ReadPSram(u8* address, u8* data , u32 length );
        
#endif /*MARTCARTOP_H_*/
