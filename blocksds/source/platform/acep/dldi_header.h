/*
    Ace3DS+
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <nds/arm9/dldi_asm.h>

#define DRIVER_IOTYPE "ACEP"
#define DRIVER_NAME "Ace3DS+"

#define DRIVER_FEATURES (FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_SLOT_NDS)
