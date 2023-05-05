/*
    Ace3DS+
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <nds/ndstypes.h>

#ifndef NULL
 #define NULL 0
#endif

// Ace3DS+ defines
// R4 ROMCTRL flags
#define IOA3P_CTRL_BASE     MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY2(24)
#define IOA3P_CTRL_GENERAL  IOA3P_CTRL_BASE | MCCNT1_LEN_4
#define IOA3P_CTRL_SD_READ  IOA3P_CTRL_BASE | MCCNT1_LEN_512
#define IOA3P_CTRL_SD_WRITE IOA3P_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_512

// Ace3DS+ CARD_COMMANDs
#define IOA3P_CMD_SD_READ_REQUEST   (0xB9ull << 56)
#define IOA3P_CMD_SD_READ_DATA      (0xBAull << 56)
#define IOA3P_CMD_SD_WRITE_START    (0xBBull << 56)
#define IOA3P_CMD_SD_WRITE_STAT     (0xBCull << 56)

void ioA3PSDReadSector(u32 sector, void* buffer);
void ioA3PSDWriteSector(u32 sector, const void* buffer);
