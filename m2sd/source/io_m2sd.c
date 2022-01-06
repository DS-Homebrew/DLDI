/*
	io_m2sd.c based on io_m3cf.c by SaTa.

	io_m3cf.c based on

	compact_flash.c
	By chishm (Michael Chisholm)

	Hardware Routines for reading a compact flash card
	using the M3 Perfect CF Adapter

	CF routines modified with help from Darkfader

	Made as DLDI driver by Rocket Robz

	This software is completely free. No warranty is provided.
	If you use it, please give me credit and email me about your
	project at chishm@hotmail.com

	See gba_nds_fat.txt for help and license details.
*/


#include "io_m2sd.h"


//SD dir control bit cmddir=bit0 clken=bit1
//output
#define SDDIR			(*(volatile u16*)0x8800000)

//SD send get control bit send=bit0 get=bit1
//output
#define SDCON			(*(volatile u16*)0x9800000)

//SD output data obyte[7:0]=AD[7:0]
//output
#define SDODA			(*(volatile u16*)0x9000000)

//SD input data AD[7:0]=ibyte[7:0]
//input
#define SDIDA			(*(volatile u16*)0x9000000)

//readsector data1
#define SDIDA1			(*(volatile u16*)0x9200000)

//readsector data2
#define SDIDA2			(*(volatile u16*)0x9400000)

//readsector data3
#define SDIDA3			(*(volatile u16*)0x9600000)

//SD stutas cmdneg=bit0 cmdpos=bit1 issend=bit2 isget=bit3
//input
#define SDSTA			(*(volatile u16*)0x9800000)

//#define CARD_TIMEOUT	10000000		// Updated due to suggestion from SaTa, otherwise card will timeout sometimes on a write
#define CARD_TIMEOUT	(500*100)		// M2SD timeout nomal:500

//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------------

extern u8 sd_crc7_s(u16* buff,u16 num);
#ifdef __cplusplus
}	   // extern "C"
#endif

u8 ppp7[8];

void send_byte2(u16 input)
{	
	u16 j;
	
	SDODA=input;//obyte[7:0]=input
	SDDIR=0x29;
	for(j=0;j<5;j++);
	SDDIR=0x09;
}

u16 sendcommand2(void)
{
	SDCON=0x8;
	/////////////////////////////////////
	//send command
	ppp7[5]=sd_crc7_s((u16*)ppp7,5);
	send_byte2(ppp7[0]);
	send_byte2(ppp7[1]);
	send_byte2(ppp7[2]);
	send_byte2(ppp7[3]);
	send_byte2(ppp7[4]);
	send_byte2(ppp7[5]);
	SDDIR=0x08;//cmd input clken=0 datadir=0 clock=1
	///////////////////////////////////
	return 1;
}

void CLK(void)
{
	u16 i;

	SDDIR=0x0;//send=0,get=0 clk low
	for(i=0;i<1;i++);
	SDDIR=0x8;//send=0,get=0 clk high
}

void readdata(u16* p, u32 sec)
{
	u16 data;
	u32 i;
	u16 w;
	
	i=0;
	while(!i)
	{
		CLK();
		data=SDIDA;
		if(!(data&0x0f00)) i=1;
	}
	
	SDDIR=0x8;//cmd input clken=0 datadir input clock=0
	SDCON=0x4;//send=0 get=0 en25=1 cmd1=0
	w = SDDIR;
	for(w=0;w<0x100;w++)
	{
		*(p+w) = SDDIR;
	}
	w = SDDIR;
	w = SDDIR;
	//showdata(p);while(1){}
}

//======================================================
bool M2SD_read1sector(u32 sectorn,u32 TAddr)
{
	ppp7[3] = (u8)(sectorn<<1);	
	ppp7[2] = (u8)(sectorn>>7);	
	ppp7[1] = (u8)(sectorn>>15);	

	sendcommand2();//respond 110000090067
	
	readdata((u16*)TAddr,sectorn);

	return true;
/*
	u32 i;
	int w;
	
	SDCON=0x8;		//	bit3:コマンドモード？
	SDIDA1=0x40+17;		//	コマンド CMD17
	SDIDA2=(sectorn>>7);//	セクタH 9ビット=アドレスH １６ビット
	SDIDA3=(sectorn<<9);//	セクタL 7ビット=アドレスL １６ビット
	SDDIR=0x29;		//	コマンド送信？
	i=0;

	while ( ((SDSTA&0x01) != 0x01)&&(i < CARD_TIMEOUT) )
	{
		i++;
	}
	SDDIR=0x09;
	i=0;
	SDDIR=0x49;
	while ( ((SDSTA&0x40) != 0x40)&&(i < CARD_TIMEOUT) )
	{
		i++;
	}
	SDDIR=0x09;
		
	SDDIR=0x8;//cmd input clken=0 datadir input clock=0
	SDCON=0x4;//send=0 get=0 en25=1 cmd1=0

	w = SDDIR;
	for(w=0;w<0x100;w++)
	{
		*(u16*)(TAddr+w*2) = SDDIR;
	}
	w = SDDIR;
	w = SDDIR;
	
	if (i >= CARD_TIMEOUT)
		return false;

	return true;
	*/
} 
//==================================================


