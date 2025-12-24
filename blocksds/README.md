# DLDI

Dynamically Linked Device Interface (DLDI) is a storage device interface for Nintendo DS (and GBA) homebrew. You may find more info about DLDI [here](https://www.chishm.com/DLDI/).

This repository hosts both complete and work-in-progress drivers, primarily for Slot-1 devices.

Currently supported devices:

| Driver name | Supported devices                                                      | Author           | SPDX-License-Identifier |
| ----------- | ---------------------------------------------------------------------- | ---------------- | ----------------------- |
| acep        | Ace3DS+, R4 i.L.S., r4isdhc.com.cn, Gateway (Blue)                     | lifehackerhansol | Zlib                    |
| ak2         | Acekard 2/2.1/2i, r4itt.net 3DS, R4i3D, r4ids.cn (and probably others) | lifehackerhansol | Zlib                    |
| ak2_cmd24   | Acekard 2 clones with broken CMD25                                     | lifehackerhansol | Zlib                    |
| ez5h        | EZ-Flash V                                                             | SaTa, EZ-Flash   | Zlib                    |
| ez5n        | EZ-Flash Parallel                                                      | lifehackerhansol | Zlib                    |
| g003        | M3i Zero (GMP-Z003)                                                    | lifehackerhansol | Zlib                    |
| gmtf        | GAMES n' MUSIC                                                         | edo9300          | Zlib                    |
| m3ds        | M3 DS Real, M3i Zero (other models), iTouchDS, r4rts.com               | lifehackerhansol | Zlib                    |
| mati        | iSmart Premium                                                         | EZ-Flash, iSmart | Zlib                    |
| r4idsn      | R4iDSN                                                                 | lifehackerhansol | Zlib                    |
| r4tf        | Original R4, M3 Simply                                                 | lifehackerhansol | Zlib                    |
| rpgs        | Acekard RPG (SD)                                                       | lifehackerhansol | Zlib                    |
| scds        | SuperCard DSONE                                                        | lifehackerhansol | Zlib                    |
| scdssdhc    | SuperCard DSONE SDHC                                                   | lifehackerhansol | Zlib                    |
| sg3d        | Stargate 3DS                                                           | lifehackerhansol | Zlib                    |
| ttio        | DSTT                                                                   | lifehackerhansol | Zlib                    |

## Compiling

0. Install BlocksDS: https://blocksds.github.io/docs/
1. Run `make PLATFORM=(driver name)`

## Adding a new device

> [!TIP]
> You may see the `00example` driver as a reference.

1. Observe the required interface functions in `include/iointerface.h` and implement it to your driver as needed
1. Add device-specific code to `source/platform/(driver name)`
