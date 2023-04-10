
#include <nds/ndstypes.h>
#define BYTES_PER_READ 512
#ifndef NULL
 #define NULL 0
#endif

// Card bus
#define	REG_CARD_DATA_RD	(*(vu32*)0x04100010)

#define REG_AUXSPICNTH	(*(vu8*)0x040001A1)
#define REG_ROMCTRL		(*(vu32*)0x040001A4)

#define REG_CARD_COMMAND	((vu8*)0x040001A8)

#define CARD_CR1_ENABLE  0x80  // in byte 1, i.e. 0x8000
#define CARD_CR1_IRQ     0x40  // in byte 1, i.e. 0x4000

// 3 bits in b10..b8 indicate something
// read bits
#define CARD_BUSY         (1<<31)           // when reading, still expecting incomming data?
#define CARD_DATA_READY   (1<<23)           // when reading, CARD_DATA_RD or CARD_DATA has another word of data and is good to go

#if defined(SCDS)
bool is_sdhc = false;
#endif

void cardWriteCommand(const uint8 * command) {
	int index;

	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;

	for (index = 0; index < 8; index++) {
		REG_CARD_COMMAND[7-index] = command[index];
	}
}

void cardWaitReady(u32 flags, u8 *command)
{
	bool ready = false;

	do {
		cardWriteCommand(command);
		REG_ROMCTRL = flags;
		do {
			if (REG_ROMCTRL & CARD_DATA_READY)
				if (!REG_CARD_DATA_RD) ready = true;
		} while (REG_ROMCTRL & CARD_BUSY);
	} while (!ready);
}

void cardPolledTransfer(u32 flags, u32 *destination, u32 length, const u8 *command) {
	u32 data;
	cardWriteCommand(command);
	REG_ROMCTRL = flags;
	u32 * target = destination + length;
	do {
		// Read data if available
		if (REG_ROMCTRL & CARD_DATA_READY) {
			data=REG_CARD_DATA_RD;
			if (NULL != destination && destination < target)
				*destination++ = data;
		}
	} while (REG_ROMCTRL & CARD_BUSY);
}

void bytecardPolledTransfer(uint32 flags, uint32 * destination, uint32 length, uint8 * command) {
	u32 data;;
	cardWriteCommand(command);
	REG_ROMCTRL = flags;
	uint32 * target = destination + length;
	do {
		// Read data if available
		if (REG_ROMCTRL & CARD_DATA_READY) {
			data=REG_CARD_DATA_RD;
			if (destination < target) {
				((uint8*)destination)[0] = data & 0xff;
				((uint8*)destination)[1] = (data >> 8) & 0xff;
				((uint8*)destination)[2] = (data >> 16) & 0xff;
				((uint8*)destination)[3] = (data >> 24) & 0xff;
			}
			destination++;
		}
	} while (REG_ROMCTRL & CARD_BUSY);
}

u32 cardWaitReply(u8 *command)
{
	u32 ret;
	command[7] = 0x50;
	cardWaitReady(0xa7180000, command);
	command[7] = 0x52;
	cardPolledTransfer(0xa7180000, &ret, 1, command);
	return ret;
}

#if defined(SCDS)
u32 sdmode_sdhc(void) {
	u8 command[8];
	u32 ret;
	u32 address=0x7fa00-0x20;
	command[7] = 0x70;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8) & 0xff;
	command[3] = address & 0xff;
	command[2] = 0;
	command[1] = 0;
	command[0] = 0;
	cardPolledTransfer(0xa7180000, &ret, 1, command);
	return ret;
}
#endif // SCDS

void LogicCardRead(u32 *destination, u32 length)
{
	u8 command[8];
	for(int i=0;i<8;i++) command[i] = 0;
	u32 ret = 0;

	command[7] = 0x80;
	do {
		cardPolledTransfer(0xa7180000, &ret, 1, command);
	} while(ret);
	command[7] = 0x81;
	if ((u32)destination & 0x03)
		bytecardPolledTransfer(0xa1180000, destination, length, command);
	else
		cardPolledTransfer(0xa1180000, destination, length, command);
}

