
#ifndef IO_EZ5_H
#define IO_EZ5_H

// #include "common.h"

#ifndef NDS
 #if defined ARM9 || defined ARM7
  #define NDS
 #endif
#endif

#ifdef NDS
 #include <nds/jtypes.h>
#else
 #include "gba_types.h"
#endif

#define BYTES_PER_READ 512

#ifndef NULL
 #define NULL 0
#endif

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

//bool SD_WriteSingleBlock(unsigned int address , unsigned char *ppbuf, int len);
//bool SD_ReadSingleBlock(unsigned int address , unsigned char *ppbuf, int len);
//bool SD_ReadMultiBlock(unsigned int address , unsigned char *ppbuf, int len);
//bool SD_initial();


#endif	// define IO_EZ5_H
