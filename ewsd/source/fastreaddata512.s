 	.global sdmFastReadData512
 	.thumb
	.align	2
	.thumb_func

@------------------------------------------------------
@------------------------------------------------------
sdmFastReadData512:
                 push	{r4, r5}

                 MOV    R1, #150
                 LSL	R1, R1, #20		@ Get SD_DATA_ADDR b00000

                 MOV 	R2, #1		@ set block size
		 LSL	R2, R2, #9


loop:
                 LDMIA   R1!, {R3,R4}
                 LSR     R3, R3, #0x10
                 STRH    R3, [R0]
                 ADD     R0, #2
		 
                 LDMIA   R1!, {R3,R4}
                 LSR     R3, R3, #0x10
                 STRH    R3, [R0]
                 ADD     R0, #2
                 
		 LDMIA   R1!, {R3,R4}
                 LSR     R3, R3, #0x10
                 STRH    R3, [R0]
                 ADD     R0, #2
                 
		 LDMIA   R1!, {R3,R4}
                 LSR     R3, R3, #0x10
                 STRH    R3, [R0]
                 ADD     R0, #2
                 
		 LDMIA   R1!, {R3,R4}
                 LSR     R3, R3, #0x10
                 STRH    R3, [R0]
                 ADD     R0, #2
                 
		 LDMIA   R1!, {R3,R4}
                 LSR     R3, R3, #0x10
                 STRH    R3, [R0]
                 ADD     R0, #2
                 
		 LDMIA   R1!, {R3,R4}
                 LSR     R3, R3, #0x10
                 STRH    R3, [R0]
                 ADD     R0, #2
                 
		 LDMIA   R1!, {R3,R4}
                 LSR     R3, R3, #0x10
                 STRH    R3, [R0]
                 ADD     R0, #2

                 SUB     R2, #0x10
                 BNE     loop

                 @ read crc code

                 LDMIA   R1!, {R3,R4}
                 LDMIA   R1!, {R3,R4}
                 LDMIA   R1!, {R3,R4}
                 LDMIA   R1!, {R3,R4}

                 LDRH    R3, [R1]
                 pop	{r4, r5}
	         bx	lr
