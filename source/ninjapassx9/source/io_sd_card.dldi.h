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

#ifndef included_io_sd_card_h
#define included_io_sd_card_h

typedef struct
{
    const u8  csdStructure              :  2;
    const u8  reserved1                 :  6;
    const u8  dataReadAccessTime1       :  8;  // TAAC
    const u8  dataReadAccessTime2       :  8;  // NSAC
    const u8  maxDataTransferRate       :  8;
    const u16 cardCommandClasses        :  12;
    const u8  maxReadDataBlockLength    :  4;
    const u8  allowPartialRead          :  1;
    const u8  allowWriteMisalign        :  1;
    const u8  allowReadMisalign         :  1;
    const u8  dsrImplemented            :  1;
    const u8  reserved2                 :  2;
    const u16 deviceSize                : 12;
    const u8  maxReadCurrentMin         :  3;
    const u8  maxReadCurrentMax         :  3;
    const u8  maxWriteCurrentMin        :  3;
    const u8  maxWriteCurrentMax        :  3;
    const u8  sizeMultiplier            :  3;
    const u8  allowEraseSingleBlock     :  1;
    const u8  eraseSectorSize           :  7;
    const u8  writeProtectGroupSize     :  7;
    const u8  writeProtectGroupEnabled  :  1;
    const u8  reserved3                 :  2;
    const u8  writeSpeedFactor          :  3;
    const u8  maxWriteLength            :  4;
    const u8  allowPartialWrite         :  1;
    const u8  reserved4                 :  5;
    const u8  fileFormatGroup           :  1;
    const u8  copyFlag                  :  1;
    const u8  permanentWriteProtection  :  1;
          u8  temporaryWriteProtection  :  1;
    const u8  fileFormat                :  2;
          u8  reserved5                 :  2;
          u8  crc7                      :  7;  // Used to ensure this header was transferred properly
    const u8  direction                 :  1;  // 1 == from card to host
} CSDRegister;

typedef struct
{
    bool outOfRange                : 1;
    bool addressError              : 1;
    bool blockLengthError          : 1;
    bool eraseSequenceError        : 1;
    bool eraseParameter            : 1;
    bool writeProtectViolation     : 1;
    bool cardIsLocked              : 1;
    bool unlockFailed              : 1;
    bool crcCheckFailure           : 1;
    bool illegalCommand            : 1;
    bool cardECCFailed             : 1;
    bool ccErrornge                : 1;
    bool error                     : 1;
    bool reserved1                 : 1;
    bool reserved2                 : 1;
    bool invalidOverwrite          : 1;
    bool writeProtectEraseSkip     : 1;
    bool cardECCDisabled           : 1;
    bool eraseReset                : 1;
    int currentState              : 4;
    bool readyForData              : 1;
    int reserved3                 : 2;
    bool appCommand                : 1;
    bool reserved4                 : 1;
    bool authSequenceError         : 1;
    bool reserved5                 : 1;
    int reserved6                 : 2;
} SDCardStatus;

typedef struct
{
    bool   start                     :  1;
    bool   direction                 :  1;
    int   commandIndex              :  6;
    u16 newRCA                    : 16;
    u16 cardStatusError           : 16;
    int   crc7                      :  7;
    bool   end                       :  1;
} SDResponseR6;

/* SD commands */
enum SDCommand
    { SetIdle                =  0 // CMD0
    , GetCID                 =  2 // CMD2   
    , GetRCA                 =  3 //         Get 
    , SendOPCommand          =  5 // IO_SEND_OP_COND (CMD5)
    , SelectCard             =  7 //
    , GetCSD                 =  9 // CMD9    Get Card Specific Data
    , AbortReadWrite         = 12 // CMD12   Data Transfer Abort
    , GetStatus              = 13 //
    , GoInactiveState        = 15 //
    , SetBlockLength         = 16 // CMD16
    , ReadSingleBlock        = 17 //
    , ReadMultipleBlocks     = 18 //
    , WriteSingleBlock       = 24 //
    , WriteMultipleBlocks    = 25 //
    , AppCommand             = 55 // APPCMD
    , Broadcast              = 0x40
    };

enum SDAppCommand
    { SetBusWidth  = 6  //
    , SetOCR       = 41 // ACMD41  Operating Condition Register
    };

enum SDState
    { SDIdleState   = 0
    , SDReadyState
    , SDIDState
    , SDStandbyState
    , SDTransferState
    , SDDataState
    , SDReceiveState
    , SDProgrammingState
    , SDDisconnectedState
    , SDInactiveState
    };



#endif // INCLUDED_io_sd_card_h

#ifdef __cplusplus
extern "C"{
#endif

extern void SDSendCommand(enum SDCommand sdCmd, u32 arg, u8* response);
extern bool SDInitialize(u32* relativeCardAddress);
extern bool SDWriteSingleBlock(u32 address, const void* buffer);
extern bool SDReadSingleBlock(u32 address, void* destination);
extern bool SDReadMultipleBlocks(u32 address, unsigned int count, void* destination);

extern void SDSendAppCommand(enum SDAppCommand sdAppCmd, u32 arg, u8* response);
extern void SDBroadcastCommand(enum SDCommand sdCmd, u32 arg, u8* response);
extern void SDBroadcastAppCommand(enum SDAppCommand sdAppCmd, u32 arg, u8* response);

#ifdef __cplusplus
}
#endif
