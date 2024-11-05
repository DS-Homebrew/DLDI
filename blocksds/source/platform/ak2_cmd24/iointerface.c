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
bool AK2Q_Startup(void) {
    return ioRPG_Initialize();
}

// Returns true if a card is present and initialized.
bool AK2Q_IsInserted(void) {
    return cardExt_ReadData4Byte(((u64)CARD_CMD_DATA_CHIPID << 56),
                                 IORPG_CTRL_READ_4B | MCCNT1_LATENCY1(4)) == 0xFC2;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool AK2Q_ReadSectors(uint32_t sector, uint32_t num_sectors, void* buffer) {
    if (num_sectors == 1)
        ioRPG_SDReadSingleSector(sector, buffer);
    else
        ioRPG_SDReadMultiSector(sector, num_sectors, buffer);
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool AK2Q_WriteSectors(uint32_t sector, uint32_t num_sectors, const void* buffer) {
    for (int i = 0; i < num_sectors; i++) {
        ioRPG_SDWriteSingleSector(sector, buffer);
        buffer = (u8*)buffer + 0x200;
    }
    return true;
}

// Clear error flags from the card. Returns true on success.
bool AK2Q_ClearStatus(void) {
    return true;
}

// Shutdowns the card. This may never be called.
bool AK2Q_Shutdown(void) {
    return true;
}

#if defined(PLATFORM_ak2_cmd24)

disc_interface_t ioInterface = {.startup = AK2Q_Startup,
                                .is_inserted = AK2Q_IsInserted,
                                .read_sectors = AK2Q_ReadSectors,
                                .write_sectors = AK2Q_WriteSectors,
                                .clear_status = AK2Q_ClearStatus,
                                .shutdown = AK2Q_Shutdown};

#endif
