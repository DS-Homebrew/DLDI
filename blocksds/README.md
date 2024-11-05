# DLDI

Dynamically Linked Device Interface (DLDI) is a storage device interface for Nintendo DS (and GBA) homebrew. You may find more info about DLDI [here](https://www.chishm.com/DLDI/).

## Compiling

0. Install BlocksDS: https://blocksds.github.io/docs/
1. Run `make PLATFORM=(driver name)`

## Adding a new device

> [!TIP]
> You may see the `00example` driver as a reference.

1. Observe the required interface functions in `include/iointerface.h` and implement it to your driver as needed
1. Add device-specific code to `source/platform/(driver name)`
