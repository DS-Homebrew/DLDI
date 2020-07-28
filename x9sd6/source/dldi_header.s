@---------------------------------------------------------------------------------
	.section ".init"
@---------------------------------------------------------------------------------
	.global _start
	.align	4
	.arm

@---------------------------------------------------------------------------------
.equ FEATURE_MEDIUM_CANREAD,		0x00000001
.equ FEATURE_MEDIUM_CANWRITE,		0x00000002
.equ FEATURE_SLOT_GBA,				0x00000010
.equ FEATURE_SLOT_NDS,				0x00000020

.equ FIX_ALL,						0x01
.equ FIX_GLUE,						0x02
.equ FIX_GOT,						0x04
.equ FIX_BSS,						0x08


@---------------------------------------------------------------------------------
@ Driver patch file standard header -- 16 bytes
	.word	0xBF8DA5ED		@ Magic number to identify this region
	.asciz	" Chishm"		@ Identifying Magic string (8 bytes with null terminator)
	.byte	0x01			@ Version number
	.byte	0x0F	@32KiB	@ Log [base-2] of the maximum size of this driver in bytes.
	.byte	FIX_GOT | FIX_BSS | FIX_GLUE	@ Sections to fix
	.byte 	0x00			@ Space allocated in the application, not important here.
	
@---------------------------------------------------------------------------------
@ Text identifier - can be anything up to 47 chars + terminating null -- 48 bytes
	.align	4
	.asciz "Ninjapass X9 (SD Card)"
	
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
	.ascii	"X9SD"			@ ioType	
	.word	FEATURE_MEDIUM_CANREAD | FEATURE_SLOT_NDS  | FEATURE_MEDIUM_CANWRITE
	.word	_X9SD_startup			@ 
	.word	_X9SD_isInserted		@ 
	.word	_X9SD_readSectors		@   Function pointers to standard device driver functions
	.word	_X9SD_writeSectors		@ 
	.word	_X9SD_clearStatus		@ 
	.word	_X9SD_shutdown			@ 
	
	
@---------------------------------------------------------------------------------
_start:
@---------------------------------------------------------------------------------
	.align
	.pool
	.end
@---------------------------------------------------------------------------------
