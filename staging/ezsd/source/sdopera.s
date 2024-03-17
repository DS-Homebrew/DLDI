@***********************************************************
@
@	by Rudolph (鼓碾)
@
@	2006/11/29
@		* CRC is read with SD_ReadData. 
@	2006/11/22
@		* The calculation routine of CRC16 is added.
@
@-----------------------------------------------------------
@   Original is EZ4FATLIB code offered by EZTEAM.
@ Hardware Routines for SD card using a EZ4 series.
@-----------------------------------------------------------
@
@***********************************************************

	.ALIGN
	.CODE 32
	.ARM

.global  SD_WriteBufferToLine, SD_WriteBufferToLine, SD_ReadResponse
.global  SD_ReadData, SD_WriteData, crc_730, SD_cal_crc16


asmCPUCopy32:
	stmfd sp!,{r3-r10}
	add  r2, r0,r2
loopcpu:
	ldmia r0!,{r3-r10}
	stmia r1!,{r3-r10}
	ldmia r0!,{r3-r10}
	stmia r1!,{r3-r10}
	ldmia r0!,{r3-r10}
	stmia r1!,{r3-r10}
	ldmia r0!,{r3-r10}
	stmia r1!,{r3-r10}
	cmp	 r2,r0
	bhi	loopcpu
	
	ldmfd sp!,{r3-r10}
	bx 	lr
	
	
SD_WriteBufferToLine:
#  r0 - char *pBuf  , r1 - int lenght
	stmfd	sp!,{r2-r7}
	ldr		r3,=0x9FFFF40
sdw_l1: 						#此段为命令字节个数的循环
	ldrb		r2, [r0],#+1
	mov		r6,#0
sdw_l2:						#此段为8字节的位发送循环
	mov		r4,r2,lsl r6
	strh		r4,[r3]
	add		r6,r6,#1
	cmp		r6,#8
	bne		sdw_l2
	subs		r1,r1,#1
	bne		sdw_l1			
	ldmfd	sp!,{r2-r7}
	bx		lr 

SD_ReadResponse:
# r0 - char *pBuf , r1 - int lenght , r2 - waitlen[timeout]
# 返回值
	stmfd	sp!,{r3-r7}
#	cmp		r1,#0			#长度为0退出
#	beq		sdexit1
#	cmp		r2,#0
#	beq		sdexit
	ldr		r3,=0x9FFFF40	
sdrr1:
	subs		r2,r2,#1
	beq		sdexit2
	ldrh		r4,[r3]
	tst		r4,#0x1
	bne		sdrr1
	mov		r4,#0
readnext:
	mov		r7,#8
	mov		r6,#0
sdr_l2:
	ldrneh	r4,[r3]
	and		r5,r4,#1
	subs		r7,r7,#1
	add		r6,r6,r5,lsl r7
	bne		sdr_l2
	strb		r6,[r0],#+1
	subs		r1,r1,#1
	bne		readnext
sdexit1:	
#	response need loop for next command
	ldrh		r4,[r3]
	ldrh		r5,[r3]
	ldrh		r6,[r3]
	ldrh		r7,[r3]
	ldrh		r4,[r3]
	ldrh		r5,[r3]
	ldrh		r6,[r3]
	ldrh		r7,[r3]
	mov		r0,#1
	ldmfd	sp!,{r3-r7}
	bx		lr 
sdexit2:
	mov		r0,#0
	ldmfd	sp!,{r3-r7}
	bx		lr


SD_ReadData:
#r0- buf , r1 - size , r2 - CRCbuf , r3 - waitlen[timeout]
	stmfd	sp!, {r3-r6}
	ldr	r5, =0x9FEA000
	mov	r4, #0xF00
sdrd_l1:
	subs	r3, r3, #1
	moveq	r0, #0
	beq	sdr_exit

	ldrh	r6, [r5]
	ands	r6, r6, r4	
	bne	sdrd_l1
@ DATA
sdrd_l2:
	ldmia	r5, {r3-r4}
	mov	r6, r4, lsr #16
	strh	r6, [r0], #+2
	ldmia	r5, {r3-r4}
	mov	r6, r4, lsr #16
	strh	r6, [r0], #+2
	ldmia	r5, {r3-r4}
	mov	r6, r4, lsr #16
	strh	r6, [r0], #+2
	ldmia	r5, {r3-r4}
	mov	r6, r4, lsr #16
	strh	r6, [r0], #+2
	subs	r1, r1, #8
	bne	sdrd_l2
@ CRC
	ldmia	r5, {r3-r4}
	mov	r6, r4, lsr #16
	strh	r6, [r2], #+2
	ldmia	r5, {r3-r4}
	mov	r6, r4, lsr #16
	strh	r6, [r2], #+2
	ldmia	r5, {r3-r4}
	mov	r6, r4, lsr #16
	strh	r6, [r2], #+2
	ldmia	r5, {r3-r4}
	mov	r6, r4, lsr #16
	strh	r6, [r2], #+2
@ STS
	ldrh	r6, [r5]
	strh	r6, [r2], #+2
	ldrh	r6, [r5]
	strh	r6, [r2], #+2

	mov	r0, #1
sdr_exit:
	ldmfd	sp!, {r3-r6}
	bx	lr



SD_WriteData:
#r0- buf , r1 - size ,  r2 - waitlen[timeout]
	stmfd	sp!,{r3-r7}
	ldr		r3,=0x9FEA000
#start bit 
	mov		r4,#0
	strh		r4,[r3]
sdwd_l1:
	ldrb		r4,[r0], #+1
	ldrb		r5,[r0], #+1
	mov		r4,r4,lsl #8
	strh		r4,[r3]
	mov		r4,r4,lsl #4
	strh		r4,[r3,#2]
	mov		r5,r5,lsl #8
	strh		r5,[r3,#4]
	mov		r5,r5, lsl #4
	strh		r5,[r3,#6]
	subs		r1,r1,#2
	bne		sdwd_l1
#end bit 
	mov		r4,#0xFFFFFFFF	
	strh		r4,[r3]
sdw_exit:

	ldmfd	sp!,{r3-r7}
	bx		lr
	
crc_730:
#r0 - buf , r1 size	
	stmfd	sp!,{r2-r4}
	mov		r4,#0  @  crc
crc_l1:
	mov		r2,#0x80
	ldrb		r3,[r0],#+1
crc_l2:
	tst		r4,#0x40
	mov 	r4,r4,lsl #1
	eorne	r4,r4,#0x9
	tst		r3,r2
	eorne	r4,r4,#0x9
	movs	r2,r2,lsr #1
	bne		crc_l2
	subs		r1,r1,#1
	bne 		crc_l1
	mov		r0,r4
	ldmfd	sp!,{r2-r4}
	bx		lr


@===========================================
@ SD_cal_crc16
@	r0:u16* buff
@	r1:u16 num byte
@	r2:u16* crc16buff
@===========================================
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
