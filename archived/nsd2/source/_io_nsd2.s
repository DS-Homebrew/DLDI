                      @ 1     | clk en  | cmd     | d3-d1   | d0      | cmd     | d3      | d2      | d1      | d0     | cmd    | d3-d1 | d0
@WR_SD_CMD:
@	.long (0x08000000 | (1<<24) | (1<<19) |           (1<<16) | (1<<15) |           (1<<12) | (1<<11) | (1<<10) | (1<<9) )
	
@RD_SD_CMD:
@	.long (0x08000000 | (1<<24) | (1<<19) | (1<<17) | (1<<16) | (1<<15) | (1<<13) | (1<<12) | (1<<11) | (1<<10) | (1<<9) | (1<<7) )
	
@RD_SD_DAT:
@	.long (0x08000000 | (1<<24) | (1<<19) | (1<<17) |                     (1<<13) |                                                 (1<<6) | (1<<5) )

	.globl	wrMmcCmdBit
wrMmcCmdBit:
	ldr		r1, = (0x08000000 | (1<<24) | (1<<19) |           (1<<16) | (1<<15) |           (1<<12) | (1<<11) | (1<<10) | (1<<9) ) @WR_SD_CMD
	mov		r0, r0, lsl #12
	strh	r0, [r1]
	bx	lr
	
	.pool

	.globl	rdMmcCmdBit
rdMmcCmdBit:
	ldr	r0, = (0x08000000 | (1<<24) | (1<<19) | (1<<17) | (1<<16) | (1<<15) | (1<<13) | (1<<12) | (1<<11) | (1<<10) | (1<<9) | (1<<7) ) @RD_SD_CMD
	ldrh	r0, [r0]
	mov	r0, r0, lsr #12
	and	r0, r0, #1
	bx	lr
	
	.pool
	
	.globl	rdMmcDatBit4
rdMmcDatBit4:
	ldr	r0, = (0x08000000 | (1<<24) | (1<<19) | (1<<17) |                     (1<<13) |                                                 (1<<6) | (1<<5) ) @RD_SD_DAT
	ldrh	r0, [r0]
	mov	r0, r0, lsr #8
	and	r0, r0, #15
	bx	lr
	
	.pool

	.globl	wrMmcCmdByte
wrMmcCmdByte:
	stmfd sp!, {r4,r5,lr}
	mov	r4, r0
	mov	r5, #0
0:
	mov		r0, #7
	sub		r0, r0, r5
	mov		r0, r4, lsr r0	
	bl		wrMmcCmdBit
	add		r5, r5, #1
	cmp		r5, #8
	blt		0b
	ldmfd sp!, {r4,r5,lr}
	bx	lr

	/*ldr	r1, = (0x08000000 | (1<<24) | (1<<19) |           (1<<16) | (1<<15) |           (1<<12) | (1<<11) | (1<<10) | (1<<9) ) @WR_SD_CMD

	mov	r0, r0, lsl #(12 - 7)	
	and	r2, r0, #(1 << 12)
	mov	r0, r0, lsl #1
	and	r3, r0, #(1 << 12)
	mov	r3, r3, lsl #16
	orr	r2, r3
	str	r2, [r1]
	
	mov	r0, r0, lsl #1
	and	r2, r0, #(1 << 12)
	mov	r0, r0, lsl #1
	and	r3, r0, #(1 << 12)
	mov	r3, r3, lsl #16
	orr	r2, r3
	str	r2, [r1]
	
	mov	r0, r0, lsl #1
	and	r2, r0, #(1 << 12)
	mov	r0, r0, lsl #1
	and	r3, r0, #(1 << 12)
	mov	r3, r3, lsl #16
	orr	r2, r3
	str	r2, [r1]
	
	mov	r0, r0, lsl #1
	and	r2, r0, #(1 << 12)
	mov	r0, r0, lsl #1
	and	r3, r0, #(1 << 12)
	mov	r3, r3, lsl #16
	orr	r2, r3
	str	r2, [r1]
	
	bx	lr
	
	.pool */

	.globl	rdMmcCmdByte
