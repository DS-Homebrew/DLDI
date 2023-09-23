/*
    This file is a part of libtwl (card.h)

    Copyright (C) 2023 Gericom

    SPDX-License-Identifier: Zlib
*/

#pragma once

#define REG_MCCNT0 (*(vu16*)0x040001A0)
#define REG_MCD0   (*(vu16*)0x040001A2)
#define REG_MCCNT1 (*(vu32*)0x040001A4)
#define REG_MCCMD0 (*(vu32*)0x040001A8)
#define REG_MCCMD1 (*(vu32*)0x040001AC)
#define REG_MCSCR0 (*(vu32*)0x040001B0)
#define REG_MCSCR1 (*(vu32*)0x040001B4)
#define REG_MCSCR2 (*(vu32*)0x040001B8)
#define REG_MCD1   (*(vu32*)0x04100010)

// REG_MCCNT0

#define MCCNT0_SPI_RATE_4_19_MHZ 0
#define MCCNT0_SPI_RATE_2_09_MHZ 1
#define MCCNT0_SPI_RATE_1_05_MHZ 2
#define MCCNT0_SPI_RATE_524_KHZ  3

#define MCCNT0_SPI_HOLD_CS (1 << 6)
#define MCCNT0_SPI_BUSY    (1 << 7)

#define MCCNT0_MODE_MASK (1 << 13)
#define MCCNT0_MODE_ROM  (0 << 13)
#define MCCNT0_MODE_SPI  (1 << 13)

#define MCCNT0_ROM_XFER_IRQ (1 << 14)
#define MCCNT0_ENABLE       (1 << 15)

// REG_MCCNT1

#define MCCNT1_LATENCY1_SHIFT 0
#define MCCNT1_LATENCY1_MASK  0x1FFF
#define MCCNT1_LATENCY1(x)    (x)

#define MCCNT1_READ_DATA_DESCRAMBLE (1 << 13)
#define MCCNT1_CLOCK_SCRAMBLER      (1 << 14)
#define MCCNT1_APPLY_SCRAMBLE_SEED  (1 << 15)

#define MCCNT1_LATENCY2_SHIFT 16
#define MCCNT1_LATENCY2_MASK  0x3F0000
#define MCCNT1_LATENCY2(x)    (((x) << MCCNT1_LATENCY2_SHIFT) & MCCNT1_LATENCY2_MASK)

#define MCCNT1_CMD_SCRAMBLE (1 << 22)

#define MCCNT1_DATA_READY (1 << 23)

#define MCCNT1_LEN_0     (0 << 24)
#define MCCNT1_LEN_512   (1 << 24)
#define MCCNT1_LEN_1024  (2 << 24)
#define MCCNT1_LEN_2048  (3 << 24)
#define MCCNT1_LEN_4096  (4 << 24)
#define MCCNT1_LEN_8192  (5 << 24)
#define MCCNT1_LEN_16384 (6 << 24)
#define MCCNT1_LEN_4     (7 << 24)

#define MCCNT1_CLK_6_7_MHZ (0 << 27)
#define MCCNT1_CLK_4_2_MHZ (1 << 27)

#define MCCNT1_LATENCY_CLK (1 << 28)

#define MCCNT1_RESET_ON  (0 << 29)
#define MCCNT1_RESET_OFF (1 << 29)

#define MCCNT1_DIR_READ  (0 << 30)
#define MCCNT1_DIR_WRITE (1 << 30)

#define MCCNT1_ENABLE (1 << 31)

#ifdef __cplusplus
extern "C"
{
#endif

    static inline void card_romSetCmd(u64 cmd)
    {
        *(vu64*)&REG_MCCMD0 = __builtin_bswap64(cmd);
    }

    static inline bool card_romIsDataReady(void)
    {
        return REG_MCCNT1 & MCCNT1_DATA_READY;
    }

    static inline void card_romWaitDataReady(void)
    {
        while(!card_romIsDataReady());
    }

    static inline u32 card_romGetData(void)
    {
        return REG_MCD1;
    }

    static inline void card_romSetData(u32 data)
    {
        REG_MCD1 = data;
    }

    static inline bool card_romIsBusy(void)
    {
        return REG_MCCNT1 & MCCNT1_ENABLE;
    }

    static inline void card_romWaitBusy(void)
    {
        while(card_romIsBusy());
    }

    static inline void card_romStartXfer(u32 settings, bool irq)
    {
        REG_MCCNT0 = (REG_MCCNT0 & ~(MCCNT0_MODE_MASK | MCCNT0_ROM_XFER_IRQ)) | MCCNT0_MODE_ROM | (irq ? MCCNT0_ROM_XFER_IRQ : 0) | MCCNT0_ENABLE;
        REG_MCCNT1 = MCCNT1_ENABLE | settings;
    }

    void card_romCpuRead(u32* dst, u32 len);
    void card_romCpuReadUnaligned(u8* dst, u32 words);

    void card_romCpuWrite(const u32* src, u32 words);
    void card_romCpuWriteUnaligned(const u8* src, u32 words);

#ifdef __cplusplus
}
#endif
