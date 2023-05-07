/*
io_sd_card.c

Hardware routines for interfacing with SD cards on a Ninjapass X9

Copyright (C) 2007 by CJ Bell
siegebell at gmail dot com


This software is provided 'as-is', without any express or implied warranty.  In
no event will the author be held liable for any damages arising from the use of
this software.  Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not claim
    that you wrote the original software. If you use this software in a
    product, an acknowledgment in the product documentation would be
    appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.
*/

#include <nds/ndstypes.h>
#include "io_x9_card.dldi.h"
#include "io_sd_card.dldi.h"


const unsigned int DATA_TIMEOUT    = 10000;
const unsigned int STARTUP_TIMEOUT = 1000;
const unsigned int READY_TIMEOUT   = 1000;

const unsigned int blockSize = 512; // bytes

// General-purpose buffer
static uint8 scratch[0x1000];


//extern void DumpMemory(unsigned char* data, int bytes);

//#define DO_DEBUG(statements) do { statements; } while(0)               
//#define DO_DEBUG(statements)

#define min2(a, b) ((a) < (b) ? (a) : (b))


void SDSendCommand(enum SDCommand sdCmd, uint32 arg, uint8* response)
{
    // Each byte in var represents one bit -- bit 0 of var[x]. Thus, each group
    // of eight bytes in var is "effectively" one byte from the SD card
    // 
	
    static uint8 var[0x200];
    // Send the SD command (0x60)
    // 0x80 words = 0x200 bytes
    X9CardIO(0x60, sdCmd, arg, 0xA1586000, (uint32*)var, 0x80);

    // Skips the first 7 effective bytes (k = 0x30)
    // 0x30 is the size in bits of an SD-mode response
    // the rest must be resulting data...
    int effectiveBit = 0x30;

    // Search for the first "1"/high bit
    // (This is used to find the first byte, which must start with a "1")
    int start = 0;
    int end = 0;
    while(true)
    {
        if(!(var[effectiveBit] & 1)){
			// start-bit
            // Start at the byte found
            start = effectiveBit;
            // Use 256 bytes of var to construct 32 effective bytes
            end = effectiveBit + 0x100;
            break;
        }

        ++effectiveBit;

        if(effectiveBit >= 0x200){
			// Hit the end of the search without success 
            // Skip the start-bit
            start = 1; //(var[effectiveBit-1] ^ 1) & 1; <-- known because of previous if-statement
            // Use 255 bytes of var to construct 31+7/8 effective bytes
            end = 0x100; // effectiveBit - 0x100 --> 0x200 - 0x100 --> 0x100
            break;
        }
    }

    // Covert 256 bytes of var into 32 bytes using the low-bits
    // This doesn't care what sd_cs_data is initially because each entry will
    // be shifted 8 bits.
    
	if(min2(start, end) == start)
	{
        const uint8* end_ptr = &(var[end]);
        uint8* ptr = &(var[start]);

        int bit = 7;
        int j = 0;
        while(ptr < end_ptr)
        {
            if(bit < 0)
            {// Go to the next effective byte
                bit = 7;
                ++j;
            }

            response[j] = (response[j] << 1) | (*ptr & 1);

            --bit;
            ++ptr;
        } 

    }
}

//coto: added proper retry-ability @ bool _X9SD_writeSectors
bool SDWriteSingleBlock(uint32 address, const void* buffer)
{
    // Preemptively send the data before issuing the command so
    // that the X9 can compute the CRC
    X9CardWriteData(0, 0, buffer);

    // Uses 32 bytes of scratch
    SDSendCommand(WriteSingleBlock,address,scratch);
    // throw away scratch

    // Uses 512 bytes of scratch
    // Overwrites scratch
    X9CardIO(0x63, 0, 0, 0xB15863FA, (uint32*)scratch, 0x80);

    int idx = 0;
    
	//coto: loop until buffer is available for sending
	while(true)
	{
		if(idx == 0){
			// Repeat SD command to absorb current data
			X9CardIO(0x62, 0, 0, 0xA1586000, (uint32*)scratch, 0x80);
		}
		
		if((idx < 0x200) && !(scratch[idx] & 0x10)){
			break;
		}
		
		
		if(idx < 0x200){
			idx++;
		}
		else{
			idx = 0;
			return false;	//retry later
		}
	}
	
	idx = 0;
	
	//coto: loop until buffer is available for sending
	while(true)
	{
		if(idx == 0){
			// Repeat SD command?
			X9CardIO(0x62, 0, 0, 0xA1586000, (uint32*)scratch, 0x80);
		}
		
		if((idx < 0x200) && (scratch[idx] & 0x10)){
			break;
		}
		
		
		if(idx < 0x200){
			idx++;
		}
		else{
			idx = 0;
			return false;	//retry later
		}
	}
	
    return true;

}


