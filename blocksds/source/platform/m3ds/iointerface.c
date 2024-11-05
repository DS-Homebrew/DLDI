// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <common/libtwl_ext.h>
#include <iointerface.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "m3ds.h"

// Initialize the driver. Returns true on success.
bool M3DS_Startup(void) {
    // Confirm card is actually responding and is actually an M3
    return (cardExt_ReadData4Byte(M3DS_CMD_CARD_INFO, M3DS_CTRL_GENERAL) & 7) == 4;
}

// Returns true if a card is present and initialized.
bool M3DS_IsInserted(void) {
    // Confirm card is actually responding and is actually an M3
    return (cardExt_ReadData4Byte(M3DS_CMD_CARD_INFO, M3DS_CTRL_GENERAL) & 7) == 4;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool M3DS_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    if (num_sectors == 1)
        M3DS_SDReadSingleSector(sector, buffer);
    else
        M3DS_SDReadMultiSector(sector, buffer, num_sectors);

    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool M3DS_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    if (num_sectors == 1)
        M3DS_SDWriteSingleSector(sector, buffer);
    else
        M3DS_SDWriteMultiSector(sector, buffer, num_sectors);

    return true;
}

// Clear error flags from the card. Returns true on success.
bool M3DS_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool M3DS_Shutdown(void) {
    return true;
}

#if defined(PLATFORM_m3ds)

disc_interface_t ioInterface = {.startup = M3DS_Startup,
                                .is_inserted = M3DS_IsInserted,
                                .read_sectors = M3DS_ReadSectors,
                                .write_sectors = M3DS_WriteSectors,
                                .clear_status = M3DS_ClearStatus,
                                .shutdown = M3DS_Shutdown};

#endif
