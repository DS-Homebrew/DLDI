# DLDI

Dynamically Linked Device Interface (DLDI) is a storage device interface for Nintendo DS (and GBA) homebrew. You may find more info about DLDI [here](https://www.chishm.com/DLDI/).

This repository hosts both complete and work-in-progress drivers, primarily for Slot-1 devices.

Currently supported devices:

| Driver name | Supported devices       | Author           | SPDX-License-Identifier |
| ----------- | ----------------------- | ---------------- | ----------------------- |
| r4tf        | Original R4, M3 Simply  | lifehackerhansol | Zlib                    |

## Compiling

0. Install BlocksDS: https://blocksds.github.io/docs/
1. Run `make PLATFORM=(driver name)`

## Adding a new device

> [!TIP]
> You may see the `00example` driver as a reference.

1. Observe the required interface functions in `include/iointerface.h` and implement it to your driver as needed
1. Add device-specific code to `source/platform/(driver name)`
