/*----------------------------------------------------------------------------
 *      by NEOTEAM 2007
 *----------------------------------------------------------------------------
 *      http://www.neoflash.com
 *----------------------------------------------------------------------------*/
 
#ifndef _DISC_IO_H
#define _DISC_IO_H

// When compiling for NDS, make sure NDS is defined
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

//----------------------------------------------------------------------
// Customisable features

// Use DMA to read the card, remove this line to use normal reads/writes
// #define _IO_USE_DMA

// Allow buffers not alligned to 16 bits when reading files. 
// Note that this will slow down access speed, so only use if you have to.
// It is also incompatible with DMA
#define _IO_ALLOW_UNALIGNED

#if defined _IO_USE_DMA && defined _IO_ALLOW_UNALIGNED
 #error You can't use both DMA and unaligned memory
#endif

#define FEATURE_MEDIUM_CANREAD		0x00000001
#define FEATURE_MEDIUM_CANWRITE		0x00000002
#define FEATURE_SLOT_GBA			0x00000010
#define FEATURE_SLOT_NDS			0x00000020

typedef bool (* FN_MEDIUM_STARTUP)(void) ;
typedef bool (* FN_MEDIUM_ISINSERTED)(void) ;
typedef bool (* FN_MEDIUM_READSECTORS)(u32 sector, u32 numSectors, void* buffer) ;
typedef bool (* FN_MEDIUM_WRITESECTORS)(u32 sector, u32 numSectors, const void* buffer) ;
typedef bool (* FN_MEDIUM_CLEARSTATUS)(void) ;
typedef bool (* FN_MEDIUM_SHUTDOWN)(void) ;

struct IO_INTERFACE_STRUCT {
	unsigned long			ioType ;
	unsigned long			features ;
	FN_MEDIUM_STARTUP		fn_startup ;
	FN_MEDIUM_ISINSERTED	fn_isInserted ;
	FN_MEDIUM_READSECTORS	fn_readSectors ;
	FN_MEDIUM_WRITESECTORS	fn_writeSectors ;
	FN_MEDIUM_CLEARSTATUS	fn_clearStatus ;
	FN_MEDIUM_SHUTDOWN		fn_shutdown ;
} ;

typedef struct IO_INTERFACE_STRUCT IO_INTERFACE ;

#endif	// define _DISC_IO_H
