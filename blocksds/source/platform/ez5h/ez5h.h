/*
    EZ-Flash V
    Card IO routines

    Copyright (C) 2023-2024 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#ifndef NULL
#define NULL 0
#endif

// EZ5 defines
// EZ5 ROMCTRL flags
#define EZ5H_CTRL_BASE                                                                 \
    (MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | \
     MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY2(24) | MCCNT1_LATENCY1(0))
#define EZ5H_CTRL_READ_0 (EZ5H_CTRL_BASE | MCCNT1_LEN_0)
#define EZ5H_CTRL_READ_4B (EZ5H_CTRL_BASE | MCCNT1_LEN_4)
#define EZ5H_CTRL_READ_512B (EZ5H_CTRL_BASE | MCCNT1_LEN_512)

// EZ5 CARD_COMMANDs
#define EZ5H_CMD_SRAM (0xB700000000000000ull)
#define EZ5H_CMD_SDMC (0xB800000000000000ull)
#define EZ5H_CMD_SDMC_READ_DATA (EZ5H_CMD_SDMC | 0x00F7000000000000ull)
#define EZ5H_CMD_SDMC_SEND_CRC_STATUS (EZ5H_CMD_SDMC | 0x00F8000000000000ull)

static inline u64 EZ5H_CMD_SDMC_PARAM_CARD(u8 idx, u8 cmd, u32 parameter) {
    return (EZ5H_CMD_SDMC | 0x00FA000000000000ull | ((u64)idx << 40) | ((u64)cmd << 32) |
            (u64)parameter);
}

static inline u64 EZ5H_CMD_SDMC_SDIO(u8 cmd, u32 parameter) {
    return EZ5H_CMD_SDMC_PARAM_CARD(0, cmd | 0x40, parameter);
}

// Sends a clock, reads data from response index if available
static inline u64 EZ5H_CMD_SDMC_SEND_CLK(u8 idx) {
    return EZ5H_CMD_SDMC_PARAM_CARD(idx, 0, 0);
}

static inline u64 EZ5H_CMD_SRAM_READ_DATA(u32 address) {
    return (EZ5H_CMD_SRAM | (1ull << 48) | ((u64)((address + 0x80000) & 0xFFFFFF) << 24));
}

static inline u64 EZ5H_CMD_SDMC_WRITE_DATA(const u8* data) {
    u64 command = (EZ5H_CMD_SDMC | 0x00F6000000000000ull);
    command |= ((u64)((data[0] >> 4) | 0xF0) << 40) | ((u64)((data[0]) | 0xF0) << 32) |
               ((u64)((data[1] >> 4) | 0xF0) << 24) | ((u64)((data[1]) | 0xF0) << 16);
    return command;
}

u32 EZ5H_SRAMReadData(u32 address);
bool EZ5H_SDInitialize(void);
bool EZ5H_SDReadSector(u32 sector, void* buffer);
bool EZ5H_SDWriteSector(u32 sector, const u8* buffer);
