#
# NDSCardFlagCheck - Take apart a card flag to needed definitions
#
# Copyright (C) 2023 lifehackerhansol
#
# SPDX-License-Identifier: Zlib
#

import sys

flags = {
    "MCCNT1_LATENCY1_SHIFT": 0,
    "MCCNT1_READ_DATA_DESCRAMBLE": (1 << 13),
    "MCCNT1_CLOCK_SCRAMBLER": (1 << 14),
    "MCCNT1_APPLY_SCRAMBLE_SEED": (1 << 15),
    "MCCNT1_CMD_SCRAMBLE": (1 << 22),
    "MCCNT1_DATA_READY": (1 << 23),
    "MCCNT1_CLK_6_7_MHZ": (0 << 27),
    "MCCNT1_CLK_4_2_MHZ": (1 << 27),
    "MCCNT1_LATENCY_CLK": (1 << 28),
    "MCCNT1_RESET_ON": (0 << 29),
    "MCCNT1_RESET_OFF": (1 << 29),
    "MCCNT1_DIR_READ": (0 << 30),
    "MCCNT1_DIR_WRITE": (1 << 30),
    "MCCNT1_ENABLE": (1 << 31)
}

flags_length = {
    "MCCNT1_LEN_0": (0 << 24),
    "MCCNT1_LEN_512": (1 << 24),
    "MCCNT1_LEN_1024": (2 << 24),
    "MCCNT1_LEN_2048": (3 << 24),
    "MCCNT1_LEN_4096": (4 << 24),
    "MCCNT1_LEN_8192": (5 << 24),
    "MCCNT1_LEN_16384": (6 << 24),
    "MCCNT1_LEN_4": (7 << 24),
}


def MCCNT1_LATENCY1(x):
    return (x)


MCCNT1_LATENCY2_SHIFT = 16
MCCNT1_LATENCY2_MASK = 0x3F0000


def MCCNT1_LATENCY2(x) -> int:
    return (((x) << MCCNT1_LATENCY2_SHIFT) & MCCNT1_LATENCY2_MASK)


if __name__ == "__main__":
    inputFlags: int = int(sys.argv[1], 16)
    result: int = 0

    valid_flags = []

    for i in reversed(flags):
        if inputFlags & flags[i]:
            valid_flags.append(i)
            result |= flags[i]
            inputFlags ^= flags[i]

    for i in reversed(range(63)):
        # Check equal instead of bitwise AND
        # this u8 is dedicated entirely to latency2
        if inputFlags & 0xFF0000 == MCCNT1_LATENCY2(i):
            valid_flags.append(f"MCCNT1_LATENCY2({i})")
            result |= MCCNT1_LATENCY2(i)
            inputFlags ^= MCCNT1_LATENCY2(i)
            break

    for i in flags_length:
        # Check equal instead of bitwise AND
        # this u8 is dedicated entirely to length
        if inputFlags & (0xFF << 24) == flags_length[i]:
            valid_flags.append(i)
            result |= flags_length[i]
            inputFlags ^= flags_length[i]
            break

    ret = ""
    for i in valid_flags:
        if valid_flags[-1] == i:
            ret += f"{i}"
        else:
            ret += f"{i} | "

    if inputFlags != 0:
        ret += f"\nUnknown flags: {inputFlags}"

    print(ret)
