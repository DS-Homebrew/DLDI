/*
io_x9sd.c

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


//#ifdef NDS

//#include <stdio.h>
//#include <string.h>
#include <nds/ndstypes.h>
#include "io_sd_card.dldi.h"
#include "io_x9_card.dldi.h"
//#include "io_sd_common.h"
#include "io_x9sd.dldi.h"

#include "tonccpy.h"

/*
bool SDSendCommand6(uint8* response, uint8 command, uint32 data)
{
    uint8 sdResponse[32];
    SDBroadcastCommand((SDCommand)command, data, sdResponse);
    tonccpy(response, sdResponse, 6);
    return true;
}

bool SDSendCommand17(uint8* response, uint8 command, uint32 data)
{
    uint8 sdResponse[32];
    SDBroadcastCommand((SDCommand)command, data, sdResponse);
    tonccpy(response, sdResponse, 17);
    return true;
}
*/

/*
int Choose();

void Pause()
{
    do
    {
        scanKeys();        
        swiWaitForVBlank();        
    } while(!(keysDown() & KEY_A));
}

void DumpMemory(unsigned char* data, int bytes)
{
    while(bytes > 0)
    {
        // Display a screen
        for(int line = 0; line < 20 && bytes > 0; ++line)
        {
            int columns = bytes < 8 ? bytes : 8;

            for(int c = 0; c < columns; ++c)
                printf("%.2x ", data[c]);

            for(int c = 0; c < columns; ++c)
            {
                if(data[c] >= 0x20)
                    printf("%c", data[c]);
                else
                    printf(".");
            }

            data+= columns;
            bytes-= columns;
        }

        Pause();
    }
}
*/

//static uint32 relativeCardAddress;

//#define DO_DEBUG(statements) do { statements; } while(0)               
//#define DO_DEBUG(statements)






bool _X9SD_isInserted(void)
{
	//coto: breaks fat initdefault
	/*
    static uint8 response[0x20];
    SDSendCommand(GetStatus, 0, response);
    // Make sure the card responded correctly
    return response[0] == GetStatus;
	*/
	
	return true;
}

bool _X9SD_clearStatus(void)
{
    return true;
    //uint8 response[0x20];
    //return SDInitialize(&relativeCardAddress);
    //return _SD_InitCard(&SDSendCommand6, SDSendCommand17, true, &relativeCardAddress);
}

bool _X9SD_shutdown(void)
{
    return true;
}

bool _X9SD_startup(void)
{
    //cardIO_1();
    //cardIO_2(0x1B00);
    cardIO_3(0);
    cardIO_4(0);

    //return SDInitialize(&relativeCardAddress);
    return true;
} 


bool _X9SD_writeSectors(uint32 sector, uint32 sectorCount, const uint8* buffer)
{
    
    const uint32 blockLength = 512;
    while(sectorCount > 0)
    {
        if(SDWriteSingleBlock(sector*blockLength, buffer) == true){
			buffer+= blockLength;
			--sectorCount;
			++sector;
		}
    }
    
    return true;
}

bool _X9SD_readSectors(uint32 sector, uint32 sectorCount, uint8* buffer)
{
    
    const uint32 blockLength = 512;

    /*
    if(sectorCount == 1)
        return SDReadSingleBlock(sector*blockLength, buffer);
    else
        return SDReadMultipleBlocks(sector*blockLength, sectorCount, buffer);
    */

    while(sectorCount > 0)
	{
        SDReadSingleBlock(sector*blockLength, buffer);
		
        buffer+= blockLength;
        --sectorCount;
        ++sector;
	}

    return true;
}

//DLDI.C hook: // Not all compilers like multi-character-constants...
//const IO_INTERFACE _io_x9sd =
//    { 'X9SD' 
//	, FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_SLOT_NDS
//	, (FN_MEDIUM_STARTUP)&_X9SD_startup
//	, (FN_MEDIUM_ISINSERTED)&_X9SD_isInserted
//	, (FN_MEDIUM_READSECTORS)&_X9SD_readSectors
//	, (FN_MEDIUM_WRITESECTORS)&_X9SD_writeSectors
//	, (FN_MEDIUM_CLEARSTATUS)&_X9SD_clearStatus
//	, (FN_MEDIUM_SHUTDOWN)&_X9SD_shutdown
//    };
