// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <common/libtwl_ext.h>
#include <iointerface.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "r4tf.h"

// Initialize the driver. Returns true on success.
bool R4TF_Startup(void) {
    // Confirm card is actually responding and is actually an R4
    return (cardExt_ReadData4Byte(R4TF_CMD_CARD_INFO, R4TF_CTRL_READ_4B) & 7) == 4;
}

// Returns true if a card is present and initialized.
bool R4TF_IsInserted(void) {
    // Confirm card is actually responding and is actually an R4
    return (cardExt_ReadData4Byte(R4TF_CMD_CARD_INFO, R4TF_CTRL_READ_4B) & 7) == 4;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool R4TF_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        R4TF_SDReadSector((sector + i) << 9, buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool R4TF_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        R4TF_SDWriteSector((sector + i) << 9, buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Clear error flags from the card. Returns true on success.
bool R4TF_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool R4TF_Shutdown(void) {
    return true;
}

#ifdef PLATFORM_r4tf

disc_interface_t ioInterface = {.startup = R4TF_Startup,
                                .is_inserted = R4TF_IsInserted,
                                .read_sectors = R4TF_ReadSectors,
                                .write_sectors = R4TF_WriteSectors,
                                .clear_status = R4TF_ClearStatus,
                                .shutdown = R4TF_Shutdown};

#endif
