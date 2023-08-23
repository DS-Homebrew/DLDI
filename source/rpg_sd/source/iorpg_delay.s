@
@ Copyright (C) 2007 Acekard, www.acekard.com
@
@ SPDX-License-Identifier: MIT
@

#include <nds/asminc.h>

    .syntax unified
    .arm

@ void ioRPG_Delay(u32 count)

BEGIN_ASM_FUNC ioRPG_Delay
    subs r0, r0, #1
    bgt ioRPG_Delay
    bx lr