bool SDReadSingleBlock(uint32 address, void* destination)
{
	
    // scratch is split in two halves

    // 0x60: send an SD command
    // 0xA3586000 means we don't expect the standard response? (but data) -- serial vs 4-bit? length?
    // 0x200 words = 0x800 bytes
    X9CardIO(0x60, (uint8)ReadSingleBlock, address, 0xA1586000, (uint32*)scratch, 0x80);

    // Search for the start of data
    unsigned int k = 0;
    for(k = 0; k < 0x200 && (scratch[k] & 0xf0); ++k);

    // Continue searching
    while(k == 0x200)
    {
        // Continue reading data from SD card
        X9CardIO(0x62, 0, address, 0xA1586000, (uint32*)scratch, 0x80);
        
        // Keep searching for the response-data
        for(k = 0; k < 0x200 && (scratch[k] & 0xf0); ++k)
            ;
    }

//    // Read another chunk because the full 512 bytes haven't yet been received
//    if(0x800 - k < 0x400) // k > 0x400    
//        X9CardIO(0x62, 0, address, 0xA3586000, (uint32*)&scratch[0x800], 0x200);
    X9CardIO(0x62, 0, address, 0xA2586000, (uint32*)&scratch[0x200], 0x100);

    unsigned int readIdx = k + 1; // just after the 0xf0
    unsigned int end = readIdx + 0x400; // read 0x400 half-bytes, or 0x200 bytes

    // IO is done with half-bytes: reconstruct full bytes
    uint8* dest = (uint8*)destination;
    for(; readIdx < end; readIdx+= 2)
        *dest++ = (scratch[readIdx] & 0xF0) | (scratch[readIdx+1] >> 4);

    // Why is the last command repeated here?
    // So we can get the response?
    // NOTE: SDSendCommand uses 0xA1586000 and gets a response
    // the data-transfer commands use 0xA3586000 and expect no response...
    // Get the response from the SD-card (always after the transfer)
    X9CardIO(0x62, 0, address, 0xA1586000, (uint32*)scratch, 0x80);

    return true;
}

