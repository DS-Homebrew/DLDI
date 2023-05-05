/*
    Ace3DS+
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "ioa3p.h"
#include "card.h"

static inline void ioA3PReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuReadUnaligned((u8 *)buffer, length);
    else
        card_romCpuRead(buffer, length);
}

static inline void ioA3PWriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuWriteUnaligned((u8 *)buffer, length);
    else
        card_romCpuWrite(buffer, length);
}

void ioA3PSDReadSector(u32 sector, void *buffer)
{
    u32 ret;
    // wait until data is ready
    // request should return 0 when ready to access
    do
    {
        ioA3PReadCardData(IOA3P_CMD_SD_READ_REQUEST | ((u64)(sector) << 24), IOA3P_CTRL_GENERAL, &ret, 1);
    } while (ret);

    ioA3PReadCardData(IOA3P_CMD_SD_READ_DATA, IOA3P_CTRL_SD_READ, buffer, 128);
}

void ioA3PSDWriteSector(u32 sector, const void *buffer)
{
    u32 ret;
    u64 command = IOA3P_CMD_SD_WRITE_START | ((u64)(sector) << 24);
    ioA3PWriteCardData(command, IOA3P_CTRL_SD_WRITE, buffer, 128);

    // Wait until write finishes
    // status should return 0 when done
    do
    {
        command = IOA3P_CMD_SD_WRITE_STAT | ((u64)(sector) << 24);
        command |= ((command >> 56) ^ (command >> 48) ^ (command >> 40) ^ (command >> 32) ^ (command >> 24)) & 0xFF;
        ioA3PReadCardData(command, IOA3P_CTRL_GENERAL, &ret, 1);
    } while (ret);
}
