// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <iointerface.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "acep.h"

// Initialize the driver. Returns true on success.
bool ACEP_Startup(void) {
    return true;
}

// Returns true if a card is present and initialized.
bool ACEP_IsInserted(void) {
    return true;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool ACEP_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        ACEP_SDReadSector((sector + i), buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool ACEP_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        ACEP_SDWriteSector((sector + i), buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Clear error flags from the card. Returns true on success.
bool ACEP_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool ACEP_Shutdown(void) {
    return true;
}

#ifdef PLATFORM_acep

disc_interface_t ioInterface = {.startup = ACEP_Startup,
                                .is_inserted = ACEP_IsInserted,
                                .read_sectors = ACEP_ReadSectors,
                                .write_sectors = ACEP_WriteSectors,
                                .clear_status = ACEP_ClearStatus,
                                .shutdown = ACEP_Shutdown};
#endif
