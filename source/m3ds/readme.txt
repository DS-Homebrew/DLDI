m3r4.dldi  - M3 DLDI based on r4tf_v2

[m3ds.dldi]
* Please note m3ds.dldi can be replaced by m3r4.dldi, but it has very slow writing speed. *

Copyright (C)
Chishm - Original R4TF.dldi
toro   - Binary patching for M3
Xenon  - Source patching and rebuild
lifehackerhansol - Multi-sector read/write

M3 Command (partial)
B0xxxxxx xxxxxxxx
Initialization / Get jumper pins

BDaaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
preparation to read from SD/TF sector. wait until reading zero.

BAaaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
read from SD/TF sector

BEaaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
write to SD/TF sector

BCaaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
completion to write SD/TF sector. wait until reading zero.

CDxxxxxx xxxxxxxx
Get cart region

B1aaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
preparation to read multiple sectors from SD/TF. wait until reading zero.

B2aaaaaaa aaxxxxxx
aaaaaaaa: sector address requested in B1 (Don't need to x512)
preparation to read next sector from SD/TF. wait until reading zero.

B3aaaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512) // might not be needed
end multi-sector read or write from SD/TF.

B4aaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
write first sector of multi-sectors to SD/TF

B6aaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512) // might not be needed
completion to write one sector of multi-sectors to SD/TF. wait until reading zero.
it appears this is also needed after a B3 cmd to end multi-sector writes

B5aaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512) // might not be needed
write next sector of multi-sectors to SD/TF
