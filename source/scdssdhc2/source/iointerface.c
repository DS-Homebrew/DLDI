// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <nds/ndstypes.h>

#include "scdssdhc.h"
#include "libtwl_card.h"

static bool is_sdhc = false;

// Initialize the driver. Returns true on success.
bool startup(void)
{
	is_sdhc = SCDS_SendCommand(SCDS_CMD_SD_IS_SDHC()) != 0 ? true : false;
    return true;
}

// Returns true if a card is present and initialized.
bool is_inserted(void)
{
    return true;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool read_sectors(uint32_t sector, uint32_t num_sectors, void *buffer)
{
	if (num_sectors == 1)
    	SCDS_SDReadSingleSector(is_sdhc ? sector : sector << 9, buffer);
	else
		SCDS_SDReadMultiSector(is_sdhc ? sector : sector << 9, buffer, num_sectors);
    return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool write_sectors(uint32_t sector, uint32_t num_sectors, const void *buffer)
{
	if (num_sectors == 1)
    	SCDS_SDWriteSingleSector(is_sdhc ? sector : sector << 9, buffer);
	else
		SCDS_SDWriteMultiSector(is_sdhc ? sector : sector << 9, buffer, num_sectors);
    return true;
}

// Clear error flags from the card. Returns true on success.
bool clear_status(void)
{
    return true;
}

// Shutdowns the card. This may never be called.
bool shutdown(void)
{
    return true;
}
