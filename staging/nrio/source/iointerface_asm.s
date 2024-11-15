	.align	4
	.arm

	.global sub_BF800098
	.global sub_BF801334
	.global sub_BF80133C
	.global sub_BF801344
	.global sub_BF80134C
	.global sub_BF801378
	.global sub_BF8013A4
	.global dword_BF804C08
	.global gotEnd

@ =============== S U B R O U T I N E =======================================

@ Attributes: bp-based frame

sub_BF800080:
				MOV			 R12, SP
				STMFD		   SP!, {R3-R12,LR,PC}
				SUB			 R11, R12, #4
				SUB			 SP, R11, #0x28
				LDMFD		   SP, {R4-R11,SP,LR}
				BX			  LR
@ End of function sub_BF800080


@ =============== S U B R O U T I N E =======================================


sub_BF800098:							@ DATA XREF: RAM:BF800048↑o
										@ RAM:BF80004C↑o ...
				STMFD		   SP!, {R4,LR}
				LDR			 R4, =byte_BF804D0C
				LDRB			R3, [R4]
				CMP			 R3, #0
				LDR			 R2, =0
				BNE			 loc_BF8000C8
				CMP			 R2, #0
				LDR			 R0, =dword_BF804C00
				MOVNE		   LR, PC
				BXNE			R2
				MOV			 R3, #1
				STRB			R3, [R4]

loc_BF8000C8:							@ CODE XREF: sub_BF800098+14↑j
				LDMFD		   SP!, {R4,LR}
				BX			  LR
@ End of function sub_BF800098

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF8000DC:							@ DATA XREF: RAM:BF804BF8↓o

				STR			 LR, [SP]!
				LDR			 R3, =0
				CMP			 R3, #0
				SUB			 SP, SP, #4
				LDR			 R0, =dword_BF804C00
				LDR			 R1, =unk_BF804D10
				MOVNE		   LR, PC
				BXNE			R3
				LDR			 R0, =dword_BF804C04
				LDR			 R3, [R0]
				CMP			 R3, #0
				LDR			 R3, =0
				BEQ			 loc_BF80011C
				CMP			 R3, #0
				MOVNE		   LR, PC
				BXNE			R3

loc_BF80011C:							@ CODE XREF: sub_BF8000DC+30↑j
				ADD			 SP, SP, #4
				LDR			 LR, [SP],#4
				BX			  LR
