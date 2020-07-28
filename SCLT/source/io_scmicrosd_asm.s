    .TEXT
@ support Super card micro sd/rumble

@--------------------------------sd data--------------------------------
.equ en_dma3,0

.equ sd_comadd,0x9800000
.equ sd_dataadd,0x9000000  
.equ sd_reset,0x9440000


.equ sd_command_bit,0x800000
.equ sd_data_bit,0

.equ sd_buff_bit,0x400000
.equ sd_port_bit,0

.equ sd_crc_bit,0x100000
.equ sd_nocrc_bit,0

.equ sd_st,(0x040000+sd_crc_bit)

.equ sd_rw4,0x200000
.equ sd_rw3,0x080000
.equ sd_rw2,0x280000
.equ sd_rw1,0

.equ en_fireware,0
.equ en_sdram,1
.equ en_sdcard,2
.equ en_write,4
.equ en_rumble,8
.equ en_rumble_user_flash,1
@rumble ver, user flash size:8Mbit,Sector Size:0x10000,map:0x8000000 - 0x80fffff
.GLOBAL scms_ReadSector,scms_WriteSector,scms_InitSCMode,scms_MemoryCard_IsInserted,scms_sdcard_reset
@extern "C" void scms_ReadSector (u32 *buff,u32 sector,u8 readnum);
@extern "C" void scms_WriteSector (u32 *buff,u32 sector,u8 writenum);
@extern "C" void scms_InitSCMode (void);
@extern "C" bool scms_MemoryCard_IsInserted (void);
@extern "C" void scms_sdcard_reset(void);

@-----------------viod sdms_data_write_s(u32 *buff,u16* crc16buff)-------------------
    .ALIGN
@    .GLOBAL sdms_data_write_s	
    .CODE 32
sdms_data_write_s:
	stmfd   r13!,{r4}
	mov	r2,#sd_dataadd
sd_data_write_busy:
	ldrh	r3,[r2]   
	tst	r3,#0x100
	beq	sd_data_write_busy
	ldrh	r3,[r2]   

	mov	r3,#0 @star bit
	strh	r3,[r2]
