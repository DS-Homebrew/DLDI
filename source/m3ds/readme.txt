m3r4.dldi  - M3 DLDI based on r4tf_v3

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

B2aaaaaa aaxxxxxx
aaaaaaaa: sector address requested in B1 (Don't need to x512) // might not be needed
preparation to read next sector from SD/TF. wait until reading zero.

B3xxxxxx xxxxxxxx
end multi-sector read or write from SD/TF.

B4aaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
write first sector of multi-sectors to SD/TF

B6aaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
completion to write one sector of multi-sectors to SD/TF. wait until reading zero.
it appears this is also needed after a B3 cmd to end multi-sector writes

B5aaaaaa aaxxxxxx
aaaaaaaa: sector address (Don't need to x512)
write next sector of multi-sectors to SD/TF


Credits:
- toro / Xenon - Single-sector read/write documentation
