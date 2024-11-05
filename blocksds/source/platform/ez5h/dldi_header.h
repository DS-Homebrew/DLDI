/*
    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#define DRIVER_IOTYPE "EZ5H"
#define DRIVER_NAME "ez5v2 DLDI based on CCITT table"

#define DRIVER_FEATURES (FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_SLOT_NDS)
