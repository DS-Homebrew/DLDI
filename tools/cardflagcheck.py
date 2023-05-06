#
# NDSCardFlagCheck - Take apart a card flag to needed definitions
#
# Copyright (C) 2023 lifehackerhansol
#
# SPDX-License-Identifier: ISC
#

import sys


class NDSCardFlagCheck():
    def __init__(self, inputFlags: int):
        self.inputFlags = inputFlags

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
        "MCCNT1_RESET_ON":  (0 << 29),
        "MCCNT1_RESET_OFF": (1 << 29),
        "MCCNT1_DIR_READ":  (0 << 30),
        "MCCNT1_DIR_WRITE": (1 << 30),
        "MCCNT1_ENABLE": (1 << 31)
    }

    flags_length = {
        "MCCNT1_LEN_0":     (0 << 24),
        "MCCNT1_LEN_512":   (1 << 24),
        "MCCNT1_LEN_1024":  (2 << 24),
        "MCCNT1_LEN_2048":  (3 << 24),
        "MCCNT1_LEN_4096":  (4 << 24),
        "MCCNT1_LEN_8192":  (5 << 24),
        "MCCNT1_LEN_16384": (6 << 24),
        "MCCNT1_LEN_4":     (7 << 24),
    }

    MCCNT1_LATENCY2_SHIFT = 16
    MCCNT1_LATENCY2_MASK = 0x3F0000

    def MCCNT1_LATENCY2(self, x) -> int:
        return (((x) << self.MCCNT1_LATENCY2_SHIFT) & self.MCCNT1_LATENCY2_MASK)

    def calculate(self) -> str:
        inputFlags = self.inputFlags
        result: int = 0

        valid_flags = []

        for i in reversed(self.flags):
            if inputFlags & self.flags[i]:
                valid_flags.append(i)
                result |= self.flags[i]
                inputFlags ^= self.flags[i]

        for i in reversed(range(63)):
            # Check equal instead of bitwise AND
            # this u8 is dedicated entirely to latency2
            if inputFlags & 0xFF0000 == self.MCCNT1_LATENCY2(i):
                valid_flags.append(f"MCCNT1_LATENCY2({i})")
                result |= self.MCCNT1_LATENCY2(i)
                inputFlags ^= self.MCCNT1_LATENCY2(i)
                break

        for i in self.flags_length:
            # Check equal instead of bitwise AND
            # this u8 is dedicated entirely to length
            if inputFlags & (0xFF << 24) == self.flags_length[i]:
                valid_flags.append(i)
                result |= self.flags_length[i]
                inputFlags ^= self.flags_length[i]
                break

        for i in reversed(range(0x1FFF)):
            # Check equal instead of bitwise AND
            # this u16 is dedicated entirely to latency1
            if inputFlags & 0x1FFF == i:
                valid_flags.append(f"MCCNT1_LATENCY1({i})")
                result |= i
                inputFlags ^= i
                break

        ret = ""
        for i in valid_flags:
            if valid_flags[-1] == i:
                ret += f"{i}"
            else:
                ret += f"{i} | "

        if inputFlags != 0:
            ret += f"\nUnknown flags: 0x{inputFlags:08X}"

        return ret


if __name__ == "__main__":
    try:
        card = NDSCardFlagCheck(int(sys.argv[1], 16))
    except Exception:
        print(f"Usage: {sys.argv[0]} <REG_ROMCTRL | REG_MCCNT1 value in hex>")
        sys.exit(1)
    print(f"0x{card.inputFlags:08X} = {card.calculate()}")
