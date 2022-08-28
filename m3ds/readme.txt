m3r4.dldi  - M3 DLDI based on r4tf_v2

[m3ds.dldi]
* Please note m3ds.dldi can be replaced by m3r4.dldi, but it has very slow writing speed. *

Copyright (C)
Chishm - Original R4TF.dldi
toro   - Binary patching for M3
Xenon  - Source patching and rebuild

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
