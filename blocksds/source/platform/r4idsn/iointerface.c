// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <iointerface.h>
#include <stdbool.h>
#include <stdint.h>

#include <common/rpg/iorpg.h>

#define BYTES_PER_READ 512

// Initialize the driver. Returns true on success.
bool R4iDSN_Startup(void) {
    return ioRPG_SDInitialize();
}

// Returns true if a card is present and initialized.
bool R4iDSN_IsInserted(void) {
    return ioRPG_CardReadChipID() == 0xFC2;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool R4iDSN_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    if (num_sectors == 1)
        ioRPG_SDReadSingleSector(sector, buffer);
    else
        ioRPG_SDReadMultiSector(sector, num_sectors, buffer);
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool R4iDSN_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    if (num_sectors == 1)
        ioRPG_SDWriteSingleSector(sector, buffer);
    else
        ioRPG_SDWriteMultiSector(sector, num_sectors, buffer);
    return true;
}

// Clear error flags from the card. Returns true on success.
bool R4iDSN_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool R4iDSN_Shutdown(void) {
    return true;
}

#if defined(PLATFORM_r4idsn)

disc_interface_t ioInterface = {.startup = R4iDSN_Startup,
                                .is_inserted = R4iDSN_IsInserted,
                                .read_sectors = R4iDSN_ReadSectors,
                                .write_sectors = R4iDSN_WriteSectors,
                                .clear_status = R4iDSN_ClearStatus,
                                .shutdown = R4iDSN_Shutdown};

#endif
