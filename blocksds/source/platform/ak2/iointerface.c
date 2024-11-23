// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <common/libtwl_ext.h>
#include <iointerface.h>
#include <libtwl/card/card.h>
#include <nds/card.h>
#include <nds/ndstypes.h>

#include <common/rpg/iorpg.h>

// Initialize the driver. Returns true on success.
bool AK2_Startup(void) {
    return ioRPG_SDInitialize();
}

// Returns true if a card is present and initialized.
bool AK2_IsInserted(void) {
    return ioRPG_CardReadChipID() == 0xFC2;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool AK2_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    if (num_sectors == 1)
        ioRPG_SDReadSingleSector(sector, buffer);
    else
        ioRPG_SDReadMultiSector(sector, num_sectors, buffer);
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool AK2_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    if (num_sectors == 1)
        ioRPG_SDWriteSingleSector(sector, buffer);
    else
        ioRPG_SDWriteMultiSector(sector, num_sectors, buffer);
    return true;
}

// Clear error flags from the card. Returns true on success.
bool AK2_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool AK2_Shutdown(void) {
    return true;
}

#if defined(PLATFORM_ak2)

disc_interface_t ioInterface = {.startup = AK2_Startup,
                                .is_inserted = AK2_IsInserted,
                                .read_sectors = AK2_ReadSectors,
                                .write_sectors = AK2_WriteSectors,
                                .clear_status = AK2_ClearStatus,
                                .shutdown = AK2_Shutdown};

#endif
