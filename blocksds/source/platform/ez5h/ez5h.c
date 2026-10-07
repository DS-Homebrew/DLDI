/*
    EZ-Flash V
    Card IO routines

    Copyright (C) 2007 Michael Chisholm (Chishm)
    Copyright (C) 2007 SaTa
    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <common/sdio.h>
#include <libcart/sdcrc16.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "ez5h.h"

static u32 isSDHC = 0;

static bool EZ5H_SDSendSDIOCommand(u8 cmd, u32 parameter, u8* buffer, int size) {
    u32 data;
    u8* u8_data = (u8*)&data;
    int timeout = 99;

    // Either it's no response, 48 bits, or 136 bits.
    // If this isn't the case, then this function doesn't work.
    if (size != 0 && size != 6 && size != 17) return false;

    cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SDIO(cmd, parameter), EZ5H_CTRL_READ_4B);

    // R0 has no response.
    if (size == 0) return true;

    // Sends response in byte-swapped u32, with the starting marker
    // Search for starting marker, with a timeout
    do {
        data = cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CLK(1), EZ5H_CTRL_READ_4B);
        timeout--;
        if (!timeout) return false;
    } while (data & 0xFF);

    // Starting marker found. Start reading response
    if (buffer != NULL) {
        buffer[0] = u8_data[1];
        buffer[1] = u8_data[2];
        buffer[2] = u8_data[3];
    }

    // Read remaining data
    data = cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CLK(2), EZ5H_CTRL_READ_4B);
    if (buffer != NULL) {
        buffer[3] = u8_data[0];
        buffer[4] = u8_data[1];
        buffer[5] = u8_data[2];
        // if we're pulling an R1 response, then we have read all of our data here
        // otherwise keep going
        if (size != 6) return true;
        buffer[6] = u8_data[3];
    } else if (size == 6)
        return true;

    data = cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CLK(3), EZ5H_CTRL_READ_4B);
    if (buffer != NULL) {
        buffer[7] = u8_data[0];
        buffer[8] = u8_data[1];
        buffer[9] = u8_data[2];
        buffer[10] = u8_data[3];
    }
    data = cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CLK(4), EZ5H_CTRL_READ_4B);
    if (buffer != NULL) {
        buffer[11] = u8_data[0];
        buffer[12] = u8_data[1];
        buffer[13] = u8_data[2];
        buffer[14] = u8_data[3];
    }
    data = cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CLK(5), EZ5H_CTRL_READ_4B);
    if (buffer != NULL) {
        buffer[15] = u8_data[0];
        buffer[16] = u8_data[1];
    }

    return true;
}

u32 EZ5H_SRAMReadData(u32 address) {
    return cardExt_RomReadData4Byte(EZ5H_CMD_SRAM_READ_DATA(address), EZ5H_CTRL_READ_4B);
}

bool EZ5H_SDInitialize(void) {
    u8 response[17] = {};
    u16 isSD20 = 0;

    // Does this flush something?
    // iSmart does this loop
    for (int i = 0; i < 128; i++)
        cardExt_RomReadData(EZ5H_CMD_SDMC_READ_DATA, EZ5H_CTRL_READ_512B, NULL, 0);
    EZ5H_SDSendSDIOCommand(SDIO_CMD0_GO_IDLE_STATE, 0, NULL, 0);
    // it does it twice
    for (int i = 0; i < 128; i++)
        cardExt_RomReadData(EZ5H_CMD_SDMC_READ_DATA, EZ5H_CTRL_READ_512B, NULL, 0);

    // CMD8 SDHC init
    if (EZ5H_SDSendSDIOCommand(SDIO_CMD8_SEND_IF_COND, 0x1AA, response, 6))
        if (response[3] == 1 && response[4] == 0xAA) isSD20 = 1;

    do {
        EZ5H_SDSendSDIOCommand(SDIO_CMD55_APP_CMD, 0, NULL, 6);
        u32 parameter = 0x00800000;
        if (isSD20) parameter |= BIT(30);
        EZ5H_SDSendSDIOCommand(SDIO_ACMD41_SD_SEND_OP_COND, parameter, response, 6);
    } while (!(response[1] & 0x80));
    isSDHC = response[1] & 0x40 ? 1 : 0;

    EZ5H_SDSendSDIOCommand(SDIO_CMD2_ALL_SEND_CID, 0, NULL, 17);
    do {
        EZ5H_SDSendSDIOCommand(SDIO_CMD3_SEND_RELATIVE_ADDR, 0, response, 6);
    } while ((response[3] & 0x1E) != 6);  // is standby

    u32 sdio_rca = (response[1] << 8) + response[2];

    EZ5H_SDSendSDIOCommand(SDIO_CMD9_SEND_CSD, (sdio_rca << 16), NULL, 17);
    EZ5H_SDSendSDIOCommand(SDIO_CMD7_SELECT_CARD, (sdio_rca << 16), NULL, 6);
    EZ5H_SDSendSDIOCommand(SDIO_CMD55_APP_CMD, (sdio_rca << 16), NULL, 6);
    EZ5H_SDSendSDIOCommand(SDIO_ACMD6_SET_BUS_WIDTH, 2, NULL, 6);
    EZ5H_SDSendSDIOCommand(SDIO_CMD16_SET_BLOCK_LEN, 512, NULL, 6);
    return true;
}

bool EZ5H_SDReadSector(u32 sector, void* buffer) {
    if (!isSDHC) sector <<= 9;

    EZ5H_SDSendSDIOCommand(17, sector, NULL, 0);

    // Wait for the start marker.
    u32 data = 0;
    u32 timeout = 0x10000;
    do {
        data = cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CLK(1), EZ5H_CTRL_READ_4B);
        timeout--;
        if (!timeout) return false;
    } while (data & 0xFF);

    cardExt_RomReadData(EZ5H_CMD_SDMC_READ_DATA, EZ5H_CTRL_READ_512B, buffer, 128);
    return true;
}

bool EZ5H_SDWriteSector(u32 sector, const u8* buffer) {
    if (!isSDHC) sector <<= 9;

    u64 crc16 = __sd_crc16(buffer);

    // CMD24
    if (!EZ5H_SDSendSDIOCommand(24, sector, NULL, 6)) return false;

    // This command needs an additional clock before sending data.
    cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CLK(1), EZ5H_CTRL_READ_4B);

    // Send data start marker.
    u8 start_marker[2] = {0xFF, 0xF0};
    cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_WRITE_DATA(start_marker), EZ5H_CTRL_READ_4B);

    // Write data.
    for (u32 i = 0; i < 512; i += 2) {
        // generate command to send while waiting for the card transfer to finish
        u64 command = EZ5H_CMD_SDMC_WRITE_DATA(buffer + i);
        card_romWaitBusy();
        card_romSetCmd(command);
        card_romStartXfer(EZ5H_CTRL_READ_0, false);
    }
    // Write CRC data.
    for (u32 i = 0; i < 8; i += 2) {
        // generate command to send while waiting for the card transfer to finish
        u64 command = EZ5H_CMD_SDMC_WRITE_DATA(((u8*)&crc16) + i);
        card_romWaitBusy();
        card_romSetCmd(command);
        card_romStartXfer(EZ5H_CTRL_READ_0, false);
    }

    // Wait for transfer to complete.
    card_romWaitBusy();

    // Wait until CRC starts
    while (cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CRC_STATUS, EZ5H_CTRL_READ_4B) & 0x1);

    // Read CRC status
    while ((cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CRC_STATUS, EZ5H_CTRL_READ_4B) & 0x1) !=
           0x1);

    // Wait until card ready
    while (cardExt_RomReadData4Byte(EZ5H_CMD_SDMC_SEND_CLK(1), EZ5H_CTRL_READ_4B) & 0xFF);
    return true;
}
