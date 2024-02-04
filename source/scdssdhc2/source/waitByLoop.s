@
@ Copyright (C) 2024 lifehackerhansol
@
@ SPDX-License-Identifier: zlib
@

#include <nds/asminc.h>

    .syntax unified
    .arm

@ void waitByLoop(u32 count)
@ because we can't use swi 0x3 in DLDI

BEGIN_ASM_FUNC waitByLoop
    subs r0, r0, #1
    bgt waitByLoop
    bx lr
