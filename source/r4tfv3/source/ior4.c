/*
    R4(DS) - Revolution for DS (v3)
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "ior4.h"
#include "card.h"

void ioR4ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuReadUnaligned((u8 *)buffer, length);
    else
        card_romCpuRead(buffer, length);
}

// currently static because nothing else uses it except ioR4SDWriteSector
static inline void ioR4WriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuWriteUnaligned((u8 *)buffer, length);
    else
        card_romCpuWrite(buffer, length);
}

void ioR4SDReadSector(u32 sector, void *buffer)
{
    u32 ret;
    // wait until data is ready
    // request should return 0 when ready to access
    do
    {
        ioR4ReadCardData(IOR4_CMD_SD_READ_REQUEST(sector), IOR4_CTRL_GENERAL, &ret, 1);
    } while (ret);

    ioR4ReadCardData(IOR4_CMD_SD_READ_DATA, IOR4_CTRL_SD_READ, buffer, 128);
}

void ioR4SDWriteSector(u32 sector, const void *buffer)
{
    u32 ret;

    ioR4WriteCardData(IOR4_CMD_SD_WRITE_START(sector), IOR4_CTRL_SD_WRITE, buffer, 128);

    // Wait until write finishes
    // status should return 0 when done
    do
    {
        ioR4ReadCardData(IOR4_CMD_SD_WRITE_STAT(sector), IOR4_CTRL_GENERAL, &ret, 1);
    } while (ret);
}

// non-DLDI functions follow
#ifndef DLDI
void ioR4SendFATEntry(u32 address)
{
    u32 ret;
    do
    {
        ioR4ReadCardData(IOR4_CMD_FAT_ENTRY_SEND(address), IOR4_CTRL_GENERAL, &ret, 1);
    } while (ret);
}
#endif
