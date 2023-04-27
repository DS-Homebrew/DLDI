@
@ Copyright (C) 2007 Acekard, www.acekard.com
@
@ SPDX-License-Identifier: MIT
@

#include <nds/asminc.h>

    .syntax unified
    .thumb

@ void ioRpgDelay(u32 us)

BEGIN_ASM_FUNC ioRpgDelay
    subs r0, r0, #1
    bgt ioRpgDelay
    bx lr
