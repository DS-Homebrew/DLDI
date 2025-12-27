/*
    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

void cardExt_RomReadData(u64 command, u32 flags, void* buffer, u32 length) {
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuReadUnaligned((u8*)buffer, length);
    else
        card_romCpuRead(buffer, length);
}

void cardExt_RomWriteData(u64 command, u32 flags, const void* buffer, u32 length) {
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuWriteUnaligned((u8*)buffer, length);
    else
        card_romCpuWrite(buffer, length);
}

u32 cardExt_RomReadData4Byte(u64 command, u32 flags) {
    card_romSetCmd(command);
    card_romStartXfer(flags | MCCNT1_LEN_4, false);
    card_romWaitDataReady();
    return card_romGetData();
}

void cardExt_RomSendCommand(u64 command, u32 flags) {
    card_romSetCmd(command);
    card_romStartXfer(flags | MCCNT1_LEN_0, false);
    card_romWaitBusy();
}
