
#include <nds/ndstypes.h>
#define BYTES_PER_READ 512
#ifndef NULL
 #define NULL 0
#endif

// Card bus
#define CARD_CR1       (*(vuint16*)0x040001A0)
#define CARD_CR1H      (*(vuint8*)0x040001A1)
#define CARD_EEPDATA   (*(vuint8*)0x040001A2)
#define CARD_CR2       (*(vuint32*)0x040001A4)
#define CARD_COMMAND   ((vuint8*)0x040001A8)

#define CARD_DATA_RD   (*(vuint32*)0x04100010)

#define CARD_1B0       (*(vuint32*)0x040001B0)
#define CARD_1B4       (*(vuint32*)0x040001B4)
#define CARD_1B8       (*(vuint16*)0x040001B8)
#define CARD_1BA       (*(vuint16*)0x040001BA)


#define CARD_CR1_ENABLE  0x80  // in byte 1, i.e. 0x8000
#define CARD_CR1_IRQ     0x40  // in byte 1, i.e. 0x4000


// CARD_CR2 register:

#define CARD_ACTIVATE   (1<<31)  // when writing, get the ball rolling
// 1<<30
#define CARD_nRESET     (1<<29)  // value on the /reset pin (1 = high out, not a reset state, 0 = low out = in reset)
#define CARD_28         (1<<28)  // when writing
#define CARD_27         (1<<27)  // when writing
#define CARD_26         (1<<26)
#define CARD_22         (1<<22)
#define CARD_19         (1<<19)
#define CARD_ENCRYPTED  (1<<14)  // when writing, this command should be encrypted
#define CARD_13         (1<<13)  // when writing
#define CARD_4          (1<<4)   // when writing

// 3 bits in b10..b8 indicate something
// read bits
#define CARD_BUSY       (1<<31)  // when reading, still expecting incomming data?
#define CARD_DATA_READY (1<<23)  // when reading, CARD_DATA_RD or CARD_DATA has another word of data and is good to go

void cardWriteCommand(const uint8 * command) {
	int index;

	CARD_CR1H = CARD_CR1_ENABLE | CARD_CR1_IRQ;

	for (index = 0; index < 8; index++) {
		CARD_COMMAND[7-index] = command[index];
	}
}

void cardWaitReady(u32 flags, u8 *command)
{
	bool ready = false;

	do {
		cardWriteCommand(command);
		CARD_CR2 = flags;
		do {
			if (CARD_CR2 & CARD_DATA_READY)
				if (!CARD_DATA_RD) ready = true;
		} while (CARD_CR2 & CARD_BUSY);
	} while (!ready);
}

void cardPolledTransfer(uint32 flags, uint32 * destination, uint32 length, const uint8 * command) {
	u32 data;;
	cardWriteCommand(command);
	CARD_CR2 = flags;
	uint32 * target = destination + length;
	do {
		// Read data if available
		if (CARD_CR2 & CARD_DATA_READY) {
			data=CARD_DATA_RD;
			if (destination < target)
				*destination = data;
			destination++;
		}
	} while (CARD_CR2 & CARD_BUSY);
}

void bytecardPolledTransfer(uint32 flags, uint32 * destination, uint32 length, uint8 * command) {
	u32 data;;
	cardWriteCommand(command);
	CARD_CR2 = flags;
	uint32 * target = destination + length;
	do {
		// Read data if available
		if (CARD_CR2 & CARD_DATA_READY) {
			data=CARD_DATA_RD;
			if (destination < target) {
				((uint8*)destination)[0] = data & 0xff;
				((uint8*)destination)[1] = (data >> 8) & 0xff;
				((uint8*)destination)[2] = (data >> 16) & 0xff;
				((uint8*)destination)[3] = (data >> 24) & 0xff;
			}
			destination++;
		}
	} while (CARD_CR2 & CARD_BUSY);
}

void LogicCardRead(u32 address, u32 *destination, u32 length)
{
	u8 command[8];

	command[7] = 0xb9;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8)  & 0xff;
	command[3] =  address        & 0xff;
	command[2] = 0;
	command[1] = 0;
	command[0] = 0;
	cardWaitReady(0xa7586000, command);
	command[7] = 0xba;
	if ((u32)destination & 0x03)
		bytecardPolledTransfer(0xa1586000, destination, length, command);
	else
		cardPolledTransfer(0xa1586000, destination, length, command);
}

u32 ReadCardInfo()
{
	u8 command[8];
	u32 ret;

	command[7] = 0xb0;
	command[6] = 0;
	command[5] = 0;
	command[4] = 0;
	command[3] = 0;
	command[2] = 0;
	command[1] = 0;
	command[0] = 0;
	cardPolledTransfer(0xa7586000, &ret, 1, command);
	return ret;
}

void LogicCardWrite(u32 address, u32 *source, u32 length)
{
	u8 command[8];
	u32 data = 0;

	command[7] = 0xbb;
	command[6] = (address >> 24) & 0xff;
	command[5] = (address >> 16) & 0xff;
	command[4] = (address >> 8)  & 0xff;
	command[3] =  address        & 0xff;
	command[2] = 0;
	command[1] = 0;
	command[0] = 0;
	cardWriteCommand(command);
	CARD_CR2 = 0xe1586000;
	u32 * target = source + length;
	do {
		// Write data if ready
		if (CARD_CR2 & CARD_DATA_READY) {
			if (source < target) {
				if ((u32)source & 0x03)
					data = ((uint8*)source)[0] | (((uint8*)source)[1] << 8) | (((uint8*)source)[2] << 16) | (((uint8*)source)[3] << 24);
				else
					data = *source;
			}
			source++;
			CARD_DATA_RD = data;
		}
	} while (CARD_CR2 & CARD_BUSY);
	command[7] = 0xbc;
	cardWaitReady(0xa7586000, command);
}

bool startup(void)
{
	u32 CardInfo;

	CardInfo = ReadCardInfo();
	return ((CardInfo & 0x07) == 0x04);
}

bool isInserted(void)
{
	u32 CardInfo;

	CardInfo = ReadCardInfo();
	return ((CardInfo & 0x07) == 0x04);
}

bool readSectors(u32 sector, u32 numSecs, void* buffer)
{
	u32 *u32_buffer = (u32*)buffer, i;

	for (i = 0; i < numSecs; i++) {
		LogicCardRead(sector << 9, u32_buffer, 128);
		sector++;
		u32_buffer += 128;
	}
	return true;
}

bool writeSectors(u32 sector, u32 numSecs, void* buffer)
{
	u32 *u32_buffer = (u32*)buffer, i;

	for (i = 0; i < numSecs; i++) {
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
