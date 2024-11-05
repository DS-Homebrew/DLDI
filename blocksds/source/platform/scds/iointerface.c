// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <iointerface.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "scds.h"

// Initialize the driver. Returns true on success.
bool SCDS_Startup(void) {
    return true;
}

// Returns true if a card is present and initialized.
bool SCDS_IsInserted(void) {
    return true;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool SCDS_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    if (num_sectors == 1)
        SCDS_SDReadSingleSector(sector << 9, buffer);
    else
        SCDS_SDReadMultiSector(sector << 9, buffer, num_sectors);
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool SCDS_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        SCDS_SDWriteSector((sector + i) << 9, (u32*)buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Clear error flags from the card. Returns true on success.
bool SCDS_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool SCDS_Shutdown(void) {
    return true;
}

#if defined(PLATFORM_scds)

disc_interface_t ioInterface = {.startup = SCDS_Startup,
                                .is_inserted = SCDS_IsInserted,
                                .read_sectors = SCDS_ReadSectors,
                                .write_sectors = SCDS_WriteSectors,
                                .clear_status = SCDS_ClearStatus,
                                .shutdown = SCDS_Shutdown};

#endif
