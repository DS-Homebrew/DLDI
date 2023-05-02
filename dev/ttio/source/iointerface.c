
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

#if defined(SDHC)
bool is_sdhc = true;
#else
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

void LogicCardRead(u32 address, u32 *destination, u32 length)
{
	u8 command[8];

	command[7] = 0x53;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8)  & 0xff;
	command[3] =  address        & 0xff;
	command[2] = 0;
	command[1] = 0;
	command[0] = 0;
	cardPolledTransfer(0xa7180000, NULL, 0, command);
	command[7] = 0x80;
	cardWaitReady(0xa7180000, command);
	command[7] = 0x81;
	if ((u32)destination & 0x03)
		bytecardPolledTransfer(0xa1180000, destination, length, command);
	else
		cardPolledTransfer(0xa1180000, destination, length, command);
}

void LogicCardReadMulti(u32 address, u32 *destination, u32 numSectors) {
	u8 command[8];

	command[7] = 0x54;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8)  & 0xff;
	command[3] =  address        & 0xff;
	command[2] = 0;
	command[1] = 0;
	command[0] = 0;
	cardPolledTransfer(0xa7180000, NULL, 0, command);

	do {
		command[7] = 0x80;
		cardWaitReady(0xa7180000, command);
		command[7] = 0x81;
		if ((u32)destination & 0x03)
			bytecardPolledTransfer(0xa1180000, destination, 128, command);
		else
			cardPolledTransfer(0xa1180000, destination, 128, command);
		destination += 128;
		if (--numSectors) {
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

void LogicCardWrite(u32 address, u32 *source, u32 length)
{
	u8 command[8];
	u32 data = 0;

	command[7] = 0x51;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8)  & 0xff;
	command[3] =  address        & 0xff;
	command[2] = 24;
	command[1] = 0;
	command[0] = 0;
	cardPolledTransfer(0xa7180000, NULL, 0, command);
	cardWaitReply(command);

	for(int i=0;i<7;i++) command[i] = 0;
	command[7] = 0x82;
	cardWriteCommand(command);
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
	command[7] = 0x50;
	cardWaitReady(0xa7180000, command);
	command[7] = 0x56;
	cardPolledTransfer(0xa7180000, NULL, 0, command);
	command[7] = 0x50;
	cardWaitReady(0xa7180000, command);
}

void LogicCardWriteMulti(u32 address, u32 *source, u32 numSectors)
{
	u8 command[8];
	u32 data = 0;
	u32 length = 128; // 512-byte writes per chunk

	command[7] = 0x51;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8)  & 0xff;
	command[3] =  address        & 0xff;
	command[2] = 25;
	command[1] = 0;
	command[0] = 0;
	cardPolledTransfer(0xa7180000, NULL, 0, command);
	cardWaitReply(command);

	for(int i=0;i<7;i++) command[i] = 0;
	do {
		command[7] = 0x82;
		cardWriteCommand(command);
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

		// if this is the last chunk, let card know
		if (!--numSectors) {
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

		command[7] = 0x50;
		cardWaitReady(0xa7180000, command);
		command[7] = 0x56;
		cardPolledTransfer(0xa7180000, NULL, 0, command);
		command[7] = 0x50;
		cardWaitReady(0xa7180000, command);
	} while (numSectors);
}

bool startup(void)
{
	return true;
}

bool isInserted(void)
{
	return true;
}

bool readSectors(u32 sector, u32 numSectors, void* buffer)
{
	u32 *u32_buffer = (u32*)buffer;

	if(numSectors == 1)	LogicCardRead(is_sdhc ? sector : sector << 9, u32_buffer, 128);
	else LogicCardReadMulti(is_sdhc ? sector : sector << 9, u32_buffer, numSectors);

	return true;
}

bool writeSectors(u32 sector, u32 numSectors, void* buffer)
{
	u32 *u32_buffer = (u32*)buffer;

	if(numSectors == 1) LogicCardWrite(is_sdhc ? sector : sector << 9, u32_buffer, 128);
	else LogicCardWriteMulti(is_sdhc ? sector : sector << 9, u32_buffer, numSectors);

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
