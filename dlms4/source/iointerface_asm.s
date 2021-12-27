@***********************************************************
@
@	by Rudolph (çcíÈ)
@		2006/11/24 First release
@
@-----------------------------------------------------------
@  Hardware Routines for microSD card using a DSLink.
@
@ Redistribution and use in source and binary forms,
@ with or without modification, are permitted provided 
@ that the following conditions are met:
@
@  Only the Homebrew application can be used. 
@  It is not possible to use it by a business purpose. 
@
@  This software is made based on information obtained 
@  by Reverse engineering. 
@
@  Please use that at once when a source code that is 
@  more formal than the official is open to the public.
@
@ <File composition>
@	io_DLMS.c
@	io_DLMS.h
@	io_DLMS_asm.s
@-----------------------------------------------------------
@
@***********************************************************


 .ALIGN
 .GLOBAL DLMS_Init_s, DLMS_stmd_s, DLMS_Read_s, DLMS_Write_s
 .CODE 32
 .ARM


.equ	TIMEOUT, 0x1000

@===========================================
@ _cal_crc8
@	r0:Cal ptr -> r0:CRC
@===========================================
_cal_crc8:
	mov	r4, r0
	mov 	r1, #0x5		@ Cal 5 byte
	mov 	r0, #0x0
_crc8_next:
	mov	r2, #0x80
	ldrb	r3, [r4], #+0x1
_crc8_lp:
	tst	r0, #0x40
	mov	r0, r0, lsl #0x1
	eorne	r0, r0, #0x9
	tst	r3, r2
	eorne	r0, r0, #0x9
	movs	r2, r2, lsr #0x1
	bne	_crc8_lp

	subs	r1, r1, #0x1
	bne	_crc8_next

	mov	r0, r0, lsl #0x1
	orr	r0, r0, #0x1

	bx	r14


@===========================================
@ _cal_crc16
@	r0:u16* buff
@	r1:u16 num byte
@	r2:u16* crc16buff
@===========================================
_cal_crc16:
	stmfd	r13!, {r3 - r9}
	mov	r9, r2

	mov	r3, #0x0
	mov	r4, #0x0
	mov	r5, #0x0
	mov	r6, #0x0

	ldr	r7, =0x80808080
	ldr	r8, =0x1021
	mov	r1, r1, lsl #0x3

_crc16_lp:
	tst	r7, #0x80
	ldrneb	r2, [r0], #0x1

	mov	r3, r3, lsl #1
	tst	r3, #0x10000
	eorne	r3, r3, r8
	tst	r2, r7, lsr #24
	eorne	r3, r3, r8

	mov	r4, r4, lsl #1
	tst	r4, #0x10000
	eorne	r4, r4, r8
	tst	r2, r7, lsr #25
	eorne	r4, r4, r8

	mov	r5, r5, lsl #1
	tst	r5, #0x10000
	eorne	r5, r5, r8
	tst	r2, r7, lsr #26
	eorne	r5, r5, r8
	
	mov	r6, r6, lsl #1
	tst	r6, #0x10000
	eorne	r6, r6, r8
	tst	r2, r7, lsr #27
	eorne	r6, r6, r8

	mov	r7, r7, ror #4
	subs	r1, r1, #4
        bne     _crc16_lp 

	mov	r2, r9
	mov	r8, #16

_crc16_write_data:
	mov	r7, r7, lsl #0x4
	tst	r3, #0x8000
	orrne	r7, r7, #0x8
	tst	r4, #0x8000
	orrne	r7, r7, #4
	tst	r5, #0x8000
	orrne	r7, r7, #0x2
	tst	r6, #0x8000
	orrne	r7, r7, #0x1

	mov	r3, r3, lsl #0x1
	mov	r4, r4, lsl #0x1
	mov	r5, r5, lsl #0x1
	mov	r6, r6, lsl #0x1

	sub	r8, r8, #0x1
	tst	r8, #0x1
	streqb	r7, [r2], #1
	cmp	r8, #0
	bne	_crc16_write_data

	ldmfd	r13!, {r3-r9}
	bx      r14


@===========================================
@ _send_cmd_rdy
@
@===========================================
_send_cmd_rdy:

@	mov	r8, #0x1
@	mov	r10, #0xA7000000
@	mov	r11, #0x4100000
@	mov	r12, #0x4000000

	mov	r0, #TIMEOUT
_send_cmd_rdy0:

	strb	r8, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x1
	str	r10, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA7000000

	mov	r1, #TIMEOUT

