# DLDI

This is both an archive and a development repository for Nintendo DS flash cartridge DLDI drivers.


### List of recommended DLDI drivers:
- `ace3ds_sd`: Ace3DS+ based on r4tfv2
    - By R4DS team, Chishm and lifehackerhansol, maintained by DS-Homebrew
- `acek`: Acekard+
- `ak2_sd`: Acekard 2/2.1/2i and related clones
    - By R4iDSN team, Yellow Wood Goblin and lifehackerhansol, maintained by DS-Homebrew
	- Supports all known Acekard 2 clones, notably R4iTT 3DS, R4(i) Ultra and all r4ids.cn carts
	- GPL 3.0
- `CEVO_CycloDS_iEvolution_v1.0`: CycloDS iEvolution
    - Dumped from memory. DDMEM is incorrect as a result.
	- Source code is not provided
- `CycloEvo`: CycloDS Evolution
    - Source code is not provided
- `cyds`: CycloDS Mini/Micro
- `default`: Default DLDI in libnds
    - This is based on outdated libnds
- `dlms4`: DSLink microSD v4
    - By Amadeus
	- Fixes issues in v3.
- `dstwo`: SuperCard DSTWO
    - Source unknown
	- Source code is not provided
- `dsx`: DS-Xtreme.
- `EDGEv1.0`: EDGE DS (and probably iEDGE)
    - Source code is not provided
- `ewsd`: Ewin2 SD card
- `ez5h`: EZ-Flash V / EZ-Flash V SDHC v2
    - By Cory1942, ps2aich and Taiju Yamada, maintained by DS-Homebrew
	- Driver is bigger than original DLDI due to containing a CCITT lookup table
- `EZ5iV3`: EZ-Flash Vi
	- Taken from kernel 3.0 OB8
	- Does not initialize the SD card, thus you must boot homebrew from the firmware first
    - Source code is not provided
- `ezsd`: EZ-Flash 4 SD card
- `G003`: M3i Zero (GMP-Z003)
    - Taken from Sakura 1.49a
	- Initial comparison seems similar to `r4tfv2`
	- Source is not provided
- `g6fl`: G6 Flash
    - By virusweb
- `g6real`: G6 DS
- `gmtf`: Datel Games 'n' Music
    - By Chishm
    - Slower DLDI but more compatible
- `gmtf2`: Datel Games 'n' Music v2
    - By Chishm
    - Faster DLDI but requires an SDHC card
- `iPlayer`: iPlayer
    - Source is unknown
- `m3cf`: M3 Adapter Compact Flash
    - By Chishm
	- Fails read alignment
	- Supposedly a built-in driver exists, but not yet in this repository
- `m3r4_m3ds`: M3 DS Real v2
    - By R4DS team, Chishm, and Taiju Yamada
	- Supposedly slower than official DLDI, but initial testing seems to have no issues
- `m3sd`: M3 Adapter SD card
    - By Chishm
- `m3sd_alt`: M3 Adapter SD card
    - By agentq
- `mati`: iSmart Premium v2
    - By Cory1942, ps2aich and Taiju Yamada, maintained by DS-Homebrew
	- Driver is bigger than original DLDI due to containing a CCITT lookup table
- `mmcf`: Datel Max Media Dock Compact Flash
    - By Chishm
- `mpcf`: GBA Movie Player Compact Flash
    - By Chishm
- `mpsd`: GBA Movie Player SD card
    - By ps2aich, maintained by DS-Homebrew
- `nmmc`: NEO Flash MK2/MK3 SD card
- `mk5n`: NEO Flash MK5 NAND
- `nsd2v2`: NEO2 SD card v2
    - By Chishm and NEO Flash team
	- Faster reads than the original driver
- `njsd`: Ninja DS SD card
- `ninjapassx9`: Ninjapass X9 v2
    - By CJ Bell and Coto
	- Much faster drive access
- `r4idsn_sd`: R4iDSN
    - By R4iDSN team and Yellow Wood Goblin
	- GPL 3.0
- `r4tfv2`: Original R4 / M3 Simply v2
    - By R4DS team and Chishm
	- Fixes ARM7 support from v1
	- Use with original R4 / M3 Simply cards
- `m3r4_r4tf`: Original R4 / M3 Simply v3
    - By R4DS team, Chishm, and Taiju Yamada, maintained by DS-Homebrew
	- Supports SDHC based original R4 clones
	- Use with aforementioned SDHC based clone flashcarts
- `rpg_nand`: Acekard RPG NAND
    - By Acekard and Yellow Wood Goblin
	- BSS is too large and cannot fit in 16KB DLDI, cannot be used for homebrew
	- TODO: see if shrinking the BSS doesn't break anything
	- GPL 3.0
- `rpg_sd`: Acekard RPG SD card
    - By Acekard and Yellow Wood Goblin
	- GPL 3.0
- `sccf`: SuperCard Compact Flash
    - By Chishm
- `DSONESDSC`: SuperCard DSONE (non-SDHC) official
    - Taken from DSONE_EOS_sp6_20130129
	- Source code is not provided
- `scds`: SuperCard DSONE (non-SDHC) v1
    - By SuperCard team and Lick
	- Unknown if writes work properly
- `scds1`: SuperCard DSONE (non-SDHC) v2
    - By cluny
