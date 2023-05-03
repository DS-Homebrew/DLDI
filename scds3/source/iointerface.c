
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

void cardWaitReply(void)
{
	u8 command[8];

	command[7] = 0x30;
	command[6] = 64;
	for (int i = 0; i < 6; i++) command[i] = 0;
	cardPolledTransfer(0xa7180000, NULL, 1, command);
	command[7] = 0x38;
	cardWaitReady(0xa7180000, command);
}

void LogicCardRead(u32 address, u32 *destination, u32 length)
{
	u8 command[8];

	command[7] = 0x33;
	command[6] = 0;
	command[5] = 17 | 0x40;
	command[4] = (address >> 24) & 0xff;
	command[3] = (address >> 16) & 0xff;
	command[2] = (address >> 8)  & 0xff;
	command[1] =  address        & 0xff;
	command[0] = 1;
	cardPolledTransfer(0xa7180000, NULL, 1, command);
	command[7] = 0x38;
	cardWaitReady(0xa7180000, command);

	command[7] = 0x34;
	for (int i = 0; i < 7; i++) command[i] = 0;
	cardPolledTransfer(0xa7180000, NULL, 1, command);
	command[7] = 0x38;
	cardWaitReady(0xa7180000, command);
	command[7] = 0x36;
	if ((u32)destination & 0x03)
		bytecardPolledTransfer(0xa1180000, destination, length, command);
	else
		cardPolledTransfer(0xa1180000, destination, length, command);
}

void LogicCardReadMulti(u32 address, u32 *destination, u32 numSectors)
{
	u8 command[8];

	command[7] = 0x33;
	command[6] = 0;
	command[5] = 18 | 0x40;
	command[4] = (address >> 24) & 0xff;
	command[3] = (address >> 16) & 0xff;
	command[2] = (address >> 8)  & 0xff;
	command[1] =  address        & 0xff;
	command[0] = 1;
	cardPolledTransfer(0xa7180000, NULL, 1, command);
	command[7] = 0x38;
	cardWaitReady(0xa7180000, command);

	for(int i = 0; i < numSectors; i++) {
		command[7] = 0x34;
		for (int j = 0; j < 7; j++) command[j] = 0;
		cardPolledTransfer(0xa7180000, NULL, 1, command);
		command[7] = 0x38;
		cardWaitReady(0xa7180000, command);
		command[7] = 0x36;
		if ((u32)destination & 0x03)
			bytecardPolledTransfer(0xa1180000, destination, 128, command);
		else
			cardPolledTransfer(0xa1180000, destination, 128, command);
		destination += 128;
	};
	command[7] = 0x33;
	command[5] = 12 | 0x40;
	for (int i = 0; i < 5; i++) command[i] = 0;
	cardPolledTransfer(0xa7180000, NULL, 1, command);
	cardWaitReply();
}

void LogicCardWrite(u32 address, u32 *source, u32 length)
{
	u8 command[8];

	command[7] = 0x33;
	command[6] = 0;
	command[5] = 24 | 0x40;
	command[4] = (address >> 24) & 0xff;
	command[3] = (address >> 16) & 0xff;
	command[2] = (address >> 8)  & 0xff;
	command[1] =  address        & 0xff;
	command[0] = 1;
	cardPolledTransfer(0xa7180000, NULL, 1, command);
	command[7] = 0x38;
	cardWaitReady(0xa7180000, command);
	cardWaitReply();

	command[7] = 0x37;
	command[0] = 0;
	// lol
	for (u16 i = 0; i < 512; i+=4) {
		command[6] = (i >> 8) & 0xff;
		command[5] = i & 0xff;
		command[4] = ((uint8*)source)[0];
		command[3] = ((uint8*)source)[1];
		command[2] = ((uint8*)source)[2];
		command[1] = ((uint8*)source)[3];
		cardPolledTransfer(0xa7180000, NULL, 1, command);
		source++;
	}
	command[7] = 0x35;
	cardPolledTransfer(0xa7180000, NULL, 1, command);
	command[7] = 0x38;
	cardWaitReady(0xa7180000, command);
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

	if(numSectors == 1) LogicCardRead(sector << 9, u32_buffer, 128);
	else LogicCardReadMulti(sector << 9, u32_buffer, numSectors);

	return true;
}

bool writeSectors(u32 sector, u32 numSectors, void* buffer)
{
	u32 *u32_buffer = (u32*)buffer, i;

	for (i = 0; i < numSectors; i++) {
		LogicCardWrite(sector << 9, u32_buffer, 128);
		sector++;
		u32_buffer += 128;
	}
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