rdMmcCmdByte:
/*	ldr	r3, RD_SD_CMD
	ldrh	r0, [r3]
	mov	r0, r0,LSR #(12 - 7)
	and	r0, r0, #(1 << 7)

	ldrh	r1, [r3]
	mov	r1, r1,LSR #(12 - 6)
	and	r1, r1, #(1 << 6)
	orr	r0, r0, r1

	ldrh	r1, [r3]
	mov	r1, r1,LSR #(12 - 5)
	and	r1, r1, #(1 << 5)
	orr	r0, r0, r1

	ldrh	r1, [r3]
	mov	r1, r1,LSR #(12 - 4)
	and	r1, r1, #(1 << 4)
	orr	r0, r0, r1

	ldrh	r1, [r3]
	mov	r1, r1,LSR #(12 - 3)
	and	r1, r1, #(1 << 3)
	orr	r0, r0, r1

	ldrh	r1, [r3]
	mov	r1, r1,LSR #(12 - 2)
	and	r1, r1, #(1 << 2)
	orr	r0, r0, r1

	ldrh	r1, [r3]
	mov	r1, r1,LSR #(12 - 1)
	and	r1, r1, #(1 << 1)
	orr	r0, r0, r1

	ldrh	r1, [r3]
	mov	r1, r1,LSR #(12 - 0)
	and	r1, r1, #(1 << 0)
	orr	r0, r0, r1 */
	
	stmfd sp!, {lr}
	mov	r0, #8
	bl	rdMmcCmdBits
	ldmfd sp!, {lr}
	bx	lr


	.globl	rdMmcCmdBits
rdMmcCmdBits:
	stmfd	sp!, {r4-r5,lr}
	mov	r5, #0
	mov	r4, r0

0:	bl	rdMmcCmdBit
	orr	r5, r0, r5,LSL #1
	subs	r4, r4, #1
	bgt	0b

	and	r0, r5, #0xFF

	ldmfd	sp!, {r4-r5,lr}
	bx	lr
	

	.globl	rdMmcDatByte4
rdMmcDatByte4:
	ldr	r1, = (0x08000000 | (1<<24) | (1<<19) | (1<<17) |                     (1<<13) |                                                 (1<<6) | (1<<5) ) @RD_SD_DAT
	ldr	r0, [r1]
	mov	r2, r0,LSR #4
	and	r2, r2, #0xF0
	mov	r0, r0,LSR #24
	and	r0, r0, #0x0F
	orr	r0, r0, r2
	bx	lr
	
	.pool




		.globl	sendMmcCmd
