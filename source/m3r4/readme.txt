r4tf.dldi  - R4 DLDI v2 recompiled
m3r4.dldi  - M3 DLDI based on r4tf_v2
ex4tf.dldi - EX4DS DLDI based on r4tf_v2 (oh dear, cannot write in Wood*)
scds.dldi / ttio.dldi - SCDS SD/SDHC DLDI based on r4tf_v2 (scds.dldi works as DSTT DLDI if DLDI ID != SCDS)

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

[ex4tf.dldi]
Copyright (C)
Chishm - Original R4TF.dldi
Xenon  - Comparing original ex4tf.dldi / patching

[scds.dldi / ttio.dldi]
Copyright (C)
cluny - Original scdssdhc1.dldi
Xenon - Some funny hack
