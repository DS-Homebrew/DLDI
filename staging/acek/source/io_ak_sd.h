/*
    Copyright (C) 2007 Acekard, www.acekard.com
    SPDX-License-Identifier: MIT
*/

#pragma once

void dsd_send_sd_cmd( u8 cmd, u32 arg );

void dsd_write_sd_sector( u32 address, const u8 * data );

bool dsd_is_sd_busy();

u16 dsd_SDCRC16( const u8 * data, u32 length );
