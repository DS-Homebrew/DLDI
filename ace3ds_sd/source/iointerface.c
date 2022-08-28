#include <nds/ndstypes.h>

#include "common.h"

#define cardWriteCommand() (REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ)

static inline void cardWaitReady(u32 flags){
	bool ready = false;
	do{
		cardWriteCommand();
		REG_ROMCTRL = flags;
		do{
			if(REG_ROMCTRL & CARD_DATA_READY)
				if(!REG_CARD_DATA_RD)ready = true;
		}while(REG_ROMCTRL & CARD_BUSY);
	}while(!ready);
}

static inline void _cardPolledTransfer(u32 flags, u32 *destination, u32 length){
	u32 data;
	cardWriteCommand();
	REG_ROMCTRL = flags;
	u32 *target = destination + length;
	do{
		// Read data if available
		if(REG_ROMCTRL & CARD_DATA_READY){
			data=REG_CARD_DATA_RD;
			if(destination < target)
				*destination++ = data;
		}
	}while(REG_ROMCTRL & CARD_BUSY);
}

static inline void bytecardPolledTransfer(u32 flags, u32 *destination, u32 length){
	u32 data;
	cardWriteCommand();
	REG_ROMCTRL = flags;
	u32 *target = destination + length;
	do{
		// Read data if available
		if(REG_ROMCTRL & CARD_DATA_READY){
			data=REG_CARD_DATA_RD;
			if(destination < target){
				((u8*)destination)[0] = data & 0xff;
				((u8*)destination)[1] = (data >> 8) & 0xff;
				((u8*)destination)[2] = (data >> 16) & 0xff;
				((u8*)destination)[3] = (data >> 24) & 0xff;
				destination++;
			}
		}
	}while(REG_ROMCTRL & CARD_BUSY);
}

static inline void LogicCardRead(u32 address, u32 *destination, u32 length){
	REG_CARD_COMMAND[0] = 0xb9;
	REG_CARD_COMMAND[1] = (address >> 24) & 0xff;
	REG_CARD_COMMAND[2] = (address >> 16) & 0xff;
	REG_CARD_COMMAND[3] = (address >> 8)  & 0xff;
	REG_CARD_COMMAND[4] =  address        & 0xff;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[7] = 0;
	cardWaitReady(0xa7586000);
	REG_CARD_COMMAND[0] = 0xba;
	if((u32)destination & 0x03)
		bytecardPolledTransfer(0xa1586000, destination, length);
	else
		_cardPolledTransfer(0xa1586000, destination, length);
}

/* unused for now, as function doesn't work on non-EX4DS
static inline u32 ReadCardInfo(){
	u32 ret;

	REG_CARD_COMMAND[0] = 0xb0;
	REG_CARD_COMMAND[1] = 0;
	REG_CARD_COMMAND[2] = 0;
	REG_CARD_COMMAND[3] = 0;
	REG_CARD_COMMAND[4] = 0;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;
	_cardPolledTransfer(0xa7586000, &ret, 1);
	return ret;
}
*/

static inline void LogicCardWrite(u32 address, u32 *source, u32 length){
	u32 data = 0;
	REG_CARD_COMMAND[0] = 0xbb;
	REG_CARD_COMMAND[1] = (address >> 24) & 0xff;
	REG_CARD_COMMAND[2] = (address >> 16) & 0xff;
	REG_CARD_COMMAND[3] = (address >> 8)  & 0xff;
	REG_CARD_COMMAND[4] =  address        & 0xff;
	REG_CARD_COMMAND[5] = 0;
	REG_CARD_COMMAND[6] = 0;
	REG_CARD_COMMAND[7] = 0;
	cardWriteCommand();
	REG_ROMCTRL = 0xe1586000;
	// Write data if ready
	u32 *target = source + length;
	if((u32)source & 0x03){
		do{
			if(REG_ROMCTRL & CARD_DATA_READY){
				if(source < target){
					data = ((u8*)source)[0] | (((u8*)source)[1] << 8) | (((u8*)source)[2] << 16) | (((u8*)source)[3] << 24);
					source++;
				}
				REG_CARD_DATA_RD = data;
			}
		}while(REG_ROMCTRL & CARD_BUSY);
	}else{
		do{
			if(REG_ROMCTRL & CARD_DATA_READY){
				if(source < target)
					data = *source++;
				REG_CARD_DATA_RD = data;
			}
		}while(REG_ROMCTRL & CARD_BUSY);
	}
	REG_CARD_COMMAND[0] = 0xbc;
	cardWaitReady(0xa7586000);
}

// DLDI frontend
bool startup() {
	// only works on EX4DS, which isn't what we want
	// return ((ReadCardInfo() & 0x04) == 0x04); //lol
	return true;
}

bool isInserted() {
	// only works on EX4DS, which isn't what we want
	// return ((ReadCardInfo() & 0x04) == 0x04); //lol
	return true;
}

// Actual disk access
bool readSectors(u32 sector, u32 numSecs, void* buffer){
	u32 *u32_buffer = (u32*)buffer, i;

	for (i = 0; i < numSecs; i++) {
		LogicCardRead(sector, u32_buffer, 128);
		sector++;
		u32_buffer += 128;
	}
	return true;
}

bool writeSectors(u32 sector, u32 numSecs, void* buffer){
	u32 *u32_buffer = (u32*)buffer, i;

	for (i = 0; i < numSecs; i++) {
		LogicCardWrite(sector, u32_buffer, 128);
		sector++;
		u32_buffer += 128;
	}
	return true;
}

bool clearStatus() {
	return true;
}

bool shutdown() {
	return true;
}