.if en_dma3
	mov	r4,#0x4000000
	str	r0,[r4,#0xd4]
	mov	r3,#(0x9000000+sd_rw4)
	str	r3,[r4,#0xd8]
	ldr	r3,=0x80000100
	str	r3,[r4,#0xdc]
dma3_w_loop:
	ldr	r3,[r4,#0xdc]  
    tst r3,#0x80000000
    bne dma3_w_loop
.else
	ldr	r2,=(0x9000000+sd_rw4)@
	mov	r3,#(512/32)
    stmfd   r13!,{r5-r11}
loaddataw:
	ldmia   r0!,{r4-r11}
	stmia   r2,{r4-r11}
	subs    r3, r3, #1 
	bne     loaddataw
	ldmfd	r13!,{r5-r11}
.endif
    ldrh	r3,[r1] @crc
    mov     r4,#(0x9000000+sd_rw4)
    strh	r3,[r4]
    ldrh	r3,[r1,#2] @crc
    strh	r3,[r4]
    ldrh	r3,[r1,#4] @crc
    strh	r3,[r4]
    ldrh	r3,[r1,#6] @crc
    strh	r3,[r4]
    mov	r2,#sd_dataadd
    mov	r3,#0xff @end bit
	strh	r3,[r2]
sd_data_write_loop2:
	ldrh	r3,[r2]   
	tst	r3,#0x100
	bne	sd_data_write_loop2
	ldmia   r2,{r3-r4} 
	ldmfd	r13!,{r4}
	bx      r14

@----------void sdms_data_read_s(u32 *buff)-------------

    .ALIGN
@    .GLOBAL sdms_data_read_s	
    .CODE 32
sdms_data_read_s:
	stmfd   r13!,{r4}

    ldr r4,=(0x9000000+sd_st)
    ldrh    r4,[r4]
    ldr r4,=(0x9000000+sd_buff_bit)
sd_data_read_loop1:
    nop
	ldrh r3,[r4]   
	tst	r3,#1
	bne	sd_data_read_loop1

.if en_dma3
	mov	r4,#0x4000000
	ldr	r2,=(0x9000000+sd_rw4)@
	ldrh	r3,[r2]
	str	r2,[r4,#0xd4]
	str	r0,[r4,#0xd8]
	ldr	r3,=0x80000100
	str	r3,[r4,#0xdc]
dma3_r_loop:
	ldr	r3,[r4,#0xdc]  
    tst r3,#0x80000000
    bne dma3_r_loop

.else
	ldr	r2,=(0x9000000+sd_rw4)@
	ldrh	r3,[r2]
	mov	r1,#(512/32)
    stmfd   r13!,{r5-r10}
loaddatar:
	ldmia   r2,{r3-r10}                  @ currently fills whole 256K of RAM,
	stmia   r0!,{r3-r10}                  @ even though the proggy is smaller
	subs    r1, r1, #1                   @
	bne     loaddatar
	ldmfd	r13!,{r5-r10}  
.endif
	ldr	r2,=(0x9000000+sd_rw4)@

	ldr	r3,[r2] @crc 
	ldrh	r3,[r2] @crc 
	ldr	r2,=(0x9000000+sd_rw1) @end bit
	ldrh	r3,[r2]

	ldmfd	r13!,{r4}  
	bx      r14
@----------end sdms_data_read_s-------------

@------void sdms_crc16_s(u16* buff,u16 num,u16* crc16buff)
    .ALIGN
 @   .GLOBAL	sdms_crc16_s 
    .CODE 32
sdms_crc16_s:
	stmfd   r13!,{r4-r9}
	mov	r9,r2

	mov	r3,#0  
	mov	r4,#0  
	mov	r5,#0  
	mov	r6,#0  

	ldr	r7,=0x80808080
	ldr	r8,=0x1021
	mov	r1,r1,lsl #3
sd_crc16_loop:

	tst	r7,#0x80
	ldrneb	r2,[r0],#1

	mov	r3,r3,lsl #1
	tst	r3,#0x10000
	eorne	r3,r3,r8
	tst	r2,r7,lsr #24
	eorne	r3,r3,r8
	
	mov	r4,r4,lsl #1
	tst	r4,#0x10000
	eorne	r4,r4,r8
	tst	r2,r7,lsr #25
	eorne	r4,r4,r8
	
	mov	r5,r5,lsl #1
	tst	r5,#0x10000
	eorne	r5,r5,r8
	tst	r2,r7,lsr #26
	eorne	r5,r5,r8
	
	mov	r6,r6,lsl #1
	tst	r6,#0x10000
	eorne	r6,r6,r8
	tst	r2,r7,lsr #27
	eorne	r6,r6,r8

	mov	r7,r7,ror #4
	subs	r1,r1,#4
        bne     sd_crc16_loop 

	mov	r2,r9
	mov	r8,#16
sd_crc16_write_data:
	mov	r7,r7,lsl #4
	tst	r3,#0x8000
	orrne	r7,r7,#8
	tst	r4,#0x8000
	orrne	r7,r7,#4
	tst	r5,#0x8000
	orrne	r7,r7,#2
	tst	r6,#0x8000
	orrne	r7,r7,#1

	mov	r3,r3,lsl #1
	mov	r4,r4,lsl #1
	mov	r5,r5,lsl #1
	mov	r6,r6,lsl #1

	sub	r8,r8,#1
	tst	r8,#1
	streqb	r7,[r2],#1
	cmp	r8,#0
	bne	sd_crc16_write_data

	ldmfd	r13!,{r4-r9}
	bx      r14
@------end sdms_com_crc16_s-----------------------------------


@--------------sdms_com_read_s(u16* buff,u32 num)------------------
		.ALIGN
@		.GLOBAL	 sdms_com_read_s 
		.CODE 32
sdms_com_read_s:
	stmfd   r13!,{r4-r6}
	mov	r2,#sd_comadd
sd_com_read_loop1:
	ldrh	r3,[r2] 
	tst	r3,#1
	bne	sd_com_read_loop1

	ldr	r3,=(0x9000000+sd_command_bit+sd_rw3)@first 8bit:1 start bit+3bit+4bit
	ldrh	r2,[r3]
	mov	r3,#(0x9000000+sd_command_bit+sd_rw4)
	ldrh	r2,[r3]
	bic	r2,r2,#0x80 
	strb	r2,[r0],#1

	sub	r1,r1,#1

sd_com_read_loop:
	ldr	r2,[r3]
	mov	r2,r2,lsr #16
	strb	r2,[r0],#1
	subs	r1,r1,#1
	bne	sd_com_read_loop

	ldmfd	r13!,{r4-r6}
	bx      r14
@--------------end sdms_com_read_s------------------

@-------------------void sdms_com_write_s(u16* buff,u32 num)-----------------
		.ALIGN
@		.GLOBAL	 sdms_com_write_s 
		.CODE 32
sdms_com_write_s:
	stmfd   r13!,{r4-r6}
	mov	r2,#sd_comadd
sd_com_write_busy:
	ldrh	r3,[r2]   
	tst	r3,#0x1
	beq	sd_com_write_busy
	ldrh	r3,[r2]  

	mov	r3,#(0x9000000+sd_buff_bit) @clr crc7
	strh	r3,[r3]
sd_com_write_loop:
	mov	r3,#(0x9000000+sd_command_bit+sd_rw4)
	ldrb   r4,[r0],#1
	add	r4,r4,r4,lsl #20
	str	r4,[r3]
	subs	r1,r1,#1
	bne	sd_com_write_loop
	mov	r3,#(0x9000000+sd_command_bit+sd_crc_bit+sd_rw4)@write crc7
	str	r3,[r3]

	ldmfd	r13!,{r4-r6}

	bx      r14
@-------------------end sdms_com_write_s-----------------

@-----------------void scms_send_clk(u32 num)---------
		.ALIGN
@		.GLOBAL	 scms_send_clk 
		.CODE 32

scms_send_clk:
	mov	r1,#sd_comadd
send_clk_loop1:
	ldrh	r3,[r1]
	subs	r0,r0,#1
	bne	send_clk_loop1
	bx	r14
@-----------------end scms_send_clk---------

@---------------------------void scms_SDCommand(u8 command,u8 num,u32 sector)--------------------
		.ALIGN
@		.GLOBAL	 scms_SDCommand 
		.CODE 32
@void scms_SDCommand(u8 command,u8 num,u32 sector )
@{
@	u8 databuff[6];
@	register u8 *char1;
@	register u8 *char2;
@
@	char1=(u8*)(((u32)&sector)+3);
@	char2=(u8*)databuff;
@	*char2++=coma+0x40;
@	*char2++=*char1--;
@	*char2++=*char1--;
@	*char2++=*char1--;
@	*char2++=*char1;
@	@*char2=sd_crc7_s((u32)databuff,5);
@
@	sdms_com_write_s((u32)databuff,5);
@
@}
scms_SDCommand:
	stmfd   r13!,{r14}

	sub	r13,r13,#16
	add	r0,r0,#0x40
	strb	r0,[r13,#4]

	mov	r1,r2,lsr #24
	strb	r1,[r13,#5]
	mov	r1,r2,lsr #16
	strb	r1,[r13,#6]
	mov	r1,r2,lsr #8
	strb	r1,[r13,#7]
	strb	r2,[r13,#8]
	add	r0,r13,#4
	mov	r1,#5
	@bl	sd_crc7_s
	@strb	r0,[r13,#9]
	@add	r0,r13,#4
	@mov	r1,#6
	bl	sdms_com_write_s

	add	r13,r13,#16
	ldmfd   r13!,{r15}
@	bx	r14
@---------------------------end scms_SDCommand--------------------

@----------void scms_ReadSector(u16 *buff,u32 sector,u8 readnum)-------------
		.ALIGN
@		.GLOBAL	 scms_ReadSector @ r0:Srcp r1:num ok
		.CODE 32

@void scms_ReadSector(u16 *buff,u32 sector,u8 readnum)
@{
@	register u16 i,j;
@   scms_sdcard_reset();
@	i=readnum;
@	sectno<<=9;
@	scms_SDCommand(18,0,sector); 
@	for (j=0;j<i ; j++)
@	{
@		sdms_data_read_s((u32)buff+j*512);
@	}
@	scms_SDCommand(12,0,0); 
@	scms_get_resp();
@	scms_send_clk(0x10);
@
@}
scms_ReadSector:
	stmfd   r13!,{r4-r6,r14}

	mov	r4,r0
	mov	r5,r2
    mov r6,r1
    mov r0,#en_sdcard
    bl  scms_mode

    bl scms_sdcard_reset
	mov	r2,r6,lsl #9
	mov	r0,#18
	mov	r1,#0
	bl	scms_SDCommand
	mov	r6,#0
beginforj_ReadSector:
	cmp	r6,r5
	bge	endforj_ReadSector
	mov	r0,r4
	add	r0,r0,r6,lsl #9 
	bl	sdms_data_read_s
	add	r6,r6,#1
	b	beginforj_ReadSector
endforj_ReadSector:
	mov	r0,#12
	mov	r1,#0
	mov	r2,#0
	bl	scms_SDCommand
	bl	scms_get_resp
	mov	r0,#0x10
	bl	scms_send_clk
	mov	r0,#1
	ldmfd   r13!,{r4-r6,r15}
@	bx	r14
@----------end ReadSector------------

@-----------void scms_get_resp(void)-------------------


		.ALIGN
@		.GLOBAL	 scms_get_resp @ r0:Srcp r1:num ok
		.CODE 32
scms_get_resp:
	stmfd   r13!,{r4-r6}
	mov	r2,#sd_comadd
get_resp_loop1:
	ldrh	r3,[r2] @star bit
	tst	r3,#1
	bne	get_resp_loop1
    mov	r1,#6
get_resp_loop:
	ldmia   r2,{r3-r6}
        subs    r1, r1, #1                  
        bne     get_resp_loop  
	ldmfd	r13!,{r4-r6}
	bx      r14

@-----------end scms_get_resp-------------------
@---------------void scms_WriteSector(u16 *buff,u32 sector,u8 writenum)---------------------
		.ALIGN
@		.GLOBAL	 scms_WriteSector @ r0:Srcp r1:num ok
		.CODE 32

@void scms_WriteSector(u16 *buff,u32 sector,u8 writenum)
@{
@	register u16 i,j;
@	u16 crc16[5];
@   scms_sdcard_reset();
@	i=writenum;
@
@	sectno<<=9;
@
@	scms_SDCommand(25,0,sector); 
@	scms_get_resp();
@	scms_send_clk(0x10); 
@
@	for (j=0;j<i ; j++)
@	{
@		sdms_crc16_s((u32)(u32)buff+j*512,512,(u32)crc16);
@		sdms_data_write_s((u32)buff+j*512,(u32)crc16);
@		scms_send_clk(0x10); 
@	}
@	scms_SDCommand(12,0,0); 
@	scms_get_resp();
@	scms_send_clk(0x10);
@	while((*(u16*)sd_dataadd &0x0100)==0);
@
@}
@
scms_WriteSector:
	stmfd   r13!,{r4-r6,r14}

	sub	r13,r13,#20

	mov	r4,r0
	mov	r5,r2
    mov r6,r1
    mov r0,#en_sdcard
    bl  scms_mode

    bl scms_sdcard_reset

	mov	r2,r6,lsl #9
	mov	r0,#25
	mov	r1,#0
	bl	scms_SDCommand
	bl	scms_get_resp
	mov	r0,#0x10
	bl	scms_send_clk
	mov	r6,#0

beginforj_WriteSector:
	cmp	r6,r5
	bge	endforj_WriteSector
	mov	r0,r4
	add	r0,r0,r6,lsl #9
	mov	r1,#512
	add	r2,r13,#4
	bl	sdms_crc16_s
	mov	r0,r4
	add	r0,r0,r6,lsl #9 
	add	r1,r13,#4
	bl	sdms_data_write_s
	mov	r0,#0x10
	bl	scms_send_clk
	add	r6,r6,#1
	b	beginforj_WriteSector
endforj_WriteSector:
	mov	r0,#12
	mov	r1,#0
	mov	r2,#0
	bl	scms_SDCommand
	bl	scms_get_resp
	mov	r0,#0x10
	bl	scms_send_clk
	ldr	r0,=sd_dataadd
beginwhile_WriteSector:
	ldrh	r1,[r0]
	tst	r1,#0x0100
	beq	beginwhile_WriteSector
	mov	r0,#1
	add	r13,r13,#20
	ldmfd   r13!,{r4-r6,r15}
@---------------end SCMS_WriteSector--------------------


@----------------void scms_InitSCMode(void)---------------
		.ALIGN
@		.GLOBAL	 scms_InitSCMode
		.CODE 32
scms_InitSCMode:
	stmfd   r13!,{r14}
    mov	r0,#(en_sdram+en_sdcard) @sdram map:0x8000000 - 0x8ffffff,sd card reg map:0x9000000-0x9ffffff
    bl scms_mode
	ldmfd   r13!,{r15}

@	mvn     r0,#0x0F6000000 
@	sub     r0,r0,#0x01
@	mov     r1,#0x0A500
@	add     r1,r1,#0x5A
@	strh    r1,[r0]
@	strh    r1,[r0]
@	mov	r2,#(en_sdram+en_sdcard)
@	strh    r2,[r0]
@	strh    r2,[r0]
@	bx	r14
@----------------end InitSCMode ---------------

@----------------bool MemoryCard_IsInserted(void)---------------
		.ALIGN
@		.GLOBAL	 scms_MemoryCard_IsInserted
		.CODE 32
scms_MemoryCard_IsInserted:
	ldr	r0,=sd_comadd
	ldrh	r1,[r0]
    and r1,r1,#0xe300
    cmp r1,#0xa000   @micro ver
    cmpne r1,#0xc000 @rumble ver
	moveq	r0,#1
	movne	r0,#0
	bx	r14
@----------------end MemoryCard_IsInserted---------------

@----------------void scms_sdcard_reset(void)---------------
scms_sdcard_reset:
    ldr r0,=0x4000204
    ldrh r1,[r0]
    bic r1,r1,#0x1c
    strh r1,[r0]   @micro sd/rumble must set
    ldr	r0, =sd_reset
    strh	r0, [r0]
	bx	r14
@----------------void scms_sdcard_reset(void)---------------

@----------------void scms_mode(u16 data)---------------
		.ALIGN
@		.GLOBAL	 scms_mode
		.CODE 32
scms_mode:
	mvn     r2,#0x0F6000000 
	sub     r2,r2,#0x01
	mov     r1,#0x0A500
	add     r1,r1,#0x5A
	strh    r1,[r2]
	strh    r1,[r2]
	strh    r0,[r2]
	strh    r0,[r2]
	bx	r14
@@----------------end scms_mode---------------
@
@@-----------------void scms_write_sdram(u16 *s,u16 *d,int num)-----not support rumble ver---------
@		.ALIGN
@@		.GLOBAL	 scms_write_sdram
@		.CODE 32
@scms_write_sdram:
@	stmfd   r13!,{r4-r6,r14}
@    mov r4,r0
@    mov r5,r1
@    mov r6,r2
@
@    ldr r0,=0x4000204
@    ldrh r1,[r0]
@    bic r1,r1,#0x1c
@    strh r1,[r0] 
@    mov r0,#(en_write+en_sdram)
@    bl scms_mode
@write_sdram_loop:
@    ldrh r3,[r4],#2
@    strh r3,[r5],#2
@    subs r6,r6,#2
@    bgt write_sdram_loop
@
@	ldmfd   r13!,{r4-r6,r15}
@@-----------------end scms_write_sdram----
@
@@-----------------void scms_read_sdram(u16 *s,u16 *d,int num)----not support rumble ver---------
@		.ALIGN
@@		.GLOBAL	 scms_read_sdram
@		.CODE 32
@scms_read_sdram:
@	stmfd   r13!,{r4-r6,r14}
@    mov r4,r0
@    mov r5,r1
@    mov r6,r2
@
@    ldr r0,=0x4000204
@    ldrh r1,[r0]
@    bic r1,r1,#0x1c
@    strh r1,[r0] 
@    mov r0,#(en_sdram)
@    bl scms_mode
@read_sdram_loop:
@    ldrh r3,[r4],#2
@    strh r3,[r5],#2
@    subs r6,r6,#2
@    bgt read_sdram_loop
@
@	ldmfd   r13!,{r4-r6,r15}
@@-----------------end scms_read_sdram----
@
@
@@----------bool scms_rumble_Inserted(void)----------only support rumble ver---------
@		.ALIGN
@@		.GLOBAL	 scms_rumble_Inserted
@		.CODE 32
@scms_rumble_Inserted:
@	stmfd   r13!,{r14}
@    mov r0,#(en_rumble)
@    bl scms_mode
@    mov r0,#0x8000000
@    ldrh r1,[r0]
@    ldrh r2,[r0,#0xe]
@    tst  r1,#2
@    tsteq r2,#2
@	moveq	r0,#1
@	movne	r0,#0
@	ldmfd   r13!,{r15}
@@----------end scms_rumble_Inserted---------
@
@@------------bool scms_rumble_flash_Erase(u32 flash_address)-----only support rumble ver---------
@		.ALIGN
@@		.GLOBAL	 scms_rumble_flash_Erase
@		.CODE 32
@scms_rumble_flash_Erase:
@	stmfd   r13!,{r4-r5,r14}
@
@    mov r4,r0
@    mov r0,#(en_rumble_user_flash)
@    bl scms_mode
@    ldr r0,=0xffff8000
@    and r0,r0,r4
@    ldr r1,=0xaaa
@    add r1,r1,r0
@    ldr r2,=0x554
@    add r2,r2,r0
@    mov r0,#0xaa
@    mov r3,#0x55
@
@    strh r0,[r1]
@    strh r3,[r2]
@    mov r5,#0x80
@    strh r5,[r1]
@
@    strh r0,[r1]
@    strh r3,[r2]
@    mov r5,#0x30
@    strh r5,[r4]
@
@    ldr r2,=0xffff
@    mov r0,#0x3d0000
@flash_Erase_loop:
@    ldrh r1,[r4]
@    subs r0,r0,#1
@    beq  flash_Erase_end
@    cmp r1,r2
@    bne flash_Erase_loop
@flash_Erase_end:
@    mov r2,#0xf0
@    mov r1,#0x8000000
@    strh r2,[r1]
@    cmp r0,#0
@    movne r0,#1
@    moveq r0,#0
@	ldmfd   r13!,{r4-r5,r15}
@@------------end scms_rumble_flash_Erase--------
@
@@------------bool scms_rumble_flash_write(u32 flash_address,u16 data)-----only support rumble ver---------
@		.ALIGN
@@		.GLOBAL	 scms_rumble_flash_write
@		.CODE 32
@scms_rumble_flash_write:
@	stmfd   r13!,{r4-r5,r14}
@    mov r4,r0
@    mov r5,r1
@    mov r0,#(en_rumble_user_flash)
@    bl scms_mode
@    ldr r0,=0xffff8000
@    and r0,r0,r4
@    ldr r1,=0xaaa
@    add r1,r1,r0
@    ldr r2,=0x554
@    add r2,r2,r0
@    mov r0,#0xaa
@    mov r3,#0x55
@
@    strh r0,[r1]
@    strh r3,[r2]
@    mov r0,#0xa0
@    strh r0,[r1]
@
@    strh r5,[r4]
@    mov r0,#0x100
@flash_write_loop:
@    ldrh r1,[r4]
@    subs r0,r0,#1
@    beq  flash_write_end
@    cmp r1,r5
@    bne flash_write_loop
@flash_write_end:
@    mov r2,#0xf0
@    mov r1,#0x8000000
@    strh r2,[r1]
@    cmp r0,#0
@    movne r0,#1
@    moveq r0,#0
@	ldmfd   r13!,{r4-r5,r15}
@@------------end scms_rumble_flash_write--------
    .END

