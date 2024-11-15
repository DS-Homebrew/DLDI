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
_dldi_start:
	.word	0xBF8DA5ED		@ Magic number to identify this region
	.asciz	" Chishm"		@ Identifying Magic string (8 bytes with null terminator)
	.byte	0x01			@ Version number
	.byte	0x0F	@32KiB	@ Log [base-2] of the size of this driver in bytes.
	.byte	0x0E			@ Sections to fix
	.byte 	0x00			@ Space allocated in the application, not important here.

@---------------------------------------------------------------------------------
@ Text identifier - can be anything up to 47 chars + terminating null -- 48 bytes
	.align	4
	.asciz "NAND FLASH CARD LIBFATNRIO"

@---------------------------------------------------------------------------------
@ Offsets to important sections within the data	-- 32 bytes
	.align	6
	.word   _dldi_start	@ data start
	.word   byte_BF804D0C		@ data end
	.word	sub_BF800098	@ Interworking glue start	-- Needs address fixing
	.word	sub_BF800098		@ Interworking glue end
	.word   dword_BF804C08		@ GOT start					-- Needs address fixing
	.word   gotEnd		@ GOT end
	.word   __bss_start		@ bss start					-- Needs setting to zero
	.word   __bss_end		@ bss end

@---------------------------------------------------------------------------------
@ IO_INTERFACE data -- 32 bytes
	.ascii	"NRIO"			@ ioType
	.word	FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_SLOT_NDS
	.word	sub_BF8013A4	@ 
	.word	sub_BF801334	@ 
	.word	sub_BF801378	@   Function pointers to standard device driver functions
	.word	sub_BF80134C	@ 
	.word	sub_BF80133C	@ 
	.word	sub_BF801344	@ 
	
@---------------------------------------------------------------------------------
_start:
@---------------------------------------------------------------------------------
	.align
	.pool
	.end
@---------------------------------------------------------------------------------
