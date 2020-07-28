/*
	io_m3sd.h  by SaTa.

	Hardware Routines for reading a compact flash card
	using the GBA Movie Player

	This software is completely free. No warranty is provided.
	If you use it, please give me credit and email me about your
	project at chishm@hotmail.com

	See gba_nds_fat.txt for help and license details.
*/

#ifndef IO_EZSD_H
#define IO_EZSD_H

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

bool SD_WriteSingleBlock(unsigned int address , unsigned char *ppbuf, int len);
bool SD_ReadSingleBlock(unsigned int address , unsigned char *ppbuf, int len);
bool SD_ReadMultiBlock(unsigned int address , unsigned char *ppbuf, int len);
bool SD_initial();


#endif	// define IO_EZSD_H
