/*
	SuperCard DSONE
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "scdssdhc.h"
#include "libtwl_card.h"

static u32 isSDHC = 0;

static u32 SCDS_SDHostSetMode(u8 sdio, u32 parameter, u8 response_type, u32 latency)
{
	u64 command = ((u64)SCDS_CMD_SD_HOST_PARAM << 56) | ((u64)parameter << 24) | ((u64)sdio << 16) | ((u64)response_type << 8);
	card_romSetCmd(command);
	card_romStartXfer(SCDS_CTRL_READ_4B | MCCNT1_LATENCY1(latency), false);
	card_romWaitDataReady();
	return card_romGetData();
}

static u32 SCDS_IsSDHostBusy(void)
{
	REG_SCDS_MCCMD[0] = SCDS_CMD_SD_HOST_BUSY;
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return card_romGetData();
}

static u32 SCDS_SDHostGetResponse(void)
{
	REG_SCDS_MCCMD[0] = SCDS_CMD_SD_HOST_RESPONSE;
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return __builtin_bswap32(card_romGetData());
}

static void SCDS_SDSendR0Command(u8 sdio, u32 parameter, u32 latency)
{
	SCDS_SDHostSetMode(sdio, parameter, SCDS_SD_HOST_NORESPONSE, latency);
	while(SCDS_IsSDHostBusy());
}

static u32 SCDS_SDSendR1Command(u8 sdio, u32 parameter, u32 latency)
{
	SCDS_SDHostSetMode(sdio, parameter, SCDS_SD_HOST_READ_4B, latency);
	while(SCDS_IsSDHostBusy());
	return SCDS_SDHostGetResponse();
}

// TODO: save the response to a buffer (also figure out in which order they're sent)
static void SCDS_SDSendR2Command(u8 sdio, u32 parameter, u32 latency)
{
	SCDS_SDHostSetMode(sdio, parameter, SCDS_SD_HOST_READ_4B_MULTI, latency);
	while(SCDS_IsSDHostBusy());

	// TODO: parse this response
	SCDS_SDHostGetResponse();

	for(int i=0; i < 4; i++)
	{
		SCDS_SDHostSetMode(sdio, parameter, SCDS_SD_HOST_NEXT_4B, latency);
		while(SCDS_IsSDHostBusy());
		// TODO: parse this response
		SCDS_SDHostGetResponse();
	}
	SCDS_SDHostSetMode(sdio, parameter, SCDS_SD_HOST_SEND_STOP_CLK, 0);
	while(SCDS_IsSDHostBusy());
}

void waitByLoop(u32 count);

static void SCDS_SDHostSetRegister(u8 bits)
{
	u64 command = ((u64)SCDS_CMD_SD_HOST_SET_REGISTER << 56) | ((u64)(0x30 | bits) << 48);
	card_romSetCmd(command);
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	card_romGetData();
	waitByLoop(0x300);
}

static u32 SCDS_IsSDFIFOBusy(void) {
	REG_SCDS_MCCMD[0] = SCDS_CMD_FIFO_BUSY;
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return card_romGetData();
}

// Reads max 512 bytes
static void SCDS_SDFIFOReadData(void *buffer, u32 length)
{
	REG_SCDS_MCCMD[0] = SCDS_CMD_FIFO_READ_DATA;
	card_romStartXfer(SCDS_CTRL_READ_512B, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

// Writes max 512 bytes
static void SCDS_SDFIFOWriteData(const void *buffer, u32 length)
{
	REG_SCDS_MCCMD[0] = SCDS_CMD_FIFO_WRITE_DATA;
	card_romStartXfer(SCDS_CTRL_WRITE_512B, false);
	if ((u32)buffer & 3)
		card_romCpuWriteUnaligned((u8 *)buffer, length);
	else
		card_romCpuWrite(buffer, length);
}

static u32 SCDS_SRAMReadData(u32 address)
{
	u64 command = ((u64)SCDS_CMD_SRAM_READ_DATA << 56) | ((u64)address << 48);
	card_romSetCmd(command);
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return card_romGetData();
}

void SCDS_SDGetSDHCStatusFromSRAM(void)
{
	isSDHC = SCDS_SRAMReadData(0x7F9E0) != 0 ? 1 : 0;
}

bool SCDS_SDInitialize(void)
{
	u32 isSD20 = 0;
	u32 response = 0;

	isSDHC = 0;

	// TODO: What is this command doing?
	card_romSetCmd(0x6600000000000000ull);
	card_romStartXfer(0xA7586000, false);
	card_romWaitDataReady();
	card_romGetData();

	// Reset SD host
	SCDS_SDHostSetRegister(0);

	// Init
	SCDS_SDHostSetRegister(SCDS_SD_HOST_REG_RESET | SCDS_SD_HOST_REG_400KHZ_CLK | SCDS_SD_HOST_REG_CLEAN_ROM_MODE);
	SCDS_SDHostSetMode(0, 0, SCDS_SD_HOST_SEND_CLK, SCDS_CTRL_SD_LOW_CLK_LATENCY);
	waitByLoop(0x1000);

	// CMD0
	SCDS_SDSendR0Command(0, 0, SCDS_CTRL_SD_LOW_CLK_LATENCY);
	SCDS_SDHostSetMode(0, 0, SCDS_SD_HOST_SEND_STOP_CLK, SCDS_CTRL_SD_LOW_CLK_LATENCY);

	// CMD8
	SCDS_SDHostSetMode(8, 0x1AA, SCDS_SD_HOST_READ_4B, SCDS_CTRL_SD_LOW_CLK_LATENCY);

	u32 retryCount = 9999;
	while(1)
	{
		if(!SCDS_IsSDHostBusy())
		{
			response = SCDS_SDHostGetResponse();
			break;
		}
		if (--retryCount == 0)
		{
			SCDS_SDHostSetRegister(0);
			SCDS_SDHostSetRegister(SCDS_SD_HOST_REG_RESET | SCDS_SD_HOST_REG_400KHZ_CLK | SCDS_SD_HOST_REG_CLEAN_ROM_MODE);
			response = 0;
			break;
		}
	}

	if(response == 0x1AA)
		isSD20 = 1;

	do
	{
		// CMD55
		SCDS_SDHostSetMode(55, 0, SCDS_SD_HOST_READ_4B, SCDS_CTRL_SD_LOW_CLK_LATENCY);
		retryCount = 9999;
		while(SCDS_IsSDHostBusy())
		{
			if (--retryCount == 0)
			{
				SCDS_SDHostSetRegister(0);
				SCDS_SDHostSetRegister(SCDS_SD_HOST_REG_RESET | SCDS_SD_HOST_REG_400KHZ_CLK | SCDS_SD_HOST_REG_CLEAN_ROM_MODE);
				return false;
			}
		}
		SCDS_SDHostGetResponse();

		// ACMD41
		u32 parameter = 0x00FC0000;
		if(isSD20)
			parameter |= BIT(30);
		response = SCDS_SDSendR1Command(41, parameter, SCDS_CTRL_SD_LOW_CLK_LATENCY);
	} while(!(response & BIT(31)));

	isSDHC = response & BIT(30) ? 1 : 0;

	// CMD2
	SCDS_SDSendR2Command(2, 0, SCDS_CTRL_SD_LOW_CLK_LATENCY);

	// CMD3
	response = SCDS_SDSendR1Command(3, 0, SCDS_CTRL_SD_LOW_CLK_LATENCY);
	u32 sdio_rca = response & 0xFFFF0000;

	// CMD7
	SCDS_SDSendR1Command(7, sdio_rca, SCDS_CTRL_SD_LOW_CLK_LATENCY);

	// ACMD6
	SCDS_SDSendR1Command(55, sdio_rca, SCDS_CTRL_SD_LOW_CLK_LATENCY);
	SCDS_SDSendR1Command(6, 2, SCDS_CTRL_SD_LOW_CLK_LATENCY);

	// CMD16
	SCDS_SDSendR1Command(16, 512, SCDS_CTRL_SD_LOW_CLK_LATENCY);

	SCDS_SDHostSetRegister(0);
	SCDS_SDHostSetRegister(SCDS_SD_HOST_REG_RESET | SCDS_SD_HOST_REG_CLEAN_ROM_MODE);
	if(isSDHC)
		SCDS_SDHostSetRegister(SCDS_SD_HOST_REG_RESET | SCDS_SD_HOST_REG_CLEAN_ROM_MODE | SCDS_SD_HOST_REG_SDHC);

	return true;
}

void SCDS_SDReadSingleSector(u32 sector, void *buffer)
{
	u64 command = ((u64)SCDS_CMD_SD_READ_SINGLE_BLOCK << 56) | ((u64)(isSDHC ? sector : sector << 9) << 24);
	card_romSetCmd(command);
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	card_romGetData();

	// wait until data ready
	while(SCDS_IsSDFIFOBusy());

	// retrieve data
	SCDS_SDFIFOReadData(buffer, 128);
}

void SCDS_SDReadMultiSector(u32 sector, void *buffer, u32 num_sectors)
{
	u64 command = ((u64)SCDS_CMD_SD_READ_MULTI_BLOCK << 56) | ((u64)(isSDHC ? sector : sector << 9) << 24);
	card_romSetCmd(command);
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	card_romGetData();

	while(1)
	{
		// wait until data ready
		while(SCDS_IsSDFIFOBusy());

		// retrieve data
		SCDS_SDFIFOReadData(buffer, 128);
		buffer = (u8 *)buffer + 0x200;
		num_sectors--;
		if(num_sectors == 0)
			break;
		SCDS_SDHostSetMode(0, 0, SCDS_SD_HOST_NEXT_DATABLOCK, 0);
	};

	// end read
	SCDS_SDSendR1Command(12, 0, 0);
}

void SCDS_SDWriteSingleSector(u32 sector, const void *buffer)
{
	// instruct cart where to write
	SCDS_SDSendR1Command(24, isSDHC ? sector : sector << 9, 0);

	// write
	SCDS_SDFIFOWriteData(buffer, 128);
	while(SCDS_IsSDHostBusy());

	// end write
	REG_SCDS_MCCMD[0] = SCDS_CMD_SD_WRITE_END;
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	card_romGetData();
	while(SCDS_IsSDHostBusy());
}

void SCDS_SDWriteMultiSector(u32 sector, const void *buffer, u32 num_sectors)
{
	// instruct cart where to write
	SCDS_SDSendR1Command(25, isSDHC ? sector : sector << 9, 0);

	while (num_sectors--) {
		// end write
		// well, it's supposed to be end write. But doing it first is a no-op, and it's also
		// a little simpler to write this way
		REG_SCDS_MCCMD[0] = SCDS_CMD_SD_WRITE_END;
		card_romStartXfer(SCDS_CTRL_READ_4B, false);
		card_romWaitDataReady();
		card_romGetData();
		while(SCDS_IsSDHostBusy());
		// write
		SCDS_SDFIFOWriteData(buffer, 128);
		while(SCDS_IsSDHostBusy());
		buffer = (u8 *)buffer + 0x200;
	}

	// *really* end write
	SCDS_SDSendR1Command(12, 0, 0);
	REG_SCDS_MCCMD[0] = SCDS_CMD_SD_WRITE_END;
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	card_romGetData();
	while(SCDS_IsSDHostBusy());
}
