#include <nds/ndstypes.h>
#include "card.h"

#define DSPICO_CMD_REQUEST_SD_READ(sector)      (0xE300000000000000ull | (sector))
#define DSPICO_CMD_POLL_SD_READY                0xE400000000000000ull
#define DSPICO_CMD_GET_SD_DATA                  0xE500000000000000ull
#define DSPICO_CMD_WRITE_SD_DATA(sector, isFirst, isLast)\
    (0xF6E10D9800000000ull | ((isFirst ? 1ULL : 0ULL) << 33) | ((isLast ? 1ULL : 0ULL) << 32) | (sector))

__attribute__((noinline)) static void requestSdRead(u32 sector)
{
    card_romSetCmd(DSPICO_CMD_REQUEST_SD_READ(sector));
    card_romStartXfer(MCCNT1_DIR_READ | MCCNT1_RESET_OFF | MCCNT1_CLK_6_7_MHZ | MCCNT1_LEN_0 | MCCNT1_CMD_SCRAMBLE |
        MCCNT1_LATENCY2(0) | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY1(0), false);
    card_romWaitBusy();
}

__attribute__((noinline)) static u32 pollSdDataReady(void)
{
    u32 result;
    card_romSetCmd(DSPICO_CMD_POLL_SD_READY);
    card_romStartXfer(MCCNT1_DIR_READ | MCCNT1_RESET_OFF | MCCNT1_CLK_6_7_MHZ | MCCNT1_LEN_4 | MCCNT1_CMD_SCRAMBLE |
        MCCNT1_LATENCY2(4) | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY1(0), false);
    card_romCpuRead(&result, 1);
    return result;
}

__attribute__((noinline)) static void getSdData(u8* dst)
{
    card_romSetCmd(DSPICO_CMD_GET_SD_DATA);
    card_romStartXfer(MCCNT1_DIR_READ | MCCNT1_RESET_OFF | MCCNT1_CLK_6_7_MHZ | MCCNT1_LEN_512 | MCCNT1_CMD_SCRAMBLE |
        MCCNT1_LATENCY2(4) | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_LATENCY1(0), false);
    if ((u32)dst & 3)
    {
        card_romCpuReadUnaligned(dst, 128);
    }
    else
    {
        card_romCpuRead((u32*)dst, 128);
    }
}

static void writeSdData(u32 sector, const u8* src, bool isFirst, bool isLast)
{
    card_romSetCmd(DSPICO_CMD_WRITE_SD_DATA(sector, isFirst, isLast));
    card_romStartXfer(MCCNT1_DIR_WRITE | MCCNT1_RESET_OFF | MCCNT1_CLK_6_7_MHZ | MCCNT1_LEN_512 | MCCNT1_CMD_SCRAMBLE |
        MCCNT1_LATENCY2(8) | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY1(0), false);
    if ((u32)src & 3)
    {
        card_romCpuWriteUnaligned(src, 128);
    }
    else
    {
        card_romCpuWrite((const u32*)src, 128);
    }
}

bool dldi_startup(void)
{
    return true;
}

bool dldi_isInserted(void)
{
    return true;
}

bool dldi_clearStatus(void)
{
    return true;
}

bool dldi_readSectors(u32 sector, u32 numSectors, void* buffer)
{
    u8* ptr = (u8*)buffer;

    requestSdRead(sector);

    do
    {
        while (!pollSdDataReady());
        getSdData(ptr);
        ptr += 512;
    } while (--numSectors);

    // Important! This makes sure that SdCard has returned
    // to State::Idle. Otherwise the next transfer may fail.
    while (!pollSdDataReady());

    return true;
}

bool dldi_writeSectors(u32 sector, u32 numSectors, void* buffer)
{
    if (numSectors > 0)
    {
        const u8* ptr = (const u8*)buffer;
        if (numSectors == 1)
        {
            writeSdData(sector, ptr, true, true); // send 0 = last
        }
        else
        {
            writeSdData(sector, ptr, true, false); // send 0
            sector++;
            ptr += 512;

            for (u32 i = 1; i < numSectors - 1; i++)
            {
                writeSdData(sector, ptr, false, false); // send i
                sector++;
                ptr += 512;
                while (!pollSdDataReady()); // wait i - 1
            }

            writeSdData(sector, ptr, false, true); // send last
            while (!pollSdDataReady()); // wait last - 1
        }

        while (!pollSdDataReady()); // wait last
    }

    return true;
}

bool dldi_shutdown(void)
{
    return true;
}
