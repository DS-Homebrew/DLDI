#ifndef HAL_INC
#define HAL_INC

unsigned long SwapINT32(unsigned long dData);

unsigned int LSwapINT16(unsigned short dData1,unsigned short dData2);
unsigned long LSwapINT32(unsigned long dData1,unsigned long dData2,unsigned long dData3,unsigned long dData4);
unsigned long SwapINT32(unsigned long dData);
void sdDelay(char number);

int sdmInitialize();


#ifndef SD_BLOCKSIZE
#define SD_BLOCKSIZE 0x200
#endif

extern unsigned char response[SD_BLOCKSIZE];
extern unsigned char argument[4];

int SdSendCommand( unsigned char cmd,
	unsigned char response_type,
	unsigned char *response,
	unsigned char *argument);

/*
	return
	1: command success and response returned
	0: fail

*/

int sdmSendCommand( unsigned char cmd,
	unsigned char response_type,
	unsigned char *response,
	unsigned char *argument);

void sdPackArg(unsigned char *argument, u32 value);

// maximum wait clocks
#define MAXWAITCLOCK (64*8)

// 0 found start bit
// 1 Max wait clocks reached, not found start bit
unsigned char sdmWaitStartBit();

/*
// read 7 bits after SdWaitStartBit_sdm
// return the first byte of the response token
*/
unsigned char sdmRead7();

// only write cmd code
// do not check response
void sdmWriteCmd( unsigned char cmd,
					  unsigned char *argument);

int sdmReadBlock ( u32 blockaddr,
	unsigned char *data);

int sdmWriteBlock( u32 blockaddr,
	unsigned char *data);


//-------------------------------------------------------------------
// gio interface
//-------------------------------------------------------------------

// 'EWSD'
#define DEVICE_TYPE_EWSD 0x44535745

bool EWSD_IsInserted();
bool EWSD_StartUp();

#endif // HAL_INC



