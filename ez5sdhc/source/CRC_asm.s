 .ALIGN
 .GLOBAL SD_cal_crc16
 .CODE 32
 .ARM


SD_cal_crc16:
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
 .END