//======================================================
bool M2SD_write1sector(u32 sectorn,u32 TAddr)
{
/*	u32 i;
	int w;
	
	SDCON=0x8;
	SDIDA1=0x40+0x18;// CMD24=write
	SDIDA2=(sectorn>>7);
	SDIDA3=(sectorn<<9);
	SDDIR=0x29;
	i=0;
	while ( ((SDSTA&0x01) != 0x01)&&(i < CARD_TIMEOUT) )
	{
		i++;
	}
	SDDIR=0x09;
	i=0;
	SDDIR=0x49;
	while ( ((SDSTA&0x40) != 0x40)&&(i < CARD_TIMEOUT) )
	{
		i++;
	}
	SDDIR=0x09;
		
	SDDIR=0x8;//cmd input clken=0 datadir input clock=0
	SDCON=0x4;//send=0 get=0 en25=1 cmd1=0

	w = SDDIR;
	SDDIR = 0xFFFE;
	for(w=0;w<0x100;w++)
	{
		SDDIR = *(u16*)(TAddr+w*2);
	}
	//書き込み完了を待つ != 0x00
	while ( ( (SDSTA&0xFF) == 0x00)&&(i < CARD_TIMEOUT) )
	{
		i++;
	}

	if (i >= CARD_TIMEOUT)
		return false;
*/
	return true;
	
} 
//==================================================



/*-----------------------------------------------------------------
M2SD_IsInserted
Is a compact flash card inserted?
bool return OUT:  true if a CF card is inserted
-----------------------------------------------------------------*/
bool isInserted (void) 
{
	return true;
}


/*-----------------------------------------------------------------
M2SD_ClearStatus
Tries to make the CF card go back to idle mode
bool return OUT:  true if a CF card is idle
-----------------------------------------------------------------*/
bool clearStatus (void) 
{

	return true;
}


/*-----------------------------------------------------------------
M2SD_ReadSectors
Read 512 byte sector numbered "sector" into "buffer"
u32 sector IN: address of first 512 byte sector on CF card to read
u8 numSecs IN: number of 512 byte sectors to read,
 1 to 256 sectors can be read, 0 = 256
void* buffer OUT: pointer to 512 byte buffer to store data in
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool readSectors (u32 sector, u8 numSecs, void* buffer)
{
/*#ifndef NDS
	return false;
#endif

#ifdef _CF_ALLOW_UNALIGNED
	return false;
#endif

#ifndef _CF_USE_DMA
	return false;
#endif*/

	//void M2SD_read1sector(u32 sectorn,u32 TAddr)
	bool r=true;
	int i;
	for(i=0;i<numSecs;i++)
	{
		if(M2SD_read1sector(i + sector , 512*i + (u32) buffer )==false)
		{
			r=false;
			break;
		}
	}
	return r;

}



/*-----------------------------------------------------------------
M2SD_WriteSectors
Write 512 byte sector numbered "sector" from "buffer"
u32 sector IN: address of 512 byte sector on CF card to read
u8 numSecs IN: number of 512 byte sectors to read,
 1 to 256 sectors can be read, 0 = 256
void* buffer IN: pointer to 512 byte buffer to read data from
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool writeSectors (u32 sector, u8 numSecs, void* buffer)
{
/*
	bool r=true;
	int i;
	for(i=0;i<numSecs;i++)
	{
		if(M2SD_write1sector(i + sector , 512*i + (u32) buffer )==false)
		{
			r=false;
			break;
		}
	}
	return r;
*/
	return false;


}


/*-----------------------------------------------------------------
M3_Unlock
Returns true if M3 was unlocked, false if failed
Added by MightyMax
-----------------------------------------------------------------*/
bool M2SD_Unlock(void) 
{

	return true;
}

bool shutdown(void) {
	return clearStatus() ;
} ;

bool startup(void) {
	ppp7[0]=0x40+17;
	ppp7[4]=0;
	return M2SD_Unlock() ;
} ;


/*IO_INTERFACE io_m2sd = {
	0x4453324D,	// 'M2SD'
	FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE,
	(FN_MEDIUM_STARTUP)&M2SD_StartUp,
	(FN_MEDIUM_ISINSERTED)&M2SD_IsInserted,
	(FN_MEDIUM_READSECTORS)&M2SD_ReadSectors,
	(FN_MEDIUM_WRITESECTORS)&M2SD_WriteSectors,
	(FN_MEDIUM_CLEARSTATUS)&M2SD_ClearStatus,
	(FN_MEDIUM_SHUTDOWN)&M2SD_Shutdown
} ;


IO_INTERFACE M2SD_GetInterface(void) {
	return io_m2sd ;
} ;*/
