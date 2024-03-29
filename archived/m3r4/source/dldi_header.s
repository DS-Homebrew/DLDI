#include <nds/arm9/dldi_asm.h>

@---------------------------------------------------------------------------------
	.section ".crt0","ax"
@---------------------------------------------------------------------------------
	.global _start
#ifdef SCDS
	.global _io_dldi
#endif
	.align	4
	.arm

@---------------------------------------------------------------------------------
@ Driver patch file standard header -- 16 bytes
	.word	0xBF8DA5ED		@ Magic number to identify this region
	.asciz	" Chishm"		@ Identifying Magic string (8 bytes with null terminator)
	.byte	0x01			@ Version number
	.byte	DLDI_SIZE_4KB
	.byte	FIX_GOT | FIX_BSS | FIX_GLUE	@ Sections to fix
	.byte 	0x00			@ Space allocated in the application, not important here.
	
@---------------------------------------------------------------------------------
@ Text identifier - can be anything up to 47 chars + terminating null -- 48 bytes
	.align	4
#if defined(M3DS)
	.asciz "M3DS DLDI based on r4tf_v2"
#elif defined(EX4TF)
	.asciz "EX4TF DLDI based on r4tf_v2"
#elif defined(SCDS)
	.asciz "SCDS SD/SDHC DLDI based on r4tf_v2"
#elif defined(IPLY)
	.asciz "iPlayer DLDI based on r4tf_v2"
#elif defined(G003)
	.asciz "GMP-Z003 DLDI based on r4tf_v2"
#else
	.asciz "R4TF DLDI v2 recompiled"
#endif
@---------------------------------------------------------------------------------
@ Offsets to important sections within the data	-- 32 bytes
	.align	6
	.word   __text_start	@ data start
	.word   __data_end		@ data end
	.word	__glue_start	@ Interworking glue start	-- Needs address fixing
	.word	__glue_end		@ Interworking glue end
	.word   __got_start		@ GOT start					-- Needs address fixing
	.word   __got_end		@ GOT end
	.word   __bss_start		@ bss start					-- Needs setting to zero
	.word   __bss_end		@ bss end

@---------------------------------------------------------------------------------
@ IO_INTERFACE data -- 32 bytes
#if defined(M3DS)
	.ascii	"M3DS"			@ ioType
#elif defined(EX4TF)
	.ascii "R4TF"
#elif defined(SCDS)
_io_dldi:
	.ascii "SCDS"
#elif defined(IPLY)
	.ascii "IPLY"
#else
	.ascii	"R4TF"			@ ioType
#endif
	.word	FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_SLOT_NDS
	.word	startup			@ 
	.word	isInserted		@ 
	.word	readSectors		@   Function pointers to standard device driver functions
	.word	writeSectors	@ 
	.word	clearStatus		@ 
	.word	shutdown		@ 
	
@---------------------------------------------------------------------------------
_start:
@---------------------------------------------------------------------------------
	.align
	.pool
	.end
@---------------------------------------------------------------------------------
