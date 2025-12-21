/*
    GMTF - Datel Games 'n' Music & Action Replay DS

    Copyright (C) 2023 lifehackerhansol
    Copyright (C) 2025 Edoardo Lolletti (edo9300)

    SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include <libtwl/card/card.h>
#include <common/libtwl_ext.h>
#include <common/sdio.h>
#include "gmtf.h"

static bool isSdhc = false;

static void GMTF_CardSendCommandF2(u32 param1, u8 param2) {
    cardExt_RomSendCommand(GMTF_CMD_F2(param1, param2), GMTF_CTRL_BASE);
}

static u8 GMTF_SpiReadByte(void) {
    return card_spiTransferByte(MCCNT0_SPI_RATE_4_19_MHZ, GMTF_SPI_READ_BYTE);
}

static void GMTF_SpiDisable(void)
{
    // Dummy SPI read to release CS
    card_spiTransferLastByte(MCCNT0_SPI_RATE_4_19_MHZ, GMTF_SPI_READ_BYTE);
    GMTF_CardSendCommandF2(0, GMTF_CMD_F2_SPI_DISABLE);
}

static void GMTF_SpiEnable(void)
{
    GMTF_CardSendCommandF2(0, GMTF_CMD_F2_SPI_ENABLE);
}

static void GMTF_SpiReinitialize(void) {
    GMTF_SpiDisable();
    GMTF_SpiEnable();
}

// Waits for timeout, then returns resulting data.
static u8 GMTF_SpiReadByteTimeout(void) {
    int timeout = GMTF_SD_CMD_TIMEOUT_LEN;
    u8 data;
    while (data = GMTF_SpiReadByte(), data == 0xFF && --timeout > 0);
    return data;
}

// Sends SDIO command to ARDS.
static u8 GMTF_SpiSendSDIOCommand(u8 cmdId, u32 arg, u8 * buffer, int len)
{
    uint8_t cmd[6];

    GMTF_SpiReinitialize();
    // Build a SPI SD command to be sent as-is.
    cmd[0] = 0x40 | (cmdId & 0x3f);
    cmd[1] = arg >> 24;
    cmd[2] = arg >> 16;
    cmd[3] = arg >> 8;
    cmd[4] = arg >> 0;
    // CRC in SPI mode is ignored for every command but CMD0 (hardcoded to 0x95)
    // and CMD8, hardcoded to 0x86 with the default 0x1AA argument.
    cmd[5] = (len > 0) ? 0x86 : 0x95;

    for (int i = 0; i < sizeof(cmd); i++) card_spiTransferByte(MCCNT0_SPI_RATE_4_19_MHZ, cmd[i]);

    u8 timeout = GMTF_SpiReadByteTimeout();

    const u8 * target = buffer == NULL ? NULL : (buffer + len);
    for(int i=0; i < len; i++)
    {
        u8 data = GMTF_SpiReadByte();
        if(buffer < target)
            *buffer++ = data;
    }

    return timeout;
}

static u8 GMTF_SpiSendSDIOCommandR0(u8 cmd, u32 arg)
{
    return GMTF_SpiSendSDIOCommand(cmd, arg, NULL, 0);
}

#define GMTF_MAX_STARTUP_TRIES 5000

bool GMTF_SDInitialize(void)
{
    bool isv2 = false;
    for (int i = 0; i < 0x100; i++) {
        GMTF_CardSendCommandF2(0x7FFFFFFF | ((i & 1) << 31), 0x00);
    }

    // Send CMD0.
    uint8_t r1 = GMTF_SpiSendSDIOCommandR0(0, 0);
    if (r1 != 0x01)  // Idle State.
    {
        // CMD 0 failed.
        return false;
    }

    uint32_t r7_answer;

    r1 = GMTF_SpiSendSDIOCommand(8, 0x1AA, (u8*)&r7_answer, 4);

    uint32_t acmd41_arg = 0;

    if (r1 == 0x1 && r7_answer == 0xAA010000) {
        isv2 = true;
        acmd41_arg |= (1 << 30);  // Set HCS bit,Supports SDHC
    }

    for (int i = 0; i < GMTF_MAX_STARTUP_TRIES; ++i) {
        // Send ACMD41.
        GMTF_SpiSendSDIOCommandR0(SDIO_CMD55_APP_CMD, 0);
        r1 = GMTF_SpiSendSDIOCommandR0(SDIO_ACMD41_SD_SEND_OP_COND, acmd41_arg);
        if (r1 == 0) {
            break;
        }
    }
    if (r1 != 0) return false;

    if (isv2) {
        uint32_t r2_answer;
        r1 = GMTF_SpiSendSDIOCommand(SDIO_CMD58_READ_OCR, 0, (u8*)&r2_answer, 4);
        isSdhc = (r2_answer & 0x40) != 0;
    }
    GMTF_SpiSendSDIOCommandR0(SDIO_CMD16_SET_BLOCK_LEN, 0x200);

    return true;
}

bool GMTF_SDReadSingleSector(u32 sector, u8 * buffer) {
    sector = isSdhc ? sector : sector << 9;

    if(GMTF_SpiSendSDIOCommandR0(SDIO_CMD17_READ_SINGLE_BLOCK, sector) != 0)
        return false;

    // Wait for data start token
    if(GMTF_SpiReadByteTimeout() != GMTF_SPI_START_DATA_TOKEN)
        return false;

    for(int i=0; i < 512; i++)
        *buffer++ = GMTF_SpiReadByte();

    (void)GMTF_SpiReadByte();
    (void)GMTF_SpiReadByte();

    return true;
}

bool GMTF_SDReadMultipleSector(u32 sector, u32 num_sectors, u8 * buffer) {
    sector = isSdhc ? sector : sector << 9;

    if(GMTF_SpiSendSDIOCommandR0(SDIO_CMD18_READ_MULTIPLE_BLOCK, sector) != 0)
        return false;

    for(int i=0; i < num_sectors; i++)
    {
        // Wait for data start token
        if(GMTF_SpiReadByteTimeout() != GMTF_SPI_START_DATA_TOKEN)
            return false;

        for(int j=0; j < 512; j++)
            *buffer++ = GMTF_SpiReadByte();

        (void)GMTF_SpiReadByte();
        (void)GMTF_SpiReadByte();
    }

    // this message returns 1 byte of response, but it needs up to 8 bytes
    // to be polled before we get the busy bytes
    GMTF_SpiSendSDIOCommand(SDIO_CMD12_STOP_TRANSMISSION, 0, NULL, 7);

    // Wait for card to finish
    int timeout = GMTF_SD_CMD_TIMEOUT_LEN;
    while (GMTF_SpiReadByte() == 0 && --timeout > 0);

    return timeout != 0;
}

bool GMTF_SDWriteSingleSector(u32 sector, const u8 * buffer)
{
    sector = isSdhc ? sector : sector << 9;

    // this message needs 1 byte of extra clock before it starts waiting for the start token
    if(GMTF_SpiSendSDIOCommand(SDIO_CMD24_WRITE_SINGLE_BLOCK, sector, NULL, 1) != 0)
        return false;

    // Send start token
    card_spiTransferByte(MCCNT0_SPI_RATE_4_19_MHZ, GMTF_SPI_START_DATA_TOKEN);

    // Send data
    for (int i = 0; i < 512; i++)
        card_spiTransferByte(MCCNT0_SPI_RATE_4_19_MHZ, *buffer++);

    // Send fake CRC
    GMTF_SpiReadByte();
    GMTF_SpiReadByte();

    // Get data response
    if((GMTF_SpiReadByte() & 0x0F) != GMTF_SD_WRITE_OK)
        return false;

    // Wait for card to write data
    int timeout = GMTF_SD_WRITE_TIMEOUT_LEN;
    while (GMTF_SpiReadByte() == 0 && --timeout > 0);

    return timeout != 0;
}

bool GMTF_SDWriteMultipleSector(u32 sector, u32 num_sectors, const u8 * buffer) {
    sector = isSdhc ? sector : sector << 9;

    // this message needs 1 byte of extra clock before it starts waiting for the start token
    if(GMTF_SpiSendSDIOCommand(SDIO_CMD25_WRITE_MULTIPLE_BLOCK, sector, NULL, 1) != 0)
        return false;

    for(int i=0; i < num_sectors; i++)
    {
        // Send start token
        card_spiTransferByte(MCCNT0_SPI_RATE_4_19_MHZ, GMTF_SPI_MULTI_BLOCK_WRITE_TOKEN);

        // Send data
        for (int j = 0; j < 512; j++)
            card_spiTransferByte(MCCNT0_SPI_RATE_4_19_MHZ, *buffer++);

        // Send fake CRC
        GMTF_SpiReadByte();
        GMTF_SpiReadByte();

        // Get data response
        if((GMTF_SpiReadByte() & 0x0F) != GMTF_SD_WRITE_OK)
            return false;

        // Wait for card to write data
        int timeout = GMTF_SD_WRITE_TIMEOUT_LEN;
        while (GMTF_SpiReadByte() == 0 && --timeout > 0);
        if(!timeout)
            return false;
    }

    // send stop token
    card_spiTransferByte(MCCNT0_SPI_RATE_4_19_MHZ, GMTF_SPI_END_MULTI_BLOCK_WRITE);
    // send 1 byte clock
    GMTF_SpiReadByte();

    int timeout = GMTF_SD_WRITE_TIMEOUT_LEN;
    while (GMTF_SpiReadByte() == 0 && --timeout > 0);

    return timeout != 0;
}