- `scdssdhc2`: SuperCard DSONE SDHC v3
    - Authors:
        - R4DS team, Chishm (framework)
        - SuperCard, cluny: initial DSONE SDHC driver code
        - lifehackerhansol: port to r4tfv2, batch read/write
- `scsd`: SuperCard SD card v1
    - By Chishm
	- Try this DLDI first before using the Moonshell version
- `scsd_moon`: SuperCard SD card v2
    - By Moonlight
	- Works on newer SuperCards that may fail with other DLDIs
	- Does not initialize the SD card, thus you must boot homebrew from the firmware first
- `sclt`: SuperCard Lite SD card
    - By brisssou
- `scrb`: SuperCard Rumble SD card
    - By Barni
	- Source code is not provided
- `template`: DLDI template
    - Use this to create a new DLDI driver
- `ttio`, `ttio_sdhc`: DSTT reimplementation
    - Authors:
        - R4DS team, Chishm (framework)
        - SuperCard, cluny: initial DSONE SDHC driver code
        - lifehackerhansol: port to r4tfv2, batch read/write

### List of archived DLDI drivers:

These drivers have working updated versions in the above list. Consider those instead.

- `amaze3ds`: Amaze3DS
    - Known Acekard 2 clone
	- Use `ak2_sd` instead
- `ak2_sd_AKAIO190`: Acekard 2, 2.1, 2i
    - By Acekard and AKAIO team
	- Taken from AKAIO 1.9.0
	- Source code is not provided
- `DEMONIOLibrary`: DLDI from timebombed cartridges
    - Dumped from memory. DDMEM is incorrect as a result.
	- Use `DSTTDLDIboyakkeyver` instead
- `DSONESlot-1`: SuperCard DSONE SDHC official
    - Taken from DSONE_SDHC_EOS_sp6_20121206
	- Source code is not provided, and initial comparison looks different from `scdssdhc`
- `DSTTDLDIboyakkeyver`: DSTT v2
    - By boyakkey
    - Appears to be based on `scdssdhc`
    - Source code is not provided
	- Due to how DSTT functions, using this as an autoboot will break non-SDHC support
        - `TTCARDIOLibrary` has the opposite problem, but since this driver supports SDHC this is the recommended driver
- `dlms`: DSLink microSD v1
    - By Rudolph / Lick
    - Doesn't seem to work.
- `dlms_moon`: DSLink microSD v2
    - By Moonlight
	- No idea on whether it is working
- `dlms3`: DSLink microSD v3
    - By Diddl
	- Because of a swiDelay call, it doesn't work in DSLinux.
	- Writes are unstable
- `ez5s`: EZ-Flash V v1
    - By Cory1942 / ps2aich
	- As of 2022 the source code does not produce a working driver
- `ez5sdhc`: EZ-Flash V SDHC v2
    - By Cory1942 / ps2aich
	- DLDI doesn't initialize the SD card and thus cannot be used as an autoboot
- `EZ5V2`: EZ-Flash V / EZ-Flash V SDHC
    - Taken from kernel 2.0 RC20
	- Initial comparison seems similar to the `ez5sdhc` driver
	- Source code is not provided
- `iTouchDS`: iTouchDS / iTouch2
    - Taken from kernel 3.9a
	- Clone of the M3 DS Real
	- Use `m3r4_m3ds` instead
- `Mat`: iSmart Premium v1
    - Source is unknown
	- Open source version is `mati`
- `nsd2`: NEO2 SD card v1
- `r4ds_pro`: R4 DS Pro
    - Known Acekard 2 clone
	- Use `ak2_sd` instead
- `r4idsn_sd_r4ig`: r4ids.cn carts
    - By r4ids.cn
	- Use `ak2_sd` instead
- `r4itt`: R4iTT 3DS
    - By R4iTT
	- Source code is not provided
	- Use `ak2_sd` instead
- `scds2`: SuperCard DSONE (non-SDHC) v3
    - By Barni
    - It seems this code is almost identical to scds1...
	- Source code is not provided
- `scdssdhc`: SuperCard DSONE SDHC
    - By SuperCard
- `scdssdhc1`: SuperCard DSONE SDHC v2
    - By SuperCard and cluny
	- Source code was cleaned up
- `TTCARDIOLibrary`: DSTT v1
    - Taken from TTMenu 1.18
	- Only known DSTT DLDI to support non-SDHC cards
	- Due to how DSTT functions, using this as an autoboot will break SDHC support
	- `DSTTDLDIboyakkeyver` is recommended, but that driver has the opposite problem
- `ttio_fwdump`: DSTT firmware dump
    - Taken from a certain DSTT's flash
	- Seems to be the same as `TTCARDIOLibrary`, and only here as a reference
- `tt_sd`: DSTT Wood4TT version
    - Taken from Wood4TT 1.25
	- This driver is bogus: the code is identical to r4tfv2
- `tt_sd (fixed name)`: DSTT Wood4TT version with correct DLDI friendlyName
    - Hex edited to show `TTCARD IO Library` instead of `R4(DS)`
	- This driver is bogus: the code is identical to r4tfv2
- `x9sd6`: Ninjapass X9 v1
    - By CJ Bell
	- `ninjapassx9` is an updated version of this driver

### List of drivers being tested (in `dev`):
- `g003`: M3i Zero GMP-Z003
    - By R4DS team, Chishm and lifehackerhansol
