// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023

#include <stdbool.h>
#include <stdint.h>

#include <iointerface.h>

extern disc_interface_t ioInterface;

// Initialize the driver. Returns true on success.
bool startup(void) {
    return ioInterface.startup();
}

// Returns true if a card is present and initialized.
bool is_inserted(void) {
    return ioInterface.is_inserted();
}

// Clear error flags from the card. Returns true on success.
bool clear_status(void) {
    return ioInterface.clear_status();
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool read_sectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    return ioInterface.read_sectors(sector, num_sectors, buffer);
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool write_sectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    return ioInterface.write_sectors(sector, num_sectors, buffer);
}

// Shutdowns the card. This may never be called.
bool shutdown(void) {
    return ioInterface.shutdown();
}
