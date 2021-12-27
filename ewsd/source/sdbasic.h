#ifndef SDBASIC_INC
#define SDBASIC_INC

#include <nds.h>

//-------------------------------------------------------------------------------

// Bit operate macro
#define setb(c,b)		c|=(1<<(b))				//set the selected bit in the charecter
#define clrb(c,b)		c&=(~(1<<(b)))			//clr the selected bit in the charecter
#define testb(c,b)		(c&(1<<(b)))				//test the selected bit in the charecter is 0 or 1
//

#define REG_DISPCNT    *(volatile u16*)0x4000000
#define Set_Mode0()     	REG_DISPCNT &= 0xFFF8
#define Set_Mode1()     	REG_DISPCNT |= 0x0001; \
						REG_DISPCNT &= 0xFFF9
#define Set_Mode2()     	REG_DISPCNT |= 0x0002; \
						REG_DISPCNT &= 0xFFFA
#define Set_Mode3()     	REG_DISPCNT |= 0x0003; \
						REG_DISPCNT &= 0xFFFB
#define Set_Mode4()     	REG_DISPCNT |= 0x0004; \
						REG_DISPCNT &= 0xFFFC
#define Set_Mode5()     	REG_DISPCNT |= 0x0005; \
						REG_DISPCNT &= 0xFFFD

// BG Macros define
#define ForceBlankon()  	setb(REG_DISPCNT ,7)
#define ForceBlankoff()  	clrb(REG_DISPCNT ,7)

#define Background0on() 	setb(REG_DISPCNT ,8)
#define Background0off()	clrb(REG_DISPCNT ,8)

#define Background1on() 	setb(REG_DISPCNT ,9)
#define Background1off()	clrb(REG_DISPCNT ,9)

#define Background2on() 	setb(REG_DISPCNT ,10)
#define Background2off()	clrb(REG_DISPCNT ,10)

#define Background3on() 	setb(REG_DISPCNT ,11)
#define Background3off()	clrb(REG_DISPCNT ,11)

// xuetao appendice
//----------------------------------------------

#define DELAY_COUNTER 100


#define SD_SUCCESS 0
#define SD_ERROR 1

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef  long	LONG;



#define SD_BLOCKSIZE 512
#define SD_BLOCKSIZE_NBITS 9

#define Rx	0x01
#define R1	0x10
#define R1B 0x12
#define R2	0x20
#define R3	0x30
#define R6	0x60

#define MSK_IDLE 0x01
#define MSK_ERASE_RST 0x02
#define MSK_ILL_CMD 0x04
#define MSK_CRC_ERR 0x08
#define MSK_ERASE_SEQ_ERR 0x10
#define MSK_ADDR_ERR 0x20
#define MSK_PARAM_ERR 0x40

#define SD_TOK_READ_STARTBLOCK 0xFE
#define SD_TOK_WRITE_STARTBLOCK 0xFE
#define SD_TOK_READ_STARTBLOCK_M 0xFE
#define SD_TOK_WRITE_STARTBLOCK_M 0xFC
#define SD_TOK_STOP_MULTI 0xFD

/* Error token is 111XXXXX */
#define MSK_TOK_DATAERROR 0xE0 /* Bit fields */
#define MSK_TOK_ERROR 0x01
#define MSK_TOK_CC_ERROR 0x02
#define MSK_TOK_ECC_FAILED 0x04
#define MSK_TOK_CC_OUTOFRANGE 0x08
#define MSK_TOK_CC_LOCKED 0x10

/* Mask off the bits in the OCR corresponding to voltage range 3.2V to
*3.4V, OCR bits 20 and 21 */
#define MSK_OCR_33 0xC0


/* Number of times to retry the probe cycle during initialization */
#define SD_INIT_TRY 50 /* Number of tries to wait for the card to go idle during initialization */
#define SD_IDLE_WAIT_MAX 100

/* Hardcoded timeout for commands. 8 words, or 64 clocks. Do 10
*  words instead */
#define SD_CMD_TIMEOUT 100


/******************************** Basic command set **************************/ /* Reset cards to idle state */
#define CMD0 0
#define CMD0_R R1 /* Read the OCR (MMC mode, do not use for SD cards) */
#define CMD1 1
#define CMD1_R R1 /* Card sends the CSD */