//
//bool SDReadMultipleBlocks(uint32 address, unsigned int count, void* destination)
//{
//    if(count == 1)
//        return SDReadSingleBlock(address, destination);
//    else if(count == 0)
//        return true;
//
//    const unsigned int readChunkSize = 0x200;
//
//    uint8 buffer[0x1000];// split in two halves
//    uint8* dest = (uint8*)destination;
//
//    // 0x60: send an SD command
//    // 0xA3586000 means we don't expect the standard response? (but data) -- serial vs 4-bit? length?
//    // 0x200 words = 0x800 bytes
//    X9CardIO(0x60, (uint8)ReadMultipleBlocks, address, 0xA1586000, (uint32*)buffer, readChunkSize/sizeof(uint32));
//    
//    // Search for the start of data
//    unsigned int readIdx = 0;
//    for(readIdx = 0; readIdx < readChunkSize && (buffer[readIdx] & 0xf0); ++readIdx)
//        ;
//
//    // Searching...
//    while(readIdx == readChunkSize)
//    {
//        // Continue reading data from SD card
//        X9CardIO(0x62, 0, address, 0xA1586000, (uint32*)buffer, readChunkSize/sizeof(uint32));
//        
//        // Keep searching for the response-data
//        for(readIdx = 0; readIdx < readChunkSize && (buffer[readIdx] & 0xf0); ++readIdx)
//            ;
//    }
//    ++readIdx;
//
//    unsigned int bytesRemaining = count * blockSize;
//    bytesRemaining-= (readChunkSize - readIdx)/2;
//
//    if(readIdx % 2 == 0)
//    {// Nybbles are aligned to byte-boundary
//
//        for(; readIdx < readChunkSize; readIdx+= 2)
//            *dest++ = (buffer[readIdx] & 0xF0) | (buffer[readIdx+1] >> 4);
//
//        // Read the remaining blocks before the last
//        while(bytesRemaining > blockSize)
//        {
//            X9CardIO(0x62, 0, address, 0xA1586000, (uint32*)buffer, blockSize*2/sizeof(uint32));
//            for(unsigned int idx = 0; idx < 2*blockSize; idx+= 2)
//                *dest++ = (buffer[idx] & 0xF0) | (buffer[idx+1] >> 4);
//
//            bytesRemaining-= blockSize;
//        }
//
//        // Read the last block
//        X9CardIO(0x62, 0, address, 0xA2586000, (uint32*)buffer, bytesRemaining*2/sizeof(uint32)+1);
//        for(unsigned int idx = 0; idx < 2*bytesRemaining; idx+= 2)
//            *dest++ = (buffer[idx] & 0xF0) | (buffer[idx+1] >> 4);
//    }
//    else
//    {
//        for(; readIdx < readChunkSize; readIdx+= 2)
//            *dest++ = (buffer[readIdx] & 0xF0) | (buffer[readIdx+1] >> 4);
//        uint8 remainder = buffer[readChunkSize-1] & 0xF0;
//
//        // Read the remaining blocks before the last
//        while(bytesRemaining > blockSize)
//        {
//            X9CardIO(0x62, 0, address, 0xA1586000, (uint32*)buffer, blockSize*2/sizeof(uint32));
//
//            *dest++ = remainder | (buffer[0] >> 4);
//
//            for(unsigned int idx = 1; idx < 2*blockSize; idx+= 2)
//                *dest++ = (buffer[idx] & 0xF0) | (buffer[idx+1] >> 4);
//            remainder = buffer[2*blockSize-1] & 0xF0;
//
//            bytesRemaining-= blockSize;
//        }
//
//        // Read the last block
//        X9CardIO(0x62, 0, address, 0xA2586000, (uint32*)buffer, bytesRemaining*2/sizeof(uint32)+1);
//        *dest++ = remainder | (buffer[0] >> 4);
//        for(unsigned int idx = 1; idx < 2*bytesRemaining; idx+= 2)
//            *dest++ = (buffer[idx] & 0xF0) | (buffer[idx+1] >> 4);
//    }
//
//    // Stop the transfer
//    SDSendCommand(AbortReadWrite, 0, buffer);
//    // Get the response from the SD-card (always after the transfer)
////    X9CardIO(0x62, 0, address, 0xA1586000, (uint32*)buffer, 0x80);
//
//    return true;
//}
//
//
//bool SDInitialize(uint32* relativeCardAddress)
//{
//    if(!relativeCardAddress)
//        return false;
//
//    unsigned int attempts;
//    uint8 response[0x20];
//
//    DO_DEBUG(iprintf("CMD0: "));
//  	// CMD0 initialize (reset)
//    SDBroadcastCommand(SetIdle,0,response);
//    DO_DEBUG(iprintf("done\n"));
//
//    // TODO: this should return the correct voltage: use that
//    DO_DEBUG(iprintf("Set"));
//    SDBroadcastCommand(AppCommand,0,response);
//    DO_DEBUG(iprintf("Voltage: "));
//    SDBroadcastAppCommand(SetOCR,0x00200000,response); // 3.3-3.4 VDD Voltage Window
//
//    //ACMD41 - identify, wait til ready..
//    // Should be repeated N times until success, else fail
//    // set voltage?
//    // sd_cs_data set by sendSDCommand()
//    for(attempts = 0; response[1] != 0x80 /* Ready */; ++attempts)
//    {
//        DO_DEBUG(iprintf("nope\n"));
//
//        if(attempts >= STARTUP_TIMEOUT)
//        {
//            DO_DEBUG(iprintf("SetVoltage: giving up\n"));
//            return false;
//        }
//
//        DO_DEBUG(iprintf("Set"));
//        SDBroadcastCommand(AppCommand,0,response);
//        DO_DEBUG(iprintf("Voltage: "));
//        SDBroadcastAppCommand(SetOCR,0x00100000,response); // 3.2-3.3 VDD Voltage Window
//    }
//    DO_DEBUG(iprintf("done\n"));
//
//    // Retrieves Card's Name
//    // CMD2 get CID
//    DO_DEBUG(iprintf("GetCID: "));
//    SDBroadcastCommand(GetCID,0,response);
//    DO_DEBUG(iprintf("done\n"));
//
//    // Should be repeated N time for success, else fail
//    attempts = 0;
//    do
//    {
//        DO_DEBUG(if(attempts > 0) iprintf("nope\n"));
//
//        if(attempts >= READY_TIMEOUT)
//        {
//            DO_DEBUG(iprintf("GetRCA: giving up\n"));
//            return false;
//        }
//
//        DO_DEBUG(iprintf("GetRCA: "));
//        SDBroadcastCommand(GetRCA,0,response);
//        ++attempts;
//    } while((response[3] & 0x1e) == (SDStandbyState << 1)); // busy
//    DO_DEBUG(iprintf("done\n"));
//
//    *relativeCardAddress = (response[1] << 24) | (response[2] << 16);
//
//
//    // Some cards won't go to higher speeds unless they think you
//    // checked their capabilities
//    SDBroadcastCommand(GetCSD,*relativeCardAddress,response);
//
//    //AL go SD data mode
//    DO_DEBUG(iprintf("SelectCard: "));
//    SDBroadcastCommand(SelectCard,*relativeCardAddress,response);
//    DO_DEBUG(iprintf("done\n"));
//
//
//    //AL set SD bus width
//    DO_DEBUG(iprintf("Set"));
//    SDBroadcastCommand(AppCommand,*relativeCardAddress,response);
//    DO_DEBUG(iprintf("BusWidth: "));
//    SDBroadcastAppCommand(SetBusWidth,2,response); // 4-bit bus
//    DO_DEBUG(iprintf("done\n"));
//
//    SDBroadcastCommand(SetBlockLength,512,response);
//
//    return true;
//}


inline void SDSendAppCommand(enum SDAppCommand sdAppCmd, uint32 arg, uint8* response)
{
    SDSendCommand((enum SDCommand)sdAppCmd,arg,response);
}

inline void SDBroadcastCommand(enum SDCommand sdCmd, uint32 arg, uint8* response)
{
    SDSendCommand((enum SDCommand)(sdCmd|Broadcast),arg,response);
}

inline void SDBroadcastAppCommand(enum SDAppCommand sdAppCmd, uint32 arg, uint8* response)
{
    SDSendCommand((enum SDCommand)(sdAppCmd|Broadcast),arg,response);
}
