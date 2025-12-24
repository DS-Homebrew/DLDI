/*
    gmtf.c -- SDHC compliant DLDI driver for the Datel Game'n'Music card

 Copyright (c) 2007 Michael "Chishm" Chisholm
 Copyright (c) 2025 Edoardo Lolletti (edo9300)

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <nds.h>

#define BYTES_PER_SECTOR 512

#define SD_COMMAND_TIMEOUT 0xFFF
#define SD_WRITE_TIMEOUT 0xFFFF
static const u32 CARD_CR2_SETTINGS = (CARD_ACTIVATE | CARD_nRESET | CARD_SEC_CMD |
                                      CARD_DELAY2(0x3F) | CARD_SEC_EN | CARD_SEC_DAT);

#define STOP_TRANSMISSION 12
#define READ_SINGLE_BLOCK 17
#define READ_MULTIPLE_BLOCK 18
#define WRITE_SINGLE_BLOCK 24
#define WRITE_MULTIPLE_BLOCK 25

#define SD_WRITE_OK 0x05

#define SPI_START 0xCC
#define SPI_STOP 0xC8

static bool isSdhc = false;

static inline void ndsarSendNtrCommand(const uint8_t cmd[8], uint32_t romctrl) {
    REG_AUXSPICNT = CARD_ENABLE;

    REG_CARD_COMMAND[0] = cmd[0];
    REG_CARD_COMMAND[1] = cmd[1];
    REG_CARD_COMMAND[2] = cmd[2];
    REG_CARD_COMMAND[3] = cmd[3];
    REG_CARD_COMMAND[4] = cmd[4];
    REG_CARD_COMMAND[5] = cmd[5];
    REG_CARD_COMMAND[6] = cmd[6];
    REG_CARD_COMMAND[7] = cmd[7];
    REG_ROMCTRL = romctrl;

    while (REG_ROMCTRL & CARD_BUSY);
}

static inline void ndsarNtrF2(uint32_t param1, uint8_t param2) {
    uint8_t cmd[8] = {0xF2,          param1 >> 24, param1 >> 16, param1 >> 8,
                      param1 & 0xFF, param2,       0x00,         0x00};

    ndsarSendNtrCommand(cmd, CARD_CR2_SETTINGS);
}

static inline uint8_t transferSpiByte(uint8_t send) {
    REG_AUXSPIDATA = send;
    while (REG_AUXSPICNT & CARD_SPI_BUSY);
    return REG_AUXSPIDATA;
}

static inline uint8_t getSpiByte(void) {
    REG_AUXSPIDATA = 0xFF;
    while (REG_AUXSPICNT & CARD_SPI_BUSY);
    return REG_AUXSPIDATA;
}

static inline void cycleSpi() {
    ndsarNtrF2(0, SPI_STOP);
    REG_AUXSPICNT = CARD_ENABLE | CARD_SPI_ENABLE | CARD_SPI_HOLD;
    getSpiByte();
    ndsarNtrF2(0, SPI_START);
    REG_AUXSPICNT = CARD_ENABLE | CARD_SPI_ENABLE | CARD_SPI_HOLD;
}

static inline uint8_t getSpiByteTimeout() {
    int timeout = SD_COMMAND_TIMEOUT;
    uint8_t r1;
    do {
        r1 = getSpiByte();
    } while (r1 == 0xFF && --timeout > 0);
    return r1;
}

static inline uint8_t sendCommandLen(uint8_t cmdId, uint32_t arg, void* buff, int len) {
    cycleSpi();
    uint8_t cmd[6];

    // Build a SPI SD command to be sent as-is.
    cmd[0] = 0x40 | (cmdId & 0x3f);
    cmd[1] = arg >> 24;
    cmd[2] = arg >> 16;
    cmd[3] = arg >> 8;
    cmd[4] = arg >> 0;
    // CRC in SPI mode is ignore for every command but CMD0 (hardcoded to 0x95)
    // and CMD8, hardcoded to 0x86 with the default 0x1AA argument.
    cmd[5] = (len > 1) ? 0x86 : 0x95;

    for (int i = 0; i < sizeof(cmd); i++) transferSpiByte(cmd[i]);

    uint8_t r1 = getSpiByteTimeout();

    uint8_t* buff_u8 = (uint8_t*)buff;

    for (int i = 0; i < (len - 1); ++i) {
        buff_u8[i] = getSpiByte();
    }

    return r1;
}

static inline uint8_t sendCommand(uint8_t cmdId, uint32_t arg) {
    return sendCommandLen(cmdId, arg, NULL, 1);
}

static bool ReadSingleSector(uint32_t sector, uint8_t* dest) {
    if (!isSdhc) sector <<= 9;

    if (sendCommand(READ_SINGLE_BLOCK, sector) != 0x00) {
        return false;
    }

    // Wait for data start token
    uint8_t spiByte = getSpiByteTimeout();

    if (spiByte != 0xFE) {
        return false;
    }

    for (int i = BYTES_PER_SECTOR; i > 0; i--) {
        *dest++ = getSpiByte();
    }

    (void)getSpiByte();
    (void)getSpiByte();

    return true;
}

static bool ReadMultipleSectors(uint32_t sector, uint8_t* dest, uint32_t numSectors) {
    if (!isSdhc) sector <<= 9;

    if (sendCommand(READ_MULTIPLE_BLOCK, sector) != 0x00) {
        return false;
    }

    while (numSectors > 0) {
        // Wait for data start token
        uint8_t spiByte = getSpiByteTimeout();

        if (spiByte != 0xFE) {
            return false;
        }

        for (int i = BYTES_PER_SECTOR; i > 0; i--) {
            *dest++ = getSpiByte();
        }

        (void)getSpiByte();
        (void)getSpiByte();

        --numSectors;
    }

    // this message returns 1 byte of response, but it needs up to 8 bytes
    // to be polled before we get the busy bytes
    uint8_t buff[7];
    (void)sendCommandLen(STOP_TRANSMISSION, 0, buff, 8);

    // Wait for card to write data
    int timeout = SD_COMMAND_TIMEOUT;
    while (getSpiByte() == 0 && --timeout > 0);

    return timeout != 0;
}

static bool WriteSingleSector(uint32_t sector, uint8_t* src) {
    if (!isSdhc) sector <<= 9;

    // this message needs 1 byte of extra clock before it starts waiting for the start token
    uint8_t buff[1];
    if (sendCommandLen(WRITE_SINGLE_BLOCK, sector, buff, 2) != 0) {
        return false;
    }

    // Send start token
    transferSpiByte(0xFE);

    // Send data
    for (int i = BYTES_PER_SECTOR; i > 0; i--) {
        REG_AUXSPIDATA = *src++;
        while (REG_AUXSPICNT & CARD_SPI_BUSY);
    }

    // Send fake CRC
    transferSpiByte(0xFF);
    transferSpiByte(0xFF);

    // Get data response
    if ((getSpiByte() & 0x0F) != SD_WRITE_OK) {
        return false;
    }

    // Wait for card to write data
    int timeout = SD_WRITE_TIMEOUT;
    while (getSpiByte() == 0 && --timeout > 0);

    return timeout != 0;
}

static bool WriteMultipleSectors(uint32_t sector, uint8_t* src, uint32_t numSectors) {
    if (!isSdhc) sector <<= 9;

    // this message needs 1 byte of extra clock before it starts waiting for the start token
    uint8_t buff[2];
    if (sendCommandLen(WRITE_MULTIPLE_BLOCK, sector, buff, 2) != 0) {
        return false;
    }

    while (numSectors > 0) {
        // Send start token
        transferSpiByte(0xFC);

        // Send data
        for (int i = BYTES_PER_SECTOR; i > 0; i--) {
            REG_AUXSPIDATA = *src++;
            while (REG_AUXSPICNT & CARD_SPI_BUSY);
        }

        // Send fake CRC
        transferSpiByte(0xFF);
        transferSpiByte(0xFF);

        // Get data response
        if ((getSpiByte() & 0x0F) != SD_WRITE_OK) {
            return false;
        }

        // Wait for card to write data
        int timeout = SD_WRITE_TIMEOUT;
        while (getSpiByte() == 0 && --timeout > 0);
        if (timeout == 0) {
            return false;
        }

        --numSectors;
    }

    // send stop token
    transferSpiByte(0xFD);
    // send 1 byte clock
    getSpiByte();

    int timeout = SD_WRITE_TIMEOUT;
    while (getSpiByte() == 0 && --timeout > 0);

    return timeout != 0;
}

#define MAX_STARTUP_TRIES 5000

static bool GNM_Startup(void) {
    bool isv2 = false;
    for (int i = 0; i < 0x100; i++) {
        ndsarNtrF2(0x7FFFFFFF | ((i & 1) << 31), 0x00);
    }

    ndsarNtrF2(0, SPI_STOP);

    // Send CMD0.
    uint8_t r1 = sendCommand(0, 0);
    if (r1 != 0x01)  // Idle State.
    {
        // CMD 0 failed.
        return false;
    }

    uint32_t r7_answer;

    r1 = sendCommandLen(8, 0x1AA, &r7_answer, 5);

    uint32_t acmd41_arg = 0;

    if (r1 == 0x1 && r7_answer == 0xAA010000) {
        isv2 = true;
        acmd41_arg |= (1 << 30);  // Set HCS bit,Supports SDHC
    }

    for (int i = 0; i < MAX_STARTUP_TRIES; ++i) {
        // Send ACMD41.
        sendCommand(55, 0);
        r1 = sendCommand(41, acmd41_arg);
        if (r1 == 0) {
            break;
        }
    }
    if (r1 != 0) return false;

    if (isv2) {
        uint32_t r2_answer;
        r1 = sendCommandLen(58, 0, &r2_answer, 5);
        isSdhc = (r2_answer & 0x40) != 0;
    }
    sendCommand(16, 0x200);

    return true;
}

static bool GNM_IsInserted(void) {
    return true;
}

static bool GNM_ClearStatus(void) {
    return true;
}

static bool GNM_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    uint8_t* data = (uint8_t*)buffer;
    if (num_sectors == 1) {
        return ReadSingleSector(sector, data);
    } else {
        return ReadMultipleSectors(sector, data, num_sectors);
    }
}

static bool GNM_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    uint8_t* data = (uint8_t*)buffer;
    if (num_sectors == 1) {
        return WriteSingleSector(sector, data);
    } else {
        return WriteMultipleSectors(sector, data, num_sectors);
    }
}

static bool GNM_Shutdown(void) {
    return true;
}

#ifdef PLATFORM_gmtf

#include <iointerface.h>

disc_interface_t ioInterface = {.startup = GNM_Startup,
                                .is_inserted = GNM_IsInserted,
                                .read_sectors = GNM_ReadSectors,
                                .write_sectors = GNM_WriteSectors,
                                .clear_status = GNM_ClearStatus,
                                .shutdown = GNM_Shutdown};

#endif
