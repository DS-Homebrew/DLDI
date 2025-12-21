// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023

#include <iointerface.h>
#include <stdbool.h>
#include <stdint.h>

#include "gmtf.h"

#define BYTES_PER_READ 512

// Initialize the driver. Returns true on success.
bool GMTF_Startup(void) {
    return GMTF_SDInitialize();
}

// Returns true if a card is present and initialized.
bool GMTF_IsInserted(void) {
    return true;
}

// Clear error flags from the card. Returns true on success.
bool GMTF_ClearStatus(void) {
    return true;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool GMTF_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    if (num_sectors == 1)
        return GMTF_SDReadSingleSector(sector, buffer);
    else
        return GMTF_SDReadMultipleSector(sector, num_sectors, buffer);
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool GMTF_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    if (num_sectors == 1)
        return GMTF_SDWriteSingleSector(sector, buffer);
    else
        return GMTF_SDWriteMultipleSector(sector, num_sectors, buffer);
}

// Shutdowns the card. This may never be called.
bool GMTF_Shutdown(void) {
    return true;
}

#ifdef PLATFORM_gmtf

disc_interface_t ioInterface = {.startup = GMTF_Startup,
                                .is_inserted = GMTF_IsInserted,
                                .read_sectors = GMTF_ReadSectors,
                                .write_sectors = GMTF_WriteSectors,
                                .clear_status = GMTF_ClearStatus,
                                .shutdown = GMTF_Shutdown};

#endif