_send_cmd_rdy_chk:
	subs	r0, r0, #0x1
	beq	_send_cmd_rdy_err
	subs	r1, r1, #0x1
	moveq	r0, r1
	beq	_send_cmd_rdy_err

	ldr	r2, [r12, #+0x1a4]		@ r4 = (CARD_CR2)
	tst	r2, #0x800000
	beq	_send_cmd_rdy_chk

	ldr	r2, [r11, #+0x10]		@ r4 = (CARD_DATA_RD)
	cmp	r2, #0x0
	bge	_send_cmd_rdy0

	mov	r0 ,#0x1
_send_cmd_rdy_err:
	bx	r14


@===========================================
@ _send_cmd_end
@
@===========================================
_send_cmd_end:

@	mov	r8, #0x1
@	mov	r10, #0xA7000000
@	mov	r11, #0x4100000
@	mov	r12, #0x4000000

	mov	r0, #TIMEOUT
_send_cmd_end0:

	strb	r8, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x1
	str	r10, [r12, #+0x1a4]		@ (CARD_CR2) = A7000000

	mov	r1, #TIMEOUT

_send_cmd_end_chk:
	subs	r0, r0, #0x1
	beq	_send_cmd_end_err
	subs	r1, r1, #0x1
	moveq	r0, r1
	beq	_send_cmd_end_err

	ldr	r2, [r12, #+0x1a4]		@ r2 = (CARD_CR2)
	tst	r2, #0x800000
	beq	_send_cmd_end_chk

	ldr	r2, [r11, #+0x10]		@ r2 = (CARD_DATA_RD)
	cmp	r2, #0x0
	blt	_send_cmd_end0

	mov	r0 ,#0x1
_send_cmd_end_err:
	bx	r14


@===========================================
@ _send_cmd
@	r0:cmd ptr
@===========================================
_send_cmd:

@	mov	r9, #0xA0000000
@	mov	r12, #0x4000000

	mov 	r1, #0x3
	strb	r1, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x3

	mov	r4, #0x0
_sd_cm:
	ldrb	r2, [r4, +r0]			@ read cmd prm
	mov	r1, #0x7
_sd_cm_lp:
	and	r3, r2, #0xFF
	strb	r3, [r12, #+0x1af]		@ (CARD_COMMAND+7) = r3
	str	r9, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA0000000
_sd_cm_chk:
	ldr	r3, [r12, #+0x1a4]		@ r3 = (CARD_CR2)
	cmp	r3, #0x0
	blt	_sd_cm_chk

	subs	r1, r1, #0x1
	movcs	r3, r2, lsl #0x11
	movcs	r2, r3, lsr #0x10
	bcs	_sd_cm_lp

	add	r4, r4, #0x1
	cmp	r4, #0x6
	bne	_sd_cm

	bx	r14


@===========================================
@ _send_cmdw
@	r0:cmd ptr
@===========================================
_send_cmdw:
@	mov	r9, #0xA0000000
@	mov	r12, #0x4000000

	mov	r1, #0x6
	strb	r1, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = r1
	strb	r1, [r12, #+0x1af]		@ (CARD_COMMAND+7) = r1
	str	r9, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA0000000

_sd_cmw_rdy:
	ldr	r3, [r12, #+0x1a4]		@ r3 = (CARD_CR2)
	cmp	r3, #0x0
	blt	_sd_cmw_rdy

	mov	r4, #0x0
_sd_cmw_lp:
	ldrb	r3, [r4, +r0]
	mov	r2, r3, lsl #0x1
	mov	r1, r2, lsl #0x1
	mov 	r10, r1, lsl #0x1
	mov	r11, r10, lsl #0x1
	mov	r8, r11, lsl #0x1
	mov	r5, r8, lsl #0x1
	and	r3, r3, #0xFF
	strb	r3, [r12, #+0x1a8]		@ r3 = (CARD_COMMAND)
	and	r2, r2, #0xFF
	and	r1, r1, #0xFF
	and	r10, r10, #0xFF
	and	r11, r11, #0xFF
	and	r8, r8, #0xFF
	mov	r3, r5, lsl #0x1
	and	r5, r5, #0xFF
	and	r3, r3, #0xFF
	strb	r2, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = r2
	strb	r1, [r12, #+0x1aa]		@ (CARD_COMMAND+2) = r1
	strb	r10, [r12, #+0x1ab]		@ (CARD_COMMAND+3) = r10
	strb	r11, [r12, #+0x1ac]		@ (CARD_COMMAND+4) = r11
	strb	r8, [r12, #+0x1ad]		@ (CARD_COMMAND+5) = r8
	strb	r5, [r12, #+0x1ae]		@ (CARD_COMMAND+6) = r5
	strb	r3, [r12, #+0x1af]		@ (CARD_COMMAND+7) = r3
	str	r9, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA0000000
_sd_cmw_chk:
	ldr	r3, [r12, #+0x1a4]		@ r3 = (CARD_CR2)
	cmp	r3, #0x0
	blt	_sd_cmw_chk

	add	r4, r4, #0x1
	cmp	r4, #0x6
	bne	_sd_cmw_lp

	bx	r14


@===========================================
@ _read_rsp	read response
@	r1:cnt
@===========================================
_read_rsp:

@	mov	r8, #0x1
@	mov	r10, #0xA7000000
@	mov	r11, #0x4100000
@	mov	r12, #0x4000000

	strb	r8, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x1

	mov	r5, #0x0

_rd_rsp:
	str	r10, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA7000000
_rd_rsp_chk:
	ldr	r3, [r12, #+0x1a4]		@ r3 = (CARD_CR2)
	tst	r3, #0x800000
	beq	_rd_rsp_chk

	ldr	r3, [r11, #+0x10]		@ r3 = (CARD_DATA_RD)
	sub	r1, r1, #0x1
	mov	r5, r5, lsl #0x11
	mov	r3, r3, lsr #0x1f
	ands	r4, r1, #0x7
	orr	r5, r3, r5, lsr #0x10
	streqb	r5, [r0], #+0x1			@ set pesponse
	moveq	r5, r4
	cmp	r1, #0x0
	bne	_rd_rsp

	bx	r14


@===========================================
@ _read_rsp_dmy	
@	r1:cnt
@===========================================
_read_rsp_dmy:

@	mov	r8, #0x1
@	mov	r10, #0xA7000000
@	mov	r11, #0x4100000
@	mov	r12, #0x4000000

	strb	r8, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x1

_rd_rsp_dmy_lp:
	str	r10, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA7000000

_rd_rsp_dmy_chk:
	ldr	r2, [r12, #+0x1a4]		@ r2 = (CARD_CR2)
	tst	r2, #0x800000
	beq	_rd_rsp_dmy_chk

	subs	r1, r1, #0x1
	ldr	r2, [r11, #+0x10]		@ r2 = (CARD_DATA_RD)
	bne	_rd_rsp_dmy_lp

	bx	r14


@===========================================
@ _read_res_rmn
@	r1:cnt
@===========================================
_read_res_rmn:

@	mov	r8, #0x1
@	mov	r10, #0xA7000000
@	mov	r11, #0x4100000
@	mov	r12, #0x4000000

	strb	r8, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x1
	str	r10, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA7000000

_rd_res_rmn_chk:
	ldr	r2, [r12, #+0x1a4]		@ r2 = (CARD_CR2)
	tst	r2, #0x800000
	beq	_rd_res_rmn_chk

	subs	r1, r1, #0x1
	ldr	r2, [r11, #+0x10]		@ r2 = (CARD_DATA_RD)
	bne	_read_res_rmn

	bx	r14


@===========================================
@ _read_res_rmn1
@
@===========================================
_read_res_rmn1:

@	mov	r8, #0x1
@	mov	r10, #0xA7000000
@	mov	r11, #0x4100000
@	mov	r12, #0x4000000

	strb	r8, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x1
	str	r10, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA7000000

_rd_res_rmn1_chk:
	ldr	r2, [r12, #+0x1a4]		@ r2 = (CARD_CR2)
	tst	r2, #0x800000
	beq	_rd_res_rmn1_chk

	ldr	r2, [r11, #+0x10]		@ r2 = (CARD_DATA_RD)
	tst	r2, #0x1000000
	beq	_read_res_rmn1

	bx	r14


@===========================================
@ _read_res_rmnw
@	r1:cnt
@===========================================
_read_res_rmnw:

@	mov	r8, #0x2
@	mov	r9, #0xA0000000
@	mov	r10, #0xA1000000
@	mov	r11, #0x4100000
@	mov	r12, #0x4000000

	strb	r8, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x1
	str	r10, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA7000000

_rd_res_rmnw_chk:
	ldr	r2, [r12, #+0x1a4]		@ r2 = (CARD_CR2)
	tst	r2, #0x800000
	beq	_rd_res_rmnw_chk

	subs	r1, r1, #0x1
	cmn	r1, #0x1
	ldr	r2, [r11, #+0x10]		@ r2 = (CARD_DATA_RD)
	bne	_rd_res_rmnw_chk

	mov 	r2, #0x0
	str	r2, [r12, #+0x1a4]		@ (CARD_CR2) = 0x0
	strb	r2, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x0
	str	r9, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA0000000

_rd_res_rmnw_lp:
	ldr	r2, [r12, #+0x1a4]		@ r2 = (CARD_CR2)
	cmp	r2, #0x0
	blt	_rd_res_rmnw_lp

	bx	r14


@===========================================
@ _write_data
@	r0:souce ptr, r1:length
@===========================================
_write_data:

@	mov	r9, #0xA0000000
@	mov	r11, #0x4100000
@	mov	r12, #0x4000000

	ldrb	r3, [r0], #+0x1
	and	r2, r3, #0xFF
	mov	r3, r3, lsr #0x4
	strb	r3, [r12, #+0x1a8]		@ (CARD_COMMAND) = r3
	strb	r2, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = r2
	ldrb	r3, [r0], #+0x1
	and	r2, r3,#0xFF
	mov	r3, r3, lsr #0x4
	strb	r3, [r12, #+0x1aa]		@ (CARD_COMMAND+2) = r3
	strb	r2, [r12, #+0x1ab]		@ (CARD_COMMAND+3) = r2
	ldrb	r3, [r0], #+0x1
	and	r2, r3,#0xFF
	mov	r3, r3, lsr #0x4
	strb	r3, [r12, #+0x1ac]		@ (CARD_COMMAND+4) = r3
	strb	r2, [r12, #+0x1ad]		@ (CARD_COMMAND+5) = r2
	ldrb	r3, [r0], #+0x1
	and	r2, r3, #0xFF
	mov	r3, r3,lsr #0x4
	strb	r3, [r12, #+0x1ae]		@ (CARD_COMMAND+6) = r3
	strb	r2, [r12, #+0x1af]		@ (CARD_COMMAND+7) = r2
	str	r9, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA0000000

_wr_data_chk:
	ldr	r3, [r12, #+0x1a4]		@ r3 = (CARD_CR2)
	cmp	r3, #0x0
	blt	_wr_data_chk

	sub	r1, r1, #0x4
	cmp	r1, #0x0
	bne	_write_data

	bx	r14



@===========================================
@ DLMS_stmd_s
@	r0:int Command
@===========================================
DLMS_stmd_s:
	stmdb	r13!, {r10, r11, r14}
	mov	r11, #0x4000000
	mov	r10, #0xC0
	strb	r10, [r11, #+0x1a1]		@ (CARD_CR1) = 0xC0
	ldr	r10, =0xEDEDDEDE		@ r2 = 0xededdede
	str	r10, [r11, #+0x1a8]		@ (CARD_COMMAND) = 0xEDEDDEDE
	mov 	r0, r0, lsl #0x18
	str	r0, [r11, #+0x1ac]		@ (CARD_COMMAND+4) = r0

	mov 	r0, #0xA0000000
	str	r0, [r11, #+0x1a4]		@ (CARD_CR2) = 0xA0000000
_st_busy:
	ldr	r0, [r11, #+0x1a4]		@ r0 = (CARD_CR2)
	tst	r0, #0x80000000
	bne	_st_busy
	ldmia	r13!, {r10, r11, r15}
	bx	r14


@===========================================
@ DLMS_Init_s
@	
@===========================================
DLMS_Init_s:
	stmdb	r13!, {r4, r5, r6, r7, r8, r9, r10, r11, r14}
	sub 	r13, r13, #0x3C

	mov	r1, #0x0
	mov	r2, #0x4000000
	mov	r9, #0xA0000000
_in_cl_01:
	strb	r1, [r2, #+0x1a9]		@ (CARD_COMMAND+1) = 0x0
	str	r9, [r2, #+0x1a4]		@ (CARD_CR2) = 0xA0000000

	mov	r0, #TIMEOUT
_in_cl_01_chk:
	ldr	r3, [r2, #+0x1a4]		@ r3 = (CARD_CR2)

	subs	r0, r0, #0x1
	beq	_in_err

	cmp	r3, #0x0
	blt	_in_cl_01_chk

	add	r1, r1, #0x1
	cmp	r1, #0x100			@ loop 0x100 cnt
	bne	_in_cl_01


	mov	r1, #0x0
	mov	r2, #0x40
	strb	r2, [r13, #+0x32]		@ 0x40
	strb	r1, [r13, #+0x33]		@ 0x00000000
	strb	r1, [r13, #+0x34]
	strb	r1, [r13, #+0x35]
	strb	r1, [r13, #+0x36]

	add	r0, r13, #0x32
	bl	_cal_crc8
	strb	r0, [r13, #+0x37]		@ CRC

	mov	r2, #0x77
	mov	r1, #0x0
	strb	r2,[r13, #+0x2c]		@ 0x77
	strb	r1,[r13, #+0x2d]		@ 0x00000000
	strb	r1,[r13, #+0x2e]
	strb	r1,[r13, #+0x2f]
	strb	r1,[r13, #+0x30]

	add	r0, r13, #0x2C
	bl	_cal_crc8
	strb	r0, [r13, #+0x31]		@ CRC

	mov	r2, #0x69
	mvn	r3, #0x3
	mov	r1, #0x0
	strb	r2, [r13, #+0x26]		@ 0x69
	strb	r1, [r13, #+0x27]		@ 0x00FC0000
	strb	r3, [r13, #+0x28]
	strb	r1, [r13, #+0x29]
	strb	r1, [r13, #+0x2a]

	add	r0, r13, #0x26
	bl	_cal_crc8
	strb	r0, [r13, #+0x2b]		@ CRC 

@----------------------------

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	mov	r1, #0x4A
	bl	_read_res_rmn

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add 	r0, r13, #0x32
	bl	_send_cmd

	mov	r1, #0x14
	bl	_read_res_rmn


	mov	r7, #TIMEOUT
_in_lp:
	subs	r7, r7,#0x1
	moveq	r0, r7
	beq	_in_err

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add 	r0, r13, #0x2C
	bl	_send_cmd

	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x2F
	bl	_read_rsp
	mov	r1, #0x12
	bl	_read_res_rmn

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x26
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x2F
	bl	_read_rsp
	mov	r1, #0x11
	bl	_read_res_rmn

	ldrsb	r3, [r13, #+0x1]
	cmp	r3, #0x0
	bge	_in_lp


	mov	r1, #0x8
	bl	_read_res_rmn

@----------------------------

	mov	r2, #0x42
	mov	r1, #0x0
	strb	r2, [r13, #+0x20]		@ 0x42
	strb	r1, [r13, #+0x21]		@ 0x00000000
	strb	r1, [r13, #+0x22]
	strb	r1, [r13, #+0x23]
	strb	r1, [r13, #+0x24]

	add	r0, r13, #0x20
	bl	_cal_crc8
	strb	r0, [r13, #+0x25]		@ CRC

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x20
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x87
	bl	_read_rsp
	mov	r1, #0xA
	bl	_read_res_rmn

@----------------------------

	mov	r2, #0x43
	mov	r1, #0x0
	strb	r2, [r13, #+0x20]		@ 0x43
	strb	r1, [r13, #+0x21]		@ 0x00000000
	strb	r1, [r13, #+0x22]
	strb	r1, [r13, #+0x23]
	strb	r1, [r13, #+0x24]

	add	r0, r13, #0x20
	bl	_cal_crc8
	strb	r0, [r13, #+0x25]		@ CRC

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x20
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x2F
	bl	_read_rsp

	ldrb	r3, [r13, #+0x2]
	ldrb	r2, [r13, #+0x1]
	mov	r3, r3,lsl #0x10
	orr	r7, r3, r2, lsl #0x18		@ SDadd

	mov	r1, #0xF
	bl	_read_res_rmn

@----------------------------

	mov	r3, r7, lsr #0x10
	and	r4, r3, #0xFF
	and	r5, r7, #0xFF
	mov	r8, r7, lsr #0x18
	mov	r6, #0x0
	mov	r3, #0x49
	strb	r3, [r13, #+0x20]		@ 0x49
	strb	r8, [r13, #+0x21]		@ SDadr
	strb	r4, [r13, #+0x22]
	strb	r6, [r13, #+0x23]
	strb 	r5, [r13, #+0x24]

	add	r0, r13, #0x20
	bl	_cal_crc8
	strb	r0, [r13, #+0x25]		@ CRC

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x20
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x87
	bl	_read_rsp_dmy
	mov	r1, #0x2D
	bl	_read_res_rmn

@----------------------------

	mov	r3, r7, lsr #0x10
	and	r4, r3, #0xFF
	and	r5, r7, #0xFF
	mov	r8, r7, lsr #0x18
	mov	r6, #0x0
	mov	r3, #0x47
	strb	r3, [r13, #+0x20]		@ 0x47
	strb	r8, [r13, #+0x21]		@ SDadr
	strb	r4, [r13, #+0x22]
	strb	r6, [r13, #+0x23]
	strb	r5, [r13, #+0x24]

	add	r0, r13, #0x20
	bl	_cal_crc8
	strb	r0, [r13, #+0x25]		@ CRC

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x20
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x2F
	bl	_read_rsp_dmy
	mov	r1, #0xA
	bl	_read_res_rmn

@----------------------------

	mov	r3, r7, lsr #0x10
	and	r4, r3, #0xFF
	and	r5, r7, #0xFF
	mov	r8, r7, lsr #0x18
	mov	r6, #0x0
	mov	r3, #0x4D
	strb	r3, [r13, #+0x20]		@ 0x4d
	strb	r8, [r13, #+0x21]		@ SDadr
	strb	r4, [r13, #+0x22]
	strb	r6, [r13, #+0x23]
	strb	r5, [r13, #+0x24]

	add	r0, r13, #0x20
	bl	_cal_crc8
	strb	r0, [r13, #+0x25]		@ CRC

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x20
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x2F
	bl	_read_rsp_dmy
	mov	r1, #0x14
	bl	_read_res_rmn

@----------------------------

	mov	r3, r7, lsr #0x10
	and	r4, r3, #0xFF
	and	r5, r7, #0xFF
	mov	r8, r7, lsr #0x18
	mov	r6, #0x0
	mov	r3, #0x77
	strb	r3, [r13, #+0x20]		@ 0x77
	strb	r8, [r13, #+0x21]		@ SDadr
	strb	r4, [r13, #+0x22]
	strb	r6, [r13, #+0x23]
	strb	r5, [r13, #+0x24]

	add	r0, r13, #0x20
	bl	_cal_crc8
	strb	r0, [r13, #+0x25]		@ CRC

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x20
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x2F
	bl	_read_rsp_dmy
	mov	r1, #0x18
	bl	_read_res_rmn

@----------------------------

	mov	r3, #0x46
	mov	r2, #0x2
	mov	r0, #0x
	strb	r3, [r13, #+0x20]		@ 0x46
	strb	r0, [r13, #+0x21]		@ 0x00000002
	strb	r0, [r13, #+0x22]
	strb	r0, [r13, #+0x23]
	strb	r2, [r13, #+0x24]

	add 	r0, r13, #0x20
	bl	_cal_crc8
	strb	r0, [r13, #+0x25]		@ CRC

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x20
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x2F
	bl	_read_rsp_dmy
	mov	r1, #0xA
	bl	_read_res_rmn

@----------------------------

	mov	r3, #0x50
	mov	r2, #0x2
	mov	r0, #0x0
	strb	r3,[r13, #+0x20]		@ 0x50
	strb	r0,[r13, #+0x21]		@ 0x00000200
	strb	r0,[r13, #+0x22]
	strb	r2,[r13, #+0x23]
	strb	r0,[r13, #+0x24]

	add	r0, r13, #0x20
	bl	_cal_crc8
	strb	r0, [r13, #+0x25]		@ CRC

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_send_cmd_rdy
	cmp	r0, #0x0
	beq	_in_err

	add	r0, r13, #0x20
	bl	_send_cmd
	bl	_send_cmd_end
	cmp	r0, #0x0
	beq	_in_err

	mov	r0, r13
	mov	r1, #0x2F
	bl	_read_rsp_dmy
	mov	r1, #0xA
	bl	_read_res_rmn

@----------------------------

	mov	r0, #0x1			@ true

_in_err:
	add	r13, r13, #0x3C
	ldmia	r13!, {r4, r5, r6, r7, r8, r9, r10, r11, r14}
	bx	r14


@===========================================
@ DLMS_Read_s
@	r0:int Start Position, r1:char *pBuf
@	r2:int lenght
@===========================================
DLMS_Read_s:
	stmdb	r13!, {r4, r5, r6, r7, r8, r9, r10, r11, r12, r14}
	stmdb	r13!, {r0,r1,r2}
	ldmia	r13!, {r8,r9,r10}

	cmp	r10, #0x0
	beq	_rd_exit

	mov	r11, #0x4100000
	mov	r12, #0x4000000
	mov 	r7, #0xA6000000
	sub	r13, r13, #0x8
	mov	r0, #0x52
	strb	r0, [r13, #+0x0]
	mov	r4, r8, lsr #0x9
	mov	r4, r4, lsl #0x9
	strb	r4, [r13, #+0x4]
	mov	r4, r4, lsr #0x8
	strb	r4, [r13, #+0x3]
	mov	r4, r4, lsr #0x8
	strb	r4, [r13, #+0x2]
	mov	r4, r4, lsr #0x8
	strb	r4, [r13, #+0x1]

	mov 	r0, r13
	bl	_cal_crc8
	strb	r0, [r13, #+0x5]		@ CRC

	bl	_rd_sub_04

_rd_cmd_sts:
	mov	r2, #0x5
	strb	r2, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x5
	ldr	r4, =0x01010101			@ r4 = 0x01010101
	mov	r3, #0x64
_rd_cmd_sts_1:
	str	r7, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA6000000
_rd_cmd_sts_2:
	ldr	r0, [r12, #+0x1a4]		@ r0 = (CARD_CR2)
	tst	r0, #0x800000
	beq	_rd_cmd_sts_3

	ldr	r0, [r11, #+0x10]		@ r0 = (CARD_DATA_RD)
	and	r0, r0, r4
	cmp	r0, r4
	bne	_rd_data

_rd_cmd_sts_3:
	ldr	r0, [r12, #+0x1a4]		@ r0 = (CARD_CR2)
	tst	r0, #0x80000000
	bne	_rd_cmd_sts_2

	mov	r5, #0x64
_rd_cmd_sts_w:
	strb	r2, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x5
	subs	r5, r5, #0x1
	bne	_rd_cmd_sts_w			@ 100 Cnt

	subs	r3, r3, #0x1
	bne	_rd_cmd_sts_1

_rd_data:
	bl	_rd_sub_01

	mov	r2, #0x2
	strb	r2, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x2
	mov	r3, r8, lsl #0x17
	mov	r3, r3, lsr #0x17
	rsb	r5, r3, #0x200
	mov	r1, r5
	cmp	r5, r10
	movhi	r5, r10
	sub	r1, r1, r5
	add	r8, r8, r5
	sub	r10, r10, r5
	add	r2, r11, #0x12
	cmp	r5, #0x200
	bne	_rd_data_sts

	tst	r9, #0x1
	bne	_rd_data_sts

	str	r2, [r12, #+0xd4]		@ (REG_DMA3SAD) = r2
	str	r9, [r12, #+0xd8]		@ (REG_DMA3DAD) = r9
	mov	r0, #0xA9000000
	add	r0, r0, #0x100
	str	r0, [r12, #+0xdc]		@ (REG_DMA3CNT) = 0xA9000000 + 0x100*n
	str	r7, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA6000000
_rd_data_chk:
	ldr	r0, [r12, #+0xdc]		@ r0 = (REG_DMA3CNT)
	tst	r0, #0x80000000
	bne	_rd_data_chk

	add	r9, r9, r5
	b	_rd_data_jp

_rd_data_sts:
	str	r7, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA6000000
_rd_data_sts_1:
	ldr	r0, [r12, #+0x1a4]		@ r0 = (CARD_CR2)
	tst	r0, #0x800000
	beq	_rd_data_sts_1

	movs	r3, r3, lsr #0x1
	beq	_rd_data_sts_2
_rd_data_sts_3:
	ldrh	r0, [r2, #+0x0]
	subs	r3, r3, #0x1
	bne	_rd_data_sts_3
_rd_data_sts_2:
	sub	r0, r8, r5
	tst	r0, #0x1
	ldrneh	r0, [r2, #+0x0]
	movne 	r0, r0, lsr #0x8
	strneb	r0, [r9], #+0x1
	subne	r5, r5, #0x1
	tst	r9, #0x1
	beq	_rd_data_sts_4
_rd_data_sts_6:
	cmp	r5, #0x1
	bls	_rd_data_sts_5

	ldrh	r0, [r2, #+0x0]
	strb	r0, [r9], #+0x1
	mov	r0, r0, lsr #0x8
	strb	r0, [r9], #+0x1
	sub	r5, r5, #0x2
	b	_rd_data_sts_6

_rd_data_sts_7:
	ldrh	r0, [r2, #+0x0]
	strh	r0, [r9],#+0x2
	sub 	r5, r5, #0x2
_rd_data_sts_4:
	cmp	r5, #0x1
	bhi	_rd_data_sts_7
_rd_data_sts_5:
	cmp	r5, #0x1
	ldrcsh	r0, [r2, #+0x0]
	strcsb	r0, [r9],#+0x1
	movs	r1, r1, lsr #0x1
	beq	_rd_data_jp
_rd_data_sts_8:
	ldrh	r0, [r2, #+0x0]
	subs	r1, r1, #0x1
	bne	_rd_data_sts_8
_rd_data_jp:
	bl	_rd_sub_01

	bl	_rd_sub_03

	cmp	r10, #0x0
	bne	_rd_cmd_sts

	mov	r4, #0x0
	str	r4, [r13, #+0x0]
	str	r4, [r13, #+0x4]
	mov 	r0, #0x4C
	strb	r0, [r13, #+0x0]
	mov	r4, #0x61
	strb	r4, [r13, #+0x5]
	bl	_rd_sub_04

	mov	r2, #0x2
	strb	r2, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x2
	str	r7, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA6000000
	mov	r3, #0x30
_rd_data_sts_9:
	ldr	r0, [r12, #+0x1a4]		@ r0 = (CARD_CR2)
	tst	r0, #0x800000
	beq	_rd_data_sts_9

	ldr	r0, [r11, #+0x10]		@ r0 = (CARD_DATA_RD)
	subs	r3, r3, #0x4
	bne	_rd_data_sts_9

	bl	_rd_sub_01

	add	r13, r13, #0x8


_rd_exit:
	ldmia	r13!, {r4, r5, r6, r7, r8, r9, r10, r11, r12, r15}
_rd_sub_01:
	stmdb	r13!, {r14}
	mov	r0, #0x0
	str	r0, [r12, #+0x1a4]		@ (CARD_CR2) = 0x00000000
	strb	r0, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x0
	bl	_rd_sub_02

	ldmia	r13!, {r15}
_rd_sub_04:
	mov	r4, r13
	stmdb	r13!, {r14}

_rd_s04_chk_lp:
	bl	_rd_sub_03

	tst	r0, #0x80000000
	beq	_rd_s04_chk_lp

	mov	r5, #0x6
	strb	r5, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x6
	strb	r5, [r12, #+0x1af]		@ (CARD_COMMAND+7) = 0x6
	bl	_rd_sub_02
_rd_s01_lp:
	ldrb	r2, [r4], #+0x1
	mov	r3, #0x8
	add	r0, r12, #0x1a8

_rd_s01_lp2:
	strb	r2, [r0], #+0x1
	mov	r2, r2, lsl #0x1
	subs	r3, r3, #0x1
	bne	_rd_s01_lp2

	bl	_rd_sub_02

	subs	r5, r5, #0x1
	bne	_rd_s01_lp

	ldmia	r13!, {r15}
_rd_sub_03:
	mov	r0, #0x1
	strb	r0, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x1
	mov	r0, #0xA7000000
	str	r0, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA7000000

_rd_s03_chk_lp:
	ldr	r0, [r12, #+0x1a4]		@ r0 = (CARD_CR2)
	tst	r0, #0x800000
	beq	_rd_s03_chk_lp

	ldr	r0, [r11, #+0x10]		@ r0 = (CARD_DATA_RD)
	bx	r14

_rd_sub_02:
	mov	r0, #0xA0000000
	str	r0, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA0000000

_rd_s02_chk_lp:
	ldr	r0, [r12, #+0x1a4]		@ r0 = (CARD_CR2)
	tst	r0, #0x80000000
	bne	_rd_s02_chk_lp

	bx	r14


@===========================================
@ DLMS_Write_s
@	r0:int Start Position, r1:char *pBuf
@	r2:int lenght
@===========================================
DLMS_Write_s:
	stmdb	r13!, {r4, r5, r6, r7, r8, r9, r10, r11, r14}
	cmp	r0, #0x0
	sub	r13, r13, #0x14

	mov	r7, r2, lsr #0x9		@ r7 = numSecs (length / 512)
	mov	r6, r1				@ r6 = pBuf
	beq	_wr_exit

	mov	r1, r0, lsr #0x18
	mov	r12, r0, lsr #0x8
	mov	r0, r0, lsr #0x10
	mov	r3, #0x59
	strb	r3, [r13, #+0xa]		@ 0x59
	strb	r1, [r13, #+0xb]		@ Start posion (sector * 512)
	strb	r0, [r13, #+0xc]
	strb	r12, [r13, #+0xd]
	strb	r2, [r13, #+0xe]

	add	r0, r13, #0xA
	bl	_cal_crc8
	strb	r0, [r13, #+0xf]		@ CRC

	mov	r8, #0x1
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000
	bl	_send_cmd_rdy

	add	r0, r13, #0xA
	mov	r9, #0xA0000000
	bl	_send_cmdw

	mov	r8, #0x1
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	bl	_send_cmd_end

	mov	r8, #0x2
	mov 	r1, #0xB
	mov	r10, #0xA1000000
	bl	_read_res_rmnw

	cmp	r7, #0x0			@ ? numSecs = 0
	ble	_wr_end_prc


@--- write dat / sector
_wr_data:
	mov	r0, r6				@ pBuf
	mov	r1, #0x200			@ 512byte
	mov	r2, r13				@ store CRC
	bl	_cal_crc16

	mov	r8, #0x1
	mov	r9, #0xA0000000
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000

	bl	_read_res_rmn1

	mov	r1, #0x4
	strb	r1, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x4
	mov	r2, #0x0
	strb	r2, [r12, #+0x1af]		@ (CARD_COMMAND+7) = 0x0
	str	r9, [r12, #+0x1a4]		@ (CARD_CR2) = 0x0A0000000
_wr_data_stchk:
	ldr	r3, [r12, #+0x1a4]		@ r3 = (CARD_CR2)
	cmp	r3, #0x0
	blt	_wr_data_stchk

	mov	r1, #0x7
	strb	r1, [r12, #+0x1a9]		@ (0x040001a9) = 0x7
	mvn	r2, #0x7D			@ r2 = 0x82
	strb	r2, [r12, #+0x1af]		@ (CARD_COMMAND+7) = 0x82
	str	r9, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA0000000
_wr_data_rdchk:
	ldr	r3, [r12, #+0x1a4]		@ r3 = (CARD_CR2)
	cmp	r3, #0x0
	blt	_wr_data_rdchk

	mov	r0, r6				@ pBuf
	mov	r1, #0x200			@ 512byte
	bl	_write_data

	mov	r0, r13				@ CRC
	mov	r1, #0x8			@ 8byte
	bl	_write_data

	mov	r8, #0x4
	strb	r8, [r12, #+0x1a9]		@ (CARD_COMMAND+1) = 0x4
	mvn	r2, #0x0
	strb	r2, [r12, #+0x1af]		@ (CARD_COMMAND+7) = 0xFF
	str	r9, [r12, #+0x1a4]		@ (CARD_CR2) = 0xA0000000
_wr_data_stchk1:
	ldr	r3, [r12, #+0x1a4]		@ r3 = (CARD_CR2)
	cmp	r3, #0x0
	blt	_wr_data_stchk1

	mov	r1, #0x10
	mov	r8, #0x1
	bl	_read_res_rmn

	bl	_read_res_rmn1

	mov 	r1, #0x14
	bl	_read_res_rmn

	sub	r7, r7, #0x1
	cmp	r7, #0x0
	add	r6, r6, #0x200
	bne	_wr_data


@--- write end proc
_wr_end_prc:
	mov	r2, #0x0
	mov	r3, #0x4C
	strb	r3, [r13, #+0xa]		@ 0x4C
	strb	r2, [r13, #+0xb]		@ 0x00000000
	strb	r2, [r13, #+0xc]
	strb	r2, [r13, #+0xd]
	strb	r2, [r13, #+0xe]

	add 	r0, r13, #0xA
	bl	_cal_crc8
	strb	r0, [r13, #+0xf]		@ CRC

	mov	r8, #0x1
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	mov	r12, #0x4000000
	bl	_send_cmd_rdy

	add	r0, r13, #0xA
	mov	r9, #0xA0000000
	bl	_send_cmdw

	mov	r8, #0x1
	mov	r10, #0xA7000000
	mov	r11, #0x4100000
	bl	_send_cmd_end

	mov	r8, #0x2
	mov 	r1, #0xB
	mov	r10, #0xA1000000
	bl	_read_res_rmnw

	mov	r8, #0x1
	mov	r10, #0xA7000000
	bl	_read_res_rmn1

_wr_exit:
	add 	r13, r13, #0x14
	ldmia	r13!, {r4, r5, r6, r7, r8, r9, r10, r11, r14}

	bx	r14

 .END
