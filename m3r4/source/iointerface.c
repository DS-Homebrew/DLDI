#include <nds/ndstypes.h>
#include <nds/card.h>

#include <stdio.h> //NULL

#if defined(SCDS)
int sdhc_shift;
extern u32 _io_dldi;
#endif

#if 0
static inline void cardWriteCommand(){
	//int index;

	REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ;

	//for(index = 0; index < 8; index++){
	//	CARD_COMMAND[7-index] = command[index];
	//}
}
#endif

#define cardWriteCommand() (REG_AUXSPICNTH = CARD_CR1_ENABLE | CARD_CR1_IRQ)

#if defined(IPLY)
static inline void IPLY_cardWaitReady(u32 flags, u32 bit){
	bool ready = false;
	do{
		cardWriteCommand();
		REG_ROMCTRL = flags;
		do{
			if(REG_ROMCTRL & CARD_DATA_READY)
				if((CARD_DATA_RD>>bit)&1)ready = true;
		}while(REG_ROMCTRL & CARD_BUSY);
	}while(!ready);
}
#else
static inline void cardWaitReady(u32 flags){
	bool ready = false;
	do{
		cardWriteCommand();
		REG_ROMCTRL = flags;
		do{
			if(REG_ROMCTRL & CARD_DATA_READY)
				if(!CARD_DATA_RD)ready = true;
		}while(REG_ROMCTRL & CARD_BUSY);
	}while(!ready);
}
#endif

