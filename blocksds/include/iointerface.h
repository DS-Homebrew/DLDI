// clang-format off

// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2024

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <nds/ndstypes.h>

#define BYTES_PER_READ 512

typedef struct disc_interface {
    // Initialize the driver. Returns true on success.
	bool (*startup)(void);
    // Returns true if a card is present and initialized.
	bool (*is_inserted)(void);
    // Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
    // success.
	bool (*read_sectors)(uint32_t sector, uint32_t num_sectors, void * buffer);
    // Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
    // success.
	bool (*write_sectors)(uint32_t sector, uint32_t num_sectors, const void * buffer);
    // Clear error flags from the card. Returns true on success.
	bool (*clear_status)(void);
    // Shutdowns the card. This may never be called.
	bool (*shutdown)(void);
} PACKED disc_interface_t;
