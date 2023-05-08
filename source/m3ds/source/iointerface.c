// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <nds/ndstypes.h>

#include "iom3.h"
#include "card.h"

// Initialize the driver. Returns true on success.
bool startup(void)
{
	// Confirm card is actually responding and is actually an M3
	return (ioM3SendCommand(IOM3_CMD_CARD_INFO) & 7) == 4;
}

// Returns true if a card is present and initialized.
bool is_inserted(void)
{
	// Confirm card is actually responding and is actually an M3
	return (ioM3SendCommand(IOM3_CMD_CARD_INFO) & 7) == 4;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool read_sectors(uint32_t sector, uint32_t num_sectors, void *buffer)
{
	if (num_sectors == 1)
		ioM3SDReadSingleSector(sector, buffer);
	else
		ioM3SDReadMultiSector(sector, buffer, num_sectors);

	return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool write_sectors(uint32_t sector, uint32_t num_sectors, const void *buffer)
{
	if (num_sectors == 1)
		ioM3SDWriteSingleSector(sector, buffer);
	else
		ioM3SDWriteMultiSector(sector, buffer, num_sectors);

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
