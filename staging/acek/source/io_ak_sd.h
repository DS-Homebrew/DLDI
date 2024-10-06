/*
    Copyright (C) 2007 Acekard, www.acekard.com
    SPDX-License-Identifier: MIT
*/

#ifndef _DIRECT_SD_H_
#define _DIRECT_SD_H_

void dsd_send_sd_cmd( u8 cmd, u32 arg );

void dsd_write_sd_sector( u32 address, const u8 * data );

bool dsd_is_sd_busy();

#endif//_DIRECT_SD_H_
