/*******************************************************
	Modified by Rudolph (çcíÈ):
	2006/11/20
		* Added write routine support

 Original is EZ4FATLIB code offered by EZTEAM.
 Hardware Routines for SD card using a EZ4 series.

********************************************************/

/*
	io_m3sd.c based on io_m3cf.c by SaTa.

	io_m3cf.c based on

	compact_flash.c
	By chishm (Michael Chisholm)

	Hardware Routines for reading a compact flash card
	using the M3 Perfect CF Adapter

	CF routines modified with help from Darkfader

	This software is completely free. No warranty is provided.
	If you use it, please give me credit and email me about your
	project at chishm@hotmail.com

	See gba_nds_fat.txt for help and license details.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nds.h>

#include "common.h"

#include "sdopera.h"

/*-----------------------------------------------------------------
M3SD_IsInserted
Is a compact flash card inserted?
bool return OUT:  true if a CF card is inserted
-----------------------------------------------------------------*/
bool EZSD_IsInserted (void) 
{
	return true;
}


/*-----------------------------------------------------------------
M3SD_ClearStatus
Tries to make the CF card go back to idle mode
bool return OUT:  true if a CF card is idle
-----------------------------------------------------------------*/
bool EZSD_ClearStatus (void) 
{
	return true;
}


/*-----------------------------------------------------------------
M3SD_ReadSectors
Read 512 byte sector numbered "sector" into "buffer"
u32 sector IN: address of first 512 byte sector on CF card to read
u8 numSecs IN: number of 512 byte sectors to read,
 1 to 256 sectors can be read, 0 = 256
void* buffer OUT: pointer to 512 byte buffer to store data in
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool EZSD_ReadSectors (u32 sector, u8 numSecs, void* buffer)
{

  u8 *pbuf=(u8*)buffer;
  while(numSecs!=0){
    if(SD_ReadSingleBlock(sector*0x200 , pbuf, 0x200)==false) return false;
    sector++;
    pbuf+=0x200;
    numSecs--;
  }
  return true;

//  return SD_ReadMultiBlock(sector*0x200 , buffer, numSecs*0x200);
}



/*-----------------------------------------------------------------
M3SD_WriteSectors
Write 512 byte sector numbered "sector" from "buffer"
u32 sector IN: address of 512 byte sector on CF card to read
u8 numSecs IN: number of 512 byte sectors to read,
 1 to 256 sectors can be read, 0 = 256
void* buffer IN: pointer to 512 byte buffer to read data from
bool return OUT: true if successful
-----------------------------------------------------------------*/
bool EZSD_WriteSectors (u32 sector, u8 numSecs, void* buffer)
{
  u8 *pbuf=(u8*)buffer;
  while(numSecs!=0){
    if(SD_WriteSingleBlock(sector*0x200 , pbuf, 0x200)==false) return false;
    sector++;
    pbuf+=0x200;
    numSecs--;
  }
  return true;
//  return SD_WriteMultiBlock(sector*0x200 , buffer, numSecs*0x200);
 }

/*-----------------------------------------------------------------
M3_Unlock
Returns true if M3 was unlocked, false if failed
Added by MightyMax
-----------------------------------------------------------------*/
bool EZSD_Shutdown(void) {
  SD_Disable();
  return true;
} ;

bool EZSD_StartUp(void) {
  SD_Enable();
  return SD_initial();
} ;

