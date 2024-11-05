// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <common/libtwl_ext.h>
#include <iointerface.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "sg3d.h"

// Initialize the driver. Returns true on success.
bool SG3D_Startup(void) {
    return true;
}

// Returns true if a card is present and initialized.
bool SG3D_IsInserted(void) {
    return true;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool SG3D_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        SG3D_SDReadSector((sector + i), buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool SG3D_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        SG3D_SDWriteSector((sector + i), buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Clear error flags from the card. Returns true on success.
bool SG3D_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool SG3D_Shutdown(void) {
    return true;
}

#ifdef PLATFORM_sg3d

disc_interface_t ioInterface = {.startup = SG3D_Startup,
                                .is_inserted = SG3D_IsInserted,
                                .read_sectors = SG3D_ReadSectors,
                                .write_sectors = SG3D_WriteSectors,
                                .clear_status = SG3D_ClearStatus,
                                .shutdown = SG3D_Shutdown};

#endif
