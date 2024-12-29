/*
	io_mpsd.c by ps2aich
	
	based on 
	
	io_m2sd.c by Movie Advance

	Hardware Routines for reading sd card
	using the GBA Movie Player SD Adapter

	based on
	
	iointerface.c template
	
 Copyright (c) 2006 Michael "Chishm" Chisholm
	
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

  1. All derivative works must be clearly marked as such. Derivatives of this file 
	 must have the author of the derivative indicated within the source.  
  2. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


// When compiling for NDS, make sure NDS is defined
#ifndef NDS
 #if defined ARM9 || defined ARM7
  #define NDS
 #endif
#endif

#ifdef NDS
 #include <nds/ndstypes.h>
#else
 #include "gba_types.h"
#endif

#define BYTES_PER_READ 512

#ifndef NULL
 #define NULL 0
#endif

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
#define CARD_TIMEOUT	(500*100)		// mpsd timeout nomal:500



//---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
//---------------------------------------------------------------

extern u8 mpsd_crc7_s(u16* buff,u16 num);
	
#ifdef __cplusplus
}	   // extern "C"
#endif

static u8 ppp7[8];

static inline void wait1()
{
	asm (
	"mov r0,r0\n"
	"mov r0,r0\n"
	"mov r0,r0\n"
	"mov r0,r0\n"
	);
}

static inline void wait5()
{
	wait1();
	wait1();
	wait1();
	wait1();
	wait1();
}

void send_byte2(u16 input)
{	
	// u16 j;
	
	SDODA=input;//obyte[7:0]=input
	SDDIR=0x29;
	// for(j=0;j<5;j++);
	wait5();
	SDDIR=0x09;
}

u16 sendcommand2(void)
{
	SDCON=0x8;
	/////////////////////////////////////
	//send command
	ppp7[5]=mpsd_crc7_s((u16*)ppp7,5);
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
	// u16 i;

	SDDIR=0x0;//send=0,get=0 clk low
	// for(i=0;i<1;i++);
	wait1();
	SDDIR=0x8;//send=0,get=0 clk high
}

void readdata(u16* p, u32 sec)
{
	u16 data;
	u32 i;
	u16 w;
	u8 *p_unaligned = (u8*) p;
	volatile u8 *data_unaligned = (u8*) &data;
	
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
		// *(p+w) = SDDIR;
		data = SDDIR;
		*(p_unaligned + (w * 2)) = *data_unaligned;
		*(p_unaligned + ((w * 2) +1)) = *(data_unaligned + 1);
		
	}
	w = SDDIR;
	w = SDDIR;
	//showdata(p);while(1){}
}

void writedata(u16* p, u32 sec)
{
	u16 data;
	volatile u32 i;
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
	SDDIR = 0xFFFE;
	for(w=0;w<0x100;w++)
	{
		SDDIR = *(p+w);
	}
	i = 0;
	while ( ( (SDSTA&0xFF) == 0x00)&&(i < CARD_TIMEOUT) )
	{
		i++;
	}
}


//======================================================
bool MPSD_read1sector(u32 sectorn,u32 TAddr)
{
	ppp7[0]=0x40+17;
	ppp7[4]=0;	
	
	ppp7[3] = (u8)(sectorn<<1);	
	ppp7[2] = (u8)(sectorn>>7);	
	ppp7[1] = (u8)(sectorn>>15);	
	
	sendcommand2();//respond 110000090067
	
	readdata((u16*)TAddr,sectorn);

	return true;
} 

bool MPSD_write1sector(u32 sectorn,u32 TAddr)
{
	ppp7[0]=0x40+0x18;
	ppp7[4]=0;	
	
	ppp7[3] = (u8)(sectorn<<1);	
	ppp7[2] = (u8)(sectorn>>7);	
	ppp7[1] = (u8)(sectorn>>15);	
	
	sendcommand2();
	
	writedata((u16*)TAddr,sectorn);

	return true;
} 

/*-----------------------------------------------------------------
startUp
Initialize the interface, geting it into an idle, ready state
returns true if successful, otherwise returns false
-----------------------------------------------------------------*/
bool startup(void) {
	// ppp7[0]=0x40+17;
	// ppp7[4]=0;	
	
	
	
	
	return true;
}

/*-----------------------------------------------------------------
isInserted
Is a card inserted?
return true if a card is inserted and usable
-----------------------------------------------------------------*/
bool isInserted (void) {
	return true;
}


/*-----------------------------------------------------------------
clearStatus
Reset the card, clearing any status errors
return true if the card is idle and ready
-----------------------------------------------------------------*/
bool clearStatus (void) {
	return startup();
}


/*-----------------------------------------------------------------
readSectors
Read "numSectors" 512-byte sized sectors from the card into "buffer", 
starting at "sector". 
The buffer may be unaligned, and the driver must deal with this correctly.
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool readSectors (u32 sector, u32 numSectors, void* buffer) {
	bool r=true;
	int i;
	for(i=0;i<numSectors;i++)
	{
		if(MPSD_read1sector(i + sector , 512*i + (u32) buffer )==false)
		{
			r=false;
			break;
		}
	}
	return r;
}



/*-----------------------------------------------------------------
writeSectors
Write "numSectors" 512-byte sized sectors from "buffer" to the card, 
starting at "sector".
The buffer may be unaligned, and the driver must deal with this correctly.
return true if it was successful, false if it failed for any reason
-----------------------------------------------------------------*/
bool writeSectors (u32 sector, u32 numSectors, void* buffer) {
	bool r=true;
	int i;
	for(i=0;i<numSectors;i++)
	{
		if(MPSD_write1sector(i + sector , 512*i + (u32) buffer )==false)
		{
			r=false;
			break;
		}
	}
	return r;
}

/*-----------------------------------------------------------------
shutdown
shutdown the card, performing any needed cleanup operations
Don't expect this function to be called before power off, 
it is merely for disabling the card.
return true if the card is no longer active
-----------------------------------------------------------------*/
bool shutdown(void) {
	return true;
}
