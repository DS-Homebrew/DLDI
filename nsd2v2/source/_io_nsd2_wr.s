.macro MASK_AND_SHIFT_BIT4_TO_HALF, r_bit4
	and	\r_bit4, \r_bit4, #15						@ mask nibble
	mov	\r_bit4, \r_bit4, lsl #8				@ shift into final position
.endm

.macro MASK_AND_SHIFT_BYTE_TO_WORD,	r_byte, r_scratch
	and	\r_scratch, \r_byte, #(15<<4)		@ mask top nibble from byte into scratch
	mov	\r_scratch, \r_scratch, lsl #4	@ shift into final position
	and	\r_byte, \r_byte, #15						@ mask bottom nibble from byte into scratch
	mov	\r_byte, \r_byte, lsl #(8+16)		@ shift into final position
	orr	\r_byte, \r_byte, \r_scratch		@ combine nibbles
.endm

WRITE_ADDR:
	.long ( 0x08000000 | ( 1<<24 ) | ( 1<<19 ) | ( 1<<18 ) | ( 1<<17 ) | ( 1<<14 ) | ( 1<<13 ) )
	
	.globl wrMmcDatBit4
wrMmcDatBit4:
	MASK_AND_SHIFT_BIT4_TO_HALF r0
	ldr	r1, WRITE_ADDR
	strh	r0,[r1]
	bx	lr

	.globl	wrMmcDatByte4
wrMmcDatByte4:
	MASK_AND_SHIFT_BYTE_TO_WORD r0, r1
	ldr	r1, WRITE_ADDR
	str	r0, [r1]
	bx	lr

	.global wrSdDat4
wrSdDat4:
	stmfd	sp!, {r4-r11}
	ldr		r7, WRITE_ADDR
0:
	ldmia	r0!, {r2,r8}
	mov		r5, r2, lsr #24
	mov		r4, r2, lsr #16
	and		r4, r4, #0xff
	mov		r3, r2, lsr #8
	and		r3, r3, #0xff
	and		r2, r2, #0xff
	mov		r11, r8, lsr #24
	mov		r10, r8, lsr #16
	and		r10, r10, #0xff
	mov		r9, r8, lsr #8
	and		r9, r9, #0xff
	and		r8, r8, #0xff
	MASK_AND_SHIFT_BYTE_TO_WORD	r2,r6
	MASK_AND_SHIFT_BYTE_TO_WORD	r3,r6
	MASK_AND_SHIFT_BYTE_TO_WORD	r4,r6
	MASK_AND_SHIFT_BYTE_TO_WORD	r5,r6
	MASK_AND_SHIFT_BYTE_TO_WORD	r8,r6
	MASK_AND_SHIFT_BYTE_TO_WORD	r9,r6
	MASK_AND_SHIFT_BYTE_TO_WORD	r10,r6
	MASK_AND_SHIFT_BYTE_TO_WORD	r11,r6	
	stmia r7, {r2-r5,r8-r11}
	subs	r1, r1, #8
	bne		0b
	ldmfd	sp!, {r4-r11}
	bx	lr
	
