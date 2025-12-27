// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <iointerface.h>
#include <stdbool.h>
#include <stdint.h>

#include <common/libtwl_ext.h>

#include "g003.h"

#define BYTES_PER_READ 512

// Initialize the driver. Returns true on success.
bool G003_Startup(void) {
    // Confirm card is actually responding and is actually a GMP-Z003
    return (cardExt_RomReadData4Byte(G003_CMD_CARD_INFO, G003_CTRL_READ_4B) & 7) == 4;
}

// Returns true if a card is present and initialized.
bool G003_IsInserted(void) {
    // Confirm card is actually responding and is actually a GMP-Z003
    return (cardExt_RomReadData4Byte(G003_CMD_CARD_INFO, G003_CTRL_READ_4B) & 7) == 4;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool G003_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        G003_SDReadSector((sector + i) << 1, buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool G003_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        G003_SDWriteSector((sector + i) << 1, buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Clear error flags from the card. Returns true on success.
bool G003_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool G003_Shutdown(void) {
    return true;
}

#ifdef PLATFORM_g003

disc_interface_t ioInterface = {.startup = G003_Startup,
                                .is_inserted = G003_IsInserted,
                                .read_sectors = G003_ReadSectors,
                                .write_sectors = G003_WriteSectors,
                                .clear_status = G003_ClearStatus,
                                .shutdown = G003_Shutdown};

#endif