void LogicCardWrite(u32 *source, u32 length)
{
	u8 command[8];
	for(int i=0;i<8;i++) command[i] = 0;
	u32 data = 0;

	REG_CARD_COMMAND[0] = 0x82;
	REG_ROMCTRL = 0xe1180000;
	u32 * target = source + length;
	do {
		// Write data if ready
		if (REG_ROMCTRL & CARD_DATA_READY) {
			if (source < target) {
				if ((u32)source & 0x03)
					data = ((uint8*)source)[0] | (((uint8*)source)[1] << 8) | (((uint8*)source)[2] << 16) | (((uint8*)source)[3] << 24);
				else
					data = *source;
			}
			source++;
			REG_CARD_DATA_RD = data;
		}
	} while (REG_ROMCTRL & CARD_BUSY);

	// wait until complete
	command[7] = 0x50;
	cardWaitReady(0xa7180000, command);
}

bool startup(void)
{
	/*	
		If is_sdhc is 0, then the BSS has been reset
		and we need to check for SDHC again.
		On the DSTT, if the BSS has been reset, then 
		this is undefined behaviour, as 0x023FFC24 is 
		volatile memory, so the only time we get reliable
		SD/SDHC check is on boot, from the DSTT bootloader
		So DSTT must have FIX_BSS unset in the header. 
		That said, not all DLDI patchers care about what 
		is shown in header, but we get there when we get 
		there...
	*/
#if defined(SCDS)
	is_sdhc = sdmode_sdhc();
#endif
	return true;
}

bool isInserted(void)
{
	return true;
}

bool readSectors(u32 sector, u32 numSectors, void* buffer)
{
	u32 *u32_buffer = (u32*)buffer;

	u8 command[8];

#if defined(SCDS)
	u32 address = is_sdhc == 1 ? sector : sector << 9;
#else // not SCDS
	u32 address = sector;
#endif

	bool batchread = numSectors > 1;

	// init
	command[7] = batchread ? 0x54 : 0x53;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8)  & 0xff;
	command[3] =  address        & 0xff;
	command[2] = 0;
	command[1] = 0;
	command[0] = 0;
	cardPolledTransfer(0xa7180000, NULL, 0, command);

	do {
		LogicCardRead(u32_buffer, 128);
		u32_buffer += 128;
		if (--numSectors && batchread) {
			command[7] = 0x51;
			command[6] = 0;
			command[5] = 0;
			command[4] = 0;
			command[3] = 0;
			command[2] = 0;
			command[1] = 7;
			command[0] = 0;
			cardPolledTransfer(0xa7180000, NULL, 1, command);
		}
	} while (numSectors);

	if (batchread) {
		command[7] = 0x51;
		command[6] = 0;
		command[5] = 0;
		command[4] = 0;
		command[3] = 0;
		command[2] = 12;
		command[1] = 1;
		command[0] = 0;
		cardPolledTransfer(0xa7180000, NULL, 1, command);
		cardWaitReply(command);
	}

	return true;
}

bool writeSectors(u32 sector, u32 numSectors, void* buffer)
{
	u32 *u32_buffer = (u32*)buffer;

	u8 command[8];

#if defined(SCDS)
	u32 address = is_sdhc == 1 ? sector : sector << 9;
#else // not SCDS
	u32 address = sector;
#endif

	bool batchwrite = numSectors > 1;

	// init
	command[7] = 0x51;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8)  & 0xff;
	command[3] =  address        & 0xff;
	command[2] = batchwrite ? 25 : 24;
	command[1] = 1;
	command[0] = 0;
	cardPolledTransfer(0xa7180000, NULL, 0, command);
	cardWaitReply(command);

	do {
		// write
		LogicCardWrite(u32_buffer, 128);
		u32_buffer += 128;
		// if this is the last chunk, let card know
		// only applies to batch writes
		if (!--numSectors && batchwrite) {
			command[7] = 0x51;
			command[6] = 0;
			command[5] = 0;
			command[4] = 0;
			command[3] = 0;
			command[2] = 12;
			command[1] = 1;
			command[0] = 0;
			cardPolledTransfer(0xa7180000, NULL, 0, command);
			cardWaitReply(command);
		}
		// finish
		command[7] = 0x56;
		cardPolledTransfer(0xa7180000, NULL, 0, command);
		command[7] = 0x50;
		cardWaitReady(0xa7180000, command);
	} while (numSectors);

	return true;
}

bool clearStatus(void)
{
	return true;
}

bool shutdown(void)
{
	return true;
}