// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <iointerface.h>
#include <nds/ndstypes.h>

#include <common/scdssdhc/scdssdhc.h>

// Initialize the driver. Returns true on success.
bool SCDSSDHC_Startup(void) {
    return SCDSSDHC_SDInitialize();
}

// Returns true if a card is present and initialized.
bool SCDSSDHC_IsInserted(void) {
    return true;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool SCDSSDHC_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    if (num_sectors == 1)
        SCDSSDHC_SDReadSingleSector(sector, buffer);
    else
        SCDSSDHC_SDReadMultiSector(sector, buffer, num_sectors);
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool SCDSSDHC_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    if (num_sectors == 1)
        SCDSSDHC_SDWriteSingleSector(sector, buffer);
    else
        SCDSSDHC_SDWriteMultiSector(sector, buffer, num_sectors);
    return true;
}

// Clear error flags from the card. Returns true on success.
bool SCDSSDHC_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool SCDSSDHC_Shutdown(void) {
    return true;
}

#if defined(PLATFORM_scdssdhc)

disc_interface_t ioInterface = {.startup = SCDSSDHC_Startup,
                                .is_inserted = SCDSSDHC_IsInserted,
                                .read_sectors = SCDSSDHC_ReadSectors,
                                .write_sectors = SCDSSDHC_WriteSectors,
                                .clear_status = SCDSSDHC_ClearStatus,
                                .shutdown = SCDSSDHC_Shutdown};

#endif
