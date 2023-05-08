// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <nds/ndstypes.h>

#include "ior4.h"
#include "card.h"

// Initialize the driver. Returns true on success.
bool startup(void)
{
	// Confirm card is actually responding and is actually an R4
	return (ioR4SendCommand(IOR4_CMD_CARD_INFO) & 7) == 4;
}

// Returns true if a card is present and initialized.
bool is_inserted(void)
{
	// Confirm card is actually responding and is actually an R4
	return (ioR4SendCommand(IOR4_CMD_CARD_INFO) & 7) == 4;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool read_sectors(uint32_t sector, uint32_t num_sectors, void *buffer)
{
	for (int i = 0; i < num_sectors; i++)
	{
		ioR4SDReadSector((sector + i) << 9, buffer);
		buffer = (u8 *)buffer + 0x200;
	}
	return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool write_sectors(uint32_t sector, uint32_t num_sectors, const void *buffer)
{
	for (int i = 0; i < num_sectors; i++)
	{
		ioR4SDWriteSector((sector + i) << 9, buffer);
		buffer = (u8 *)buffer + 0x200;
	}
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
