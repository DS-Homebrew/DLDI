# DLDI

This is both an archive and a development repository for Nintendo DS flash cartridge DLDI drivers.

The folder structure are as follows:

- `archived`: unused source codes and prebuilts. Usually means an open-source or better-performing alternative exists
- `default`: default DLDI, present in libnds. Here for reference.
- `prebuilts`: DLDI files that currently aren't built from source. This usually means that the driver is proprietary, or the source code is not yet compilable.
- `source`: DLDI files that are built from source
- `staging`: DLDI source codes in various states of WIP, may be a new DLDI or older source codes that do not yet compile on newer toolchains
- `template`: example DLDI, taken from https://github.com/devkitPro/nds-examples
- `tools`: PC tools useful for DLDI development