#define CMD2 2
#define CMD2_R R17 /* Card sends the CID */

#define CMD3 3
#define CMD3_R R1 //?

#define CMD7 7
#define CMD7_R R1 // Select Card

#define CMD9 9
#define CMD9_R R1 /* Card sends CID */
#define CMD10 10
#define CMD10_R R1 /* Stop a multiple block (stream) read/write operation */
#define CMD12 12
#define CMD12_R R1B /* Get the addressed card¡¯s status register */
#define CMD13 13
#define CMD13_R R2
/***************************** Block read commands **************************/
/* Set the block length */
//----------------------------------------------------------

#define CMD16 16

#define CMD16_R R1

/* Read a single block */
#define CMD17 17

#define CMD17_R R1

/* Read multiple blocks until a CMD12 */
#define CMD18 18

#define CMD18_R R1

/***************************** Block write commands *************************/
/* Write a block of the size selected with CMD16 */

#define CMD24 24

#define CMD24_R R1

/* Multiple block write until a CMD12 */
#define CMD25 25

#define CMD25_R R1
/* Program the programmable bits of the CSD */

#define CMD27 27

#define CMD27_R R1

/***************************** Write protection *****************************/
/* Set the write protection bit of the addressed group */

#define CMD28 28

#define CMD28_R R1B
/* Clear the write protection bit of the addressed group */

#define CMD29 29

#define CMD29_R R1B
/* Ask the card for the status of the write protection bits */

#define CMD30 30

#define CMD30_R R1

/***************************** Erase commands *******************************/
/* Set the address of the first write block to be erased */

#define CMD32 32

#define CMD32_R R1
/* Set the address of the last write block to be erased */

#define CMD33 33

#define CMD33_R R1
/* Erase the selected write blocks */

#define CMD38 38

#define CMD38_R R1B

/***************************** Lock Card commands ***************************/
/* Commands from 42 to 54, not defined here */

/***************************** Application-specific commands ****************/
/* Flag that the next command is application-specific */

#define CMD55 55

#define CMD55_R R1
/* General purpose I/O for application-specific commands */

#define CMD56 56

#define CMD56_R R1
/* Read the OCR (SPI mode only) */

#define CMD58 58

#define CMD58_R R3
/* Turn CRC on or off */

#define CMD59 59

#define CMD59_R R1

/***************************** Application-specific commands ***************/
// set bus width
#define ACMD6 6

/* Get the SD card¡¯s status */

#define ACMD13 13

#define ACMD13_R R2
/* Get the number of written write blocks (Minus errors ) */

#define ACMD22 22
#define ACMD22_R R1 /* Set the number of write blocks to be pre-erased before writing */
#define ACMD23 23
#define ACMD23_R R1 /* Get the card¡¯s OCR (SD mode) */
#define ACMD41 41
#define ACMD41_R R1 /* Connect or disconnect the 50kOhm internal pull-up on CD/DAT[3] */
#define ACMD42 42
#define ACMD42_R R1 /* Get the SD configuration register */
#define ACMD51 42
#define ACMD51_R R1

//------------------------------------------------------------------------

// check if SD card is inserted properly
//
#define CARD_WR_PROTECT		2			// 0 1 0
#define CARD_HALFWAY		4			// 1 0 0
#define CARD_NOT_FOUND		6			// 1 1 0
#define CARD_READY			0	// sd card inserted, unlocked

// SD mode, read 4bits data from SD bus
inline u32 SDB_SDM_GetDataOut();

void SDB_InitEwinSD();

inline u32 SDB_SDM_GetResponse();

u16 SDB_SDM_CheckCard();

void SDB_SDM_SetDataOut(BYTE hbyte);

void SDB_SetEwinSD();


#define __ADDR_SD_DATA ((0xb00000) * 2 + 0x08000000 )
#define __ADDR_SD_DATA1 ((0xb00001) * 2 + 0x08000000 )

#define __ADDR_SD_CMD ((0xc00000) * 2 + 0x08000000 )

#define BYTE_PER_READ 512

#define COPY_BUFFER_SIZE (BYTE_PER_READ*2*64)
extern BYTE myBuffer[COPY_BUFFER_SIZE];


#define SRAM_BANK_SIZE (64*1024)

#endif // SDBASIC_INC