@ End of function sub_BF8000DC

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF80013C:
				MOV			 R0, #0xA7000000
				ADD			 R0, R0, #0x580000
				MOV			 R12, #0x4000000
				MOV			 R1, #0x4000000
				ADD			 R12, R12, #0x100
				ADD			 R0, R0, #0x6000
				MOV			 R3, #0xFFFFFFC0
				MOV			 R2, #0x66
				STRB			R3, [R1,#0x1A1]
				STRB			R2, [R12,#0xA8]
				STR			 R0, [R1,#0x1A4]

loc_BF800168:							@ CODE XREF: sub_BF80013C+34↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800168
				MOV			 R3, #0x4100000
				LDR			 R0, [R3,#0x10]
				BX			  LR
@ End of function sub_BF80013C


@ =============== S U B R O U T I N E =======================================


sub_BF800180:
				STMFD		   SP!, {R4-R6,LR}
				MOV			 R1, #0x4000000
				MOV			 R4, R0,LSR#16
				MOV			 R5, R0,LSR#8
				MOV			 R3, #0xFFFFFFC0
				MOV			 LR, #0x4000000
				STRB			R3, [R1,#0x1A1]
				MOV			 R12, #0
				ADD			 LR, LR, #0x100
				AND			 R6, R0, #0xFF
				AND			 R4, R4, #0xFF
				AND			 R5, R5, #0xFF
				MOV			 R0, R0,LSR#24
				MOV			 R2, #0xFFFFFFB7
				MOV			 R3, #0xA7000000
				STRB			R2, [LR,#0xA8]
				STRB			R0, [R1,#0x1A9]
				STRB			R4, [R1,#0x1AA]
				STRB			R5, [R1,#0x1AB]
				STRB			R6, [LR,#0xAC]
				STRB			R12, [R1,#0x1AD]
				STRB			R12, [R1,#0x1AE]
				STRB			R12, [R1,#0x1AF]
				STR			 R3, [R1,#0x1A4]

loc_BF8001E0:							@ CODE XREF: sub_BF800180+68↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF8001E0
				MOV			 R3, #0x4100000
				LDR			 R0, [R3,#0x10]
				LDMFD		   SP!, {R4-R6,LR}
				BX			  LR
@ End of function sub_BF800180


@ =============== S U B R O U T I N E =======================================


sub_BF8001FC:
				STMFD		   SP!, {R4-R7,LR}
				MOV			 LR, #0x4000000
				MOV			 R12, #0x4000000
				MOV			 R4, R0,LSR#16
				MOV			 R5, R0,LSR#8
				ADD			 LR, LR, #0x100
				MOV			 R3, #0xFFFFFFC0
				MOV			 R2, #0xFFFFFF87
				STRB			R3, [R12,#0x1A1]
				AND			 R6, R0, #0xFF
				STRB			R2, [LR,#0xA8]
				AND			 R7, R1, #0xFF
				AND			 R4, R4, #0xFF
				AND			 R5, R5, #0xFF
				MOV			 R0, R0,LSR#24
				MOV			 R1, R1,LSR#8
				ADD			 R3, R3, #0x40
				MOV			 R2, #0xA7000000
				STRB			R0, [R12,#0x1A9]
				STRB			R4, [R12,#0x1AA]
				STRB			R5, [R12,#0x1AB]
				STRB			R6, [LR,#0xAC]
				STRB			R1, [R12,#0x1AD]
				STRB			R7, [R12,#0x1AE]
				STRB			R3, [R12,#0x1AF]
				STR			 R2, [R12,#0x1A4]

loc_BF800264:							@ CODE XREF: sub_BF8001FC+70↓j
				LDR			 R3, [R12,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800264
				MOV			 R3, #0x4100000
				LDR			 R0, [R3,#0x10]
				LDMFD		   SP!, {R4-R7,LR}
				BX			  LR
@ End of function sub_BF8001FC


@ =============== S U B R O U T I N E =======================================


sub_BF800280:
				STMFD		   SP!, {R4-R7,LR}
				MOV			 R4, R1,LSR#16
				MOV			 R5, R1,LSR#8
				MOV			 LR, #0x4000000
				MOV			 R12, #0x4000000
				ADD			 LR, LR, #0x100
				AND			 R6, R1, #0xFF
				AND			 R7, R2, #0xFF
				MOV			 R3, #0xFFFFFFC0
				MOV			 R2, R2,LSR#8
				AND			 R4, R4, #0xFF
				AND			 R5, R5, #0xFF
				MOV			 R1, R1,LSR#24
				STRB			R3, [R12,#0x1A1]
				STRB			R0, [LR,#0xA8]
				ADD			 R3, R3, #0x40
				STRB			R1, [R12,#0x1A9]
				STRB			R4, [R12,#0x1AA]
				STRB			R5, [R12,#0x1AB]
				STRB			R6, [LR,#0xAC]
				STRB			R2, [R12,#0x1AD]
				MOV			 R2, #0xA7000000
				STRB			R7, [R12,#0x1AE]
				STRB			R3, [R12,#0x1AF]
				STR			 R2, [R12,#0x1A4]

loc_BF8002E4:							@ CODE XREF: sub_BF800280+6C↓j
				LDR			 R3, [R12,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF8002E4
				MOV			 R3, #0x4100000
				LDR			 R0, [R3,#0x10]
				LDMFD		   SP!, {R4-R7,LR}
				BX			  LR
@ End of function sub_BF800280


@ =============== S U B R O U T I N E =======================================


sub_BF800300:							@ CODE XREF: sub_BF800390+3C↓p
										@ sub_BF800390+68↓p ...

				STMFD		   SP!, {R4-R9,R11,LR}
				LDRB			LR, [SP,#0x34]
				LDRB			R6, [SP,#0x20]
				LDRB			R7, [SP,#0x24]
				LDRB			R8, [SP,#0x28]
				LDRB			R9, [SP,#0x2C]
				MOV			 R5, #0xA7000000
				CMP			 LR, #4
				MOV			 R12, #0xA1000000
				MOV			 R4, #0x4000000
				ADDNE		   R5, R12, #0x180000
				ADD			 R4, R4, #0x100
				ADD			 R12, R12, #0x63000000
				ADDEQ		   R5, R5, #0x180000
				MOV			 LR, #0xFFFFFFC0
				LDR			 R11, [SP,#0x20+0x10]
				STRB			LR, [R12,#0x1A1]
				STRB			R0, [R4,#0xA8]
				STRB			R1, [R12,#0x1A9]
				STRB			R2, [R12,#0x1AA]
				STRB			R3, [R12,#0x1AB]
				STRB			R6, [R4,#0xAC]
				STRB			R7, [R12,#0x1AD]
				STRB			R8, [R12,#0x1AE]
				STRB			R9, [R12,#0x1AF]
				STR			 R5, [R12,#0x1A4]
				MOV			 R2, #0x4100000

loc_BF80036C:							@ CODE XREF: sub_BF800300+84↓j
				LDR			 R3, [R12,#0x1A4]
				TST			 R3, #0x800000
				LDRNE		   R3, [R2,#0x10]
				STRNE		   R3, [R11],#4
				LDR			 R3, [R12,#0x1A4]
				CMP			 R3, #0
				BLT			 loc_BF80036C
				LDMFD		   SP!, {R4-R9,R11,LR}
				BX			  LR
@ End of function sub_BF800300


@ =============== S U B R O U T I N E =======================================


sub_BF800390:							@ CODE XREF: sub_BF8034EC+10↓p

				STMFD		   SP!, {R4-R6,LR}
				MOV			 R4, #0
				SUB			 SP, SP, #0x18
				MOV			 R5, R0
				MOV			 R6, #4
				MOV			 R3, R4
				MOV			 R0, #0xD1
				MOV			 R1, #0x61
				MOV			 R2, #0x90
				STR			 R4, [SP]
				STR			 R4, [SP,#0x4]
				STR			 R4, [SP,#0x8]
				STR			 R4, [SP,#0xC]
				STR			 R5, [SP,#0x10]
				STR			 R6, [SP,#0x14]
				BL			  sub_BF800300
				MOV			 R2, R4
				MOV			 R3, R4
				MOV			 R0, #0xD1
				MOV			 R1, #0x51
				STR			 R4, [SP]
				STR			 R4, [SP,#0x4]
				STR			 R4, [SP,#0x8]
				STR			 R4, [SP,#0xC]
				STR			 R5, [SP,#0x10]
				STR			 R6, [SP,#0x14]
				BL			  sub_BF800300
				MOV			 R1, R4
				MOV			 R0, #0xD0
				MOV			 R2, R4
				MOV			 R3, R4
				STR			 R5, [SP]
				STR			 R4, [SP,#0x4]
				STR			 R4, [SP,#0x8]
				STR			 R4, [SP,#0xC]
				STR			 R5, [SP,#0x10]
				STR			 R6, [SP,#0x14]
				BL			  sub_BF800300
				MOV			 R0, #1
				ADD			 SP, SP, #0x18
				LDMFD		   SP!, {R4-R6,LR}
				BX			  LR
@ End of function sub_BF800390


@ =============== S U B R O U T I N E =======================================


sub_BF800438:							@ CODE XREF: sub_BF800534+10↓j
										@ sub_BF800630+18↓p ...
				STMFD		   SP!, {R4-R7,LR}
				MOV			 R2, R0,LSR#16
				MOV			 LR, #0x4000000
				MOV			 R3, #0x4000000
				MOV			 R12, R0,LSR#8
				MOV			 R4, R1,LSR#16
				MOV			 R5, R1,LSR#8
				ADD			 LR, LR, #0x100
				AND			 R2, R2, #0xFF
				AND			 R6, R0, #0xFF
				MOV			 R0, R0,LSR#24
				STRB			R0, [LR,#0xA8]
				AND			 R7, R1, #0xFF
				STRB			R2, [R3,#0x1A9]
				AND			 R12, R12, #0xFF
				MOV			 R2, #0xA7000000
				AND			 R4, R4, #0xFF
				AND			 R5, R5, #0xFF
				MOV			 R1, R1,LSR#24
				STRB			R12, [R3,#0x1AA]
				STRB			R6, [R3,#0x1AB]
				STRB			R1, [LR,#0xAC]
				STRB			R4, [R3,#0x1AD]
				STRB			R5, [R3,#0x1AE]
				STRB			R7, [R3,#0x1AF]
				STR			 R2, [R3,#0x1A4]
				MOV			 R2, R3

loc_BF8004A4:							@ CODE XREF: sub_BF800438+74↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF8004A4
				MOV			 R3, #0x4100000
				LDR			 R0, [R3,#0x10]
				LDMFD		   SP!, {R4-R7,LR}
				BX			  LR
@ End of function sub_BF800438


@ =============== S U B R O U T I N E =======================================


sub_BF8004C0:							@ CODE XREF: sub_BF8005A0+C↓j
										@ sub_BF8005B0+40↓j ...
				STMFD		   SP!, {R4-R7,LR}
				MOV			 R4, R1,LSR#8
				MOV			 R5, R1,LSR#16
				MOV			 LR, #0x4000000
				MOV			 R6, #0xB7000000
				MOV			 R3, #0x4000000
				ADD			 LR, LR, #0x100
				MOV			 R7, R1,LSR#24
				AND			 R4, R4, #0xFF
				AND			 R5, R5, #0xFF
				ADD			 R6, R6, #1
				AND			 R1, R1, #0xFF
				MOV			 R12, #0xFFFFFFD1
				STRB			R12, [LR,#0xA8]
				STRB			R0, [R3,#0x1A9]
				STRB			R1, [R3,#0x1AA]
				STRB			R4, [R3,#0x1AB]
				STRB			R5, [LR,#0xAC]
				STRB			R7, [R3,#0x1AD]
				STRB			R2, [R3,#0x1AE]
				STR			 R6, [R3,#0x1A4]
				MOV			 R2, R3

loc_BF800518:							@ CODE XREF: sub_BF8004C0+60↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800518
				MOV			 R3, #0x4100000
				LDR			 R2, [R3,#0x10]
				LDMFD		   SP!, {R4-R7,LR}
				BX			  LR
@ End of function sub_BF8004C0


@ =============== S U B R O U T I N E =======================================


sub_BF800534:							@ CODE XREF: sub_BF800668+C↓p
										@ sub_BF800668+1C↓p ...
				MOV			 R3, #0xD1000000
				ADD			 R3, R3, #0x610000
				ORR			 R0, R3, R0,LSL#8
				MOV			 R1, #0
				B			   sub_BF800438
@ End of function sub_BF800534


@ =============== S U B R O U T I N E =======================================


sub_BF800548:							@ CODE XREF: sub_BF800668+20↓p
										@ sub_BF800758+D4↓j ...
				MOV			 R3, #0x4000000
				MOV			 R1, #0x8000
				MOV			 R2, #0xFFFFFFC2
				ADD			 R3, R3, #0x100
				ADD			 R1, R1, #0x80
				STRB			R2, [R3,#0xA8]
				ORR			 R1, R1, R1,LSL#16
				MOV			 R2, #0x4000000
				MOV			 R12, #0xA7000000
				MOV			 R0, #0x4100000

loc_BF800570:							@ CODE XREF: sub_BF800548+50↓j
				STR			 R12, [R2,#0x1A4]

loc_BF800574:							@ CODE XREF: sub_BF800548+34↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800574
				LDR			 R3, [R0,#0x10]
				BIC			 R3, R3, #0x7F000000
				BIC			 R3, R3, #0x7F0000
				BIC			 R3, R3, #0x7F00
				BIC			 R3, R3, #0x7F
				CMP			 R3, R1
				BNE			 loc_BF800570
				BX			  LR
@ End of function sub_BF800548


@ =============== S U B R O U T I N E =======================================


sub_BF8005A0:							@ CODE XREF: sub_BF8006E0+1C↓p
										@ sub_BF800A5C+20↓p
				MOV			 R1, R0
				MOV			 R2, #0
				MOV			 R0, #0x52
				B			   sub_BF8004C0
@ End of function sub_BF8005A0


@ =============== S U B R O U T I N E =======================================


sub_BF8005B0:							@ CODE XREF: sub_BF800668+14↓p
										@ sub_BF800758+40↓p ...

				STR			 R10, [SP,#-4]!
				LDR			 R10, =(dword_BF804C08 - 0xBF8005C4)
				LDR			 R3, =0xC4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R1, [R10,R3]
				LDR			 R2, [R1]
				MOV			 R1, #0x7F0
				ADD			 R1, R1, #0xF
				MOV			 R3, #0x80000000
				AND			 R3, R2, R3,ASR#20
				AND			 R1, R2, R1
				ORR			 R1, R1, R3,LSL#5
				ORR			 R1, R1, R0,LSL#11
				MOV			 R2, R2,LSR#27
				MOV			 R0, #0x55
				LDMFD		   SP!, {R10}
				B			   sub_BF8004C0
@ End of function sub_BF8005B0

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF8005FC:							@ CODE XREF: sub_BF800A08+38↓p

				STR			 R10, [SP,#-4]!
				LDR			 R10, =(dword_BF804C08 - 0xBF800610)
				LDR			 R3, =0xC4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R2, [R10,R3]
				LDR			 R1, [R2]
				MOV			 R0, #0x53
				MOV			 R1, R1,LSR#11
				MOV			 R2, #0
				LDMFD		   SP!, {R10}
				B			   sub_BF8004C0
@ End of function sub_BF8005FC

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF800630:							@ CODE XREF: sub_BF80191C+8↓p

				STR			 LR, [SP,#-4]!
				MOV			 R0, #0xD1000000
				ADD			 R0, R0, #0x610000
				SUB			 SP, SP, #4
				MOV			 R1, #0
				ADD			 R0, R0, #0x7000
				BL			  sub_BF800438
				MOV			 R1, #0
				MOV			 R0, #0xD0000000
				BL			  sub_BF800438
				AND			 R0, R0, #0xFF
				ADD			 SP, SP, #4
				LDR			 LR, [SP],#4
				BX			  LR
@ End of function sub_BF800630


@ =============== S U B R O U T I N E =======================================


sub_BF800668:							@ CODE XREF: sub_BF80195C+150↓p
				STMFD		   SP!, {R4,LR}
				MOV			 R4, R0
				MOV			 R0, #0
				BL			  sub_BF800534
				MOV			 R0, #1
				BL			  sub_BF8005B0
				MOV			 R0, #0x30
				BL			  sub_BF800534
				BL			  sub_BF800548
				MOV			 R3, #0x4000000
				MOV			 R2, #0xFFFFFFD0
				MOV			 R1, #0
				ADD			 R3, R3, #0x100
				MOV			 R0, #0x4000000
				STRB			R2, [R3,#0xA8]
				MOV			 R12, #0xA7000000
				STRB			R1, [R0,#0x1A9]
				ADD			 R2, R4, #0x3C
				ADD			 R1, R1, #0x4100000

loc_BF8006B4:							@ CODE XREF: sub_BF800668+6C↓j
				STR			 R12, [R0,#0x1A4]

loc_BF8006B8:							@ CODE XREF: sub_BF800668+58↓j
				LDR			 R3, [R0,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF8006B8
				LDR			 R3, [R1,#0x10]
				CMP			 R4, R2
				STR			 R3, [R4]
				ADDNE		   R4, R4, #4
				BNE			 loc_BF8006B4
				LDMFD		   SP!, {R4,LR}
				BX			  LR
@ End of function sub_BF800668


@ =============== S U B R O U T I N E =======================================


sub_BF8006E0:							@ CODE XREF: sub_BF80195C+140↓p
				STMFD		   SP!, {R4,R5,LR}
				MOV			 R4, R1
				SUB			 SP, SP, #4
				MOV			 R5, R0
				MOV			 R0, #5
				BL			  sub_BF800534
				MOV			 R0, R4,LSL#9
				BL			  sub_BF8005A0
				MOV			 R0, #0xE0
				BL			  sub_BF800534
				MOV			 R3, #0x4000000
				ADD			 R3, R3, #0x100
				MOV			 R2, #0xFFFFFFD0
				STRB			R2, [R3,#0xA8]
				MOV			 R0, #0x4000000
				MOV			 R1, #0
				MOV			 R3, #0xA1000000
				STRB			R1, [R0,#0x1A9]
				STR			 R3, [R0,#0x1A4]
				MOV			 R2, #0x4100000

loc_BF800730:							@ CODE XREF: sub_BF8006E0+68↓j
				LDR			 R3, [R0,#0x1A4]
				TST			 R3, #0x800000
				LDRNE		   R3, [R2,#0x10]
				STRNE		   R3, [R5],#4
				LDR			 R3, [R0,#0x1A4]
				CMP			 R3, #0
				BLT			 loc_BF800730
				ADD			 SP, SP, #4
				LDMFD		   SP!, {R4,R5,LR}
				BX			  LR
@ End of function sub_BF8006E0


@ =============== S U B R O U T I N E =======================================


sub_BF800758:
				STMFD		   SP!, {R4-R7,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF80076C)
				LDR			 R3, =0xC4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R1, [R10,R3]
				LDR			 R2, [R1]
				MOV			 R2, R2,LSR#18
				MOV			 R2, R2,LSL#18
				CMP			 R2, #0
				MOV			 R4, R0
				BNE			 loc_BF80078C
				LDMFD		   SP!, {R4-R7,R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF80078C:							@ CODE XREF: sub_BF800758+28↑j
				MOV			 R0, #0x80
				BL			  sub_BF800534
				MOV			 R0, #1
				BL			  sub_BF8005B0
				MOV			 R0, #0x4000000
				ADD			 R0, R0, #0x100
				MOV			 R3, #0xFFFFFFD1
				MOV			 R1, #0x4000000
				STRB			R3, [R0,#0xA8]
				MOV			 R2, #0x44
				ADD			 R3, R3, #0x2F
				STRB			R2, [R1,#0x1A9]
				STRB			R3, [R1,#0x1AE]
				MOV			 LR, R4
				MOV			 R12, R1
				MOV			 R7, R0
				ADD			 R4, R4, #0x40
				MOV			 R6, #0xA7000000
				MOV			 R5, #0x4100000

loc_BF8007D8:							@ CODE XREF: sub_BF800758+C4↓j
				LDR			 R3, [LR],#4
				MOV			 R2, R3,LSR#8
				MOV			 R1, R3,LSR#16
				MOV			 R0, R3,LSR#24
				AND			 R2, R2, #0xFF
				AND			 R1, R1, #0xFF
				AND			 R3, R3, #0xFF
				STRB			R3, [R12,#0x1AA]
				STRB			R2, [R12,#0x1AB]
				STRB			R1, [R7,#0xAC]
				STRB			R0, [R12,#0x1AD]
				STR			 R6, [R12,#0x1A4]

loc_BF800808:							@ CODE XREF: sub_BF800758+B8↓j
				LDR			 R3, [R12,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800808
				CMP			 LR, R4
				LDR			 R3, [R5,#0x10]
				BNE			 loc_BF8007D8
				MOV			 R0, #0x10
				BL			  sub_BF800534
				LDMFD		   SP!, {R4-R7,R10,LR}
				B			   sub_BF800548
@ End of function sub_BF800758

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF800838:							@ CODE XREF: sub_BF80195C+218↓p
										@ sub_BF80195C+2F0↓p
				STMFD		   SP!, {R4-R7,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF80084C)
				LDR			 R3, =0xC4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R1, [R10,R3]
				LDR			 R2, [R1]
				MOV			 R2, R2,LSR#18
				MOV			 R2, R2,LSL#18
				CMP			 R2, #0
				MOV			 R12, R0
				BEQ			 loc_BF8008F8
				MOV			 R0, #0x4000000
				ADD			 R0, R0, #0x100
				MOV			 R3, #0xFFFFFFD1
				MOV			 R1, #0x4000000
				STRB			R3, [R0,#0xA8]
				MOV			 R2, #0x44
				ADD			 R3, R3, #0x2F
				MOV			 LR, R12
				STRB			R2, [R1,#0x1A9]
				STRB			R3, [R1,#0x1AE]
				MOV			 R12, R1
				MOV			 R7, R0
				ADD			 R4, LR, #0x40
				MOV			 R6, #0xA7000000
				MOV			 R5, #0x4100000

loc_BF8008A0:							@ CODE XREF: sub_BF800838+AC↓j
				LDR			 R3, [LR],#4
				MOV			 R2, R3,LSR#8
				MOV			 R1, R3,LSR#16
				MOV			 R0, R3,LSR#24
				AND			 R2, R2, #0xFF
				AND			 R1, R1, #0xFF
				AND			 R3, R3, #0xFF
				STRB			R3, [R12,#0x1AA]
				STRB			R2, [R12,#0x1AB]
				STRB			R1, [R7,#0xAC]
				STRB			R0, [R12,#0x1AD]
				STR			 R6, [R12,#0x1A4]

loc_BF8008D0:							@ CODE XREF: sub_BF800838+A0↓j
				LDR			 R3, [R12,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF8008D0
				CMP			 LR, R4
				LDR			 R3, [R5,#0x10]
				BNE			 loc_BF8008A0
				MOV			 R0, #0x10
				BL			  sub_BF800534
				LDMFD		   SP!, {R4-R7,R10,LR}
				B			   sub_BF800548
@ ---------------------------------------------------------------------------

loc_BF8008F8:							@ CODE XREF: sub_BF800838+28↑j
				LDMFD		   SP!, {R4-R7,R10,LR}
				BX			  LR
@ End of function sub_BF800838

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF800908:							@ CODE XREF: sub_BF80195C+98↓p
										@ sub_BF80195C+1EC↓p ...
				STMFD		   SP!, {R4,R5,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF80091C)
				LDR			 R3, =0xC4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R12, [R10,R3]
				LDR			 R2, [R12]
				MOV			 R2, R2,LSR#18
				MOV			 R2, R2,LSL#18
				CMP			 R2, #0
				MOV			 R5, R0
				MOV			 R4, R1
				BEQ			 loc_BF8009E4
				CMP			 R1, #0
				BEQ			 loc_BF8009EC

loc_BF800940:							@ CODE XREF: sub_BF800908+F4↓j
				MOV			 R3, #0x4000000
				MOV			 LR, #0x4000000
				ADD			 R3, R3, #0x100
				MOV			 R2, #0xFFFFFFD3
				MOV			 R1, #0
				STRB			R2, [R3,#0xA8]
				STRB			R1, [LR,#0x1A9]
				MOV			 R2, R5
				LDRB			R1, [R2],#1
				MOV			 R3, #0xE1000000
				ADD			 R3, R3, #0x8
				STRB			R1, [LR,#0x1AF]
				STR			 R3, [LR,#0x1A4]
				LDRB			R0, [R2,#1]
				LDRB			R1, [R5,#1]
				ADD			 R2, R2, #1
				LDRB			R12, [R2,#1]
				ADD			 R2, R2, #1
				LDRB			R3, [R2,#1]
				ORR			 R1, R1, R0,LSL#8
				ORR			 R1, R1, R12,LSL#16
				ORR			 R1, R1, R3,LSL#24
				ADD			 R2, R2, #2
				MOV			 R4, #0x4100000

loc_BF8009A0:							@ CODE XREF: sub_BF800908+D8↓j
				LDR			 R3, [LR,#0x1A4]
				TST			 R3, #0x800000
				MOV			 R0, R2
				BEQ			 loc_BF8009D8
				STR			 R1, [R4,#0x10]
				LDRB			R3, [R0],#1
				LDRB			R1, [R2,#1]
				LDRB			R12, [R0,#1]
				ADD			 R0, R0, #1
				ORR			 R3, R3, R1,LSL#8
				LDRB			R2, [R0,#1]
				ORR			 R3, R3, R12,LSL#16
				ORR			 R1, R3, R2,LSL#24
				ADD			 R2, R0, #2

loc_BF8009D8:							@ CODE XREF: sub_BF800908+A4↑j
				LDR			 R3, [LR,#0x1A4]
				CMP			 R3, #0
				BLT			 loc_BF8009A0

loc_BF8009E4:							@ CODE XREF: sub_BF800908+2C↑j
				LDMFD		   SP!, {R4,R5,R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF8009EC:							@ CODE XREF: sub_BF800908+34↑j
				MOV			 R0, #0x80
				BL			  sub_BF800534
				MOV			 R0, R4
				BL			  sub_BF8005B0
				B			   loc_BF800940
@ End of function sub_BF800908

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF800A08:							@ CODE XREF: sub_BF80195C:loc_BF801B7C↓p
				STMFD		   SP!, {R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF800A1C)
				LDR			 R3, =0xC4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R1, [R10,R3]
				LDR			 R2, [R1]
				MOV			 R2, R2,LSR#18
				MOV			 R2, R2,LSL#18
				CMP			 R2, #0
				MOV			 R0, #0x60
				BNE			 loc_BF800A3C
				LDMFD		   SP!, {R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF800A3C:							@ CODE XREF: sub_BF800A08+28↑j
				BL			  sub_BF800534
				BL			  sub_BF8005FC
				MOV			 R0, #0xD0
				BL			  sub_BF800534
				LDMFD		   SP!, {R10,LR}
				B			   sub_BF800548
@ End of function sub_BF800A08

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF800A5C:							@ CODE XREF: sub_BF80195C+100↓p
										@ sub_BF80195C+31C↓p

				STMFD		   SP!, {R4-R9,R11,LR}
				MOV			 R4, R1
				SUB			 SP, SP, #0x18
				MOV			 R5, R0
				MOV			 R0, #5
				STR			 R2, [SP,#0x4]
				BL			  sub_BF800534
				MOV			 R0, R4,LSL#9
				BL			  sub_BF8005A0
				MOV			 R0, #0xE0
				BL			  sub_BF800534
				MOV			 R2, #0x4000000
				MOV			 R1, #0
				ADD			 R2, R2, #0x100
				MOV			 R0, #0x4000000
				MOV			 R3, #0xFFFFFFD0
				STRB			R3, [R2,#0xA8]
				STR			 R1, [SP,#0x8]
				STRB			R1, [R0,#0x1A9]
				LDR			 R9, [SP,#0x8]
				MOV			 R3, #0xA1000000
				AND			 R2, R5, #3
				STR			 R3, [R0,#0x1A4]
				STR			 R1, [SP,#0xC]
				STR			 R9, [SP,#0x10]
				STR			 R9, [SP,#0x14]
				ADD			 R5, R5, R2
				MOV			 R7, R9
				MOV			 R11, R9
				MOV			 R6, #0x4100000

loc_BF800AD4:							@ CODE XREF: sub_BF800A5C+84↓j
										@ sub_BF800A5C+1C4↓j
				MOV			 R1, #0x4000000
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800AD4
				LDR			 R0, [R6,#0x10]
				STR			 R0, [R5],#4

loc_BF800AEC:							@ CODE XREF: sub_BF800A5C+98↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800AEC
				LDR			 R4, [R6,#0x10]
				STR			 R4, [R5],#4
				EOR			 R12, R0, R4
				MOV			 R1, #0x4000000

loc_BF800B08:							@ CODE XREF: sub_BF800A5C+B4↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800B08
				LDR			 R0, [R6,#0x10]
				STR			 R0, [R5],#4
				EOR			 LR, R12, R0
				MOV			 R1, #0x4000000

loc_BF800B24:							@ CODE XREF: sub_BF800A5C+D0↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800B24
				LDR			 R12, [R6,#0x10]
				EOR			 R3, R7, R0
				STR			 R12, [R5],#4
				EOR			 R0, LR, R12
				EOR			 R8, R3, R12
				MOV			 R1, #0x4000000

loc_BF800B48:							@ CODE XREF: sub_BF800A5C+F4↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800B48
				LDR			 R2, [R6,#0x10]
				STR			 R2, [R5],#4
				EOR			 R0, R0, R2
				MOV			 R1, #0x4000000

loc_BF800B64:							@ CODE XREF: sub_BF800A5C+110↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800B64
				LDR			 R1, [SP,#0x10]
				LDR			 LR, [R6,#0x10]
				EOR			 R3, R1, R4
				EOR			 R3, R3, R12
				STR			 LR, [R5],#4
				EOR			 R0, R0, LR
				EOR			 R7, R3, LR
				MOV			 R1, #0x4000000

loc_BF800B90:							@ CODE XREF: sub_BF800A5C+13C↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800B90
				LDR			 R12, [R6,#0x10]
				STR			 R12, [R5],#4
				EOR			 R0, R0, R12
				EOR			 R4, R8, R12
				MOV			 R1, #0x4000000

loc_BF800BB0:							@ CODE XREF: sub_BF800A5C+15C↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800BB0
				LDR			 R1, [R6,#0x10]
				EOR			 R0, R0, R1
				EOR			 R3, R0, R0,LSR#16
				LDR			 R8, [SP,#0xC]
				EOR			 R3, R3, R3,LSR#8
				EOR			 R3, R3, R3,LSR#4
				EOR			 R3, R3, R3,LSR#2
				EOR			 R2, R8, R2
				AND			 R3, R3, #3
				EOR			 R2, R2, LR
				SUB			 R3, R3, #1
				EOR			 R2, R2, R12
				EOR			 R7, R7, R1
				EOR			 R2, R2, R1
				CMP			 R3, #1
				STR			 R7, [SP,#0x10]
				STR			 R2, [SP,#0xC]
				STR			 R1, [R5]
				EOR			 R7, R4, R1
				EOR			 R11, R11, R0
				BLS			 loc_BF800C24
				ADD			 R9, R9, #1
				CMP			 R9, #0xF
				BHI			 loc_BF800C48

loc_BF800C1C:							@ CODE XREF: sub_BF800A5C+1E8↓j
				ADD			 R5, R5, #4
				B			   loc_BF800AD4
@ ---------------------------------------------------------------------------

loc_BF800C24:							@ CODE XREF: sub_BF800A5C+1B0↑j
				LDR			 R12, [SP,#0x8]
				LDR			 R0, [SP,#0x14]
				EOR			 R12, R12, R9
				ADD			 R9, R9, #1
				EOR			 R0, R0, #1
				CMP			 R9, #0xF
				STR			 R12, [SP,#0x8]
				STR			 R0, [SP,#0x14]
				BLS			 loc_BF800C1C

loc_BF800C48:							@ CODE XREF: sub_BF800A5C+1BC↑j
				ADD			 R2, SP, #0xC
				LDMIA		   R2, {R2,R5}
				BIC			 R4, R11, #0xF000000
				EOR			 R1, R2, R2,LSR#16
				EOR			 R12, R7, R7,LSR#16
				BIC			 R4, R4, #0xF0000
				BIC			 R0, R11, #0x55000000
				EOR			 LR, R5, R5,LSR#16
				EOR			 R1, R1, R1,LSL#8
				EOR			 R12, R12, R12,LSR#8
				BIC			 R4, R4, #0xF00
				BIC			 R0, R0, #0x550000
				BIC			 R3, R11, #0x33000000
				EOR			 R1, R1, R1,LSR#4
				EOR			 R12, R12, R12,LSL#4
				EOR			 LR, LR, LR,LSR#8
				BIC			 R4, R4, #0xF
				BIC			 R0, R0, #0x5500
				BIC			 R3, R3, #0x330000
				BIC			 R6, R11, #0xFF0000
				EOR			 R1, R1, R1,LSR#2
				EOR			 R12, R12, R12,LSL#2
				EOR			 LR, LR, LR,LSL#4
				EOR			 R4, R4, R4,LSL#16
				BIC			 R0, R0, #0x55
				BIC			 R6, R6, #0xFF
				BIC			 R3, R3, #0x3300
				EOR			 R1, R1, R1,LSL#1
				EOR			 R12, R12, R12,LSL#1
				EOR			 LR, LR, LR,LSR#2
				EOR			 R4, R4, R4,LSR#8
				EOR			 R0, R0, R0,LSL#16
				EOR			 R6, R6, R6,LSR#16
				AND			 R12, R12, #0x80
				BIC			 R3, R3, #0x33
				MOV			 R2, R11,LSR#24
				AND			 R1, R1, #0x200
				EOR			 LR, LR, LR,LSL#1
				EOR			 R4, R4, R4,LSL#2
				EOR			 R0, R0, R0,LSR#8
				ORR			 R1, R1, R12
				EOR			 R2, R2, R11,LSR#16
				EOR			 R3, R3, R3,LSL#16
				MOV			 R5, R6,LSR#12
				AND			 LR, LR, #0x20
				EOR			 R4, R4, R4,LSL#1
				EOR			 R0, R0, R0,LSR#4
				LDR			 R8, [SP,#0x8]
				EOR			 R5, R5, R6,LSR#8
				EOR			 R2, R2, R2,LSR#4
				EOR			 R3, R3, R3,LSR#8
				ORR			 R1, R1, LR
				AND			 R4, R4, #0x800000
				EOR			 R0, R0, R0,LSL#2
				AND			 R0, R0, #0x80000
				EOR			 R2, R2, R2,LSL#2
				EOR			 R5, R5, R5,LSR#2
				EOR			 R3, R3, R3,LSL#4
				ORR			 R1, R1, R4
				ORR			 R1, R1, R0
				EOR			 R2, R2, R2,LSL#1
				AND			 R0, R8, #2
				EOR			 R5, R5, R5,LSL#1
				MOV			 R3, R3,LSR#2
				ORR			 R1, R1, R0,LSL#12
				AND			 R12, R8, #8
				AND			 R2, R2, #8
				AND			 R5, R5, #2
				EOR			 R3, R3, R3,LSL#1
				ORR			 R1, R1, R12,LSL#14
				AND			 R3, R3, #0x200000
				LDR			 R12, [SP,#0x14]
				ORR			 R2, R2, R5
				ORR			 R2, R2, R3
				AND			 R3, R8, #1
				CMP			 R12, #0
				ORR			 R2, R2, R3,LSL#11
				AND			 R3, R8, #4
				ORR			 R2, R2, R3,LSL#13
				MOVNE		   R3, #0xAA0000
				ADDNE		   R3, R3, #0xAA00
				ORR			 R0, R2, R1
				ADDNE		   R3, R3, #0xAA
				EORNE		   R3, R0, R3
				ORRNE		   R0, R0, R3,LSR#1
				ORREQ		   R0, R0, R0,LSR#1
				MVN			 R3, R0
				LDR			 R0, [SP,#0x4]
				MOV			 R2, R3,LSR#16
				MOV			 R1, R3,LSR#8
				STRB			R3, [R0]
				STRB			R2, [R0,#2]
				STRB			R1, [R0,#1]
				ADD			 SP, SP, #0x18
				LDMFD		   SP!, {R4-R9,R11,LR}
				BX			  LR
@ End of function sub_BF800A5C


@ =============== S U B R O U T I N E =======================================


sub_BF800DC8:							@ CODE XREF: sub_BF80195C+254↓p
										@ sub_BF80195C+284↓p

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF800DDC)
				LDR			 R3, =0xC4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 LR, [R10,R3]
				LDR			 R12, [LR]
				MOV			 R12, R12,LSR#18
				MOV			 R12, R12,LSL#18
				SUB			 SP, SP, #0x2C
				CMP			 R12, #0
				MOV			 R5, R0
				MOV			 R4, R1
				STR			 R2, [SP,#0x4]
				BEQ			 loc_BF80130C
				CMP			 R1, #0
				BEQ			 loc_BF801318

loc_BF800E08:							@ CODE XREF: sub_BF800DC8+560↓j
				MOV			 R2, #0x4000000
				MOV			 R0, #0x4000000
				MOV			 LR, #0
				ADD			 R2, R2, #0x100
				MOV			 R3, #0xFFFFFFD3
				STRB			R3, [R2,#0xA8]
				STRB			LR, [R0,#0x1A9]
				MOV			 R2, R5
				LDRB			R1, [R2],#1
				MOV			 R3, #0xE1000000
				ADD			 R3, R3, #0x8
				STRB			R1, [R0,#0x1AF]
				STR			 R3, [R0,#0x1A4]
				LDRB			R3, [R2,#1]
				LDRB			R1, [R5,#1]
				ADD			 R2, R2, #1
				LDRB			R0, [R2,#1]
				ADD			 R2, R2, #1
				LDRB			R12, [R2,#1]
				ORR			 R1, R1, R3,LSL#8
				STR			 LR, [SP,#0xC]
				ADD			 LR, R2, #2
				MOV			 R2, #0
				ORR			 R1, R1, R0,LSL#16
				AND			 R3, R5, #3
				STR			 R2, [SP,#0x8]
				STR			 R2, [SP,#0x10]
				STR			 R2, [SP,#0x14]
				STR			 R2, [SP,#0x18]
				STR			 R2, [SP,#0x1C]
				STR			 R2, [SP,#0x20]
				ORR			 R1, R1, R12,LSL#24
				ADD			 R0, R5, R3
				MOV			 R9, #0x4100000

loc_BF800E90:							@ CODE XREF: sub_BF800DC8+D4↓j
										@ sub_BF800DC8+388↓j
				MOV			 R6, #0x4000000
				LDR			 R3, [R6,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800E90
				STR			 R1, [R9,#0x10]
				MOV			 R3, LR
				LDRB			R7, [R3],#1
				LDR			 R8, [R0],#4
				ADD			 R2, R3, #1
				LDRB			R4, [R3,#1]
				LDRB			R5, [LR,#1]
				ADD			 R1, R2, #2
				LDRB			LR, [R2,#1]
				MOV			 R12, R0
				MOV			 R2, R6

loc_BF800ECC:							@ CODE XREF: sub_BF800DC8+10C↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800ECC
				ORR			 R3, R7, R5,LSL#8
				ORR			 R3, R3, R4,LSL#16
				ORR			 R3, R3, LR,LSL#24
				STR			 R3, [R9,#0x10]
				MOV			 R2, R1
				LDRB			R3, [R2],#1
				LDRB			R1, [R1,#1]
				LDRB			R0, [R2,#1]
				ADD			 R2, R2, #1
				ORR			 R3, R3, R1,LSL#8
				LDR			 R11, [R12],#4
				LDRB			R1, [R2,#1]
				ORR			 R3, R3, R0,LSL#16
				ORR			 R0, R3, R1,LSL#24
				MOV			 LR, R12
				ADD			 R1, R2, #2
				EOR			 R4, R8, R11
				MOV			 R2, #0x4000000

loc_BF800F20:							@ CODE XREF: sub_BF800DC8+160↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800F20
				STR			 R0, [R9,#0x10]
				MOV			 R3, R1
				LDRB			R2, [R3],#1
				LDRB			R1, [R1,#1]
				LDRB			R0, [R3,#1]
				ADD			 R3, R3, #1
				ORR			 R2, R2, R1,LSL#8
				LDR			 R12, [LR],#4
				LDRB			R1, [R3,#1]
				ORR			 R2, R2, R0,LSL#16
				ORR			 R2, R2, R1,LSL#24
				ADD			 R0, R3, #2
				EOR			 R5, R4, R12
				MOV			 R1, #0x4000000

loc_BF800F64:							@ CODE XREF: sub_BF800DC8+1A4↓j
				LDR			 R3, [R1,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800F64
				STR			 R2, [R9,#0x10]
				MOV			 R3, R0
				LDRB			R2, [R3],#1
				LDRB			R1, [R0,#1]
				LDRB			R0, [R3,#1]
				ORR			 R2, R2, R1,LSL#8
				ORR			 R2, R2, R0,LSL#16
				ADD			 R3, R3, #1
				LDR			 R0, [SP,#0x14]
				LDR			 R4, [LR],#4
				LDRB			R1, [R3,#1]
				EOR			 R12, R12, R0
				EOR			 R5, R5, R4
				ORR			 R0, R2, R1,LSL#24
				EOR			 R8, R12, R4
				ADD			 R1, R3, #2
				MOV			 R2, #0x4000000

loc_BF800FB4:							@ CODE XREF: sub_BF800DC8+1F4↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800FB4
				STR			 R0, [R9,#0x10]
				MOV			 R3, R1
				LDRB			R2, [R3],#1
				LDRB			R1, [R1,#1]
				LDRB			R0, [R3,#1]
				ADD			 R3, R3, #1
				ORR			 R2, R2, R1,LSL#8
				LDR			 R7, [LR],#4
				LDRB			R1, [R3,#1]
				ORR			 R2, R2, R0,LSL#16
				ORR			 R0, R2, R1,LSL#24
				EOR			 R6, R5, R7
				ADD			 R1, R3, #2
				MOV			 R2, #0x4000000

loc_BF800FF8:							@ CODE XREF: sub_BF800DC8+238↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF800FF8
				STR			 R0, [R9,#0x10]
				MOV			 R3, R1
				LDR			 R5, [SP,#0x18]
				LDRB			R2, [R3],#1
				LDRB			R0, [R1,#1]
				EOR			 R1, R11, R5
				LDR			 R5, [LR],#4
				LDRB			R12, [R3,#1]
				EOR			 R1, R1, R4
				ADD			 R3, R3, #1
				ORR			 R2, R2, R0,LSL#8
				EOR			 R1, R1, R5
				LDRB			R0, [R3,#1]
				ORR			 R2, R2, R12,LSL#16
				STR			 R1, [SP,#0x24]
				ORR			 R4, R2, R0,LSL#24
				EOR			 R12, R6, R5
				ADD			 R0, R3, #2
				MOV			 R2, #0x4000000

loc_BF801050:							@ CODE XREF: sub_BF800DC8+290↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF801050
				STR			 R4, [R9,#0x10]
				MOV			 R3, R0
				LDRB			R2, [R3],#1
				LDRB			R1, [R0,#1]
				LDRB			R0, [R3,#1]
				ADD			 R3, R3, #1
				LDR			 R4, [LR],#4
				ORR			 R2, R2, R1,LSL#8
				LDRB			R1, [R3,#1]
				ORR			 R2, R2, R0,LSL#16
				ORR			 R1, R2, R1,LSL#24
				ADD			 R6, R3, #2
				MOV			 R11, LR
				EOR			 R12, R12, R4
				EOR			 R8, R8, R4
				MOV			 R2, #0x4000000

loc_BF80109C:							@ CODE XREF: sub_BF800DC8+2DC↓j
				LDR			 R3, [R2,#0x1A4]
				TST			 R3, #0x800000
				BEQ			 loc_BF80109C
				STR			 R1, [R9,#0x10]
				LDR			 R0, [R11]
				LDR			 LR, [SP,#0x10]
				EOR			 R12, R12, R0
				EOR			 R2, R7, LR
				EOR			 R3, R12, R12,LSR#16
				EOR			 R2, R2, R5
				EOR			 R3, R3, R3,LSR#8
				EOR			 R2, R2, R4
				EOR			 R3, R3, R3,LSR#4
				MOV			 R1, R6
				EOR			 R2, R2, R0
				EOR			 R3, R3, R3,LSR#2
				LDRB			R7, [R1],#1
				LDR			 R5, [SP,#0x1C]
				STR			 R2, [SP,#0x10]
				AND			 R3, R3, #3
				LDR			 R2, [SP,#0x24]
				SUB			 R3, R3, #1
				ADD			 LR, R1, #1
				EOR			 R2, R2, R0
				CMP			 R3, #1
				EOR			 R8, R8, R0
				EOR			 R5, R5, R12
				LDRB			R4, [R1,#1]
				LDRB			R3, [R6,#1]
				LDRB			R1, [LR,#1]
				STR			 R2, [SP,#0x18]
				STR			 R8, [SP,#0x14]
				STR			 R5, [SP,#0x1C]
				ADD			 R2, LR, #1
				BLS			 loc_BF801154
				LDR			 R5, [SP,#0x8]
				ADD			 R5, R5, #1
				CMP			 R5, #0xF
				STR			 R5, [SP,#0x8]
				BHI			 loc_BF801184

loc_BF80113C:							@ CODE XREF: sub_BF800DC8+3B8↓j
				ORR			 R3, R7, R3,LSL#8
				ORR			 R3, R3, R4,LSL#16
				ORR			 R1, R3, R1,LSL#24
				ADD			 LR, R2, #1
				ADD			 R0, R11, #4
				B			   loc_BF800E90
@ ---------------------------------------------------------------------------

loc_BF801154:							@ CODE XREF: sub_BF800DC8+35C↑j
				ADD			 R5, SP, #0x8
				LDMIA		   R5, {R5,R12}
				LDR			 LR, [SP,#0x8]
				LDR			 R0, [SP,#0x20]
				ADD			 R5, R5, #1
				EOR			 R12, R12, LR
				EOR			 R0, R0, #1
				CMP			 R5, #0xF
				STR			 R12, [SP,#0xC]
				STR			 R0, [SP,#0x20]
				STR			 R5, [SP,#0x8]
				BLS			 loc_BF80113C

loc_BF801184:							@ CODE XREF: sub_BF800DC8+370↑j
				LDR			 R12, [SP,#0x1C]
				LDR			 R2, [SP,#0x1C]
				LDR			 LR, [SP,#0x10]
				ADD			 R0, SP, #0x14
				LDMIA		   R0, {R0,R5}
				BIC			 R4, R12, #0xF000000
				BIC			 R3, R12, #0x33000000
				EOR			 R1, LR, LR,LSR#16
				EOR			 R12, R0, R0,LSR#16
				BIC			 R4, R4, #0xF0000
				BIC			 R0, R2, #0x55000000
				EOR			 LR, R5, R5,LSR#16
				EOR			 R1, R1, R1,LSL#8
				EOR			 R12, R12, R12,LSR#8
				BIC			 R4, R4, #0xF00
				BIC			 R0, R0, #0x550000
				BIC			 R6, R2, #0xFF0000
				BIC			 R3, R3, #0x330000
				EOR			 R1, R1, R1,LSR#4
				EOR			 R12, R12, R12,LSL#4
				EOR			 LR, LR, LR,LSR#8
				BIC			 R4, R4, #0xF
				BIC			 R0, R0, #0x5500
				LDR			 R5, [SP,#0x1C]
				BIC			 R6, R6, #0xFF
				BIC			 R3, R3, #0x3300
				EOR			 R1, R1, R1,LSR#2
				EOR			 R12, R12, R12,LSL#2
				EOR			 LR, LR, LR,LSL#4
				EOR			 R4, R4, R4,LSL#16
				BIC			 R0, R0, #0x55
				EOR			 R6, R6, R6,LSR#16
				BIC			 R3, R3, #0x33
				MOV			 R2, R2,LSR#24
				EOR			 R1, R1, R1,LSL#1
				EOR			 R12, R12, R12,LSL#1
				EOR			 LR, LR, LR,LSR#2
				EOR			 R4, R4, R4,LSR#8
				EOR			 R0, R0, R0,LSL#16
				EOR			 R2, R2, R5,LSR#16
				AND			 R12, R12, #0x80
				EOR			 R3, R3, R3,LSL#16
				MOV			 R5, R6,LSR#12
				AND			 R1, R1, #0x200
				EOR			 LR, LR, LR,LSL#1
				EOR			 R4, R4, R4,LSL#2
				EOR			 R0, R0, R0,LSR#8
				ORR			 R1, R1, R12
				AND			 LR, LR, #0x20
				EOR			 R5, R5, R6,LSR#8
				EOR			 R2, R2, R2,LSR#4
				EOR			 R3, R3, R3,LSR#8
				EOR			 R4, R4, R4,LSL#1
				EOR			 R0, R0, R0,LSR#4
				LDR			 R12, [SP,#0xC]
				ORR			 R1, R1, LR
				EOR			 R2, R2, R2,LSL#2
				EOR			 R5, R5, R5,LSR#2
				EOR			 R3, R3, R3,LSL#4
				AND			 R4, R4, #0x800000
				EOR			 R0, R0, R0,LSL#2
				AND			 R0, R0, #0x80000
				EOR			 R2, R2, R2,LSL#1
				EOR			 R5, R5, R5,LSL#1
				MOV			 R3, R3,LSR#2
				ORR			 R1, R1, R4
				AND			 R5, R5, #2
				ORR			 R1, R1, R0
				AND			 R2, R2, #8
				AND			 R0, R12, #2
				EOR			 R3, R3, R3,LSL#1
				LDR			 LR, [SP,#0xC]
				ORR			 R2, R2, R5
				AND			 R3, R3, #0x200000
				ORR			 R1, R1, R0,LSL#12
				LDR			 R0, [SP,#0x20]
				ORR			 R2, R2, R3
				AND			 R3, R12, #1
				CMP			 R0, #0
				ORR			 R2, R2, R3,LSL#11
				AND			 R3, LR, #4
				ORR			 R2, R2, R3,LSL#13
				AND			 R12, R12, #8
				MOVNE		   R3, #0xAA0000
				ADDNE		   R3, R3, #0xAA00
				ORR			 R1, R1, R12,LSL#14
				ORR			 R0, R2, R1
				ADDNE		   R3, R3, #0xAA
				EORNE		   R3, R0, R3
				ORRNE		   R0, R0, R3,LSR#1
				ORREQ		   R0, R0, R0,LSR#1
				MVN			 R3, R0
				LDR			 R5, [SP,#0x4]
				MOV			 R2, R3,LSR#16
				MOV			 R1, R3,LSR#8
				STRB			R3, [R5]
				STRB			R2, [R5,#2]
				STRB			R1, [R5,#1]

loc_BF80130C:							@ CODE XREF: sub_BF800DC8+34↑j
				ADD			 SP, SP, #0x2C
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF801318:							@ CODE XREF: sub_BF800DC8+3C↑j
				MOV			 R0, #0x80
				BL			  sub_BF800534
				MOV			 R0, R4
				BL			  sub_BF8005B0
				B			   loc_BF800E08
@ End of function sub_BF800DC8

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801334:							@ DATA XREF: RAM:BF80006C↑o
				MOV			 R0, #1
				BX			  LR
@ End of function sub_BF801334


@ =============== S U B R O U T I N E =======================================


sub_BF80133C:							@ DATA XREF: RAM:BF800078↑o
				MOV			 R0, #1
				BX			  LR
@ End of function sub_BF80133C


@ =============== S U B R O U T I N E =======================================


sub_BF801344:							@ DATA XREF: RAM:BF80007C↑o
				MOV			 R0, #1
				BX			  LR
@ End of function sub_BF801344


@ =============== S U B R O U T I N E =======================================


sub_BF80134C:							@ DATA XREF: RAM:BF800074↑o

				STR			 LR, [SP,#-4]!
				MOV			 R3, R1,LSL#9
				SUB			 SP, SP, #4
				MOV			 R1, R2
				MOV			 R0, R0,LSL#9
				MOV			 R2, R3
				BL			  sub_BF804978
				MOV			 R0, #1
				ADD			 SP, SP, #4
				LDR			 LR, [SP],#4
				BX			  LR
@ End of function sub_BF80134C


@ =============== S U B R O U T I N E =======================================


sub_BF801378:							@ DATA XREF: RAM:BF800070↑o

				STR			 LR, [SP,#-4]!
				MOV			 R3, R1,LSL#9
				SUB			 SP, SP, #4
				MOV			 R1, R2
				MOV			 R0, R0,LSL#9
				MOV			 R2, R3
				BL			  sub_BF8030EC
				MOV			 R0, #1
				ADD			 SP, SP, #4
				LDR			 LR, [SP],#4
				BX			  LR
@ End of function sub_BF801378


@ =============== S U B R O U T I N E =======================================


sub_BF8013A4:							@ DATA XREF: RAM:BF800068↑o

				STR			 LR, [SP,#-4]!
				MOV			 R0, #0
				SUB			 SP, SP, #4
				BL			  sub_BF8034EC
				MOV			 R0, #1
				ADD			 SP, SP, #4
				LDR			 LR, [SP],#4
				BX			  LR
@ End of function sub_BF8013A4


@ =============== S U B R O U T I N E =======================================


sub_BF8013C4:							@ CODE XREF: sub_BF8031A8+2C↓p
										@ sub_BF8031A8+48↓p ...
				TST			 R0, #3
				BXEQ			LR

loc_BF8013CC:							@ CODE XREF: sub_BF8013C4:loc_BF8013CC↓j
				B			   loc_BF8013CC
@ End of function sub_BF8013C4


@ =============== S U B R O U T I N E =======================================


sub_BF8013D0:							@ CODE XREF: sub_BF8013F8+5C↓p
										@ sub_BF8013F8+68↓p ...
				CMP			 R0, #0
				BXEQ			LR
				MOV			 R2, #0

loc_BF8013DC:							@ CODE XREF: sub_BF8013D0+1C↓j
				TST			 R0, #1
				ADD			 R3, R2, #1
				ANDNE		   R2, R3, #0xFF
				MOVS			R0, R0,LSR#1
				BNE			 loc_BF8013DC
				MOV			 R0, R2
				BX			  LR
@ End of function sub_BF8013D0


@ =============== S U B R O U T I N E =======================================


sub_BF8013F8:							@ CODE XREF: sub_BF80195C+120↓p
										@ sub_BF80195C+33C↓p
				STMFD		   SP!, {R4-R7,LR}
				LDRB			R6, [R0]
				LDRB			R5, [R0,#1]
				LDRB			R4, [R1]
				LDRB			R12, [R1,#1]
				LDRB			R3, [R1,#2]
				LDRB			LR, [R0,#2]
				EOR			 R4, R6, R4
				EOR			 R12, R5, R12
				EOR			 LR, LR, R3
				AND			 R1, R4, #0xFF
				AND			 R5, R12, #0xFF
				AND			 R6, LR, #0xFF
				ORR			 R3, R1, R5
				ORRS			R0, R6, R3
				SUB			 SP, SP, #4
				MOV			 R7, R2
				BEQ			 loc_BF801484
				EOR			 R3, R1, R1,LSR#1
				AND			 R3, R3, #0x55
				CMP			 R3, #0x55
				BEQ			 loc_BF801490

loc_BF801450:							@ CODE XREF: sub_BF8013F8+A4↓j
										@ sub_BF8013F8+B4↓j
				MOV			 R0, R1
				BL			  sub_BF8013D0
				MOV			 R4, R0
				MOV			 R0, R5
				BL			  sub_BF8013D0
				MOV			 R5, R0
				MOV			 R0, R6
				BL			  sub_BF8013D0
				ADD			 R4, R4, R5
				ADD			 R4, R4, R0
				CMP			 R4, #1
				MOVNE		   R0, #3
				MOVEQ		   R0, #2

loc_BF801484:							@ CODE XREF: sub_BF8013F8+44↑j
										@ sub_BF8013F8+144↓j
				ADD			 SP, SP, #4
				LDMFD		   SP!, {R4-R7,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF801490:							@ CODE XREF: sub_BF8013F8+54↑j
				EOR			 R3, R5, R5,LSR#1
				AND			 R3, R3, #0x55
				CMP			 R3, #0x55
				BNE			 loc_BF801450
				EOR			 R3, R6, R6,LSR#1
				AND			 R3, R3, #0x55
				CMP			 R3, #0x55
				BNE			 loc_BF801450
				TST			 R12, #0x20
				AND			 R5, R5, #0x80
				BEQ			 loc_BF8014C8
				CMP			 R5, #0
				MOVNE		   R5, #0xC0
				MOVEQ		   R5, #0x40

loc_BF8014C8:							@ CODE XREF: sub_BF8013F8+C0↑j
				TST			 R12, #8
				ORRNE		   R5, R5, #0x20
				TST			 R12, #2
				ORRNE		   R5, R5, #0x10
				TST			 R1, #0x80
				ORRNE		   R5, R5, #8
				TST			 R4, #0x20
				ORRNE		   R5, R5, #4
				TST			 R4, #8
				ORRNE		   R5, R5, #2
				TST			 R4, #2
				ORRNE		   R5, R5, #1
				TST			 R6, #0x80
				MOVEQ		   R1, #0
				MOVNE		   R1, #4
				TST			 LR, #0x20
				BEQ			 loc_BF801518
				CMP			 R1, #0
				MOVNE		   R1, #6
				MOVEQ		   R1, #2

loc_BF801518:							@ CODE XREF: sub_BF8013F8+110↑j
				TST			 LR, #8
				ORRNE		   R1, R1, #1
				TST			 R6, #2
				ORRNE		   R5, R5, #0x100
				LDRB			R3, [R5,R7]
				MOV			 R2, #1
				EOR			 R3, R3, R2,LSL R1
				MOV			 R0, R2
				STRB			R3, [R5,R7]
				B			   loc_BF801484
@ End of function sub_BF8013F8


@ =============== S U B R O U T I N E =======================================


sub_BF801540:							@ CODE XREF: sub_BF802228:loc_BF802418↓p
										@ sub_BF802228+560↓p ...

				STR			 R10, [SP,#-4]!
				LDR			 R10, =(dword_BF804C08 - 0xBF801554)
				LDR			 R3, =0xB8
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R0, [R10,R3]
				MOV			 R2, #0
				MOV			 R1, #0xFFFFFFFF

loc_BF80155C:							@ CODE XREF: sub_BF801540+2C↓j
				LDR			 R3, [R0]
				STRB			R1, [R3,R2]
				ADD			 R2, R2, #1
				CMP			 R2, #0x3E
				BNE			 loc_BF80155C
				LDMFD		   SP!, {R10}
				BX			  LR
@ End of function sub_BF801540

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801580:							@ CODE XREF: sub_BF8031A8+2A8↓p

				STR			 R10, [SP,#-4]!
				LDR			 R10, =(dword_BF804C08 - 0xBF801594)
				LDR			 R3, =0x54
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R0, [R10,R3]
				MOV			 R2, #0
				MOV			 R1, #0xFFFFFFFF

loc_BF80159C:							@ CODE XREF: sub_BF801580+2C↓j
				LDR			 R3, [R0]
				STRB			R1, [R3,R2]
				ADD			 R2, R2, #1
				CMP			 R2, #0x40
				BNE			 loc_BF80159C
				LDMFD		   SP!, {R10}
				BX			  LR
@ End of function sub_BF801580

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF8015C0:							@ CODE XREF: sub_BF801CB8+2C↓p
										@ sub_BF801DC4:loc_BF801DF4↓p

				STR			 R10, [SP,#-4]!
				LDR			 R10, =(dword_BF804C08 - 0xBF8015D4)
				LDR			 R3, =0xC4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R2, [R10,R3]
				STR			 R0, [R2]
				LDMFD		   SP!, {R10}
				BX			  LR
@ End of function sub_BF8015C0

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF8015E8:							@ CODE XREF: sub_BF801E68+34↓p
										@ sub_BF801EC8+60↓p ...
				STMFD		   SP!, {R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF8015FC)
				LDR			 R3, =0x28
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x68
				LDR			 R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 LR, [R12,R0,LSL#2]
				LDRB			R0, [R2]
				MLA			 R12, LR, R0, R1
				LDR			 R3, =0x58
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xC4
				LDR			 R1, [R2]
				LDR			 R2, [R10,R3]
				ADD			 R1, R1, R12,LSL#11
				STR			 R1, [R2]
				LDMFD		   SP!, {R10,LR}
				BX			  LR
@ End of function sub_BF8015E8

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF80164C:							@ CODE XREF: sub_BF801E68+38↓p
										@ sub_BF801EC8+F4↓p ...

				STR			 R10, [SP,#-4]!
				LDR			 R10, =(dword_BF804C08 - 0xBF801660)
				LDR			 R2, =0x74
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R3, [R10,R2]
				LDRB			R0, [R3,#0xB]
				MOV			 R2, #0x55
				MOV			 R1, #0xFFFFFFAA
				STRB			R0, [R3,#7]
				STRB			R2, [R3,#2]
				STRB			R1, [R3,#3]
				STRB			R2, [R3]
				STRB			R1, [R3,#1]
				STRB			R0, [R3,#0xF]
				LDMFD		   SP!, {R10}
				BX			  LR
@ End of function sub_BF80164C

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801694:							@ CODE XREF: sub_BF801CB8+38↓p
										@ sub_BF801DC4+3C↓p ...

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF8016A8)
				LDR			 R0, =0x74
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R2, [R10,R0]
				LDRB			R3, [R2]
				CMP			 R3, #0xFF
				SUB			 SP, SP, #0xC
				BEQ			 loc_BF8017D0
				CMP			 R3, #0x55
				BEQ			 loc_BF8016E0

loc_BF8016C0:							@ CODE XREF: sub_BF801694+144↓j
				LDR			 R3, [R10,R0]
				LDRB			R2, [R3,#2]
				CMP			 R2, #0x55
				BEQ			 loc_BF8016E0

loc_BF8016D0:							@ CODE XREF: sub_BF801694+64↓j
										@ sub_BF801694+90↓j ...
				MOV			 R0, #0xFE

loc_BF8016D4:							@ CODE XREF: sub_BF801694+138↓j
										@ sub_BF801694+174↓j
				ADD			 SP, SP, #0xC
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF8016E0:							@ CODE XREF: sub_BF801694+28↑j
										@ sub_BF801694+38↑j
				LDR			 R2, [R10,R0]
				LDRB			R3, [R2,#1]
				CMP			 R3, #0xAA
				BEQ			 loc_BF8016FC
				LDRB			R3, [R2,#3]
				CMP			 R3, #0xAA
				BNE			 loc_BF8016D0

loc_BF8016FC:							@ CODE XREF: sub_BF801694+58↑j
				LDRB			R1, [R2,#0xF]
				LDRB			R3, [R2,#0xB]
				CMP			 R3, R1
				BEQ			 loc_BF801718
				LDRB			R3, [R2,#7]
				CMP			 R3, R1
				STREQB		  R3, [R2,#0xB]

loc_BF801718:							@ CODE XREF: sub_BF801694+74↑j
				LDR			 R2, [R10,R0]
				LDRB			R3, [R2,#0xB]
				CMP			 R3, #0xFF
				BEQ			 loc_BF8016D0
				MOV			 R8, R2
				MOV			 R7, R2
				MOV			 R1, #0
				MOV			 R9, R2

loc_BF801738:							@ CODE XREF: sub_BF801694+130↓j
				ORR			 R0, R1, #0xA
				ADD			 R3, R1, #0x10
				ORR			 LR, R1, #8
				ORR			 R2, R1, #0xC
				ORR			 R5, R1, #9
				STR			 R0, [SP,#0x4]
				ORR			 R12, R1, #0xD
				ORR			 R6, R1, #0xE
				ORR			 R0, R1, #4
				ORR			 R4, R1, #5
				ORR			 R11, R1, #6
				LDRB			R2, [R8,R2]
				AND			 R1, R3, #0xFF
				LDRB			R3, [R8,LR]
				CMP			 R3, R2
				BEQ			 loc_BF801784
				LDRB			R3, [R8,R0]
				CMP			 R3, R2
				STREQB		  R3, [R8,LR]

loc_BF801784:							@ CODE XREF: sub_BF801694+E0↑j
				LDRB			R2, [R7,R12]
				LDRB			R3, [R7,R5]
				CMP			 R3, R2
				BEQ			 loc_BF8017A0
				LDRB			R3, [R7,R4]
				CMP			 R3, R2
				STREQB		  R3, [R7,R5]

loc_BF8017A0:							@ CODE XREF: sub_BF801694+FC↑j
				LDR			 R0, [SP,#0x4]
				LDRB			R2, [R9,R6]
				LDRB			R3, [R9,R0]
				CMP			 R3, R2
				BEQ			 loc_BF8017C0
				LDRB			R3, [R9,R11]
				CMP			 R3, R2
				STREQB		  R3, [R9,R0]

loc_BF8017C0:							@ CODE XREF: sub_BF801694+11C↑j
				CMP			 R1, #0x40
				BNE			 loc_BF801738
				MOV			 R0, #0
				B			   loc_BF8016D4
@ ---------------------------------------------------------------------------

loc_BF8017D0:							@ CODE XREF: sub_BF801694+20↑j
				LDRB			R3, [R2,#2]
				CMP			 R3, #0xFF
				BNE			 loc_BF8016C0
				LDRB			R1, [R2,#0xF]
				LDRB			R3, [R2,#0xB]
				CMP			 R3, R1
				BEQ			 loc_BF8017F8
				LDRB			R3, [R2,#7]
				CMP			 R3, R1
				STREQB		  R3, [R2,#0xB]

loc_BF8017F8:							@ CODE XREF: sub_BF801694+154↑j
				LDR			 R3, [R10,R0]
				LDRB			R0, [R3,#0xB]
				CMP			 R0, #0xFF
				BNE			 loc_BF8016D0
				B			   loc_BF8016D4
@ End of function sub_BF801694

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801814:							@ CODE XREF: sub_BF802F58:loc_BF803090↓p
										@ sub_BF803874:loc_BF803B40↓p ...

				STR			 R10, [SP,#-4]!
				LDR			 R10, =(dword_BF804C08 - 0xBF801828)
				LDR			 R3, =0x40
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x6C
				LDRH			R1, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x54
				LDRB			R12, [R2,R1]
				LDR			 R2, [R10,R3]
				LDR			 R2, [R2]
				MOV			 R0, R12

loc_BF801848:							@ CODE XREF: sub_BF801814+54↓j
				LDRB			R3, [R0,R2]
				CMP			 R3, #0xFF
				ADD			 R1, R0, #1
				BEQ			 loc_BF801870
				AND			 R0, R1, #0xFF
				CMP			 R0, #0x3F
				MOVHI		   R0, #0
				CMP			 R0, R12
				BNE			 loc_BF801848
				MOV			 R0, #0

loc_BF801870:							@ CODE XREF: sub_BF801814+40↑j
				LDMFD		   SP!, {R10}
				BX			  LR
@ End of function sub_BF801814

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801888:							@ CODE XREF: sub_BF803D28:loc_BF803E18↓p
										@ sub_BF803D28+104↓p ...
				STMFD		   SP!, {R4,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF8018A0)
				LDR			 R3, =0xE4
				LDR			 R2, =0x54
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R12, [R10,R3]
				LDR			 R3, [R10,R2]
				LDRB			R2, [R12]
				LDR			 R1, [R3]
				MOV			 R4, #0
				STRB			R4, [R2,R1]
				LDR			 R3, =0xEC
				LDR			 R2, =0xB8
				LDR			 LR, [R10,R3]
				LDR			 R3, [R10,R2]
				LDRB			R2, [LR]
				LDR			 R0, [R3]
				LDRB			R3, [R12]
				STRB			R3, [R2,R0]
				LDR			 R3, =0x4C
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x44
				STRB			R4, [R2]
				LDR			 R2, [R10,R3]
				MOV			 R1, #0xFFFFFFFF
				STRB			R1, [LR]
				STRB			R1, [R12]
				STRB			R1, [R2]
				LDMFD		   SP!, {R4,R10,LR}
				BX			  LR
@ End of function sub_BF801888

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF80191C:							@ CODE XREF: sub_BF801E68+44↓p
										@ sub_BF801EC8+100↓p ...

				STR			 LR, [SP,#-4]!
				SUB			 SP, SP, #4
				BL			  sub_BF800630
				TST			 R0, #0x40
				BEQ			 loc_BF801954
				TST			 R0, #0x80
				MOV			 R3, #3
				BNE			 loc_BF80194C

loc_BF80193C:							@ CODE XREF: sub_BF80191C+34↓j
										@ sub_BF80191C+3C↓j
				MOV			 R0, R3
				ADD			 SP, SP, #4
				LDR			 LR, [SP],#4
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF80194C:							@ CODE XREF: sub_BF80191C+1C↑j
				ANDS			R3, R0, #1
				BEQ			 loc_BF80193C

loc_BF801954:							@ CODE XREF: sub_BF80191C+10↑j
				MOV			 R3, #2
				B			   loc_BF80193C
@ End of function sub_BF80191C


@ =============== S U B R O U T I N E =======================================


sub_BF80195C:							@ CODE XREF: sub_BF801CB8+34↓p
										@ sub_BF801CB8+7C↓p ...

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF801970)
				SUB			 R0, R0, #1
				ADD			 R10, PC, R10 @ dword_BF804C08
				SUB			 SP, SP, #0xC
				CMP			 R0, #0xA @ switch 11 cases
				ADDLS		   PC, PC, R0,LSL#2 @ switch jump
@ ---------------------------------------------------------------------------

loc_BF801978:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF80197C:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801AA4 @ jumptable BF801974 cases 0,9
@ ---------------------------------------------------------------------------

loc_BF801980:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801A3C @ jumptable BF801974 case 1
@ ---------------------------------------------------------------------------

loc_BF801984:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801C54 @ jumptable BF801974 case 2
@ ---------------------------------------------------------------------------

loc_BF801988:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801BE8 @ jumptable BF801974 case 3
@ ---------------------------------------------------------------------------

loc_BF80198C:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801BB8 @ jumptable BF801974 case 4
@ ---------------------------------------------------------------------------

loc_BF801990:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801B84 @ jumptable BF801974 case 5
@ ---------------------------------------------------------------------------

loc_BF801994:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801B7C @ jumptable BF801974 case 6
@ ---------------------------------------------------------------------------

loc_BF801998:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801AB4 @ jumptable BF801974 case 7
@ ---------------------------------------------------------------------------

loc_BF80199C:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF8019A8 @ jumptable BF801974 case 8
@ ---------------------------------------------------------------------------

loc_BF8019A0:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801AA4 @ jumptable BF801974 cases 0,9
@ ---------------------------------------------------------------------------

loc_BF8019A4:							@ CODE XREF: sub_BF80195C+18↑j
				B			   loc_BF801A84 @ jumptable BF801974 case 10
@ ---------------------------------------------------------------------------

loc_BF8019A8:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF80199C↑j
				LDR			 R0, =0xD4 @ jumptable BF801974 case 8
				LDR			 R11, =0xAC
				LDR			 R3, [R10,R0]
				LDR			 R1, [R10,R11]
				LDRB			R2, [R3]
				ADD			 R3, R1, #0x200
				MOV			 R2, R2,LSL#9
				ADD			 R3, R2, R3
				ADD			 R2, R2, R1
				CMP			 R2, R3
				BCS			 loc_BF8019E4
				MOV			 R1, #0xFFFFFFFF

loc_BF8019D8:							@ CODE XREF: sub_BF80195C+84↓j
				STR			 R1, [R2],#4
				CMP			 R3, R2
				BHI			 loc_BF8019D8

loc_BF8019E4:							@ CODE XREF: sub_BF80195C+74↑j
				LDR			 R4, [R10,R0]
				LDR			 R0, [R10,R11]
				LDRB			R1, [R4]
				ADD			 R0, R0, R1,LSL#9
				BL			  sub_BF800908
				LDR			 R3, =0x74
				LDRB			R1, [R4]
				LDR			 R2, [R10,R3]
				MOV			 R3, #0xFFFFFFFF
				ADD			 R2, R2, R1,LSL#4
				STRB			R3, [R2,#6]
				STRB			R3, [R2,#8]
				STRB			R3, [R2,#0xC]
				STRB			R3, [R2,#4]
				STRB			R3, [R2,#9]
				STRB			R3, [R2,#0xD]
				STRB			R3, [R2,#5]
				STRB			R3, [R2,#0xA]
				STRB			R3, [R2,#0xE]

loc_BF801A30:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF801978↑j ...
				ADD			 SP, SP, #0xC @ jumptable BF801974 default case
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF801A3C:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF801980↑j
				LDR			 R3, =0xA4 @ jumptable BF801974 case 1
				LDR			 R2, =0xD4
				LDR			 R6, [R10,R3]
				LDR			 R4, [R10,R2]
				ADD			 R5, SP, #0x4
				LDR			 R0, [R6]
				LDRB			R1, [R4]
				MOV			 R2, R5
				BL			  sub_BF800A5C
				LDR			 R0, =0x74
				LDR			 R0, [R10,R0]
				LDRB			R3, [R4]
				ADD			 R0, R0, #8
				ADD			 R0, R0, R3,LSL#4
				MOV			 R1, R5
				LDR			 R2, [R6]
				BL			  sub_BF8013F8
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF801A84:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF8019A4↑j
				LDR			 R3, =0xA4 @ jumptable BF801974 case 10
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD4
				LDR			 R0, [R2]
				LDR			 R2, [R10,R3]
				LDRB			R1, [R2]
				BL			  sub_BF8006E0
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF801AA4:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF80197C↑j ...
				LDR			 R3, =0x74 @ jumptable BF801974 cases 0,9
				LDR			 R0, [R10,R3]
				BL			  sub_BF800668
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF801AB4:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF801998↑j
				LDR			 R11, =0xAC @ jumptable BF801974 case 7
				LDR			 R3, [R10,R11]
				ADD			 R1, R3, #0x200
				CMP			 R3, R1
				BCS			 loc_BF801AD8
				MOV			 R2, #0xFFFFFFFF

loc_BF801ACC:							@ CODE XREF: sub_BF80195C+178↓j
				STR			 R2, [R3],#4
				CMP			 R3, R1
				BCC			 loc_BF801ACC

loc_BF801AD8:							@ CODE XREF: sub_BF80195C+168↑j
				LDR			 R3, =0x74
				LDR			 R9, [R10,R3]
				MOV			 R7, #0
				MOV			 R8, #0xFFFFFFFF

loc_BF801AE8:							@ CODE XREF: sub_BF80195C+1DC↓j
				ORR			 R3, R7, #8
				ORR			 R2, R7, #0xC
				ORR			 R1, R7, #4
				ORR			 R0, R7, #9
				ORR			 R12, R7, #0xD
				ORR			 LR, R7, #5
				ORR			 R4, R7, #0xA
				ORR			 R5, R7, #0xE
				ORR			 R6, R7, #6
				ADD			 R7, R7, #0x10
				CMP			 R7, #0x40
				STRB			R8, [R9,R3]
				STRB			R8, [R9,R2]
				STRB			R8, [R9,R1]
				STRB			R8, [R9,R0]
				STRB			R8, [R9,R12]
				STRB			R8, [R9,LR]
				STRB			R8, [R9,R4]
				STRB			R8, [R9,R5]
				STRB			R8, [R9,R6]
				BNE			 loc_BF801AE8
				LDR			 R4, [R10,R11]
				MOV			 R1, #0
				MOV			 R0, R4
				BL			  sub_BF800908
				MOV			 R0, R4
				MOV			 R1, #1
				BL			  sub_BF800908
				MOV			 R0, R4
				MOV			 R1, #2
				BL			  sub_BF800908
				MOV			 R0, R4
				MOV			 R1, #3
				BL			  sub_BF800908
				MOV			 R0, R9
				BL			  sub_BF800838
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF801B7C:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF801994↑j
				BL			  sub_BF800A08 @ jumptable BF801974 case 6
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF801B84:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF801990↑j
				LDR			 R3, =0xD4 @ jumptable BF801974 case 5
				LDR			 R2, =0x74
				LDR			 R1, [R10,R3]
				LDR			 R3, =0xAC
				LDR			 R2, [R10,R2]
				LDRB			R12, [R1]
				LDR			 R0, [R10,R3]
				ADD			 R2, R2, #8
				MOV			 R1, R12
				ADD			 R0, R0, R12,LSL#9
				ADD			 R2, R2, R12,LSL#4
				BL			  sub_BF800DC8
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF801BB8:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF80198C↑j
				LDR			 R3, =0xD4 @ jumptable BF801974 case 4
				LDR			 R2, =0x74
				LDR			 R0, [R10,R3]
				LDR			 R3, =0x18
				LDR			 R2, [R10,R2]
				LDRB			R1, [R0]
				LDR			 R12, [R10,R3]
				ADD			 R2, R2, #8
				LDR			 R0, [R12]
				ADD			 R2, R2, R1,LSL#4
				BL			  sub_BF800DC8
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF801BE8:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF801988↑j
				LDR			 R3, =0x74 @ jumptable BF801974 case 3
				LDR			 LR, [R10,R3]
				MOV			 R12, #0

loc_BF801BF4:							@ CODE XREF: sub_BF80195C+2E8↓j
				ORR			 R3, R12, #8
				LDRB			R0, [LR,R3]
				ORR			 R2, R12, #0xC
				ORR			 R1, R12, #4
				STRB			R0, [LR,R2]
				STRB			R0, [LR,R1]
				ORR			 R3, R12, #9
				LDRB			R0, [LR,R3]
				ORR			 R2, R12, #0xD
				ORR			 R1, R12, #5
				STRB			R0, [LR,R2]
				STRB			R0, [LR,R1]
				ORR			 R3, R12, #0xA
				LDRB			R2, [LR,R3]
				ORR			 R1, R12, #0xE
				ORR			 R3, R12, #6
				ADD			 R12, R12, #0x10
				CMP			 R12, #0x40
				STRB			R2, [LR,R1]
				STRB			R2, [LR,R3]
				BNE			 loc_BF801BF4
				MOV			 R0, LR
				BL			  sub_BF800838
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ ---------------------------------------------------------------------------

loc_BF801C54:							@ CODE XREF: sub_BF80195C+18↑j
										@ sub_BF80195C:loc_BF801984↑j
				LDR			 R3, =0xD4 @ jumptable BF801974 case 2
				LDR			 R2, =0xAC
				LDR			 R4, [R10,R3]
				LDR			 R5, [R10,R2]
				LDRB			R0, [R4]
				ADD			 R6, SP, #0x4
				MOV			 R1, R0
				MOV			 R2, R6
				ADD			 R0, R5, R0,LSL#9
				BL			  sub_BF800A5C
				LDR			 R0, =0x74
				LDR			 R0, [R10,R0]
				LDRB			R3, [R4]
				ADD			 R0, R0, #8
				ADD			 R2, R5, R3,LSL#9
				ADD			 R0, R0, R3,LSL#4
				MOV			 R1, R6
				BL			  sub_BF8013F8
				B			   loc_BF801A30 @ jumptable BF801974 default case
@ End of function sub_BF80195C

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801CB8:							@ CODE XREF: sub_BF802228+22C↓p
										@ sub_BF802228+578↓p ...
				STMFD		   SP!, {R4-R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF801CCC)
				LDR			 R3, =0x40
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R4, [R10,R3]
				LDR			 R3, =0xF8
				LDR			 R2, [R10,R3]
				LDRH			R0, [R4]
				LDR			 R1, [R2]
				MOV			 R0, R0,LSR#4
				ADD			 R0, R1, R0,LSL#11
				BL			  sub_BF8015C0
				MOV			 R0, #1
				BL			  sub_BF80195C
				BL			  sub_BF801694
				SUBS			R9, R0, #0
				BNE			 loc_BF801D10
				LDR			 R3, =0x74
				LDR			 R2, [R10,R3]
				LDRB			R1, [R2,#0xB]
				CMP			 R1, #0xAB
				BEQ			 loc_BF801D18

loc_BF801D10:							@ CODE XREF: sub_BF801CB8+40↑j
										@ sub_BF801CB8+E4↓j
				LDMFD		   SP!, {R4-R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF801D18:							@ CODE XREF: sub_BF801CB8+54↑j
				LDRH			R3, [R4]
				LDR			 R2, =0xD4
				AND			 R3, R3, #0xF
				LDR			 R1, [R10,R2]
				MOV			 R3, R3,ASR#2
				STRB			R3, [R1]
				ADD			 R0, R0, #3
				BL			  sub_BF80195C
				LDR			 R3, =0x28
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x58
				LDR			 R8, [R2]
				LDR			 R7, [R10,R3]
				LDR			 R2, =0x68
				LDR			 R3, =0xAC
				LDRH			R1, [R4]
				LDR			 R6, [R10,R2]
				LDR			 R2, [R10,R3]
				AND			 R1, R1, #0xF
				ADD			 R5, R2, R1,LSL#7
				MOV			 R4, R9

loc_BF801D6C:							@ CODE XREF: sub_BF801CB8+E0↓j
				LDRH			R3, [R4,R5]
				LDR			 R2, [R7]
				MOV			 R0, R3,LSL#8
				LDRB			R1, [R6]
				ORR			 R0, R0, R3,LSR#8
				RSB			 R0, R2, R0,LSL#16
				MOV			 R1, R1,LSL#11
				BL			  sub_BF804A7C
				STR			 R0, [R8,R4,LSL#1]
				ADD			 R4, R4, #2
				CMP			 R4, #0x80
				BNE			 loc_BF801D6C
				B			   loc_BF801D10
@ End of function sub_BF801CB8

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801DC4:							@ CODE XREF: sub_BF8031A8+16C↓p
				STMFD		   SP!, {R4-R7,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF801DD8)
				LDR			 R6, =0x58
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R3, [R10,R6]
				LDR			 R1, [R3]
				LDR			 R2, =0x68
				LDR			 R3, =0x74
				LDR			 R5, [R10,R2]
				LDR			 R7, [R10,R3]
				SUB			 R4, R1, #0x100000
				B			   loc_BF801E20
@ ---------------------------------------------------------------------------

loc_BF801DF4:							@ CODE XREF: sub_BF801DC4+6C↓j
				BL			  sub_BF8015C0
				MOV			 R0, #0xA
				BL			  sub_BF80195C
				BL			  sub_BF801694
				CMP			 R0, #0
				BNE			 loc_BF801E18
				LDRB			R3, [R7,#0xB]
				CMP			 R3, #0xAB
				BEQ			 loc_BF801E40

loc_BF801E18:							@ CODE XREF: sub_BF801DC4+44↑j
				LDRB			R3, [R5]
				ADD			 R4, R4, R3,LSL#11

loc_BF801E20:							@ CODE XREF: sub_BF801DC4+2C↑j
				LDR			 R3, [R10,R6]
				LDR			 R2, [R3]
				CMP			 R4, R2
				MOV			 R0, R4
				BNE			 loc_BF801DF4
				MOV			 R0, #0

loc_BF801E38:							@ CODE XREF: sub_BF801DC4+8C↓j
				LDMFD		   SP!, {R4-R7,R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF801E40:							@ CODE XREF: sub_BF801DC4+50↑j
				LDR			 R3, =0xF8
				LDR			 R2, [R10,R3]
				MOV			 R0, #1
				STR			 R4, [R2]
				B			   loc_BF801E38
@ End of function sub_BF801DC4

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801E68:							@ CODE XREF: sub_BF803D28+C4↓p
										@ sub_BF803FC0+388↓p
				STMFD		   SP!, {R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF801E7C)
				LDR			 R3, =0xEC
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE4
				LDRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x74
				MOV			 R1, R0
				LDRB			R0, [R2]
				LDR			 R2, [R10,R3]
				STRB			R12, [R2,#0xB]
				BL			  sub_BF8015E8
				BL			  sub_BF80164C
				MOV			 R0, #8
				BL			  sub_BF80195C
				BL			  sub_BF80191C
				LDMFD		   SP!, {R10,LR}
				BX			  LR
@ End of function sub_BF801E68

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF801EC8:							@ CODE XREF: sub_BF803C30+60↓p
										@ sub_BF803D28+60↓p ...

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF801EDC)
				CMP			 R0, #0xFF
				ADD			 R10, PC, R10 @ dword_BF804C08
				MOV			 R8, R1
				SUB			 SP, SP, #0xC
				MOV			 R7, R0
				MOV			 R1, R2
				BEQ			 loc_BF801FE4
				CMP			 R2, R3
				BCS			 loc_BF801FE4
				LDR			 R2, =0xD4
				STR			 R2, [SP,#0x4]
				RSB			 R2, R1, R3
				LDR			 R3, [SP,#0x4]
				LDR			 R11, [R10,R3]
				LDR			 R3, =0x74
				ADD			 R2, R1, R2
				LDR			 R9, [R10,R3]
				STR			 R11, [SP]
				MOV			 R5, R1
				AND			 R6, R2, #0xFF

loc_BF801F20:							@ CODE XREF: sub_BF801EC8+118↓j
				MOV			 R1, R5
				MOV			 R0, R7
				BL			  sub_BF8015E8
				MOV			 R0, #1
				BL			  sub_BF80195C
				BL			  sub_BF801694
				CMP			 R0, #0xFF
				BEQ			 loc_BF801FD4
				LDR			 R3, [SP,#0x4]
				LDR			 R2, [R10,R3]
				MOV			 R3, #0
				STRB			R3, [R2]
				MOV			 R4, R11

loc_BF801F54:							@ CODE XREF: sub_BF801EC8+A8↓j
				MOV			 R0, #3
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R3, #3
				STRB			R3, [R4]
				BLS			 loc_BF801F54
				MOV			 R0, R8
				MOV			 R1, R5
				BL			  sub_BF8015E8
				MOV			 R3, #0
				STRB			R3, [R4]
				LDR			 R4, [SP]

loc_BF801F8C:							@ CODE XREF: sub_BF801EC8+E0↓j
				MOV			 R0, #6
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R3, #3
				STRB			R3, [R4]
				BLS			 loc_BF801F8C
				LDR			 R3, =0xEC
				LDR			 R2, [R10,R3]
				LDRB			R1, [R2]
				STRB			R1, [R9,#0xB]
				BL			  sub_BF80164C
				MOV			 R0, #4
				BL			  sub_BF80195C
				BL			  sub_BF80191C
				CMP			 R0, #0
				BNE			 loc_BF801FE8

loc_BF801FD4:							@ CODE XREF: sub_BF801EC8+74↑j
				ADD			 R3, R5, #1
				AND			 R5, R3, #0xFF
				CMP			 R5, R6
				BNE			 loc_BF801F20

loc_BF801FE4:							@ CODE XREF: sub_BF801EC8+20↑j
										@ sub_BF801EC8+28↑j
				MOV			 R0, #0

loc_BF801FE8:							@ CODE XREF: sub_BF801EC8+108↑j
				ADD			 SP, SP, #0xC
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ End of function sub_BF801EC8

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF802004:							@ CODE XREF: sub_BF803C30+40↓p
										@ sub_BF803FC0+134↓p
				STMFD		   SP!, {R4-R7,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF802018)
				LDR			 R3, =0x40
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R0, [R10,R3]
				LDR			 R2, =0x6C
				LDRH			R3, [R0]
				LDR			 R1, [R10,R2]
				LDR			 R2, =0x54
				LDRB			R4, [R1,R3]
				LDR			 R5, [R10,R2]
				MOV			 R7, R1
				MOV			 R6, R0

loc_BF802038:							@ CODE XREF: sub_BF802004+68↓j
				LDR			 R3, [R5]
				LDRB			R2, [R4,R3]
				CMP			 R2, #0xFF
				MOV			 R0, R4
				MOV			 R1, #0
				BEQ			 loc_BF80207C

loc_BF802050:							@ CODE XREF: sub_BF802004+98↓j
				ADD			 R2, R4, #1
				LDRH			R3, [R6]
				AND			 R4, R2, #0xFF
				CMP			 R4, #0x3F
				LDRB			R2, [R7,R3]
				MOVHI		   R4, #0
				CMP			 R2, R4
				BNE			 loc_BF802038
				MOV			 R0, #0xFF

loc_BF802074:							@ CODE XREF: sub_BF802004+B0↓j
				LDMFD		   SP!, {R4-R7,R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF80207C:							@ CODE XREF: sub_BF802004+48↑j
				BL			  sub_BF8015E8
				MOV			 R0, #7
				BL			  sub_BF80195C
				BL			  sub_BF80191C
				CMP			 R0, #0
				LDRNE		   R2, [R5]
				MOVNE		   R3, #0xFFFFFFFE
				STRNEB		  R3, [R4,R2]
				BNE			 loc_BF802050
				LDR			 R3, [R5]
				STRB			R0, [R4,R3]
				LDRH			R2, [R6]
				MOV			 R0, R4
				STRB			R4, [R7,R2]
				B			   loc_BF802074
@ End of function sub_BF802004

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF8020C8:							@ CODE XREF: sub_BF802114+B0↓p
										@ sub_BF803BB8+20↓p
				STMFD		   SP!, {R4,R5,LR}
				MOV			 R5, R0
				SUB			 SP, SP, #4
				MOV			 R4, #2

loc_BF8020D8:							@ CODE XREF: sub_BF8020C8+3C↓j
				MOV			 R1, #0
				MOV			 R0, R5
				BL			  sub_BF8015E8
				MOV			 R0, #7
				BL			  sub_BF80195C
				BL			  sub_BF80191C
				SUB			 R3, R4, #1
				CMP			 R0, #0
				AND			 R4, R3, #0xFF
				BEQ			 loc_BF802108
				CMP			 R4, #0xFF
				BNE			 loc_BF8020D8

loc_BF802108:							@ CODE XREF: sub_BF8020C8+34↑j
				ADD			 SP, SP, #4
				LDMFD		   SP!, {R4,R5,LR}
				BX			  LR
@ End of function sub_BF8020C8


@ =============== S U B R O U T I N E =======================================


sub_BF802114:							@ CODE XREF: sub_BF802228+244↓p
										@ sub_BF802228+58C↓p
				STMFD		   SP!, {R4-R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF802128)
				LDR			 R3, =0x74
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R7, [R10,R3]
				LDR			 R2, =0xE4
				LDR			 R3, =0xBC
				LDR			 R8, [R10,R2]
				LDR			 R6, [R10,R3]
				MOV			 R5, R0
				MOV			 R4, #2

loc_BF802140:							@ CODE XREF: sub_BF802114+5C↓j
				MOV			 R1, #0
				MOV			 R0, R5
				BL			  sub_BF8015E8
				MOV			 R0, #0xA
				BL			  sub_BF80195C
				BL			  sub_BF801694
				CMP			 R0, #0xFE
				MOV			 R0, R5
				BNE			 loc_BF802180

loc_BF802164:							@ CODE XREF: sub_BF802114+88↓j
				SUB			 R3, R4, #1
				AND			 R4, R3, #0xFF
				CMP			 R4, #0xFF
				BNE			 loc_BF802140

loc_BF802174:							@ CODE XREF: sub_BF802114+9C↓j
										@ sub_BF802114+B8↓j
				MOV			 R0, #0xFE

loc_BF802178:							@ CODE XREF: sub_BF802114+C0↓j
										@ sub_BF802114+E0↓j ...
				LDMFD		   SP!, {R4-R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF802180:							@ CODE XREF: sub_BF802114+4C↑j
				LDRB			R1, [R6]
				LDRB			R9, [R7,#0xB]
				BL			  sub_BF8015E8
				MOV			 R0, #0xA
				BL			  sub_BF80195C
				BL			  sub_BF801694
				CMP			 R0, #0xFE
				BEQ			 loc_BF802164
				LDRB			R3, [R7,#0xB]
				CMP			 R9, R3
				BEQ			 loc_BF8021D8
				CMP			 R3, #0xFF
				BNE			 loc_BF802174
				LDRB			R3, [R8]
				CMP			 R3, #0xFF
				BEQ			 loc_BF8021F8
				MOV			 R0, R5
				BL			  sub_BF8020C8
				CMP			 R0, #0
				BNE			 loc_BF802174

loc_BF8021D0:							@ CODE XREF: sub_BF802114+C8↓j
				MOV			 R0, #0xFF
				B			   loc_BF802178
@ ---------------------------------------------------------------------------

loc_BF8021D8:							@ CODE XREF: sub_BF802114+94↑j
				CMP			 R9, #0xFF
				BEQ			 loc_BF8021D0
				LDR			 R3, =0xB8
				LDR			 R2, [R10,R3]
				LDR			 R1, [R2]
				MOV			 R0, #0
				STRB			R5, [R9,R1]
				B			   loc_BF802178
@ ---------------------------------------------------------------------------

loc_BF8021F8:							@ CODE XREF: sub_BF802114+A8↑j
				LDR			 R3, =0xEC
				LDR			 R2, [R10,R3]
				MOV			 R0, #0xFA
				STRB			R9, [R2]
				STRB			R5, [R8]
				B			   loc_BF802178
@ End of function sub_BF802114

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF802228:							@ CODE XREF: sub_BF802F58+B0↓p
										@ sub_BF8031A8+2AC↓p ...

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF80223C)
				LDR			 R3, =0x60
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R9, [R10,R3]
				LDRB			R2, [R9]
				CMP			 R2, #0
				SUB			 SP, SP, #0x5C
				BNE			 loc_BF80267C
				LDR			 R3, =0x40
				LDR			 R2, [R10,R3]
				LDRH			R11, [R2]
				CMP			 R11, #0
				BEQ			 loc_BF802540
				LDR			 R12, =0x34
				LDR			 R3, [R10,R12]
				LDRH			R2, [R3]
				CMP			 R2, R11
				STR			 R12, [SP,#0xC]
				BEQ			 loc_BF802BC0
				LDR			 LR, =0xC0
				LDR			 R3, [R10,LR]
				LDRH			R2, [R3]
				CMP			 R2, R11
				STR			 LR, [SP,#0x8]
				BEQ			 loc_BF8028E8
				LDR			 R12, =0x78
				STR			 R12, [SP,#0x14]
				LDR			 R3, [R10,R12]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BNE			 loc_BF802A54
				LDR			 R2, =0xEC
				LDR			 R3, =0x4C
				LDR			 LR, =0xF0
				LDR			 R0, =0xE4
				LDR			 R1, =0x44
				STR			 R2, [SP,#0x18]
				STR			 R3, [SP,#0x1C]
				STR			 LR, [SP,#0x10]
				STR			 R0, [SP,#0x4]
				STR			 R1, [SP]
				LDR			 R12, [SP,#0x18]
				LDR			 R3, [R10,LR]
				LDR			 LR, [SP,#0x1C]
				LDR			 R2, [R10,R0]
				LDR			 R0, [R10,R12]
				LDR			 R12, [R10,LR]
				LDRB			LR, [R3]
				LDR			 R3, =0x90
				LDR			 R1, [R10,R1]
				LDRB			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD0
				LDRB			R5, [R1]
				STRB			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x10
				LDRB			R6, [R0]
				STRB			R5, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x50
				LDRB			R1, [R12]
				STRB			R6, [R2]
				LDR			 R2, [R10,R3]
				CMP			 LR, #0
				STRB			R1, [R2]
				BNE			 loc_BF802AE0

loc_BF802338:							@ CODE XREF: sub_BF802228+C0C↓j
				LDR			 R3, =0x54
				LDR			 R2, =0xB8
				LDR			 R6, [R10,R3]
				LDR			 R3, =0xC8
				STR			 R2, [SP,#0x20]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x98
				LDRB			R7, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x30
				LDRB			R8, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x8C
				LDRB			R2, [R2]
				STR			 R2, [SP,#0x30]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x8]
				LDRB			R2, [R2]
				LDR			 R3, [R10,R3]
				LDR			 R12, [SP,#0x10]
				LDR			 LR, [SP,#0x20]
				LDR			 R12, [R10,R12]
				LDR			 R1, [SP,#0x4]
				STR			 R2, [SP,#0x2C]
				STR			 R3, [SP,#0x28]
				LDR			 R2, [SP]
				LDR			 R3, [SP,#0x18]
				LDR			 R0, [R10,LR]
				LDR			 R4, [R10,R3]
				LDR			 LR, [R10,R2]
				STR			 R12, [SP,#0x24]
				LDR			 R3, =0x94
				LDR			 R12, [R10,R1]
				LDR			 R2, =0x28
				LDR			 R1, [SP,#0x1C]
				LDR			 R5, [R10,R1]
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0x48
				STR			 R1, [R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0xA8
				STR			 R3, [R0]
				LDR			 R3, [R10,R2]
				STR			 R3, [R6]
				STRB			R7, [R12]
				STRB			R8, [LR]
				LDR			 R2, [SP,#0x30]
				STRB			R2, [R4]
				LDR			 R3, [SP,#0x2C]
				STRB			R3, [R5]
				LDR			 R12, [SP,#0x28]
				LDR			 LR, [SP,#0x24]
				MOV			 R3, #1
				STRH			R11, [R12]
				STRB			R3, [LR]

loc_BF802418:							@ CODE XREF: sub_BF802228+994↓j
				BL			  sub_BF801540
				LDR			 R1, [SP,#0x4]
				LDR			 R2, [SP]
				ADD			 R3, SP, #0x18
				LDMIA		   R3, {R3,LR}
				LDR			 R0, [R10,R1]
				LDR			 R12, [R10,R2]
				LDR			 R1, [R10,LR]
				LDR			 R2, [R10,R3]
				MOV			 R4, #0
				MOV			 R3, #0xFFFFFFFF
				STRB			R3, [R2]
				STRB			R3, [R0]
				STRB			R3, [R12]
				STRB			R4, [R1]
				BL			  sub_BF801CB8
				LDR			 R3, =0x54
				LDR			 R6, [R10,R3]
				MOV			 R5, R4

loc_BF802464:							@ CODE XREF: sub_BF802228+254↓j
				AND			 R0, R5, #0xFF
				LDR			 R4, [R6]
				BL			  sub_BF802114
				STRB			R0, [R5,R4]
				ADD			 R5, R5, #1
				CMP			 R5, #0x40
				BNE			 loc_BF802464
				LDR			 R0, [SP,#0x4]
				LDR			 R3, [R10,R0]
				LDRB			R2, [R3]
				CMP			 R2, #0xFF
				BEQ			 loc_BF802524
				LDR			 R1, [SP,#0x18]
				LDR			 R3, [SP,#0x20]
				LDR			 R2, [R10,R1]
				LDR			 R1, [R10,R3]
				LDR			 R5, =0x68
				LDRB			R12, [R2]
				LDR			 R3, [R1]
				LDR			 R0, [R10,R5]
				LDRB			R1, [R12,R3]
				LDR			 R12, [SP]
				LDRB			LR, [R0]
				LDR			 R2, [R10,R12]
				CMP			 LR, #0
				STRB			R1, [R2]
				BEQ			 loc_BF802524
				MOV			 R4, #0

loc_BF8024D4:							@ CODE XREF: sub_BF802228+2F8↓j
				LDR			 LR, [SP,#0x4]
				LDR			 R3, [R10,LR]
				MOV			 R1, R4
				LDRB			R0, [R3]
				BL			  sub_BF8015E8
				MOV			 R0, #0xA
				BL			  sub_BF80195C
				BL			  sub_BF801694
				CMP			 R0, #0
				LDREQ		   R0, [SP,#0x1C]
				ADD			 R3, R4, #1
				ADD			 R1, R4, #1
				LDREQ		   R3, [R10,R0]
				ANDEQ		   R4, R1, #0xFF
				ANDNE		   R4, R3, #0xFF
				STREQB		  R4, [R3]
				LDR			 R3, [R10,R5]
				LDRB			R2, [R3]
				CMP			 R2, R4
				BHI			 loc_BF8024D4

loc_BF802524:							@ CODE XREF: sub_BF802228+268↑j
										@ sub_BF802228+2A4↑j
				LDR			 R1, [SP,#0x14]
				LDR			 R2, [R10,R1]
				MOV			 R3, #1
				STRB			R3, [R2]

loc_BF802534:							@ CODE XREF: sub_BF802228+328↓j
										@ sub_BF802228+450↓j ...
				ADD			 SP, SP, #0x5C
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF802540:							@ CODE XREF: sub_BF802228+34↑j
				LDR			 R3, =0x78
				LDR			 R0, [R10,R3]
				LDRB			R2, [R0]
				CMP			 R2, #0
				BEQ			 loc_BF802534
				LDR			 R3, =0xF0
				LDR			 R2, [R10,R3]
				LDRB			R1, [R2]
				CMP			 R1, #0
				STRB			R11, [R0]
				BEQ			 loc_BF802870
				LDR			 R12, =0x44
				LDR			 LR, =0xEC
				LDR			 R3, =0xE4
				LDR			 R0, =0x4C
				STR			 R3, [SP,#0x4]
				STR			 R0, [SP,#0x1C]
				STR			 R12, [SP]
				STR			 LR, [SP,#0x18]
				LDR			 R3, [R10,R3]
				LDR			 R2, [R10,R12]
				LDRB			R12, [R3]
				LDR			 R3, =0xC8
				LDR			 R1, [R10,LR]
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x98
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x30
				LDRB			R4, [R1]
				STRB			LR, [R2]
				LDR			 R0, [R10,R0]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x8C
				STRB			R4, [R2]
				LDRB			R5, [R0]
				LDR			 R2, [R10,R3]
				STRB			R5, [R2]

loc_BF8025DC:							@ CODE XREF: sub_BF802228+6BC↓j
				LDR			 R3, =0x90
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD0
				LDRB			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x10
				LDRB			R6, [R2]
				LDR			 R2, [SP,#0x18]
				LDR			 R7, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x1C]
				LDR			 LR, [R10,R3]
				LDR			 R3, =0x50
				LDR			 R0, [SP,#0x4]
				LDRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R1, [SP]
				LDR			 R8, [R10,R0]
				LDR			 R3, =0x88
				LDRB			R0, [R2]
				LDR			 R2, =0x28
				LDR			 R5, [R10,R1]
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0xCC
				STR			 R1, [R3]
				LDR			 R3, =0xB8
				LDR			 R1, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD8
				STR			 R1, [R2]
				LDR			 R2, =0x54
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				STRB			R4, [R8]
				STR			 R1, [R3]
				STRB			R6, [R5]
				STRB			R12, [R7]
				STRB			R0, [LR]
				B			   loc_BF802534
@ ---------------------------------------------------------------------------

loc_BF80267C:							@ CODE XREF: sub_BF802228+20↑j
				LDR			 R3, =0x54
				LDR			 R3, [R10,R3]
				LDR			 R1, =0xE4
				STR			 R3, [SP,#0x54]
				LDR			 R0, =0xB8
				LDR			 R2, =0x44
				LDR			 R12, =0xEC
				LDR			 LR, =0x4C
				LDR			 R3, =0x90
				STR			 R0, [SP,#0x20]
				STR			 R2, [SP]
				STR			 R1, [SP,#0x4]
				STR			 R12, [SP,#0x18]
				STR			 LR, [SP,#0x1C]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD0
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x10
				LDRB			R11, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x50
				LDRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x1C]
				LDRB			R2, [R2]
				LDR			 R0, [SP]
				LDR			 R6, [R10,R3]
				STR			 R2, [SP,#0x50]
				LDR			 R3, =0xF0
				LDR			 R2, [SP,#0x20]
				LDR			 R7, [R10,R1]
				LDR			 R1, [SP,#0x18]
				LDR			 R8, [R10,R0]
				LDR			 R0, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R5, [R10,R1]
				MOV			 R1, #1
				STRB			R1, [R2]
				LDR			 R3, =0x88
				LDR			 R2, =0x28
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0xCC
				STR			 R1, [R3]
				LDR			 R3, [R10,R2]
				STR			 R3, [R0]
				LDR			 R3, =0x78
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x34
				MOV			 R4, #0
				STRB			R4, [R9]
				STRB			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xC0
				STRH			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD8
				LDR			 R0, [SP,#0x54]
				STRB			R12, [R5]
				STRB			LR, [R7]
				STRB			R11, [R8]
				LDR			 R1, [SP,#0x50]
				STRH			R4, [R2]
				LDR			 R2, [R10,R3]
				STRB			R1, [R6]
				STR			 R2, [R0]
				BL			  sub_BF801540
				MOV			 R3, #0xFFFFFFFF
				STRB			R3, [R5]
				STRB			R4, [R6]
				STRB			R3, [R7]
				STRB			R3, [R8]
				BL			  sub_BF801CB8
				LDR			 R6, [SP,#0x54]
				MOV			 R5, R4

loc_BF8027AC:							@ CODE XREF: sub_BF802228+59C↓j
				AND			 R0, R5, #0xFF
				LDR			 R4, [R6]
				BL			  sub_BF802114
				STRB			R0, [R5,R4]
				ADD			 R5, R5, #1
				CMP			 R5, #0x40
				BNE			 loc_BF8027AC
				LDR			 R2, [SP,#0x4]
				LDR			 R3, [R10,R2]
				LDRB			R2, [R3]
				CMP			 R2, #0xFF
				BEQ			 loc_BF802534
				LDR			 R3, [SP,#0x18]
				LDR			 R12, [SP,#0x20]
				LDR			 R5, =0x68
				LDR			 R2, [R10,R3]
				LDR			 R1, [R10,R12]
				LDR			 R0, [R10,R5]
				LDRB			R12, [R2]
				LDRB			LR, [R0]
				LDR			 R3, [R1]
				LDR			 R0, [SP]
				LDRB			R1, [R12,R3]
				LDR			 R2, [R10,R0]
				CMP			 LR, #0
				STRB			R1, [R2]
				BEQ			 loc_BF802534
				MOV			 R4, #0

loc_BF80281C:							@ CODE XREF: sub_BF802228+640↓j
				LDR			 R1, [SP,#0x4]
				LDR			 R3, [R10,R1]
				MOV			 R1, R4
				LDRB			R0, [R3]
				BL			  sub_BF8015E8
				MOV			 R0, #0xA
				BL			  sub_BF80195C
				BL			  sub_BF801694
				CMP			 R0, #0
				LDREQ		   R2, [SP,#0x1C]
				ADD			 R3, R4, #1
				ADD			 R1, R4, #1
				LDREQ		   R3, [R10,R2]
				ANDEQ		   R4, R1, #0xFF
				ANDNE		   R4, R3, #0xFF
				STREQB		  R4, [R3]
				LDR			 R3, [R10,R5]
				LDRB			R2, [R3]
				CMP			 R2, R4
				BHI			 loc_BF80281C
				B			   loc_BF802534
@ ---------------------------------------------------------------------------

loc_BF802870:							@ CODE XREF: sub_BF802228+340↑j
				LDR			 R1, =0xE4
				LDR			 R12, =0x4C
				LDR			 R2, =0x44
				LDR			 R3, =0xEC
				STR			 R1, [SP,#0x4]
				STR			 R3, [SP,#0x18]
				STR			 R2, [SP]
				STR			 R12, [SP,#0x1C]
				LDR			 R3, [R10,R1]
				LDR			 LR, [SP,#0x18]
				LDR			 R2, [R10,R2]
				LDR			 R0, [R10,R12]
				LDRB			R12, [R3]
				LDR			 R3, =0xF4
				LDR			 R1, [R10,LR]
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x5C
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x38
				LDRB			R4, [R1]
				STRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE8
				STRB			R4, [R2]
				LDRB			R5, [R0]
				LDR			 R2, [R10,R3]
				STRB			R5, [R2]
				B			   loc_BF8025DC
@ ---------------------------------------------------------------------------

loc_BF8028E8:							@ CODE XREF: sub_BF802228+64↑j
				LDR			 R0, =0xF0
				STR			 R0, [SP,#0x10]
				LDR			 R3, [R10,R0]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BEQ			 loc_BF802E38
				LDR			 R1, =0x78
				STR			 R1, [SP,#0x14]
				LDR			 R3, [R10,R1]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BNE			 loc_BF802534

loc_BF802918:							@ CODE XREF: sub_BF802228+C24↓j
				LDR			 R12, =0x44
				LDR			 LR, =0xEC
				LDR			 R3, =0xE4
				LDR			 R0, =0x4C
				STR			 R3, [SP,#0x4]
				STR			 R0, [SP,#0x1C]
				STR			 R12, [SP]
				STR			 LR, [SP,#0x18]
				LDR			 R3, [R10,R3]
				LDR			 R2, [R10,R12]
				LDRB			R12, [R3]
				LDR			 R3, =0x90
				LDR			 R1, [R10,LR]
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD0
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x10
				LDRB			R4, [R1]
				STRB			LR, [R2]
				LDR			 R0, [R10,R0]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x50
				STRB			R4, [R2]
				LDRB			R5, [R0]
				LDR			 R2, [R10,R3]
				STRB			R5, [R2]

loc_BF802988:							@ CODE XREF: sub_BF802228+C9C↓j
				LDR			 R0, [SP,#0x4]
				LDR			 R1, [SP]
				LDR			 R0, [R10,R0]
				LDR			 R3, =0xC8
				STR			 R0, [SP,#0x48]
				LDR			 R2, [R10,R3]
				LDR			 R1, [R10,R1]
				LDR			 R3, =0x98
				LDRB			R11, [R2]
				STR			 R1, [SP,#0x44]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x30
				LDRB			R12, [R2]
				LDR			 R2, [SP,#0x18]
				LDR			 LR, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x1C]
				LDR			 R5, [R10,R3]
				LDR			 R3, =0x8C
				LDRB			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R1, [SP,#0x14]
				LDRB			R6, [R2]
				LDR			 R3, =0x94
				LDR			 R2, =0x28
				LDR			 R8, [R10,R1]
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0x48
				STR			 R1, [R3]
				LDR			 R3, =0xB8
				LDR			 R1, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xA8
				STR			 R1, [R2]
				LDR			 R2, =0x54
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, [SP,#0x48]
				LDR			 R0, [SP,#0x10]
				STRB			R11, [R2]
				LDR			 R7, [R10,R0]
				STR			 R1, [R3]
				LDR			 R3, [SP,#0x44]
				MOV			 R0, #1
				STRB			R12, [R3]
				STRB			R4, [LR]
				STRB			R6, [R5]
				STRB			R0, [R8]
				STRB			R0, [R7]
				B			   loc_BF802534
@ ---------------------------------------------------------------------------

loc_BF802A54:							@ CODE XREF: sub_BF802228+7C↑j
				LDR			 R0, =0xF0
				STR			 R0, [SP,#0x10]
				LDR			 R3, [R10,R0]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BEQ			 loc_BF802DBC
				LDR			 R1, =0xE4
				LDR			 R12, =0x4C
				LDR			 R2, =0x44
				LDR			 R3, =0xEC
				STR			 R1, [SP,#0x4]
				STR			 R3, [SP,#0x18]
				STR			 R2, [SP]
				STR			 R12, [SP,#0x1C]
				LDR			 R3, [R10,R1]
				LDR			 LR, [SP,#0x18]
				LDR			 R2, [R10,R2]
				LDR			 R0, [R10,R12]
				LDRB			R12, [R3]
				LDR			 R3, =0xC8
				LDR			 R1, [R10,LR]
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x98
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x30
				LDRB			R4, [R1]
				STRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x8C
				LDRB			R5, [R0]
				STRB			R4, [R2]
				LDR			 R2, [R10,R3]
				STRB			R5, [R2]

loc_BF802AE0:							@ CODE XREF: sub_BF802228+10C↑j
				LDR			 R3, =0x54
				LDR			 R0, =0xB8
				LDR			 R6, [R10,R3]
				LDR			 R3, =0xF4
				STR			 R0, [SP,#0x20]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x5C
				LDRB			R7, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x38
				LDRB			R8, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE8
				LDRB			R2, [R2]
				STR			 R2, [SP,#0x40]
				LDR			 R2, [R10,R3]
				LDRB			R2, [R2]
				STR			 R2, [SP,#0x3C]
				LDR			 R2, [SP,#0x10]
				LDR			 R3, [SP,#0x4]
				LDR			 R2, [R10,R2]
				LDR			 R1, [SP,#0xC]
				LDR			 R12, [R10,R3]
				LDR			 R1, [R10,R1]
				LDR			 R3, [SP,#0x1C]
				STR			 R2, [SP,#0x34]
				LDR			 R2, [SP,#0x18]
				LDR			 R5, [R10,R3]
				LDR			 R4, [R10,R2]
				STR			 R1, [SP,#0x38]
				LDR			 R3, =0x70
				LDR			 R1, [SP]
				LDR			 R2, =0x28
				LDR			 LR, [R10,R1]
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0xB0
				LDR			 R0, [R10,R0]
				STR			 R1, [R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0x1C
				STR			 R3, [R0]
				LDR			 R3, [R10,R2]
				STR			 R3, [R6]
				STRB			R7, [R12]
				STRB			R8, [LR]
				LDR			 R12, [SP,#0x40]
				STRB			R12, [R4]
				LDR			 LR, [SP,#0x3C]
				STRB			LR, [R5]
				LDR			 R0, [SP,#0x38]
				LDR			 R1, [SP,#0x34]
				MOV			 R3, #0
				STRH			R11, [R0]
				STRB			R3, [R1]
				B			   loc_BF802418
@ ---------------------------------------------------------------------------

loc_BF802BC0:							@ CODE XREF: sub_BF802228+4C↑j
				LDR			 LR, =0xF0
				STR			 LR, [SP,#0x10]
				LDR			 R3, [R10,LR]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BNE			 loc_BF802D2C
				LDR			 R0, =0x78
				STR			 R0, [SP,#0x14]
				LDR			 R3, [R10,R0]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BNE			 loc_BF802534

loc_BF802BF0:							@ CODE XREF: sub_BF802228+B18↓j
				LDR			 R3, =0x44
				LDR			 R2, =0xE4
				LDR			 R12, =0xEC
				LDR			 LR, =0x4C
				STR			 R3, [SP]
				STR			 R2, [SP,#0x4]
				STR			 R12, [SP,#0x18]
				STR			 LR, [SP,#0x1C]
				LDR			 R0, [SP]
				LDR			 R3, [R10,R2]
				LDR			 R1, [R10,R12]
				LDR			 R2, [R10,R0]
				LDRB			R12, [R3]
				LDR			 R3, =0x90
				LDR			 R0, [R10,LR]
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD0
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x10
				LDRB			R4, [R1]
				STRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x50
				STRB			R4, [R2]
				LDRB			R5, [R0]
				LDR			 R2, [R10,R3]
				STRB			R5, [R2]

loc_BF802C64:							@ CODE XREF: sub_BF802228+B90↓j
				LDR			 R3, =0xF4
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x5C
				LDRB			R12, [R2]
				LDR			 R2, [SP]
				LDR			 LR, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x18]
				LDR			 R5, [R10,R3]
				LDR			 R3, =0x38
				LDRB			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE8
				LDRB			R6, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x14]
				LDR			 R1, [SP,#0x4]
				LDRB			R8, [R2]
				LDR			 R3, [R10,R3]
				LDR			 R2, [SP,#0x10]
				STR			 R3, [SP,#0x4C]
				LDR			 R0, [R10,R1]
				LDR			 R11, [R10,R2]
				LDR			 R1, [SP,#0x1C]
				LDR			 R3, =0x70
				LDR			 R2, =0x28
				LDR			 R7, [R10,R1]
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0xB0
				STR			 R1, [R3]
				LDR			 R3, =0xB8
				LDR			 R1, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x1C
				STR			 R1, [R2]
				LDR			 R2, =0x54
				LDR			 R1, [R10,R3]
				LDR			 R3, [R10,R2]
				STR			 R1, [R3]
				MOV			 R3, #0
				STRB			R12, [R0]
				STRB			R4, [LR]
				STRB			R6, [R5]
				STRB			R8, [R7]
				STRB			R3, [R11]
				LDR			 R12, [SP,#0x4C]
				MOV			 R2, #1
				STRB			R2, [R12]
				B			   loc_BF802534
@ ---------------------------------------------------------------------------

loc_BF802D2C:							@ CODE XREF: sub_BF802228+9AC↑j
				LDR			 R1, =0x78
				STR			 R1, [SP,#0x14]
				LDR			 R3, [R10,R1]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BEQ			 loc_BF802BF0
				LDR			 R1, =0xE4
				LDR			 R12, =0x4C
				LDR			 R2, =0x44
				LDR			 R3, =0xEC
				STR			 R1, [SP,#0x4]
				STR			 R3, [SP,#0x18]
				STR			 R2, [SP]
				STR			 R12, [SP,#0x1C]
				LDR			 R3, [R10,R1]
				LDR			 LR, [SP,#0x18]
				LDR			 R2, [R10,R2]
				LDR			 R0, [R10,R12]
				LDRB			R12, [R3]
				LDR			 R3, =0xC8
				LDR			 R1, [R10,LR]
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x98
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x30
				LDRB			R4, [R1]
				STRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x8C
				STRB			R4, [R2]
				LDRB			R5, [R0]
				LDR			 R2, [R10,R3]
				STRB			R5, [R2]
				B			   loc_BF802C64
@ ---------------------------------------------------------------------------

loc_BF802DBC:							@ CODE XREF: sub_BF802228+840↑j
				LDR			 R2, =0xEC
				LDR			 R0, =0xE4
				LDR			 R1, =0x44
				LDR			 R3, =0x4C
				STR			 R2, [SP,#0x18]
				STR			 R3, [SP,#0x1C]
				STR			 R0, [SP,#0x4]
				STR			 R1, [SP]
				LDR			 R12, [SP,#0x18]
				LDR			 R3, [R10,R0]
				LDR			 LR, [SP,#0x1C]
				LDR			 R2, [R10,R1]
				LDR			 R1, [R10,R12]
				LDRB			R12, [R3]
				LDR			 R3, =0xF4
				LDR			 R0, [R10,LR]
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x5C
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x38
				LDRB			R4, [R1]
				STRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE8
				LDRB			R5, [R0]
				STRB			R4, [R2]
				LDR			 R2, [R10,R3]
				STRB			R5, [R2]
				B			   loc_BF802338
@ ---------------------------------------------------------------------------

loc_BF802E38:							@ CODE XREF: sub_BF802228+6D4↑j
				LDR			 R2, =0x78
				STR			 R2, [SP,#0x14]
				LDR			 R3, [R10,R2]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BEQ			 loc_BF802918
				LDR			 R1, =0xE4
				LDR			 R12, =0x4C
				LDR			 R2, =0x44
				LDR			 R3, =0xEC
				STR			 R1, [SP,#0x4]
				STR			 R3, [SP,#0x18]
				STR			 R2, [SP]
				STR			 R12, [SP,#0x1C]
				LDR			 R3, [R10,R1]
				LDR			 LR, [SP,#0x18]
				LDR			 R2, [R10,R2]
				LDR			 R0, [R10,R12]
				LDRB			R12, [R3]
				LDR			 R3, =0xF4
				LDR			 R1, [R10,LR]
				LDRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x5C
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x38
				LDRB			R4, [R1]
				STRB			LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE8
				STRB			R4, [R2]
				LDRB			R5, [R0]
				LDR			 R2, [R10,R3]
				STRB			R5, [R2]
				B			   loc_BF802988
@ End of function sub_BF802228

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF802F58:							@ CODE XREF: sub_BF8030EC+78↓p
				STMFD		   SP!, {R4-R6,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF802F70)
				LDR			 R3, =0x64
				LDR			 R2, =0x7C
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R5, [R10,R3]
				LDR			 R3, [R10,R2]
				LDR			 R0, [R3]
				LDR			 R3, =0xA0
				LDR			 R2, [R10,R3]
				LDR			 R12, [R5]
				LDR			 R1, [R2]
				MOV			 LR, R0,LSR#2
				CMP			 LR, R12
				SUB			 SP, SP, #4
				ADD			 R6, R0, R1
				BEQ			 loc_BF803048
				LDR			 R3, =0xBC
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x68
				LDRB			R4, [R2]
				LDR			 R2, [R10,R3]
				MOV			 R12, R6,LSR#2
				MOV			 R0, R12
				LDRB			R1, [R2]
				STR			 LR, [R5]
				AND			 R5, R12, R4
				BL			  sub_BF804A7C
				MOV			 R3, #0x84000003
				ADD			 R3, R3, #0x210000
				MOV			 R1, R0,LSR#1
				ADD			 R3, R3, #0x840
				UMULL		   R12, R2, R3, R1
				LDR			 R12, =0x40
				LDR			 R12, [R10,R12]
				MOV			 R2, R2,LSR#4
				MOV			 R3, R2,LSL#16
				LDRH			R1, [R12]
				MOV			 R3, R3,LSR#16
				RSB			 R2, R2, R2,LSL#5
				SUB			 R0, R0, R2,LSL#1
				CMP			 R1, R3
				AND			 R4, R0, #0xFF
				STRNEH		  R3, [R12]
				BLNE			sub_BF802228
				LDR			 R3, =0xEC
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xB8
				LDRB			R0, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R1, [R2]
				CMP			 R0, R4
				LDRB			R0, [R4,R1]
				BEQ			 loc_BF803068
				CMP			 R0, #0xFF
				BEQ			 loc_BF803090

loc_BF803038:							@ CODE XREF: sub_BF802F58+134↓j
				MOV			 R1, R5
				BL			  sub_BF8015E8

loc_BF803040:							@ CODE XREF: sub_BF802F58+144↓j
										@ sub_BF802F58+15C↓j
				MOV			 R0, #1
				BL			  sub_BF80195C

loc_BF803048:							@ CODE XREF: sub_BF802F58+40↑j
				LDR			 R2, =0xD4
				LDR			 R1, [R10,R2]
				AND			 R3, R6, #3
				MOV			 R0, #0xB
				STRB			R3, [R1]
				ADD			 SP, SP, #4
				LDMFD		   SP!, {R4-R6,R10,LR}
				B			   sub_BF80195C
@ ---------------------------------------------------------------------------

loc_BF803068:							@ CODE XREF: sub_BF802F58+D4↑j
				LDR			 R3, =0x4C
				LDR			 R2, [R10,R3]
				LDRB			R1, [R2]
				CMP			 R1, R5
				BHI			 loc_BF8030A0
				LDR			 R3, =0x44
				LDR			 R2, [R10,R3]
				LDRB			R0, [R2]
				CMP			 R0, #0xFF
				BNE			 loc_BF803038

loc_BF803090:							@ CODE XREF: sub_BF802F58+DC↑j
				BL			  sub_BF801814
				MOV			 R1, #0
				BL			  sub_BF8015E8
				B			   loc_BF803040
@ ---------------------------------------------------------------------------

loc_BF8030A0:							@ CODE XREF: sub_BF802F58+120↑j
				LDR			 R3, =0xE4
				LDR			 R2, [R10,R3]
				MOV			 R1, R5
				LDRB			R0, [R2]
				BL			  sub_BF8015E8
				B			   loc_BF803040
@ End of function sub_BF802F58

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF8030EC:							@ CODE XREF: sub_BF801378+18↑p
				STMFD		   SP!, {R4-R8,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF803100)
				LDR			 R3, =0x20
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R12, [R10,R3]
				LDR			 R3, =0x7C
				LDR			 R5, [R10,R3]
				LDR			 R3, =0x24
				MOV			 R0, R0,LSR#9
				LDR			 LR, [R12]
				MOV			 R6, R2,LSR#9
				ADD			 R12, R0, #1
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xDC
				CMP			 R12, LR
				MOV			 R12, #1
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				SUB			 SP, SP, #4
				STR			 R6, [R2]
				MOV			 R4, R1
				STR			 R0, [R5]
				BHI			 loc_BF803184
				CMP			 R6, #0
				BEQ			 loc_BF803184
				LDR			 R3, =0xA4
				LDR			 R8, [R10,R3]
				MOV			 R7, R5
				MOV			 R5, #0

loc_BF803160:							@ CODE XREF: sub_BF8030EC+94↓j
				STR			 R4, [R8]
				BL			  sub_BF802F58
				LDR			 R3, [R7]
				ADD			 R5, R5, #1
				ADD			 R3, R3, #1
				CMP			 R6, R5
				STR			 R3, [R7]
				ADD			 R4, R4, #0x200
				BNE			 loc_BF803160

loc_BF803184:							@ CODE XREF: sub_BF8030EC+58↑j
										@ sub_BF8030EC+60↑j
				ADD			 SP, SP, #4
				LDMFD		   SP!, {R4-R8,R10,LR}
				BX			  LR
@ End of function sub_BF8030EC

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF8031A8:							@ CODE XREF: sub_BF8034EC+26C↓p
										@ sub_BF8034EC+298↓p ...

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF8031C4)
				MOV			 R8, R1
				LDR			 R3, =0xAC
				LDR			 R1, =0xFFFFFFA4
				ADD			 R10, PC, R10 @ dword_BF804C08
				SUB			 SP, SP, #0x2C
				MOV			 R4, R0
				ADD			 R1, R10, R1
				LDR			 R0, [R10,R3]
				STR			 R2, [SP,#0x24]
				BL			  sub_BF8013C4
				LDR			 R3, =0x74
				LDR			 R1, =0xFFFFFFB4
				LDR			 R2, =0xB8
				LDR			 R0, [R10,R3]
				ADD			 R1, R10, R1
				STR			 R2, [SP,#0x4]
				BL			  sub_BF8013C4
				LDR			 R2, [SP,#0x4]
				LDR			 R1, =0xFFFFFFC0
				LDR			 R3, [R10,R2]
				LDR			 R2, =0x54
				LDR			 R0, [R3]
				ADD			 R1, R10, R1
				STR			 R2, [SP,#0xC]
				BL			  sub_BF8013C4
				LDR			 R2, [SP,#0xC]
				LDR			 R1, =0xFFFFFFD0
				LDR			 R3, [R10,R2]
				ADD			 R1, R10, R1
				LDR			 R0, [R3]
				BL			  sub_BF8013C4
				LDR			 R3, =0x6C
				LDR			 R1, =0xFFFFFFE0
				LDR			 R11, [R10,R3]
				MOV			 R4, R4,LSR#11
				AND			 R4, R4, #0xFF
				ADD			 R1, R10, R1
				MOV			 R0, R11
				BL			  sub_BF8013C4
				MOV			 LR, R4,LSR#6
				ADD			 LR, LR, #5
				MOV			 R0, #0x3E
				MOV			 R0, R0,LSL LR
				LDR			 R3, =0xE0
				LDR			 R2, =0xC
				STR			 R3, [SP]
				LDR			 R3, =0x3C
				STR			 R2, [SP,#0x14]
				STR			 R3, [SP,#0x10]
				LDR			 R2, =0xA0
				LDR			 R3, [SP]
				STR			 R2, [SP,#0x8]
				LDR			 R9, [R10,R3]
				LDR			 R2, [SP,#0x14]
				LDR			 R3, [SP,#0x10]
				LDR			 R5, [R10,R2]
				LDR			 R6, [R10,R3]
				LDR			 R2, [SP,#0x8]
				LDR			 R3, =0xBC
				LDR			 R7, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x58
				SUB			 R12, R4, #1
				STRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x24]
				STR			 R3, [R2]
				LDR			 R3, =0x68
				MOV			 R1, R8,LSL#16
				LDR			 R2, [R10,R3]
				MOV			 R1, R1,LSR#22
				STRH			R8, [R5]
				STRH			R1, [R6]
				STRB			LR, [R9]
				STR			 R0, [R7]
				STRB			R4, [R2]
				MOV			 R0, R11
				MOV			 R2, #0
				ADD			 R3, R11, #0x100

loc_BF8032EC:							@ CODE XREF: sub_BF8031A8+14C↓j
				STRB			R2, [R0],#1
				CMP			 R0, R3
				BNE			 loc_BF8032EC
				LDR			 R3, =0x28
				LDR			 R2, =0x88
				LDR			 R3, [R10,R3]
				LDR			 R2, [R10,R2]
				STR			 R2, [R3]
				STR			 R3, [SP,#0x20]
				STR			 R2, [SP,#0x1C]
				BL			  sub_BF801DC4
				CMP			 R0, #0
				BNE			 loc_BF803324

loc_BF803320:							@ CODE XREF: sub_BF8031A8:loc_BF803320↓j
				B			   loc_BF803320
@ ---------------------------------------------------------------------------

loc_BF803324:							@ CODE XREF: sub_BF8031A8+174↑j
				LDR			 R3, =0x40
				LDR			 R4, [R10,R3]
				MOV			 R2, #0xFFFF01FF
				STRH			R2, [R4]
				BL			  sub_BF801CB8
				LDR			 R2, [SP,#0x20]
				LDR			 R3, [R2]
				LDR			 R5, [R3,#0xFC]
				LDR			 R3, [SP]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x8]
				MOV			 R12, R5,LSL#16
				LDR			 R0, [R10,R3]
				MOV			 R12, R12,LSR#22
				LDRB			R1, [R2]
				RSB			 R3, R12, R12,LSL#5
				LDR			 LR, [R0]
				MOV			 R3, R3,LSL#1
				ADD			 R1, R1, #2
				RSB			 LR, LR, R3,LSL R1
				LDR			 R2, =0x90
				LDR			 R3, [R10,R2]
				LDR			 R2, =0xD0
				LDRB			R0, [R3]
				LDR			 R3, [R10,R2]
				LDRB			R6, [R3]
				LDR			 R3, [SP,#0x10]
				LDR			 R7, [R10,R3]
				LDR			 R3, [SP,#0xC]
				LDR			 R2, [SP,#0x14]
				LDR			 R3, [R10,R3]
				LDR			 R1, [R10,R2]
				STR			 R3, [SP,#0x18]
				LDR			 R2, [SP,#0x4]
				LDR			 R3, =0x10
				LDR			 R8, [R10,R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x20
				LDRB			R11, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x60
				STR			 LR, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xCC
				STRH			R5, [R1]
				MOV			 R1, #1
				STRB			R1, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xD8
				STR			 R2, [R8]
				LDR			 R2, [R10,R3]
				LDR			 R3, [SP,#0x18]
				STR			 R2, [R3]
				LDR			 R3, =0xE4
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x44
				STRB			R0, [R2]
				LDR			 R2, [R10,R3]
				STRB			R6, [R2]
				LDR			 R3, [SP,#0x1C]
				LDR			 R2, [SP,#0x20]
				STR			 R3, [R2]
				LDR			 R3, =0xEC
				MOV			 R2, #0
				STRH			R2, [R4]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x50
				STRB			R11, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x4C
				LDRB			R1, [R2]
				LDR			 R2, [R10,R3]
				STRH			R12, [R7]
				STRB			R1, [R2]
				BL			  sub_BF801540
				BL			  sub_BF801580
				BL			  sub_BF802228
				MOV			 R0, #0
				ADD			 SP, SP, #0x2C
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ End of function sub_BF8031A8

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF8034EC:							@ CODE XREF: sub_BF8013A4+C↑p

				STMFD		   SP!, {R4,LR}
				SUB			 SP, SP, #8
				MOV			 R4, R0
				ADD			 R0, SP, #0x4
				BL			  sub_BF800390
				LDRB			R3, [SP,#0x4]
				CMP			 R3, #0xEC
				CMPNE		   R3, #0xAD
				BEQ			 loc_BF803524
				CMP			 R3, #0x2C
				BEQ			 loc_BF803524
				CMP			 R3, #0x98
				BEQ			 loc_BF803524

loc_BF803520:							@ CODE XREF: sub_BF8034EC:loc_BF803520↓j
				B			   loc_BF803520
@ ---------------------------------------------------------------------------

loc_BF803524:							@ CODE XREF: sub_BF8034EC+20↑j
										@ sub_BF8034EC+28↑j ...
				LDRB			R3, [SP,#0x5]
				SUB			 R3, R3, #0x71
				CMP			 R3, #0x80 @ switch 129 cases
				ADDLS		   PC, PC, R3,LSL#2 @ switch jump
@ ---------------------------------------------------------------------------

loc_BF803534:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803538:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF80378C @ jumptable BF803530 cases 0,89,105
@ ---------------------------------------------------------------------------

loc_BF80353C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803540:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803544:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037D8 @ jumptable BF803530 cases 3,8,80,128
@ ---------------------------------------------------------------------------

loc_BF803548:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80354C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803550:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803554:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803558:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037D8 @ jumptable BF803530 cases 3,8,80,128
@ ---------------------------------------------------------------------------

loc_BF80355C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803560:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803564:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803568:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80356C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803570:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803574:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803578:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80357C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803580:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803584:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803588:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80358C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803590:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803594:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803598:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80359C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035A0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035A4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035A8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035AC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035B0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035B4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035B8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035BC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035C0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035C4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035C8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035CC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035D0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035D4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035D8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035DC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035E0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035E4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035E8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035EC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035F0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035F4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035F8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8035FC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803600:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803604:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803608:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80360C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803610:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803614:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803618:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80361C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803620:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803624:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803628:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80362C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803630:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803634:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803638:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80363C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803640:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803644:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803648:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80364C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803650:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803654:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803658:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80365C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803660:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803664:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803668:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80366C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803670:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803674:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803678:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037D8 @ jumptable BF803530 cases 3,8,80,128
@ ---------------------------------------------------------------------------

loc_BF80367C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803680:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF80373C @ jumptable BF803530 cases 82,98
@ ---------------------------------------------------------------------------

loc_BF803684:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803688:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF803768 @ jumptable BF803530 cases 84,100
@ ---------------------------------------------------------------------------

loc_BF80368C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803690:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803694:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803698:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80369C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF80378C @ jumptable BF803530 cases 0,89,105
@ ---------------------------------------------------------------------------

loc_BF8036A0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036A4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B4 @ jumptable BF803530 cases 91,107
@ ---------------------------------------------------------------------------

loc_BF8036A8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036AC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036B0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036B4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036B8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036BC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036C0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF80373C @ jumptable BF803530 cases 82,98
@ ---------------------------------------------------------------------------

loc_BF8036C4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036C8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF803768 @ jumptable BF803530 cases 84,100
@ ---------------------------------------------------------------------------

loc_BF8036CC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036D0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036D4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036D8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036DC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF80378C @ jumptable BF803530 cases 0,89,105
@ ---------------------------------------------------------------------------

loc_BF8036E0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036E4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B4 @ jumptable BF803530 cases 91,107
@ ---------------------------------------------------------------------------

loc_BF8036E8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036EC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036F0:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036F4:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036F8:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8036FC:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803700:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803704:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803708:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80370C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803710:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803714:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803718:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80371C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803720:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803724:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803728:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF80372C:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803730:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803734:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF803738:							@ CODE XREF: sub_BF8034EC+44↑j
				B			   loc_BF8037D8 @ jumptable BF803530 cases 3,8,80,128
@ ---------------------------------------------------------------------------

loc_BF80373C:							@ CODE XREF: sub_BF8034EC+44↑j
										@ sub_BF8034EC:loc_BF803680↑j ...
				LDRB			R3, [SP,#0x7] @ jumptable BF803530 cases 82,98
				TST			 R3, #0x20
				BEQ			 loc_BF8037FC
				MOV			 R3, R4
				MOV			 R1, #0xFC0
				MOV			 R2, #0x1000000
				MOV			 R0, #0x40000
				BL			  sub_BF8031A8

loc_BF80375C:							@ CODE XREF: sub_BF8034EC+29C↓j
										@ sub_BF8034EC+2C0↓j ...
				ADD			 SP, SP, #8
				LDMFD		   SP!, {R4,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF803768:							@ CODE XREF: sub_BF8034EC+44↑j
										@ sub_BF8034EC:loc_BF803688↑j ...
				LDRB			R3, [SP,#0x7] @ jumptable BF803530 cases 84,100
				TST			 R3, #0x20
				BEQ			 loc_BF803844
				MOV			 R3, R4
				MOV			 R1, #0x1FC0
				MOV			 R2, #0x1000000
				MOV			 R0, #0x40000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ ---------------------------------------------------------------------------

loc_BF80378C:							@ CODE XREF: sub_BF8034EC+44↑j
										@ sub_BF8034EC:loc_BF803538↑j ...
				LDRB			R3, [SP,#0x7] @ jumptable BF803530 cases 0,89,105
				TST			 R3, #0x20
				BEQ			 loc_BF803814
				MOV			 R3, R4
				MOV			 R1, #0x3C0
				MOV			 R2, #0x1000000
				MOV			 R0, #0x40000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ ---------------------------------------------------------------------------

loc_BF8037B0:							@ CODE XREF: sub_BF8034EC+44↑j
										@ sub_BF8034EC:loc_BF803534↑j ...
				B			   loc_BF8037B0 @ jumptable BF803530 default case
@ ---------------------------------------------------------------------------

loc_BF8037B4:							@ CODE XREF: sub_BF8034EC+44↑j
										@ sub_BF8034EC:loc_BF8036A4↑j ...
				LDRB			R3, [SP,#0x7] @ jumptable BF803530 cases 91,107
				TST			 R3, #0x20
				BEQ			 loc_BF80382C
				MOV			 R3, R4
				MOV			 R1, #0x7C0
				MOV			 R2, #0x1000000
				MOV			 R0, #0x40000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ ---------------------------------------------------------------------------

loc_BF8037D8:							@ CODE XREF: sub_BF8034EC+44↑j
										@ sub_BF8034EC:loc_BF803544↑j ...
				LDRB			R3, [SP,#0x7] @ jumptable BF803530 cases 3,8,80,128
				TST			 R3, #0x20
				BEQ			 loc_BF80385C
				MOV			 R3, R4
				MOV			 R1, #0x1C0
				MOV			 R2, #0x1000000
				MOV			 R0, #0x40000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ ---------------------------------------------------------------------------

loc_BF8037FC:							@ CODE XREF: sub_BF8034EC+258↑j
				MOV			 R3, R4
				MOV			 R1, #0x1FC0
				MOV			 R2, #0x800000
				MOV			 R0, #0x20000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ ---------------------------------------------------------------------------

loc_BF803814:							@ CODE XREF: sub_BF8034EC+2A8↑j
				MOV			 R3, R4
				MOV			 R1, #0x7C0
				MOV			 R2, #0x800000
				MOV			 R0, #0x20000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ ---------------------------------------------------------------------------

loc_BF80382C:							@ CODE XREF: sub_BF8034EC+2D0↑j
				MOV			 R3, R4
				MOV			 R1, #0xFC0
				MOV			 R2, #0x800000
				MOV			 R0, #0x20000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ ---------------------------------------------------------------------------

loc_BF803844:							@ CODE XREF: sub_BF8034EC+284↑j
				MOV			 R3, R4
				MOV			 R1, #0x3FC0
				MOV			 R2, #0x800000
				MOV			 R0, #0x20000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ ---------------------------------------------------------------------------

loc_BF80385C:							@ CODE XREF: sub_BF8034EC+2F4↑j
				MOV			 R3, R4
				MOV			 R1, #0x3C0
				MOV			 R2, #0x800000
				MOV			 R0, #0x20000
				BL			  sub_BF8031A8
				B			   loc_BF80375C
@ End of function sub_BF8034EC


@ =============== S U B R O U T I N E =======================================


sub_BF803874:

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF803888)
				LDR			 R9, =0x24
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R3, [R10,R9]
				LDRB			R2, [R3]
				CMP			 R2, #0
				SUB			 SP, SP, #0x14
				BEQ			 loc_BF803A38
				LDR			 R3, =0x7C
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xA0
				LDR			 R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE0
				LDR			 R1, [R2]
				LDR			 R2, [R10,R3]
				ADD			 R12, R12, R1
				LDRB			LR, [R2]
				MOV			 R4, R12,LSR#2
				MOV			 LR, R4,LSR LR
				MOV			 R3, #0x84000003
				ADD			 R3, R3, #0x210000
				ADD			 R3, R3, #0x840
				MOV			 R2, LR,LSR#1
				UMULL		   R1, R0, R3, R2
				LDR			 R2, =0xB4
				LDR			 R3, =0x9C
				STR			 R2, [SP,#0x8]
				STR			 R3, [SP,#0xC]
				LDR			 R3, =0xBC
				LDR			 R5, [SP,#0x8]
				LDR			 R1, =0x40
				MOV			 R0, R0,LSR#4
				LDR			 R2, [R10,R3]
				MOV			 R3, R0,LSL#16
				MOV			 R7, R3,LSR#16
				LDR			 R8, [R10,R1]
				LDR			 R3, [SP,#0xC]
				LDR			 R5, [R10,R5]
				LDR			 R11, =0x80
				LDRB			R6, [R2]
				STR			 R5, [SP,#0x4]
				LDRH			R2, [R8]
				LDR			 R5, [R10,R3]
				LDR			 R3, =0x2C
				LDR			 R1, [R10,R11]
				CMP			 R2, R7
				AND			 R12, R12, #3
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x84
				STRB			R12, [R1]
				LDR			 R1, [SP,#0x4]
				RSB			 R0, R0, R0,LSL#5
				STR			 LR, [R2]
				LDR			 R2, [R10,R3]
				SUB			 R0, LR, R0,LSL#1
				AND			 R4, R4, R6
				STRB			R4, [R1]
				STRB			R0, [R5]
				STRH			R7, [R2]
				STRNEH		  R7, [R8]
				BLNE			sub_BF802228

loc_BF803970:							@ CODE XREF: sub_BF803874+2C8↓j
				LDR			 R3, [R10,R11]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BNE			 loc_BF803A88

loc_BF803980:							@ CODE XREF: sub_BF803874+210↓j
										@ sub_BF803874+220↓j ...
				LDR			 R1, [SP,#0xC]
				LDR			 R2, =0xB8
				LDR			 R3, [R10,R1]
				LDRB			R12, [R3]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0xEC
				LDR			 R0, [R3]
				LDR			 R3, [R10,R2]
				LDRB			R1, [R3]
				LDR			 R3, =0x14
				LDRB			R0, [R12,R0]
				LDR			 R2, [R10,R3]
				CMP			 R1, R12
				STRB			R0, [R2]
				BEQ			 loc_BF803A9C
				CMP			 R0, #0xFF
				BEQ			 loc_BF803B40
				LDR			 R5, [SP,#0x8]
				LDR			 R3, [R10,R5]
				LDRB			R1, [R3]
				BL			  sub_BF8015E8

loc_BF8039D4:							@ CODE XREF: sub_BF803874+264↓j
										@ sub_BF803874+2D8↓j ...
				MOV			 R0, #1
				BL			  sub_BF80195C
				BL			  sub_BF801694

loc_BF8039E0:							@ CODE XREF: sub_BF803874+1D4↓j
										@ sub_BF803874+224↓j
				LDR			 R12, [R10,R11]
				LDR			 R3, =0xDC
				LDRB			R5, [R12]
				LDR			 R1, [R10,R3]
				ADD			 R3, R5, #1
				LDR			 R2, [R1]
				AND			 R3, R3, #3
				STRB			R3, [R12]
				LDR			 R3, =0xD4
				SUB			 R2, R2, #1
				LDR			 LR, [R10,R9]
				STR			 R2, [R1]
				LDR			 R2, [R10,R3]
				MOV			 R4, #0
				MOV			 R0, #2
				STRB			R4, [LR]
				STRB			R5, [R2]
				BL			  sub_BF80195C
				MOV			 R0, R4
				ADD			 SP, SP, #0x14
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF803A38:							@ CODE XREF: sub_BF803874+20↑j
				LDR			 R11, =0x80
				LDR			 R3, [R10,R11]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BNE			 loc_BF8039E0
				LDR			 R2, =0xB4
				STR			 R2, [SP,#0x8]
				LDR			 R12, [R10,R2]
				LDR			 R2, =0xBC
				LDR			 R1, [R10,R2]
				LDRB			R3, [R12]
				LDRB			R0, [R1]
				ADD			 R3, R3, #1
				AND			 R3, R3, R0
				CMP			 R3, #0
				STRB			R3, [R12]
				BEQ			 loc_BF803ADC
				LDR			 LR, =0x9C
				STR			 LR, [SP,#0xC]
				B			   loc_BF803980
@ ---------------------------------------------------------------------------

loc_BF803A88:							@ CODE XREF: sub_BF803874+108↑j
				LDR			 R3, [R10,R9]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BNE			 loc_BF803980
				B			   loc_BF8039E0
@ ---------------------------------------------------------------------------

loc_BF803A9C:							@ CODE XREF: sub_BF803874+144↑j
				LDR			 R2, [SP,#0x8]
				LDR			 R3, [R10,R2]
				LDR			 R2, =0x4C
				LDRB			R12, [R3]
				LDR			 R3, [R10,R2]
				LDRB			R1, [R3]
				CMP			 R1, R12
				BHI			 loc_BF803B50
				LDR			 R3, =0x44
				LDR			 R2, [R10,R3]
				LDRB			R0, [R2]
				CMP			 R0, #0xFF
				BEQ			 loc_BF803B40
				MOV			 R1, R12
				BL			  sub_BF8015E8
				B			   loc_BF8039D4
@ ---------------------------------------------------------------------------

loc_BF803ADC:							@ CODE XREF: sub_BF803874+204↑j
				LDR			 R3, =0x9C
				STR			 R3, [SP,#0xC]
				LDR			 R12, [R10,R3]
				LDRB			R2, [R12]
				MOV			 R1, #0x84000003
				ADD			 R2, R2, #1
				AND			 R2, R2, #0xFF
				ADD			 R1, R1, #0x210000
				MOV			 R0, R2,LSR#1
				ADD			 R1, R1, #0x840
				UMULL		   R5, R3, R1, R0
				MOV			 R3, R3,LSR#4
				RSB			 R3, R3, R3,LSL#5
				SUB			 R2, R2, R3,LSL#1
				AND			 R2, R2, #0xFF
				CMP			 R2, #0
				STRB			R2, [R12]
				BNE			 loc_BF803980
				LDR			 R3, =0x40
				LDR			 R1, [R10,R3]
				LDRH			R2, [R1]
				ADD			 R2, R2, #1
				STRH			R2, [R1]
				BL			  sub_BF802228
				B			   loc_BF803970
@ ---------------------------------------------------------------------------

loc_BF803B40:							@ CODE XREF: sub_BF803874+14C↑j
										@ sub_BF803874+258↑j
				BL			  sub_BF801814
				MOV			 R1, #0
				BL			  sub_BF8015E8
				B			   loc_BF8039D4
@ ---------------------------------------------------------------------------

loc_BF803B50:							@ CODE XREF: sub_BF803874+244↑j
				LDR			 R3, =0xE4
				LDR			 R2, [R10,R3]
				MOV			 R1, R12
				LDRB			R0, [R2]
				BL			  sub_BF8015E8
				B			   loc_BF8039D4
@ End of function sub_BF803874

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF803BB8:							@ CODE XREF: sub_BF803C30+7C↓p
										@ sub_BF803C30+B4↓p ...
				STMFD		   SP!, {R4,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF803BCC)
				CMP			 R0, #0xFF
				ADD			 R10, PC, R10 @ dword_BF804C08
				SUB			 SP, SP, #4
				MOV			 R4, R0
				MOV			 R12, #0
				BEQ			 loc_BF803C00
				BL			  sub_BF8020C8
				CMP			 R0, #0
				MOV			 R12, R0
				BEQ			 loc_BF803C10
				LDR			 R3, =0x54
				LDR			 R2, [R10,R3]
				LDR			 R1, [R2]
				MOV			 R3, #0xFFFFFFFE
				STRB			R3, [R4,R1]
				MOV			 R12, #0xFE

loc_BF803C00:							@ CODE XREF: sub_BF803BB8+1C↑j
										@ sub_BF803BB8+6C↓j
				MOV			 R0, R12
				ADD			 SP, SP, #4
				LDMFD		   SP!, {R4,R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF803C10:							@ CODE XREF: sub_BF803BB8+2C↑j
				LDR			 R3, =0x54
				LDR			 R2, [R10,R3]
				LDR			 R1, [R2]
				MOV			 R3, #0xFFFFFFFF
				STRB			R3, [R4,R1]
				B			   loc_BF803C00
@ End of function sub_BF803BB8

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF803C30:							@ CODE XREF: sub_BF803D28+6C↓p
										@ sub_BF803D28+D0↓p ...
				STMFD		   SP!, {R4-R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF803C44)
				LDR			 R3, =0xE4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R7, [R10,R3]
				LDR			 R2, =0x4C
				LDR			 R3, =0x54
				LDR			 R8, [R10,R2]
				LDR			 R9, [R10,R3]
				MOV			 R5, #2

loc_BF803C58:							@ CODE XREF: sub_BF803C30+8C↓j
				LDRB			R3, [R7]
				CMP			 R3, #0xFF
				BEQ			 loc_BF803CD0
				LDRB			R3, [R8]
				CMP			 R3, #0
				BEQ			 loc_BF803CD0
				BL			  sub_BF802004
				CMP			 R0, #0xFF
				MOV			 R4, R0
				MOV			 R1, R0
				MOV			 R2, #0
				BEQ			 loc_BF803CD8
				LDRB			R3, [R8]
				LDRB			R0, [R7]
				BL			  sub_BF801EC8
				SUBS			R6, R0, #0
				MOV			 R0, R4
				BEQ			 loc_BF803CE0
				LDR			 R3, [R9]
				MOV			 R2, #0xFFFFFFFE
				STRB			R2, [R4,R3]
				BL			  sub_BF803BB8
				SUB			 R3, R5, #1
				AND			 R5, R3, #0xFF
				CMP			 R5, #0xFF
				BNE			 loc_BF803C58
				MOV			 R2, R6

loc_BF803CC4:							@ CODE XREF: sub_BF803C30+A4↓j
										@ sub_BF803C30+AC↓j ...
				MOV			 R0, R2
				LDMFD		   SP!, {R4-R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF803CD0:							@ CODE XREF: sub_BF803C30+30↑j
										@ sub_BF803C30+3C↑j
				MOV			 R2, #0
				B			   loc_BF803CC4
@ ---------------------------------------------------------------------------

loc_BF803CD8:							@ CODE XREF: sub_BF803C30+54↑j
				MOV			 R2, #9
				B			   loc_BF803CC4
@ ---------------------------------------------------------------------------

loc_BF803CE0:							@ CODE XREF: sub_BF803C30+6C↑j
				LDRB			R0, [R7]
				BL			  sub_BF803BB8
				LDR			 R3, =0x54
				LDR			 R12, [R10,R3]
				LDRB			R1, [R7]
				LDR			 R2, [R12]
				MOV			 R3, #0xFFFFFFFE
				STRB			R3, [R1,R2]
				LDR			 R0, [R12]
				SUB			 R3, R3, #4
				MOV			 R2, R6
				STRB			R3, [R4,R0]
				STRB			R4, [R7]
				B			   loc_BF803CC4
@ End of function sub_BF803C30

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF803D28:							@ CODE XREF: sub_BF803E5C:loc_BF803F78↓p
										@ sub_BF803FC0:loc_BF8040E8↓p
				STMFD		   SP!, {R4-R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF803D3C)
				LDR			 R6, =0xE4
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R3, [R10,R6]
				LDRB			R2, [R3]
				CMP			 R2, #0xFF
				BEQ			 loc_BF803E1C
				LDR			 R3, =0x44
				LDR			 R3, [R10,R3]
				LDRB			R2, [R3]
				CMP			 R2, #0xFF
				BEQ			 loc_BF803DBC
				LDR			 R9, =0x4C
				LDR			 R8, =0x68
				MOV			 R7, R3
				MOV			 R4, #2

loc_BF803D6C:							@ CODE XREF: sub_BF803D28+84↓j
				LDR			 R3, [R10,R6]
				LDR			 R0, [R10,R9]
				LDR			 R12, [R10,R8]
				LDRB			R1, [R3]
				LDRB			R2, [R0]
				LDRB			R3, [R12]
				LDRB			R0, [R7]
				BL			  sub_BF801EC8
				SUBS			R5, R0, #0
				BEQ			 loc_BF803E24
				BL			  sub_BF803C30
				SUB			 R3, R4, #1
				CMP			 R0, #0
				AND			 R4, R3, #0xFF
				BNE			 loc_BF803DB4
				CMP			 R4, #0xFF
				BNE			 loc_BF803D6C

loc_BF803DB0:							@ CODE XREF: sub_BF803D28+EC↓j
				MOV			 R0, #2

loc_BF803DB4:							@ CODE XREF: sub_BF803D28+7C↑j
										@ sub_BF803D28+E0↓j ...
				LDMFD		   SP!, {R4-R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF803DBC:							@ CODE XREF: sub_BF803D28+30↑j
				LDR			 R3, =0x4C
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x68
				LDRB			R0, [R2]
				LDR			 R2, [R10,R3]
				LDRB			R1, [R2]
				CMP			 R0, R1
				BEQ			 loc_BF803E18
				LDR			 R6, =0xBC
				MOV			 R5, #2

loc_BF803DE4:							@ CODE XREF: sub_BF803D28+E8↓j
				LDR			 R3, [R10,R6]
				LDRB			R0, [R3]
				BL			  sub_BF801E68
				SUBS			R4, R0, #0
				BEQ			 loc_BF803E38
				BL			  sub_BF803C30
				SUB			 R3, R5, #1
				CMP			 R0, #0
				AND			 R5, R3, #0xFF
				BNE			 loc_BF803DB4
				CMP			 R5, #0xFF
				BNE			 loc_BF803DE4
				B			   loc_BF803DB0
@ ---------------------------------------------------------------------------

loc_BF803E18:							@ CODE XREF: sub_BF803D28+B0↑j
				BL			  sub_BF801888

loc_BF803E1C:							@ CODE XREF: sub_BF803D28+1C↑j
				MOV			 R0, #0
				B			   loc_BF803DB4
@ ---------------------------------------------------------------------------

loc_BF803E24:							@ CODE XREF: sub_BF803D28+68↑j
				LDRB			R0, [R7]
				BL			  sub_BF803BB8
				BL			  sub_BF801888
				MOV			 R0, R5
				B			   loc_BF803DB4
@ ---------------------------------------------------------------------------

loc_BF803E38:							@ CODE XREF: sub_BF803D28+CC↑j
				BL			  sub_BF801888
				MOV			 R0, R4
				B			   loc_BF803DB4
@ End of function sub_BF803D28

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF803E5C:
				STMFD		   SP!, {R4-R8,R10,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF803E70)
				LDR			 R3, =0x20
				ADD			 R10, PC, R10 @ dword_BF804C08
				LDR			 R2, [R10,R3]
				MOV			 R0, R0,LSR#9
				LDR			 R1, [R2]
				LDR			 R2, =0x7C
				ADD			 R3, R0, #1
				CMP			 R3, R1
				LDR			 R3, [R10,R2]
				SUB			 SP, SP, #4
				STR			 R0, [R3]
				BHI			 loc_BF803F84
				LDR			 R3, =0xA0
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x68
				LDR			 R1, [R2]
				LDR			 R8, [R10,R3]
				LDR			 R3, =0xBC
				ADD			 R6, R0, R1
				LDR			 R2, [R10,R3]
				MOV			 R5, R6,LSR#2
				LDRB			R1, [R8]
				MOV			 R0, R5
				LDRB			R7, [R2]
				BL			  sub_BF804A7C
				MOV			 R3, #0x84000003
				ADD			 R3, R3, #0x210000
				MOV			 R1, R0,LSR#1
				ADD			 R3, R3, #0x840
				UMULL		   R12, R2, R3, R1
				LDR			 R12, =0x40
				LDR			 R12, [R10,R12]
				MOV			 R2, R2,LSR#4
				MOV			 R3, R2,LSL#16
				LDRH			R1, [R12]
				MOV			 R3, R3,LSR#16
				RSB			 R2, R2, R2,LSL#5
				SUB			 R0, R0, R2,LSL#1
				CMP			 R1, R3
				AND			 R4, R0, #0xFF
				STRNEH		  R3, [R12]
				BLNE			sub_BF802228
				LDR			 R3, =0xEC
				LDR			 R2, [R10,R3]
				LDRB			R1, [R2]
				CMP			 R1, R4
				BEQ			 loc_BF803F78

loc_BF803F20:							@ CODE XREF: sub_BF803E5C+124↓j
				LDR			 R3, =0xB8
				LDR			 R2, [R10,R3]
				LDR			 R1, [R2]
				LDRB			R0, [R4,R1]
				CMP			 R0, #0xFF
				BEQ			 loc_BF803F8C

loc_BF803F38:							@ CODE XREF: sub_BF803E5C+134↓j
				LDR			 R3, =0x28
				LDR			 R2, [R10,R3]
				LDR			 R1, [R2]
				LDRB			R2, [R8]
				LDR			 R0, [R1,R0,LSL#2]
				AND			 R3, R7, R5
				MLA			 R12, R0, R2, R3
				LDR			 R3, =0x58
				LDR			 R2, [R10,R3]
				AND			 R1, R6, #3
				LDR			 R0, [R2]
				ADD			 R1, R1, R12,LSL#2
				ADD			 R0, R0, R1,LSL#9

loc_BF803F6C:							@ CODE XREF: sub_BF803E5C+12C↓j
				ADD			 SP, SP, #4
				LDMFD		   SP!, {R4-R8,R10,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF803F78:							@ CODE XREF: sub_BF803E5C+C0↑j
				BL			  sub_BF803D28
				CMP			 R0, #0
				BEQ			 loc_BF803F20

loc_BF803F84:							@ CODE XREF: sub_BF803E5C+34↑j
				MOV			 R0, #0
				B			   loc_BF803F6C
@ ---------------------------------------------------------------------------

loc_BF803F8C:							@ CODE XREF: sub_BF803E5C+D8↑j
				BL			  sub_BF801814
				B			   loc_BF803F38
@ End of function sub_BF803E5C

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF803FC0:							@ CODE XREF: sub_BF804978+B8↓p

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF803FD4)
				LDR			 R1, =0x24
				ADD			 R10, PC, R10 @ dword_BF804C08
				SUB			 SP, SP, #0x14
				STR			 R1, [SP,#0xC]
				LDR			 R3, [R10,R1]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BEQ			 loc_BF804224
				LDR			 R3, =0x7C
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xA0
				LDR			 R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE0
				LDR			 R1, [R2]
				LDR			 R2, [R10,R3]
				ADD			 R12, R12, R1
				LDRB			LR, [R2]
				MOV			 R4, R12,LSR#2
				MOV			 LR, R4,LSR LR
				MOV			 R3, #0x84000003
				ADD			 R3, R3, #0x210000
				ADD			 R3, R3, #0x840
				MOV			 R2, LR,LSR#1
				UMULL		   R1, R0, R3, R2
				LDR			 R2, =0xB4
				LDR			 R3, =0xBC
				MOV			 R0, R0,LSR#4
				STR			 R2, [SP,#0x4]
				LDR			 R2, [R10,R3]
				MOV			 R3, R0,LSL#16
				MOV			 R7, R3,LSR#16
				LDR			 R1, =0x40
				LDR			 R3, [SP,#0x4]
				LDR			 R11, =0x80
				LDR			 R8, [R10,R1]
				LDR			 R5, [R10,R3]
				LDR			 R3, [R10,R11]
				AND			 R12, R12, #3
				LDRB			R6, [R2]
				STRB			R12, [R3]
				LDRH			R2, [R8]
				LDR			 R3, =0x2C
				LDR			 R9, =0x9C
				CMP			 R2, R7
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x84
				LDR			 R1, [R10,R9]
				RSB			 R0, R0, R0,LSL#5
				STR			 LR, [R2]
				LDR			 R2, [R10,R3]
				SUB			 R0, LR, R0,LSL#1
				AND			 R4, R4, R6
				STRB			R4, [R5]
				STRB			R0, [R1]
				STRH			R7, [R2]
				BEQ			 loc_BF8040C8
				STRH			R7, [R8]
				BL			  sub_BF802228

loc_BF8040B4:							@ CODE XREF: sub_BF803FC0+5BC↓j
				LDR			 R1, [SP,#0xC]
				LDR			 R3, [R10,R1]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BEQ			 loc_BF804238

loc_BF8040C8:							@ CODE XREF: sub_BF803FC0+E8↑j
				LDR			 R2, =0xEC
				LDR			 R4, [R10,R9]
				STR			 R2, [SP,#0x8]
				LDR			 R6, [R10,R2]
				LDRB			R2, [R4]
				LDRB			R3, [R6]
				CMP			 R2, R3
				BEQ			 loc_BF8042E0

loc_BF8040E8:							@ CODE XREF: sub_BF803FC0+340↓j
										@ sub_BF803FC0+3D4↓j
				BL			  sub_BF803D28
				CMP			 R0, #0
				BNE			 loc_BF804758
				BL			  sub_BF802004
				LDR			 R5, =0xE4
				LDR			 R3, [R10,R5]
				CMP			 R0, #0xFF
				STRB			R0, [R3]
				MOVEQ		   R4, #9
				BEQ			 loc_BF8042D0
				LDR			 R3, =0x54
				LDR			 R2, [R10,R3]
				LDR			 R1, [R2]
				MOV			 R3, #0xFFFFFFFA
				STRB			R3, [R0,R1]
				LDR			 R3, =0xB8
				LDRB			R0, [R4]
				LDR			 R2, [R10,R3]
				STRB			R0, [R6]
				LDR			 R8, =0x44
				LDR			 R1, [R2]
				LDR			 R3, [R10,R8]
				LDRB			R2, [R0,R1]
				LDR			 LR, =0xB4
				STR			 LR, [SP,#0x4]
				STRB			R2, [R3]
				LDR			 R3, [R10,R8]
				LDRB			R2, [R3]
				CMP			 R2, #0xFF
				LDR			 R12, =0x4C
				BEQ			 loc_BF804318

loc_BF804164:							@ CODE XREF: sub_BF803FC0+354↓j
										@ sub_BF803FC0+364↓j ...
				LDR			 R3, =0x4C
				LDR			 R2, =0xB4
				LDR			 R7, [R10,R3]
				LDR			 R6, [R10,R2]
				LDR			 R5, =0xE4
				MOV			 R4, #2

loc_BF80417C:							@ CODE XREF: sub_BF803FC0+1F8↓j
				LDR			 R3, [R10,R8]
				LDR			 R2, [R10,R5]
				LDRB			R0, [R3]
				LDRB			R1, [R2]
				LDRB			R3, [R6]
				LDRB			R2, [R7]
				BL			  sub_BF801EC8
				CMP			 R0, #0
				BEQ			 loc_BF8048B4
				BL			  sub_BF803C30
				SUB			 R3, R4, #1
				CMP			 R0, #0
				AND			 R4, R3, #0xFF
				BNE			 loc_BF804758
				CMP			 R4, #0xFF
				BNE			 loc_BF80417C

loc_BF8041BC:							@ CODE XREF: sub_BF803FC0+3B0↓j
										@ sub_BF803FC0+8FC↓j ...
				LDR			 R2, [R10,R11]
				LDRB			R3, [R2]
				CMP			 R3, #0
				RSB			 R3, R3, #4
				AND			 R6, R3, #0xFF
				BNE			 loc_BF804250
				LDR			 R7, =0xDC
				LDR			 R4, [R10,R7]
				LDR			 R3, [R4]
				CMP			 R6, R3
				BHI			 loc_BF804658

loc_BF8041E8:							@ CODE XREF: sub_BF803FC0+6A4↓j
										@ sub_BF803FC0+6C8↓j
				LDR			 R8, =0xD4

loc_BF8041EC:							@ CODE XREF: sub_BF803FC0+6E0↓j
										@ sub_BF803FC0+708↓j
				LDR			 R1, [SP,#0x8]
				LDR			 R3, [SP,#0x4]
				LDR			 R2, [R10,R1]
				LDR			 R12, [R10,R3]
				LDR			 R3, =0xE4
				LDRB			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x74
				LDRB			R0, [R2]
				LDR			 R2, [R10,R3]
				LDRB			R1, [R12]
				STRB			R4, [R2,#0xB]
				BL			  sub_BF8015E8
				B			   loc_BF80426C
@ ---------------------------------------------------------------------------

loc_BF804224:							@ CODE XREF: sub_BF803FC0+24↑j
				LDR			 R11, =0x80
				LDR			 R3, [R10,R11]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BEQ			 loc_BF8044F0

loc_BF804238:							@ CODE XREF: sub_BF803FC0+104↑j
										@ sub_BF803FC0+55C↓j ...
				LDR			 R3, [R10,R11]
				LDRB			R3, [R3]
				CMP			 R3, #0
				RSBNE		   R3, R3, #4
				ANDNE		   R6, R3, #0xFF
				BEQ			 loc_BF804374

loc_BF804250:							@ CODE XREF: sub_BF803FC0+210↑j
				LDR			 R1, [SP,#0xC]
				LDR			 R3, [R10,R1]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BNE			 loc_BF80439C
				LDR			 R8, =0xD4
				LDR			 R7, =0xDC

loc_BF80426C:							@ CODE XREF: sub_BF803FC0+260↑j
										@ sub_BF803FC0+4FC↓j ...
				LDR			 R4, [R10,R11]
				LDR			 R5, [R10,R8]
				LDRB			R3, [R4]
				MOV			 R0, #5
				STRB			R3, [R5]
				BL			  sub_BF80195C
				LDR			 R3, [R10,R7]
				LDR			 R1, [R3]
				CMP			 R1, #1
				BEQ			 loc_BF804580

loc_BF804294:							@ CODE XREF: sub_BF803FC0+5C4↓j
				LDR			 LR, [R10,R11]
				LDRB			R3, [LR]
				CMP			 R3, #3
				BEQ			 loc_BF804760
				LDR			 R2, [SP,#0xC]
				LDR			 R0, [R10,R7]
				LDR			 R12, [R10,R2]
				ADD			 R3, R3, #1
				MOV			 R2, #0
				SUB			 R1, R1, #1
				AND			 R3, R3, #3
				STR			 R1, [R0]
				STRB			R3, [LR]
				STRB			R2, [R12]
				MOV			 R4, R2

loc_BF8042D0:							@ CODE XREF: sub_BF803FC0+14C↑j
										@ sub_BF803FC0+694↓j ...
				MOV			 R0, R4
				ADD			 SP, SP, #0x14
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF8042E0:							@ CODE XREF: sub_BF803FC0+124↑j
										@ sub_BF803FC0+3D8↓j
				LDR			 R3, =0xB4
				LDR			 R12, =0x4C
				STR			 R3, [SP,#0x4]
				LDR			 R2, [R10,R3]
				LDR			 R1, [R10,R12]
				LDRB			R0, [R2]
				LDRB			R3, [R1]
				CMP			 R0, R3
				BCC			 loc_BF8040E8
				LDR			 R8, =0x44
				LDR			 R3, [R10,R8]
				LDRB			R2, [R3]
				CMP			 R2, #0xFF
				BNE			 loc_BF804164

loc_BF804318:							@ CODE XREF: sub_BF803FC0+1A0↑j
				LDR			 R1, [R10,R12]
				LDRB			R3, [R1]
				CMP			 R3, #0
				BNE			 loc_BF804164
				LDR			 R2, [SP,#0x4]
				LDR			 R3, [R10,R2]
				LDRB			R2, [R3]
				CMP			 R2, #0
				BEQ			 loc_BF804164
				MOV			 R5, R1
				MOV			 R4, #2

loc_BF804344:							@ CODE XREF: sub_BF803FC0+3AC↓j
				MOV			 R0, #0
				BL			  sub_BF801E68
				CMP			 R0, #0
				BEQ			 loc_BF8048F4
				BL			  sub_BF803C30
				SUB			 R3, R4, #1
				CMP			 R0, #0
				AND			 R4, R3, #0xFF
				BNE			 loc_BF804758
				CMP			 R4, #0xFF
				BNE			 loc_BF804344
				B			   loc_BF8041BC
@ ---------------------------------------------------------------------------

loc_BF804374:							@ CODE XREF: sub_BF803FC0+28C↑j
				LDR			 R2, =0xEC
				LDR			 R9, =0x9C
				STR			 R2, [SP,#0x8]
				LDR			 R6, [R10,R2]
				LDR			 R4, [R10,R9]
				LDRB			R3, [R6]
				LDRB			R2, [R4]
				CMP			 R2, R3
				BNE			 loc_BF8040E8
				B			   loc_BF8042E0
@ ---------------------------------------------------------------------------

loc_BF80439C:							@ CODE XREF: sub_BF803FC0+2A0↑j
				LDR			 R3, =0x44
				LDR			 R2, [R10,R3]
				LDRB			R0, [R2]
				CMP			 R0, #0xFF
				BEQ			 loc_BF8047EC
				LDR			 R2, =0xB4
				STR			 R2, [SP,#0x4]
				LDR			 R4, [R10,R2]
				LDRB			R1, [R4]
				BL			  sub_BF8015E8
				MOV			 R0, #1
				BL			  sub_BF80195C
				BL			  sub_BF801694
				CMP			 R0, #0
				BNE			 loc_BF8046CC
				LDR			 R2, [R10,R11]
				LDR			 R8, =0xD4
				LDRB			R1, [R2]
				LDR			 R3, [R10,R8]
				CMP			 R1, #0
				STRB			R0, [R3]
				BEQ			 loc_BF804420
				MOV			 R4, R3
				MOV			 R5, R2

loc_BF8043FC:							@ CODE XREF: sub_BF803FC0+45C↓j
				MOV			 R0, #3
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				LDRB			R1, [R5]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R1, R3
				STRB			R3, [R4]
				BHI			 loc_BF8043FC

loc_BF804420:							@ CODE XREF: sub_BF803FC0+430↑j
				LDR			 R7, =0xDC
				LDR			 R3, [R10,R7]
				LDR			 R2, [R3]
				CMP			 R6, R2
				BLS			 loc_BF804470
				ADD			 R3, R1, #1
				AND			 R3, R3, #0xFF
				LDR			 R2, [R10,R8]
				CMP			 R3, #3
				STRB			R3, [R2]
				BHI			 loc_BF804470
				MOV			 R4, R2

loc_BF804450:							@ CODE XREF: sub_BF803FC0+4AC↓j
				MOV			 R0, #3
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R3, #3
				STRB			R3, [R4]
				BLS			 loc_BF804450

loc_BF804470:							@ CODE XREF: sub_BF803FC0+470↑j
										@ sub_BF803FC0+488↑j
				LDR			 R3, [SP,#0x4]
				LDR			 R12, [R10,R3]
				LDR			 R3, =0xEC
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE4
				LDRB			R4, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x74
				LDRB			R0, [R2]
				LDR			 R2, [R10,R3]
				LDRB			R1, [R12]
				STRB			R4, [R2,#0xB]
				BL			  sub_BF8015E8
				LDR			 R0, [R10,R11]
				LDRB			R2, [R0]
				LDR			 R1, [R10,R8]
				MOV			 R3, #0
				CMP			 R2, #0
				STRB			R3, [R1]
				BEQ			 loc_BF80426C
				MOV			 R4, R1
				MOV			 R5, R0

loc_BF8044C8:							@ CODE XREF: sub_BF803FC0+528↓j
				MOV			 R0, #6
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				LDRB			R2, [R5]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R2, R3
				STRB			R3, [R4]
				BHI			 loc_BF8044C8
				B			   loc_BF80426C
@ ---------------------------------------------------------------------------

loc_BF8044F0:							@ CODE XREF: sub_BF803FC0+274↑j
				LDR			 LR, =0xB4
				LDR			 R2, =0xBC
				STR			 LR, [SP,#0x4]
				LDR			 R12, [R10,LR]
				LDR			 R1, [R10,R2]
				LDRB			R3, [R12]
				LDRB			R0, [R1]
				ADD			 R3, R3, #1
				AND			 R3, R3, R0
				CMP			 R3, #0
				STRB			R3, [R12]
				BNE			 loc_BF804238
				LDR			 R9, =0x9C
				LDR			 R12, [R10,R9]
				LDRB			R2, [R12]
				MOV			 R1, #0x84000003
				ADD			 R2, R2, #1
				AND			 R2, R2, #0xFF
				ADD			 R1, R1, #0x210000
				MOV			 R0, R2,LSR#1
				ADD			 R1, R1, #0x840
				UMULL		   LR, R3, R1, R0
				MOV			 R3, R3,LSR#4
				RSB			 R3, R3, R3,LSL#5
				SUB			 R2, R2, R3,LSL#1
				AND			 R2, R2, #0xFF
				CMP			 R2, #0
				STRB			R2, [R12]
				BNE			 loc_BF804238
				LDR			 R3, =0x40
				LDR			 R1, [R10,R3]
				LDRH			R2, [R1]
				ADD			 R2, R2, #1
				STRH			R2, [R1]
				BL			  sub_BF802228
				B			   loc_BF8040B4
@ ---------------------------------------------------------------------------

loc_BF804580:							@ CODE XREF: sub_BF803FC0+2D0↑j
				CMP			 R6, #1
				BLS			 loc_BF804294
				LDR			 R8, =0x44
				LDR			 R3, [R10,R8]
				LDRB			R2, [R3]
				CMP			 R2, #0xFF
				BEQ			 loc_BF804874
				LDRB			R3, [R4]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R3, #3
				STRB			R3, [R5]
				BHI			 loc_BF8045D8
				MOV			 R4, R5

loc_BF8045B8:							@ CODE XREF: sub_BF803FC0+614↓j
				MOV			 R0, #6
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R3, #3
				STRB			R3, [R4]
				BLS			 loc_BF8045B8

loc_BF8045D8:							@ CODE XREF: sub_BF803FC0+5F0↑j
										@ sub_BF803FC0+8C8↓j ...
				BL			  sub_BF80164C
				MOV			 R0, #4
				BL			  sub_BF80195C
				BL			  sub_BF80191C
				CMP			 R0, #0
				BNE			 loc_BF80474C

loc_BF8045F0:							@ CODE XREF: sub_BF803FC0+794↓j
				LDR			 R3, =0x4C
				LDR			 R12, [R10,R3]
				LDR			 R3, =0x68
				LDRB			R2, [R12]
				LDR			 R1, [R10,R3]
				ADD			 R2, R2, #1
				LDRB			R0, [R1]
				AND			 R2, R2, #0xFF
				CMP			 R0, R2
				STRB			R2, [R12]
				BEQ			 loc_BF8048C0

loc_BF80461C:							@ CODE XREF: sub_BF803FC0+914↓j
				LDR			 R12, [R10,R11]
				LDR			 R0, [R10,R7]
				LDR			 R1, [SP,#0xC]
				LDRB			R3, [R12]
				LDR			 R2, [R0]
				LDR			 LR, [R10,R1]
				ADD			 R3, R3, #1
				MOV			 R1, #0
				SUB			 R2, R2, #1
				AND			 R3, R3, #3
				MOV			 R4, R1
				STR			 R2, [R0]
				STRB			R3, [R12]
				STRB			R1, [LR]
				B			   loc_BF8042D0
@ ---------------------------------------------------------------------------

loc_BF804658:							@ CODE XREF: sub_BF803FC0+224↑j
				LDR			 R3, [R10,R8]
				LDRB			R0, [R3]
				CMP			 R0, #0xFF
				BEQ			 loc_BF8041E8
				LDR			 LR, [SP,#0x4]
				LDR			 R3, [R10,LR]
				LDRB			R1, [R3]
				BL			  sub_BF8015E8
				MOV			 R0, #1
				BL			  sub_BF80195C
				BL			  sub_BF801694
				CMP			 R0, #0
				BNE			 loc_BF8041E8
				LDR			 R8, =0xD4
				LDRB			R3, [R4]
				LDR			 R2, [R10,R8]
				CMP			 R3, #3
				STRB			R3, [R2]
				BHI			 loc_BF8041EC
				MOV			 R4, R2

loc_BF8046A8:							@ CODE XREF: sub_BF803FC0+704↓j
				MOV			 R0, #3
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R3, #3
				STRB			R3, [R4]
				BLS			 loc_BF8046A8
				B			   loc_BF8041EC
@ ---------------------------------------------------------------------------

loc_BF8046CC:							@ CODE XREF: sub_BF803FC0+414↑j
				LDR			 R3, =0xEC
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE4
				LDRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x74
				LDRB			R0, [R2]
				LDR			 R2, [R10,R3]
				LDRB			R1, [R4]
				STRB			R12, [R2,#0xB]
				BL			  sub_BF8015E8
				LDR			 R0, [R10,R11]
				LDR			 R8, =0xD4
				LDRB			R2, [R0]
				LDR			 R1, [R10,R8]
				MOV			 R3, #0
				CMP			 R2, #0
				STRB			R3, [R1]
				BEQ			 loc_BF804744
				MOV			 R4, R1
				MOV			 R5, R0

loc_BF804720:							@ CODE XREF: sub_BF803FC0+780↓j
				MOV			 R0, #9
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				LDRB			R2, [R5]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R2, R3
				STRB			R3, [R4]
				BHI			 loc_BF804720

loc_BF804744:							@ CODE XREF: sub_BF803FC0+754↑j
										@ sub_BF803FC0+87C↓j
				LDR			 R7, =0xDC
				B			   loc_BF80426C
@ ---------------------------------------------------------------------------

loc_BF80474C:							@ CODE XREF: sub_BF803FC0+62C↑j
				BL			  sub_BF803C30
				CMP			 R0, #0
				BEQ			 loc_BF8045F0

loc_BF804758:							@ CODE XREF: sub_BF803FC0+130↑j
										@ sub_BF803FC0+1F0↑j ...
				MOV			 R4, R0
				B			   loc_BF8042D0
@ ---------------------------------------------------------------------------

loc_BF804760:							@ CODE XREF: sub_BF803FC0+2E0↑j
				BL			  sub_BF80164C
				MOV			 R0, #4
				BL			  sub_BF80195C
				BL			  sub_BF80191C
				CMP			 R0, #0
				MOVEQ		   R4, R0
				BNE			 loc_BF804908

loc_BF80477C:							@ CODE XREF: sub_BF803FC0+954↓j
				LDR			 R3, =0x4C
				LDR			 R12, [R10,R3]
				LDR			 R3, =0x68
				LDRB			R2, [R12]
				LDR			 R1, [R10,R3]
				ADD			 R2, R2, #1
				LDRB			R0, [R1]
				AND			 R2, R2, #0xFF
				CMP			 R0, R2
				STRB			R2, [R12]
				BEQ			 loc_BF8048D8

loc_BF8047A8:							@ CODE XREF: sub_BF803FC0+930↓j
				LDR			 LR, [R10,R11]
				LDR			 R0, [R10,R7]
				LDR			 R3, [SP,#0xC]
				LDRB			R1, [LR]
				LDR			 R2, [R0]
				LDR			 R12, [R10,R3]
				CMP			 R4, #0
				ADD			 R1, R1, #1
				SUB			 R2, R2, #1
				AND			 R1, R1, #3
				MOV			 R3, #0
				MOVNE		   R4, #0x10
				MOVEQ		   R4, #0
				STR			 R2, [R0]
				STRB			R3, [R12]
				STRB			R1, [LR]
				B			   loc_BF8042D0
@ ---------------------------------------------------------------------------

loc_BF8047EC:							@ CODE XREF: sub_BF803FC0+3EC↑j
				LDR			 R3, =0xEC
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xE4
				LDRB			R12, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0xB4
				LDRB			R0, [R2]
				LDR			 R2, [R10,R3]
				LDR			 R3, =0x74
				LDRB			R1, [R2]
				LDR			 R2, [R10,R3]
				STRB			R12, [R2,#0xB]
				BL			  sub_BF8015E8
				LDR			 R0, [R10,R11]
				LDR			 R8, =0xD4
				LDRB			R2, [R0]
				LDR			 R1, [R10,R8]
				MOV			 R3, #0
				CMP			 R2, #0
				STRB			R3, [R1]
				BEQ			 loc_BF804744
				MOV			 R4, R1
				MOV			 R5, R0

loc_BF804848:							@ CODE XREF: sub_BF803FC0+8A8↓j
				MOV			 R0, #9
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				LDRB			R2, [R5]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R2, R3
				STRB			R3, [R4]
				BHI			 loc_BF804848
				LDR			 R7, =0xDC
				B			   loc_BF80426C
@ ---------------------------------------------------------------------------

loc_BF804874:							@ CODE XREF: sub_BF803FC0+5D8↑j
				LDRB			R3, [R4]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R3, #3
				STRB			R3, [R5]
				BHI			 loc_BF8045D8
				MOV			 R4, R5

loc_BF804890:							@ CODE XREF: sub_BF803FC0+8EC↓j
				MOV			 R0, #9
				BL			  sub_BF80195C
				LDRB			R3, [R4]
				ADD			 R3, R3, #1
				AND			 R3, R3, #0xFF
				CMP			 R3, #3
				STRB			R3, [R4]
				BLS			 loc_BF804890
				B			   loc_BF8045D8
@ ---------------------------------------------------------------------------

loc_BF8048B4:							@ CODE XREF: sub_BF803FC0+1DC↑j
				LDRB			R3, [R6]
				STRB			R3, [R7]
				B			   loc_BF8041BC
@ ---------------------------------------------------------------------------

loc_BF8048C0:							@ CODE XREF: sub_BF803FC0+658↑j
				LDR			 R3, [R10,R8]
				LDRB			R0, [R3]
				CMP			 R0, #0xFF
				BLNE			sub_BF803BB8
				BL			  sub_BF801888
				B			   loc_BF80461C
@ ---------------------------------------------------------------------------

loc_BF8048D8:							@ CODE XREF: sub_BF803FC0+7E4↑j
				LDR			 R3, =0x44
				LDR			 R2, [R10,R3]
				LDRB			R0, [R2]
				CMP			 R0, #0xFF
				BLNE			sub_BF803BB8
				BL			  sub_BF801888
				B			   loc_BF8047A8
@ ---------------------------------------------------------------------------

loc_BF8048F4:							@ CODE XREF: sub_BF803FC0+390↑j
				LDR			 LR, [SP,#0x4]
				LDR			 R3, [R10,LR]
				LDRB			R2, [R3]
				STRB			R2, [R5]
				B			   loc_BF8041BC
@ ---------------------------------------------------------------------------

loc_BF804908:							@ CODE XREF: sub_BF803FC0+7B8↑j
				BL			  sub_BF803C30
				CMP			 R0, #0
				MOVEQ		   R4, #1
				BEQ			 loc_BF80477C
				MOV			 R4, R0
				B			   loc_BF8042D0
@ End of function sub_BF803FC0

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF804978:							@ CODE XREF: sub_BF80134C+18↑p

				STMFD		   SP!, {R4-R11,LR}
				LDR			 R10, =(dword_BF804C08 - 0xBF80498C)
				LDR			 R3, =0x7C
				ADD			 R10, PC, R10 @ dword_BF804C08
				SUB			 SP, SP, #0x14
				LDR			 R12, =0x20
				STR			 R3, [SP,#0x8]
				LDR			 R3, [R10,R3]
				LDR			 LR, [R3]
				LDR			 R3, [R10,R12]
				LDR			 R4, [R3]
				ADD			 LR, LR, #1
				CMP			 LR, R4
				MOV			 R11, R1
				MOV			 R12, R2
				BHI			 loc_BF804A54
				LDR			 R3, =0x64
				LDR			 R2, [R10,R3]
				MOV			 R1, #0xFFFFFFFF
				STR			 R1, [R2]
				LDR			 R3, =0x24
				LDR			 R2, =0x18
				LDR			 R3, [R10,R3]
				LDR			 R2, [R10,R2]
				LDR			 R1, =0xDC
				STR			 R3, [SP,#0xC]
				STR			 R2, [SP]
				STR			 R1, [SP,#0x4]
				MOV			 R9, R0,LSR#9
				MOV			 R7, R12,LSR#9

loc_BF8049F0:							@ CODE XREF: sub_BF804978+D8↓j
				LDR			 R2, [SP,#0x8]
				LDR			 R12, [SP,#0x4]
				LDR			 R3, [R10,R2]
				LDR			 R1, [SP,#0xC]
				LDR			 R2, [R10,R12]
				STR			 R9, [R3]
				CMP			 R7, #0
				MOV			 R3, #1
				STR			 R7, [R2]
				STRB			R3, [R1]
				BEQ			 loc_BF804A54
				MOV			 R6, #0
				LDR			 R8, [SP]
				MOV			 R4, R11
				MOV			 R5, R6

loc_BF804A2C:							@ CODE XREF: sub_BF804978+D0↓j
				STR			 R4, [R8]
				BL			  sub_BF803FC0
				ADD			 R5, R5, #1
				CMP			 R0, #0x10
				MOVEQ		   R6, #1
				CMP			 R5, R7
				ADD			 R4, R4, #0x200
				BNE			 loc_BF804A2C
				CMP			 R6, #0
				BNE			 loc_BF8049F0

loc_BF804A54:							@ CODE XREF: sub_BF804978+3C↑j
										@ sub_BF804978+A0↑j
				ADD			 SP, SP, #0x14
				LDMFD		   SP!, {R4-R11,LR}
				BX			  LR
@ End of function sub_BF804978

@ ---------------------------------------------------------------------------
.pool

@ =============== S U B R O U T I N E =======================================


sub_BF804A7C:							@ CODE XREF: sub_BF801CB8+D0↑p
										@ sub_BF802F58+6C↑p ...

				SUBS			R2, R1, #1
				BXEQ			LR
				BCC			 loc_BF804B64
				CMP			 R0, R1
				BLS			 loc_BF804B1C
				TST			 R1, R2
				BEQ			 loc_BF804B28
				TST			 R1, #0xE0000000
				MOVEQ		   R1, R1,LSL#3
				MOVEQ		   R3, #8
				MOVNE		   R3, #1

loc_BF804AA8:							@ CODE XREF: sub_BF804A7C+3C↓j
				CMP			 R1, #0x10000000
				CMPCC		   R1, R0
				MOVCC		   R1, R1,LSL#4
				MOVCC		   R3, R3,LSL#4
				BCC			 loc_BF804AA8

loc_BF804ABC:							@ CODE XREF: sub_BF804A7C+50↓j
				CMP			 R1, #0x80000000
				CMPCC		   R1, R0
				MOVCC		   R1, R1,LSL#1
				MOVCC		   R3, R3,LSL#1
				BCC			 loc_BF804ABC
				MOV			 R2, #0

loc_BF804AD4:							@ CODE XREF: sub_BF804A7C+94↓j
				CMP			 R0, R1
				SUBCS		   R0, R0, R1
				ORRCS		   R2, R2, R3
				CMP			 R0, R1,LSR#1
				SUBCS		   R0, R0, R1,LSR#1
				ORRCS		   R2, R2, R3,LSR#1
				CMP			 R0, R1,LSR#2
				SUBCS		   R0, R0, R1,LSR#2
				ORRCS		   R2, R2, R3,LSR#2
				CMP			 R0, R1,LSR#3
				SUBCS		   R0, R0, R1,LSR#3
				ORRCS		   R2, R2, R3,LSR#3
				CMP			 R0, #0
				MOVNES		  R3, R3,LSR#4
				MOVNE		   R1, R1,LSR#4
				BNE			 loc_BF804AD4
				MOV			 R0, R2
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF804B1C:							@ CODE XREF: sub_BF804A7C+10↑j
				MOVEQ		   R0, #1
				MOVNE		   R0, #0
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF804B28:							@ CODE XREF: sub_BF804A7C+18↑j
				CMP			 R1, #0x10000
				MOVCS		   R1, R1,LSR#16
				MOVCS		   R2, #0x10
				MOVCC		   R2, #0
				CMP			 R1, #0x100
				MOVCS		   R1, R1,LSR#8
				ADDCS		   R2, R2, #8
				CMP			 R1, #0x10
				MOVCS		   R1, R1,LSR#4
				ADDCS		   R2, R2, #4
				CMP			 R1, #4
				ADDHI		   R2, R2, #3
				ADDLS		   R2, R2, R1,LSR#1
				MOV			 R0, R0,LSR R2
				BX			  LR
@ ---------------------------------------------------------------------------

loc_BF804B64:							@ CODE XREF: sub_BF804A7C+8↑j
				STR			 LR, [SP,#-8]!
				BL			  nullsub_1
				MOV			 R0, #0
				LDR			 LR, [SP],#8
				BX			  LR
@ End of function sub_BF804A7C

@ ---------------------------------------------------------------------------
				STMFD		   SP!, {R0,R1,LR}
				BL			  sub_BF804A7C
				LDMFD		   SP!, {R1,R2,LR}
				MUL			 R3, R2, R0
				SUB			 R1, R1, R3
				BX			  LR
@ ---------------------------------------------------------------------------
nullsub_1:
				BX			  LR
@ ---------------------------------------------------------------------------
				MOV			 R12, SP
				STMFD		   SP!, {R3-R12,LR,PC}
				SUB			 R11, R12, #4
				SUB			 SP, R11, #0x28
				LDMFD		   SP, {R4-R11,SP,LR}
				BX			  LR
@ ---------------------------------------------------------------------------
aMftlSectors:	.ascii "mftl_sectors"
.word 0
aMftlSpare:		.ascii "mftl_spare"
.hword 0
aMftlBlocktab:	.ascii "mftl_blocktab"
.hword 0
.byte 0
aMftlFreetab:	.ascii "mftl_freetab"
.word 0
aMftlFreeptr:	.ascii "mftl_freeptr"
.word 0
				.word sub_BF8000DC
				.word sub_BF800098
dword_BF804C00:	.word 0				   @ DATA XREF: sub_BF800098+1C↑o
										@ RAM:off_BF8000D8↑o ...
dword_BF804C04:	.word 0				   @ DATA XREF: sub_BF8000DC+20↑o
										@ sub_BF8000DC+24↑r ...
dword_BF804C08:	.word 0				   @ DATA XREF: RAM:BF800050↑o
										@ sub_BF8005B0+C↑o ...
				.word 0
				.word 0
				.word unk_BF804D2C
				.word unk_BF804D30
				.word unk_BF804D34
				.word unk_BF804D38
				.word unk_BF804D3C
				.word unk_BF804D7C
				.word unk_BF804D80
				.word unk_BF804D84
				.word unk_BF804D88
				.word unk_BF804D8C
				.word unk_BF804D90
				.word unk_BF804D94
				.word unk_BF804D98
				.word unk_BF804D9C
				.word unk_BF804DA0
				.word unk_BF804DA4
				.word unk_BF804DE4
				.word unk_BF804DE8
				.word unk_BF804DEC
				.word unk_BF804DF0
				.word unk_BF804DF4
				.word unk_BF804DF8
				.word dword_BF804D08
				.word unk_BF804DFC
				.word unk_BF804E00
				.word unk_BF804F00
				.word unk_BF805000
				.word unk_BF805040
				.word unk_BF805044
				.word unk_BF805048
				.word unk_BF80504C
				.word unk_BF805050
				.word unk_BF805150
				.word unk_BF805154
				.word unk_BF805158
				.word unk_BF805258
				.word unk_BF80525C
				.word unk_BF805260
				.word unk_BF805264
				.word unk_BF805268
				.word unk_BF8052A8
				.word unk_BF805AA8
				.word unk_BF805AE8
				.word unk_BF805AEC
				.word unk_BF805AF0
				.word unk_BF805AF4
				.word unk_BF804D28
				.word unk_BF805AF8
				.word unk_BF805AFC
				.word unk_BF805B3C
				.word unk_BF805B40
				.word unk_BF805B44
				.word unk_BF805B84
				.word unk_BF805B88
				.word unk_BF805B8C
				.word unk_BF805B90
				.word unk_BF805B94
				.word unk_BF805B98
				.word unk_BF805B9C
				.word unk_BF805BA0
gotEnd: