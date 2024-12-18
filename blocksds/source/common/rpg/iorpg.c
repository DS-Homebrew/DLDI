/*
    Acekard RPG
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <libtwl/card/card.h>
#include <nds/card.h>
#include <nds/ndstypes.h>

#include <common/rpg/iorpg.h>

static u32 isSDHC = 0;

extern void waitByLoop(u32 count);

// SDIO commands send every bit in a byte in order from MSB to LSB
// This function doesn't handle unaligned reads
static void ioRPG_SDSendSDIOCommand(u64 command, u8* buffer, u32 length) {
    u32 flags;
    if (length == 0)
        flags = IORPG_CTRL_POLL | MCCNT1_LATENCY1(80);
    else
        // actually the original driver used 4KB reads.
        // but with SDIO, it only really reads up to 136 bytes,
        // so 4KB is unnecessary.
        flags = IORPG_CTRL_READ_512B | MCCNT1_LATENCY1(40);

    // param SDIO, get response
    card_romSetCmd(command);
    card_romStartXfer(flags, false);

    // don't poll if we aren't reading
    if (length == 0) {
        card_romWaitBusy();
        return;
    }

    u32 i = 0;
    u32 data[2];
    u8* target = buffer + (length >> 3);
    do {
        // Read data if available
        if (card_romIsDataReady()) {
            data[i & 1] = card_romGetData();
            if (i & 1) {
                target--;
                // Since every byte is a bit, read two words,
                // then pack it into a single byte to represent the
                // true value
                // BIT(7) is the needed bit from the return values
                for (int j = 0; j < 8; ++j) {
                    u8 bit = (((u8*)data)[7 - j] & BIT(7)) >> 7;
                    *target |= bit << j;
                }
            }
            i++;
        }
        // Exit loop when we reach the number of words needed
        if (i >= (length >> 2)) {
            break;
        }
    } while (card_romIsBusy());

    // Here we cut the transfer short. We don't actually read all 512 bytes as seen
    // in MCCNT1.
    // Cutting the transfer length in the middle of transfer causes undefined
    // behaviour, so wait until then
    card_romWaitDataReady();
    // add a delay just in case
    waitByLoop(33);
    // Cut transmission
    REG_MCCNT1 = 0;
    cardExt_SendCommand(command, IORPG_CTRL_POLL);
}

// This function gets the full R1 response, and truncates it to bits
// 8 - 39 in a single u32.
// QUIRK: RPG doesn't send the MSB. We must assume the MSB is 0, and shift the response >> 1.
static u32 ioRPG_SDSendR1Command(u8 cmd, u32 argument) {
    ALIGN(4) u64 buffer = 0;
    ioRPG_SDSendSDIOCommand(IORPG_CMD_SDIO(cmd, IORPG_SDIO_READ_RESPONSE, argument), (u8*)&buffer,
                            48);

    return (u32)(buffer >> 9);
}

// This function gets the full R2 response, and truncates it to bits
// 8 - 135. (CID/CSD with no CRC7.)
// QUIRK: RPG doesn't send the MSB. We must assume the MSB is 0, and shift the response >> 1.
// TODO: actually verify this buffer. We don't actually need it in DLDI, just need to send command
// and clear the FIFO.
static void ioRPG_SDSendR2Command(u8 cmd, u32 argument) {
    ALIGN(4) u8 ret[136 >> 3] = {};
    ioRPG_SDSendSDIOCommand(IORPG_CMD_SDIO(cmd, IORPG_SDIO_READ_RESPONSE, argument), ret, 136);
}

static bool ioRPG_WaitBusy(void) {
    bool timeout = true;
    u64 command = IORPG_CMD_CARD_WAIT_BUSY;

    u32 i;
    u32 data = 0;
    while (timeout) {
        i = 0;
        card_romSetCmd(command);
        card_romStartXfer(IORPG_CTRL_READ_16KB, false);
        do {
            if (card_romIsDataReady()) {
                data = card_romGetData();
                i++;
                if (data == 0x00000FC2) {
                    timeout = false;
                    break;
                }
                waitByLoop(16);  // 1us
            }
        } while (card_romIsBusy());

        // If we were able to get the chip ID before we read all 16KiB,
        // we need to cut transmission short
        // See comments in ioRPG_SDSendSDIOCommand for reference
        if (i != 0x1000) {
            card_romWaitDataReady();
            waitByLoop(33);
            REG_MCCNT1 = 0;
            cardExt_SendCommand(command, IORPG_CTRL_POLL);
        }
    }

    return !timeout;
}

// RPG uses MCCNT1 to transfer data instead of MCD1
// So we write two words for each transfer
static void ioRPG_SDWriteData(const u32* buffer, u32 length) {
    for (u32 i = 0; i < length; i += 2) {
        cardExt_ReadData(IORPG_CMD_SD_WRITE_DATA(buffer + i), IORPG_CTRL_POLL, NULL, 0);
    }
}

// TODO: what are each of the states?
static void ioRPG_SDWaitForState(uint8_t state) {
    u32 data = 0;
    u32 mask = 0x0F;
#ifndef PLATFORM_rpgs
    mask = 0xF0;
    state <<= 4;
#endif

    do {
        data = cardExt_ReadData4Byte(IORPG_CMD_SD_READ_STATE,
                                     IORPG_CTRL_READ_4B | MCCNT1_LATENCY1(4)) &
               mask;
    } while (data != state);
}

u32 ioRPG_CardReadChipID(void) {
    return cardExt_ReadData4Byte(((u64)CARD_CMD_DATA_CHIPID << 56),
                                 IORPG_CTRL_READ_4B | MCCNT1_LATENCY1(4));
}

// SDIO initialization
bool ioRPG_SDInitialize(void) {
    int isSD20 = 0;
    u32 responseR1 = 0;

    // CMD0
    ioRPG_SDSendSDIOCommand(IORPG_CMD_SDIO(0, IORPG_SDIO_NORESPONSE, 0), NULL, 0);

    // CMD8
    responseR1 = ioRPG_SDSendR1Command(8, 0x1AA);
    isSD20 = responseR1 == 0x1AA ? 1 : 0;

    for (u32 i = 0; i < 10000; i++) {
        // CMD55
        ioRPG_SDSendR1Command(55, 0);

        // ACMD41
        u32 argument = 0x00FF8000;
        if (isSD20) argument |= BIT(30);
        responseR1 = ioRPG_SDSendR1Command(41, argument);
        if (responseR1 & BIT(31)) {
            isSDHC = responseR1 & BIT(30) ? 1 : 0;
            break;
        }
        waitByLoop(1666);
    }

    // CMD2
    ioRPG_SDSendR2Command(2, 0);

    // CMD3
    responseR1 = ioRPG_SDSendR1Command(3, 0);
    u32 sdio_rca = responseR1 >> 16;

    // CMD9
    ioRPG_SDSendR2Command(9, (sdio_rca << 16));

    // CMD7
    ioRPG_SDSendR1Command(7, (sdio_rca << 16));

    // ACMD6
    ioRPG_SDSendR1Command(55, (sdio_rca << 16));
    ioRPG_SDSendR1Command(6, 2);

    // CMD13
    responseR1 = ioRPG_SDSendR1Command(13, (sdio_rca << 16));
    if ((responseR1 >> 9) != 4) return false;

    if (isSDHC) {
        card_romSetCmd(IORPG_CMD_SET_SD_MODE_SDHC);
        card_romStartXfer(IORPG_CTRL_POLL, false);
        card_romWaitBusy();
    }

    return true;
}

void ioRPG_SDReadSingleSector(u32 sector, void* buffer) {
    u32 address = isSDHC ? sector : sector << 9;
    // CMD17
    ioRPG_SDSendSDIOCommand(IORPG_CMD_SDIO(17, IORPG_SDIO_READ_SINGLE_BLOCK, address), NULL, 0);
    ioRPG_WaitBusy();

    cardExt_ReadData(IORPG_CMD_CARD_READ_DATA, (IORPG_CTRL_READ_512B | MCCNT1_LATENCY1(4)), buffer,
                     128);
    waitByLoop(80);  // wait for sd crc auto complete
}

void ioRPG_SDReadMultiSector(u32 sector, u32 num_sectors, void* buffer) {
    u32 address = isSDHC ? sector : sector << 9;
    // CMD18
    ioRPG_SDSendSDIOCommand(IORPG_CMD_SDIO(18, IORPG_SDIO_READ_MULTI_BLOCK, address), NULL, 0);
    ioRPG_WaitBusy();

    while (num_sectors--) {
        cardExt_ReadData(IORPG_CMD_CARD_READ_DATA, (IORPG_CTRL_READ_512B | MCCNT1_LATENCY1(4)),
                         buffer, 128);
        ioRPG_SDWaitForState(0x7);
        buffer = (u8*)buffer + 0x200;
    };

    // CMD12
    ioRPG_SDSendR1Command(12, 0);
}

void ioRPG_SDWriteSingleSector(u32 sector, const void* buffer) {
    u32 address = isSDHC ? sector : sector << 9;
    // CMD24
    ioRPG_SDSendSDIOCommand(IORPG_CMD_SDIO(24, IORPG_SDIO_WRITE_SINGLE_BLOCK, address), NULL, 0);

    ioRPG_SDWriteData(buffer, 128);
    ioRPG_SDWaitForState(0);
}

void ioRPG_SDWriteMultiSector(u32 sector, u32 num_sectors, const void* buffer) {
    u32 address = isSDHC ? sector : sector << 9;

    while (num_sectors--) {
        // CMD25
        ioRPG_SDSendSDIOCommand(IORPG_CMD_SDIO(25, IORPG_SDIO_WRITE_MULTI_BLOCK, address), NULL, 0);
        ioRPG_SDWriteData(buffer, 128);
        ioRPG_SDWaitForState(0xE);
        buffer = (u8*)buffer + 0x200;
    };

    // CMD12
    ioRPG_SDSendR1Command(12, 0);
    ioRPG_SDWaitForState(0);
}
