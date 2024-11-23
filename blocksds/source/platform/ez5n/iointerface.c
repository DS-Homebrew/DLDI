// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2024

#include <iointerface.h>
#include <stdbool.h>
#include <stdint.h>

#include "ez5n.h"

// Initialize the driver. Returns true on success.
bool EZ5N_Startup(void) {
    return (EZ5N_CardReadChipID() & 7) == 2;
}

// Returns true if a card is present and initialized.
bool EZ5N_IsInserted(void) {
    return (EZ5N_CardReadChipID() & 7) == 2;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool EZ5N_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    EZ5N_SDReadSectors(sector, num_sectors, buffer);
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool EZ5N_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    EZ5N_SDWriteSectors(sector, num_sectors, buffer);
    return true;
}

// Clear error flags from the card. Returns true on success.
bool EZ5N_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool EZ5N_Shutdown(void) {
    return true;
}

#ifdef PLATFORM_ez5n

disc_interface_t ioInterface = {.startup = EZ5N_Startup,
                                .is_inserted = EZ5N_IsInserted,
                                .read_sectors = EZ5N_ReadSectors,
                                .write_sectors = EZ5N_WriteSectors,
                                .clear_status = EZ5N_ClearStatus,
                                .shutdown = EZ5N_Shutdown};

#endif
