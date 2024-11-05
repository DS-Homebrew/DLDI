// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023

#include <iointerface.h>
#include <stdbool.h>
#include <stdint.h>

#define BYTES_PER_READ 512

// Initialize the driver. Returns true on success.
bool DLDI_Startup(void) {
    return false;
}

// Returns true if a card is present and initialized.
bool DLDI_IsInserted(void) {
    return false;
}

// Clear error flags from the card. Returns true on success.
bool DLDI_ClearStatus(void) {
    return false;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool DLDI_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    return false;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool DLDI_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    return false;
}

// Shutdowns the card. This may never be called.
bool DLDI_Shutdown(void) {
    return false;
}

#ifdef PLATFORM_00example

disc_interface_t ioInterface = {.startup = DLDI_Startup,
                                .is_inserted = DLDI_IsInserted,
                                .read_sectors = DLDI_ReadSectors,
                                .write_sectors = DLDI_WriteSectors,
                                .clear_status = DLDI_ClearStatus,
                                .shutdown = DLDI_Shutdown};

#endif
