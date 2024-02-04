// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <nds/ndstypes.h>

#include "scdssdhc.h"
#include "libtwl_card.h"

// Initialize the driver. Returns true on success.
bool startup(void)
{
#ifndef TTIO
	SCDS_SDGetSDHCStatusFromSRAM();
	return true;
#else
	return SCDS_SDInitialize();
#endif
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
// Some devices based on TTIO breaks when trying to do both types of reads,
// so stick with multi-sector reads.
#ifndef TTIO
	if (num_sectors == 1)
		SCDS_SDReadSingleSector(sector, buffer);
	else
#endif
		SCDS_SDReadMultiSector(sector, buffer, num_sectors);
	return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool write_sectors(uint32_t sector, uint32_t num_sectors, const void *buffer)
{
	if (num_sectors == 1)
		SCDS_SDWriteSingleSector(sector, buffer);
	else
		SCDS_SDWriteMultiSector(sector, buffer, num_sectors);
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
