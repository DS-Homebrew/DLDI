/*
    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <nds/ndstypes.h>

// Wrapper for reading from a cartridge.
void cardExt_ReadData(u64 command, u32 flags, void* buffer, u32 length);

// Wrapper for writing to a cartridge.
void cardExt_WriteData(u64 command, u32 flags, const void* buffer, u32 length);

// Wrapper for reading 4 bytes from a cartridge. Usually used for simple commands.
u32 cardExt_ReadData4Byte(u64 command, u32 flags);

// Wrapper for sending a command without an expected return value.
void cardExt_SendCommand(u64 command, u32 flags);
