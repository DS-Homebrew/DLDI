// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2023
// SPDX-FileContributor: lifehackerhansol, 2023

#include <nds/ndstypes.h>
#include <nds/card.h>

#include "iorpg.h"
#include "libtwl_card.h"

// Initialize the driver. Returns true on success.
bool startup(void)
{
	//if(ioRPG_SendCommand(((u64)CARD_CMD_DATA_CHIPID << 56)) != 0xFC2)
	//	return false;
	return ioRPG_Initialize();
}

// Returns true if a card is present and initialized.
bool is_inserted(void)
{
	return ioRPG_SendCommand(((u64)CARD_CMD_DATA_CHIPID << 56)) == 0xFC2;
}

// Reads 512 byte sectors into a buffer that may be unaligned. Returns true on
// success.
bool read_sectors(uint32_t sector, uint32_t num_sectors, void *buffer)
{
	if(num_sectors == 1)
		ioRPG_SDReadSingleSector(sector, buffer);
	else
		ioRPG_SDReadMultiSector(sector, num_sectors, buffer);
	return true;
}

// Writes 512 byte sectors from a buffer that may be unaligned. Returns true on
// success.
bool write_sectors(uint32_t sector, uint32_t num_sectors, const void *buffer)
{
#ifndef TARGET_HAS_BROKEN_CMD25
	if(num_sectors == 1)
		ioRPG_SDWriteSingleSector(sector, buffer);
	else
		ioRPG_SDWriteMultiSector(sector, num_sectors, buffer);
#else
	for(int i = 0; i < num_sectors; i++) {
		ioRPG_SDWriteSingleSector(sector + i, buffer);
		buffer = (u8 *)buffer + 0x200;
	}
#endif
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
