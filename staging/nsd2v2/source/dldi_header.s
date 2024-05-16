#include <nds/arm9/dldi_asm.h>

@---------------------------------------------------------------------------------
	.section ".crt0","ax"
@---------------------------------------------------------------------------------
	.global _start
	.align	4
	.arm

@---------------------------------------------------------------------------------
@ Driver patch file standard header -- 16 bytes
	.word	0xBF8DA5ED		@ Magic number to identify this region
	.asciz	" Chishm"		@ Identifying Magic string (8 bytes with null terminator)
	.byte	0x01			@ Version number
	.byte	DLDI_SIZE_8KB
	.byte	FIX_GOT | FIX_BSS | FIX_GLUE	@ Sections to fix
	.byte 	0x00			@ Space allocated in the application, not important here.
	
@---------------------------------------------------------------------------------
@ Text identifier - can be anything up to 47 chars + terminating null -- 48 bytes
	.align	4
	.asciz "Neo2 (SD Card)"
	
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
	.ascii	"nsd2"			@ ioType
	.word	FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_SLOT_GBA
	.word	NSD2_StartUp		@ 
	.word	NSD2_IsInserted		@ 
	.word	NSD2_ReadSectors_asm	@   Function pointers to standard device driver functions
	.word	NSD2_WriteSectors	@ 
	.word	NSD2_ClearStatus	@ 
	.word	NSD2_Shutdown		@ 
	
@---------------------------------------------------------------------------------
_start:
@---------------------------------------------------------------------------------
	.align
	.pool
	.end
@---------------------------------------------------------------------------------
