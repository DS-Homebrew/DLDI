    .TEXT
@--------------u8 sd_crc7_s(u16* buff,u16 num)----------------------------
    .ALIGN
    .GLOBAL	mpsd_crc7_s 
    .CODE 32
mpsd_crc7_s:
	stmfd   r13!,{r4}

	mov	r3,#0
	ldr	r4,=0x80808080
	mov	r1,r1,lsl #3 @ *8
sd_crc7_loop:
	tst	r4,#0x80
	ldrneb	r2,[r0],#1

	mov	r3,r3,lsl #1

	tst	r3,#0x80
	eorne	r3,r3,#0x9

	tst	r2,r4,lsr #24
	eorne	r3,r3,#0x9

	mov	r4,r4,ror #1
	subs	r1,r1,#1
        bne     sd_crc7_loop 

	mov	r3,r3,lsl #1
	add	r0,r3,#1
	ldmfd	r13!,{r4}
	bx      r14
@--------------end sd_crc7_s----------------------------


    .END