static inline void _cardPolledTransfer(u32 flags, u32 *destination, u32 length){
	u32 data;
	cardWriteCommand();
	REG_ROMCTRL = flags;
	u32 *target = destination + length;
	do{
		// Read data if available
		if(REG_ROMCTRL & CARD_DATA_READY){
			data=CARD_DATA_RD;
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
			data=CARD_DATA_RD;
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

#if defined(IPLY)
/*
static inline void IPLY_clearcmd(u32 cmd){
	CARD_COMMAND[0]=cmd;
	CARD_COMMAND[1]=CARD_COMMAND[2]=CARD_COMMAND[3]=CARD_COMMAND[4]=CARD_COMMAND[5]=CARD_COMMAND[6]=CARD_COMMAND[7]=0;
}
*/
#define IPLY_clearcmd(cmd) (CARD_COMMAND[0]=(cmd),CARD_COMMAND[1]=CARD_COMMAND[2]=CARD_COMMAND[3]=CARD_COMMAND[4]=CARD_COMMAND[5]=CARD_COMMAND[6]=CARD_COMMAND[7]=0)
#endif

static inline void LogicCardRead(u32 address, u32 *destination, u32 length){
#if defined(G003)
	address<<=1;
	CARD_COMMAND[0] = 0xc9;
#elif defined(IPLY)
	IPLY_clearcmd(0xe1);
	_cardPolledTransfer(0xa7180010, NULL, 0);
	CARD_COMMAND[0] = 0x50;
#elif defined(M3DS)
	CARD_COMMAND[0] = 0xbd;
#elif defined(SCDS)
	CARD_COMMAND[0] = 0x53;
#else
	CARD_COMMAND[0] = 0xb9;
#endif
	CARD_COMMAND[1] = (address >> 24) & 0xff;
	CARD_COMMAND[2] = (address >> 16) & 0xff;
	CARD_COMMAND[3] = (address >> 8)  & 0xff;
	CARD_COMMAND[4] =  address        & 0xff;
	CARD_COMMAND[5] = 0;
#if defined(IPLY)
	CARD_COMMAND[6] = 2;
#else
	CARD_COMMAND[6] = 0;
#endif
	CARD_COMMAND[7] = 0;

#if defined(IPLY)
	_cardPolledTransfer(0xa0180010, NULL, 0);
	IPLY_clearcmd(0xe0);
	IPLY_cardWaitReady(0xa7180010, 28);
	IPLY_clearcmd(0xe8);
	if((u32)destination & 0x03)
		bytecardPolledTransfer(0xa1180010, destination, length);
	else
		_cardPolledTransfer(0xa1180010, destination, length);
#elif defined(SCDS)
	_cardPolledTransfer(0xa7180000, NULL, 0);
	CARD_COMMAND[0] = 0x80;
	cardWaitReady(0xa7180000);
	CARD_COMMAND[0] = 0x81;
	if((u32)destination & 0x03)
		bytecardPolledTransfer(0xa1180000, destination, length);
	else
		_cardPolledTransfer(0xa1180000, destination, length);
#else
	cardWaitReady(0xa7586000);
#if defined(G003)
	CARD_COMMAND[0] = 0xca;
#else
	CARD_COMMAND[0] = 0xba;
#endif
	if((u32)destination & 0x03)
		bytecardPolledTransfer(0xa1586000, destination, length);
	else
		_cardPolledTransfer(0xa1586000, destination, length);
#endif
}

#if !defined(SCDS) && !defined(IPLY)
static inline u32 ReadCardInfo(){
	u32 ret;

	CARD_COMMAND[0] = 0xb0;
	CARD_COMMAND[1] = 0;
	CARD_COMMAND[2] = 0;
	CARD_COMMAND[3] = 0;
	CARD_COMMAND[4] = 0;
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 0;
	CARD_COMMAND[7] = 0;
	_cardPolledTransfer(0xa7586000, &ret, 1);
	return ret;
}
#endif

static inline void LogicCardWrite(u32 address, u32 *source, u32 length){
	u32 data = 0;

#if defined(IPLY)
	IPLY_clearcmd(0xe1);
	_cardPolledTransfer(0xa7180010, NULL, 0);
	IPLY_clearcmd(0xe9);
	cardWriteCommand();
	REG_ROMCTRL = 0xe1180010;

	// Write data if ready
	u32 *target = source + length;
	if((u32)source & 0x03){
		do{
			if(REG_ROMCTRL & CARD_DATA_READY){
				if(source < target){
					data = ((u8*)source)[0] | (((u8*)source)[1] << 8) | (((u8*)source)[2] << 16) | (((u8*)source)[3] << 24);
					source++;
				}
				CARD_DATA_RD = data;
			}
		}while(REG_ROMCTRL & CARD_BUSY);
	}else{
		do{
			if(REG_ROMCTRL & CARD_DATA_READY){
				if(source < target)
					data = *source++;
				CARD_DATA_RD = data;
			}
		}while(REG_ROMCTRL & CARD_BUSY);
	}

	CARD_COMMAND[0] = 0x51;
	CARD_COMMAND[1] = (address >> 24) & 0xff;
	CARD_COMMAND[2] = (address >> 16) & 0xff;
	CARD_COMMAND[3] = (address >> 8)  & 0xff;
	CARD_COMMAND[4] =  address        & 0xff;
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 2;
	CARD_COMMAND[7] = 0;
	_cardPolledTransfer(0xa0180010, NULL, 0);
	IPLY_clearcmd(0xe0);
	IPLY_cardWaitReady(0xa7180010, 21);
#else //not IPLY
#if defined(G003)
	address<<=1;
	CARD_COMMAND[0] = 0xc5;
#elif defined(M3DS)
	CARD_COMMAND[0] = 0xbe;
#elif defined(SCDS)
	CARD_COMMAND[0] = 0x51;
#else
	CARD_COMMAND[0] = 0xbb;
#endif
	CARD_COMMAND[1] = (address >> 24) & 0xff;
	CARD_COMMAND[2] = (address >> 16) & 0xff;
	CARD_COMMAND[3] = (address >> 8)  & 0xff;
	CARD_COMMAND[4] =  address        & 0xff;

#if defined(SCDS)
	CARD_COMMAND[5] = 24;
	CARD_COMMAND[6] = 1;
	CARD_COMMAND[7] = 0;
	_cardPolledTransfer(0xa7180000, NULL, 0);
	CARD_COMMAND[0] = 0x50;
	cardWaitReady(0xa7180000);
	CARD_COMMAND[0] = 0x52;
	_cardPolledTransfer(0xa7180000, NULL, 0);
	CARD_COMMAND[0] = 0x82;
	cardWriteCommand();
	REG_ROMCTRL = 0xe1180000;

	// Write data if ready
	u32 *target = source + length;
	if((u32)source & 0x03){
		do{
			if(REG_ROMCTRL & CARD_DATA_READY){
				if(source < target){
					data = ((u8*)source)[0] | (((u8*)source)[1] << 8) | (((u8*)source)[2] << 16) | (((u8*)source)[3] << 24);
					source++;
				}
				CARD_DATA_RD = data;
			}
		}while(REG_ROMCTRL & CARD_BUSY);
	}else{
		do{
			if(REG_ROMCTRL & CARD_DATA_READY){
				if(source < target)
					data = *source++;
				CARD_DATA_RD = data;
			}
		}while(REG_ROMCTRL & CARD_BUSY);
	}
	CARD_COMMAND[0] = 0x50;
	cardWaitReady(0xa7180000);
	CARD_COMMAND[0] = 0x56;
	_cardPolledTransfer(0xa7180000, NULL, 0);
	CARD_COMMAND[0] = 0x50;
	cardWaitReady(0xa7180000);
#else //not SCDS
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 0;
	CARD_COMMAND[7] = 0;
	cardWriteCommand();
#if defined(M3DS)
	REG_ROMCTRL = 0xe1586100;
#else
	REG_ROMCTRL = 0xe1586000;
#endif
	// Write data if ready
	u32 *target = source + length;
	if((u32)source & 0x03){
		do{
			if(REG_ROMCTRL & CARD_DATA_READY){
				if(source < target){
					data = ((u8*)source)[0] | (((u8*)source)[1] << 8) | (((u8*)source)[2] << 16) | (((u8*)source)[3] << 24);
					source++;
				}
				CARD_DATA_RD = data;
			}
		}while(REG_ROMCTRL & CARD_BUSY);
	}else{
		do{
			if(REG_ROMCTRL & CARD_DATA_READY){
				if(source < target)
					data = *source++;
				CARD_DATA_RD = data;
			}
		}while(REG_ROMCTRL & CARD_BUSY);
	}
#if defined(G003)
	CARD_COMMAND[0] = 0xc6;
#else
	CARD_COMMAND[0] = 0xbc;
#endif
	cardWaitReady(0xa7586000);
#endif //not SCDS
#endif //not IPLY
}

// DLDI frontend
bool startup(){
#if defined(IPLY)
	return true;
#elif defined(SCDS)
	u32 addr=0x7f9e0,ret;
	CARD_COMMAND[0] = 0xb9;
	CARD_COMMAND[1] = (addr >> 24) & 0xff;
	CARD_COMMAND[2] = (addr >> 16) & 0xff;
	CARD_COMMAND[3] = (addr >> 8)  & 0xff;
	CARD_COMMAND[4] =  addr        & 0xff;
	CARD_COMMAND[5] = 0;
	CARD_COMMAND[6] = 0;
	CARD_COMMAND[7] = 0;
	_cardPolledTransfer(0xa7180000, &ret, 1);
	if(ret!=0x32564353)return false; //SCV2

	if(_io_dldi==0x53444353){ //SCDS
		CARD_COMMAND[0] = 0x70;
		_cardPolledTransfer(0xa7180000, &ret, 1); //it seems DSTT always return true...
		sdhc_shift=ret?0:9;
		*(u32*)0x023ffc24=ret?1:0;
	}else{ //TTIO or DEMO
		sdhc_shift=(*(u32*)0x023ffc24)?0:9; //I assume ttreset is done on DSTT.
	}
	return true;
#elif defined(EX4TF)
	return ((ReadCardInfo() & 0x04) == 0x04); //lol
#else
	return ((ReadCardInfo() & 0x07) == 0x04);
#endif
}

// Stub
#if defined(IPLY)
__attribute__((alias("startup"))) bool clearStatus();
#else
bool clearStatus(){
	return true;
}
#endif

__attribute__((alias("clearStatus"))) bool shutdown();

/*
bool shutdown(){
	return true;
}
*/

#if defined(SCDS)
__attribute__((alias("clearStatus"))) bool isInserted();
#else
__attribute__((alias("startup"))) bool isInserted();
#endif

/*
bool isInserted(){
	return ((ReadCardInfo() & 0x07) == 0x04);
}
*/

// Actual disk access
bool readSectors(u32 sector, u32 numSecs, void* buffer){
	u32 *u32_buffer = (u32*)buffer, i;

	for (i = 0; i < numSecs; i++) {
#if defined(M3DS) || defined(G003) || defined(EX4TF) || defined(IPLY)
		LogicCardRead(sector, u32_buffer, 128);
#elif defined(SCDS)
		LogicCardRead(sector << sdhc_shift, u32_buffer, 128);
#else
		LogicCardRead(sector << 9, u32_buffer, 128);
#endif
		sector++;
		u32_buffer += 128;
	}
	return true;
}

bool writeSectors(u32 sector, u32 numSecs, void* buffer){
	u32 *u32_buffer = (u32*)buffer, i;

	for (i = 0; i < numSecs; i++) {
#if defined(M3DS) || defined(G003) || defined(EX4TF) || defined(IPLY)
		LogicCardWrite(sector, u32_buffer, 128);
#elif defined(SCDS)
		LogicCardWrite(sector << sdhc_shift, u32_buffer, 128);
#else
		LogicCardWrite(sector << 9, u32_buffer, 128);
#endif
		sector++;
		u32_buffer += 128;
	}
	return true;
}
