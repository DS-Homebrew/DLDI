# DLDI template

This template builds a DLDI driver.

There are two ways to build it:

### Using devkitARM

The devkitARM makefile only supports building DLDIs for ARM7.

Simply run:

```bash
make
```

### Using BlocksDS

To build an ARM7 DLDI driver, just run:

```bash
make
```

To build an ARM9 DLDI driver, run:

```bash
make DLDI_ARM9=1
```
