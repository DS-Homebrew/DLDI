/*
    This file is a part of libtwl (card.c)

    Copyright (C) 2023 Gericom

    SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "libtwl_card.h"

void card_romCpuRead(u32* dst, u32 words)
{
    u32* target = dst + words;
    do
    {
        // Read data if available
        if (card_romIsDataReady())
        {
            u32 data = card_romGetData();
            if (dst < target)
                *dst++ = data;
        }
    } while (card_romIsBusy());
}

void card_romCpuReadUnaligned(u8* dst, u32 words)
{
    u8* target = dst + (words << 2);
    do
    {
        // Read data if available
        if (card_romIsDataReady())
        {
            u32 data = card_romGetData();
            if (dst < target)
            {
                *dst++ = data & 0xFF;
                *dst++ = (data >> 8) & 0xFF;
                *dst++ = (data >> 16) & 0xFF;
                *dst++ = (data >> 24) & 0xFF;
            }
        }
    } while (card_romIsBusy());
}

void card_romCpuWrite(const u32* src, u32 words)
{
    u32 data = 0;
    const u32* target = src + words;
    do
    {
        // Write data if ready
        if (card_romIsDataReady())
        {
            if (src < target)
                data = *src++;
            card_romSetData(data);
        }
    } while (card_romIsBusy());
}

void card_romCpuWriteUnaligned(const u8* src, u32 words)
{
    u32 data = 0;
    const u8* target = src + (words << 2);
    do
    {
        // Write data if ready
        if (card_romIsDataReady())
        {
            if (src < target)
            {
                data = src[0] | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
                src += 4;
            }
            card_romSetData(data);
        }
    } while (card_romIsBusy());
}