sendMmcCmd:
	stmfd	sp!, {r4,lr}
	sub	sp, sp, #8
	
	orr	r0, r0, #0x40
	strb	r0, [sp,#0]
	mov	r0, r1,LSR #24
	strb	r0, [sp,#1]
	mov	r0, r1,LSR #16
	strb	r0, [sp,#2]
	mov	r0, r1,LSR #8
	strb	r0, [sp,#3]
	strb	r1, [sp,#4]

	mov	r0, sp
@	mov r1, #5
	bl	crc7
	mov	r0, r0,LSL #1
	orr	r0, r0, #1
	strb	r0, [sp,#5]
	
	mov	r0, #0xFF
	bl	wrMmcCmdByte
	mov	r0, #0xFF
	bl	wrMmcCmdByte
	
	mov	r4,#0
sendMmcCmdLoop:
	ldrb	r0, [sp,r4]
	bl	wrMmcCmdByte
	add		r4, r4, #1
	cmp		r4, #6
	bne		sendMmcCmdLoop
	
/*	ldrb	r0, [sp,#0]
	bl	wrMmcCmdByte
	ldrb	r0, [sp,#1]
	bl	wrMmcCmdByte
	ldrb	r0, [sp,#2]
	bl	wrMmcCmdByte
	ldrb	r0, [sp,#3]
	bl	wrMmcCmdByte
	ldrb	r0, [sp,#4]
	bl	wrMmcCmdByte
	ldrb	r0, [sp,#5]
	bl	wrMmcCmdByte */
	
	add	sp, sp, #8
	ldmfd	sp!, {r4,lr}
	bx lr
	
	.globl	crc7_byte
crc7_byte:
	STR     LR, [SP,#-4]! 
	MOV     R12, R1
	MOV     LR, #0
loc_2281BFC:
	MOV     R2, R0,LSL#1
	AND     R2, R2, #0xFF
	EOR     R3, R2, R12
	AND     R3, R3, #0x80
	ADD     R1, LR, #1
	CMP     R3, #0
	AND     LR, R1, #0xFF
	MOVNE   R3, #9
	MOVEQ   R3, #0
	MOV     R1, R12,LSL#1
	CMP     LR, #8
	EOR     R0, R2, R3
	AND     R12, R1, #0xFF
	BNE     loc_2281BFC
	LDR     LR, [SP],#4
	BX      LR
	
	.globl crc7
crc7:
	STMFD   SP!, {R4-R6,LR}
	mov			r1, #5
	SUBS    R6, R1, #0
	MOV     R5, R0
	MOVLE   R0, #0
	BLE     loc_2281C6C
	MOV     R0, #0
	MOV     R4, R0
loc_2281C58:
	ADD     R4, R4, #1
	LDRB    R1, [R5],#1
	BL      crc7_byte
	CMP     R6, R4
	BNE     loc_2281C58
loc_2281C6C:
	LDMFD   SP!, {R4-R6,LR}
	BX      LR

/*crc7:
	stmfd	sp!, {r4}
	adr	r3, crc7Tab
	mov	r2, #5
	mov	r1, #0

0:	ldrb	r4, [r0],#1
	eor	r1, r4, r1,LSL #1
	ldrb	r1, [r3,r1]
	subs	r2, r2, #1
	bgt	0b

	mov	r0, r1
	ldmfd	sp!, {r4}
	bx	lr
	
	
crc7Tab:
	.byte	0x00, 0x09, 0x12, 0x1B, 0x24, 0x2D, 0x36, 0x3F, 0x48, 0x41, 0x5A, 0x53, 0x6C, 0x65, 0x7E, 0x77
	.byte	0x19, 0x10, 0x0B, 0x02, 0x3D, 0x34, 0x2F, 0x26, 0x51, 0x58, 0x43, 0x4A, 0x75, 0x7C, 0x67, 0x6E
	.byte	0x32, 0x3B, 0x20, 0x29, 0x16, 0x1F, 0x04, 0x0D, 0x7A, 0x73, 0x68, 0x61, 0x5E, 0x57, 0x4C, 0x45
	.byte	0x2B, 0x22, 0x39, 0x30, 0x0F, 0x06, 0x1D, 0x14, 0x63, 0x6A, 0x71, 0x78, 0x47, 0x4E, 0x55, 0x5C
	.byte	0x64, 0x6D, 0x76, 0x7F, 0x40, 0x49, 0x52, 0x5B, 0x2C, 0x25, 0x3E, 0x37, 0x08, 0x01, 0x1A, 0x13
	.byte	0x7D, 0x74, 0x6F, 0x66, 0x59, 0x50, 0x4B, 0x42, 0x35, 0x3C, 0x27, 0x2E, 0x11, 0x18, 0x03, 0x0A
	.byte	0x56, 0x5F, 0x44, 0x4D, 0x72, 0x7B, 0x60, 0x69, 0x1E, 0x17, 0x0C, 0x05, 0x3A, 0x33, 0x28, 0x21
	.byte	0x4F, 0x46, 0x5D, 0x54, 0x6B, 0x62, 0x79, 0x70, 0x07, 0x0E, 0x15, 0x1C, 0x23, 0x2A, 0x31, 0x38
	.byte	0x41, 0x48, 0x53, 0x5A, 0x65, 0x6C, 0x77, 0x7E, 0x09, 0x00, 0x1B, 0x12, 0x2D, 0x24, 0x3F, 0x36
	.byte	0x58, 0x51, 0x4A, 0x43, 0x7C, 0x75, 0x6E, 0x67, 0x10, 0x19, 0x02, 0x0B, 0x34, 0x3D, 0x26, 0x2F
	.byte	0x73, 0x7A, 0x61, 0x68, 0x57, 0x5E, 0x45, 0x4C, 0x3B, 0x32, 0x29, 0x20, 0x1F, 0x16, 0x0D, 0x04
	.byte	0x6A, 0x63, 0x78, 0x71, 0x4E, 0x47, 0x5C, 0x55, 0x22, 0x2B, 0x30, 0x39, 0x06, 0x0F, 0x14, 0x1D
	.byte	0x25, 0x2C, 0x37, 0x3E, 0x01, 0x08, 0x13, 0x1A, 0x6D, 0x64, 0x7F, 0x76, 0x49, 0x40, 0x5B, 0x52
	.byte	0x3C, 0x35, 0x2E, 0x27, 0x18, 0x11, 0x0A, 0x03, 0x74, 0x7D, 0x66, 0x6F, 0x50, 0x59, 0x42, 0x4B
	.byte	0x17, 0x1E, 0x05, 0x0C, 0x33, 0x3A, 0x21, 0x28, 0x5F, 0x56, 0x4D, 0x44, 0x7B, 0x72, 0x69, 0x60
	.byte	0x0E, 0x07, 0x1C, 0x15, 0x2A, 0x23, 0x38, 0x31, 0x46, 0x4F, 0x54, 0x5D, 0x62, 0x6B, 0x70, 0x79 */


	.globl	recvMmcCmdResp1
recvMmcCmdResp1:
	mov	r1, #(48 / 8)



	.globl	recvMmcCmdResp
recvMmcCmdResp:
	stmfd	sp!, {r0-r1,r4-r7,lr}
	ldmfd	sp!, {r4-r5}
	
	sub	r7, r5, #1			@ r7 = number of bytes to recieve-1

	mov	r6, #(1024 * 8)
0:	bl	rdMmcCmdBit
	cmp	r0, #0
	beq	0f
	subs	r6, r6, #1
	bgt	0b
	mov	r0, #0
	b	1f

0:	mov	r0, #7
	bl	rdMmcCmdBits
	strb	r0, [r4],#1

	sub	r5, r5, #1
0:	bl	rdMmcCmdByte
	strb	r0, [r4],#1
	subs	r5, r5, #1
	bgt	0b

	mov	r0, #1

1:	ldmfd	sp!, {r4-r7,lr}
	bx	lr

	.globl	sendSdReadSingleBlockCmd4
sendSdReadSingleBlockCmd4:
	stmfd	sp!, {r4-r9,lr}
	sub	sp, sp, #8
	mov	r9, r0

	mov	r0, #17
	bl	sendMmcCmd
	mov	r0, sp
	bl	recvMmcCmdResp1
	cmp	r0, #0
	beq	2f
@	ldrb	r0, [sp]			@ comment these checks to make the routine smmmalllerrrrrrrrr :(((((
@	cmp	r0, #17
@	movne	r0, #0
@	bne	2f

	ldr	r1,=(0x08000000 | (1<<24) | (1<<19) | (1<<17) |                     (1<<13) |                                                 (1<<6) | (1<<5) ) @RD_SD_DAT

	mov	r3, #(1024 * 128)		@ increase this if its timing out too fast for some sd cards eg scrambled or no icons on valid nds files in the browser
						@ better still sort the FAT driver to handle this returning true or false and lower this to a sensible value!
0:	ldrh	r2, [r1]			@ wait for start bit
	tst	r2, #(1 << 8)
	beq	1f
	subs	r3, r3, #1
	bne	0b
	mov	r0, #0				@ return zero if start bit times out
	b	2f

1:	bl sendSdReadSingleBlockCmd4_1
		mov	r0, #1

2:	add	sp, sp, #8
	ldmfd	sp!, {r4-r9,lr}
	bx	lr
	
	.pool
	
	.globl	sendSdReadSingleBlockCmd4_1
sendSdReadSingleBlockCmd4_1:
		mov	r3, #(512 + 8)
0:	ldmia	r1, {r5-r8}

	cmp	r3, #8				@ Set output pointer to sp after 512 bytes have been read
	moveq	r9, sp

	mov	r2, r5,LSR #4
	and	r2, r2, #0x000000F0
	mov	r5, r5,LSR #24
	and	r5, r5, #0x0000000F
	orr	r4, r5, r2

	mov	r2, r6,LSL #4
	and	r2, r2, #0x0000F000
	mov	r6, r6,LSR #16
	and	r6, r6, #0x00000F00
	orr	r6, r6, r2
	orr	r4, r4, r6

	mov	r2, r7,LSL #12
	and	r2, r2, #0x00F00000
	mov	r7, r7,LSR #8
	and	r7, r7, #0x000F0000
	orr	r7, r7, r2
	orr	r4, r4, r7

	mov	r2, r8,LSL #20
	and	r2, r2, #0xF0000000
	and	r8, r8, #0x0F000000
	orr	r8, r8, r2
	orr	r4, r4, r8

	str	r4, [r9],#4
	subs	r3, r3, #4
	bgt	0b

	ldrh	r4, [r1]			@ Clock out Stop Bit

	@mov	r0, #1
	
	bx	lr



/*	.globl	sendSdStopTransmissionCmd4
sendSdStopTransmissionCmd4:
	stmfd	sp!, {lr}
	sub	sp, sp, #8

	mov	r0, #12
	bl	sendMmcCmd

	mov	r0, sp
	bl	recvMmcCmdResp1

	add	sp, sp, #8
	ldmfd	sp!, {lr}
	bx	lr */


/*
	.globl	sendSdReadMultipleBlockCmd4
sendSdReadMultipleBlockCmd4:
	stmfd	sp!, {lr}
	sub	sp, sp, #8

	mov	r1, r0
	mov	r0, #18
	bl	sendMmcCmd

	mov	r0, sp
	bl	recvMmcCmdResp1

	@ todo: consider checking for error flags in the response
	add	sp, sp, #8
	ldmfd	sp!, {lr}
	bx	lr



	.globl	recvSdReadMultipleBlock4
recvSdReadMultipleBlock4:
	stmfd	sp!, {r4-r9,lr}
	sub	sp, sp, #8
	mov	r9, r0

	ldr	r1, RD_SD_DAT

	mov	r3, #(1024 * 128)		@ increase this if its timing out too fast for some sd cards eg scrambled or no icons on valid nds files in the browser
						@ better still sort the FAT driver to handle this returning true or false and lower this to a sensible value!
0:	ldrh	r2, [r1]			@ wait for start bit
	tst	r2, #(1 << 8)
	beq	1f
	subs	r3, r3, #1
	bne	0b
	mov	r0, #0				@ return zero if start bit times out
	b	2f

1:	mov	r3, #(512 + 8)
0:	ldmia	r1, {r5-r8}

	cmp	r3, #8				@ Set output pointer to sp after 512 bytes have been read
	moveq	r0, sp

	mov	r2, r5,LSR #4
	and	r2, r2, #0x000000F0
	mov	r5, r5,LSR #24
	and	r5, r5, #0x0000000F
	orr	r4, r5, r2

	mov	r2, r6,LSL #4
	and	r2, r2, #0x0000F000
	mov	r6, r6,LSR #16
	and	r6, r6, #0x00000F00
	orr	r6, r6, r2
	orr	r4, r4, r6

	mov	r2, r7,LSL #12
	and	r2, r2, #0x00F00000
	mov	r7, r7,LSR #8
	and	r7, r7, #0x000F0000
	orr	r7, r7, r2
	orr	r4, r4, r7

	mov	r2, r8,LSL #20
	and	r2, r2, #0xF0000000
	and	r8, r8, #0x0F000000
	orr	r8, r8, r2
	orr	r4, r4, r8

	str	r4, [r0],#4
	subs	r3, r3, #4
	bgt	0b

	ldrh	r4, [r1]			@ Clock out Stop Bit

	mov	r0, #1

2:	add	sp, sp, #8
	ldmfd	sp!, {r4-r9,lr}
	bx	lr
*/
