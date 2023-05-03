import os
import struct
from binascii import crc32


# DLDI header
#
# typedef struct DLDI_INTERFACE {
#0	u32 	magicNumber;
#4	char	magicString [8];
#C	u8		versionNumber;
#D	u8		driverSize;			// log-2 of driver size in bytes
#E	u8		fixSectionsFlags;
#F	u8		allocatedSize;		// log-2 of the allocated space in bytes
#
#10	char	friendlyName [48];
#	
#	// Pointers to sections that need address fixing
#40	void*	dldiStart; // u32
#44	void*	dldiEnd; // u32
#48	void*	interworkStart; // u32
#4C	void*	interworkEnd; // u32
#50	void*	gotStart; // u32
#54	void*	gotEnd; // u32
#58	void*	bssStart; // u32
#5C	void*	bssEnd; // u32
#	
#	// Original I/O interface data
#   struct DISC_INTERFACE_STRUCT {
#60     unsigned long			ioType ;
#64     unsigned long			features ;
#68     FN_MEDIUM_STARTUP		startup ;
#6C     FN_MEDIUM_ISINSERTED	isInserted ;
#70     FN_MEDIUM_READSECTORS	readSectors ;
#74     FN_MEDIUM_WRITESECTORS	writeSectors ;
#78     FN_MEDIUM_CLEARSTATUS	clearStatus ;
#7C     FN_MEDIUM_SHUTDOWN		shutdown ;
#   } ;
#} DLDI_INTERFACE;
#

wanted_magicNumber = 0xBF8DA5ED
wanted_magicString = " Chishm\0"
wanted_versionNumber = 1

FIX_ALL	= 0x01
FIX_GLUE = 0x02
FIX_GOT = 0x04
FIX_BSS = 0x08

def scan_dldi_files():
    print('Fix Feature ddmem    all  glue got  bss  sizes filesize crc32    name')
    for i in os.listdir():
        if ".dldi" not in i:
            continue
        with open(i, "rb") as f:
            dldi = f.read()

            # Initial DLDI information
            magicNumber = struct.unpack("<I", dldi[0x0:0x4])[0]
            magicString = dldi[0x4:0xC].decode('ascii')
            versionNumber = struct.unpack("<B", dldi[0xC:0xD])[0]
            fixSectionsFlags = struct.unpack("<B", dldi[0xE:0xF])[0]
            print(f"{fixSectionsFlags:02X}", end=' ')
            features = struct.unpack("<I", dldi[0x64:0x68])[0]
            print(f"{features:08X}", end=' ')
            dldiStart = struct.unpack("<I", dldi[0x40:0x44])[0]
            print(f"{dldiStart:08X}", end=' ')
            dldiEnd = struct.unpack("<I", dldi[0x44:0x48])[0]
            dldiSize = dldiEnd - dldiStart
            print(f"{dldiSize:04X}", end=' ')
            interworkStart = struct.unpack("<I", dldi[0x48:0x4C])[0]
            interworkEnd = struct.unpack("<I", dldi[0x4C:0x50])[0]
            glueSize = interworkEnd - interworkStart
            print(f"{glueSize:04X}", end=' ')
            gotStart = struct.unpack("<I", dldi[0x50:0x54])[0]
            gotEnd = struct.unpack("<I", dldi[0x54:0x58])[0]
            gotSize = gotEnd - gotStart
            print(f"{gotSize:04X}", end=' ')
            bssStart = struct.unpack("<I", dldi[0x58:0x5C])[0]
            bssEnd = struct.unpack("<I", dldi[0x5C:0x60])[0]
            bssSize = bssEnd - bssStart
            print(f"{bssSize:04X}", end=' ')
            driverSize = struct.unpack("<B", dldi[0xD:0xE])[0]
            allocatedSize = struct.unpack("<B", dldi[0xF:0x10])[0]
            print(f"{driverSize:02X} {allocatedSize:02X}", end=' ')
            dldiDDMEMEnd = 2 ** driverSize + dldiStart
            filesize = len(dldi)
            print(f"{filesize:08X}", end=' ')
            dldiHash = crc32(dldi)
            print(f"{dldiHash:08X}", end=' ')
            ioType = dldi[0x60:0x64]
            print(f"{ioType.decode('ascii')}", end=' ')

            # WARN: exceed 32Kbytes
            if driverSize > 0xF:
                print(f"WARN:EXCEED32K", end=' ')

            # WARN: ensure no FIX_ALL
            if fixSectionsFlags & FIX_ALL:
                print("WARN:UsesBuggedFixAll", end=' ')

            # WARN: check if functions with THUMB-bit
            offs = 0x68
            thumbBit = 0
            for j in range(6):
                thumbBit |= struct.unpack("<I", dldi[offs:offs+4])[0]
                offs += 4
            if thumbBit & 3:
                print("WARN:THUMB", end=' ')

            # WARN: unaligned GOT+GLUE+ALL+BSS ptrs
            offs = 0x40
            thumbBit = 0
            for j in range(8):
                thumbBit |= struct.unpack("<I", dldi[offs:offs+4])[0]
                offs += 4
            if thumbBit & 3:
                print("WARN:UNALIGNED", end=' ')

            # WARN: non-ddmem values in GOT+GLUE area
            while True:
                if gotSize != 0:
                    file_gotEnd = gotEnd - dldiDDMEMEnd
                    if file_gotEnd > 0:
                        print("WARN:STRUCT", end=' ')
                        break
                    file_gotStart = gotStart - dldiStart
                    if file_gotStart < 0:
                        print("WARN:STRUCT", end=' ')
                        break
                    if file_gotStart < 0x80:
                        print("WARN:STRUCT", end=' ')
                        break
                    for j in range(file_gotStart, file_gotEnd, 4):
                        data = struct.unpack("<I", dldi[j:j+4])[0]
                        print(data)
                        if data == 0:
                            continue
                        elif data < dldiStart:
                            print("WARN:NONDDMEM", end=' ')
                            break
                        elif data >= dldiDDMEMEnd:
                            print("WARN:NONDDMEM", end=' ')
                            break
                break

            # WARN: check MAX end ptr's
            while True:
                # Check BSS first
                if fixSectionsFlags & FIX_BSS and bssSize != 0:
                    if bssEnd > dldiDDMEMEnd:
                        print("WARN:BSS_EXCESS", end=' ')
                        break
                # avoid adjusted WORDs exceeding end (vs total size)
                # avoid adjusted WORDs exceeding end (vs FILESIZE)
                adjustedEnd = dldiDDMEMEnd - 3
                adjustedSize = filesize + dldiStart - 3
                # Check GOT second
                if fixSectionsFlags & FIX_GOT and gotSize != 0:
                    if gotEnd > adjustedEnd or gotEnd > adjustedSize:
                        print("WARN:OVERKILLEND", end=' ')
                        break
                # Check GLUE third
                if fixSectionsFlags & FIX_GLUE and glueSize != 0:
                    if interworkEnd > adjustedEnd or interworkEnd > adjustedSize:
                        print("WARN:OVERKILLEND", end=' ')
                        break
                # Check ALL last
                if fixSectionsFlags & FIX_ALL and dldiSize != 0:
                    if dldiEnd > adjustedEnd or dldiEnd > adjustedSize:
                        print("WARN:OVERKILLEND", end=' ')
                        break
                break

            # WARN: verify ID+version, show ok/bad
            if magicNumber != wanted_magicNumber or magicString != wanted_magicString or versionNumber != wanted_versionNumber:
                print("WARN:ID/VERSION", end=' ')
            print(i)
    return


if __name__ == "__main__":
    print("DLDISCAN - DLDI Scanning tool 2018 nocash")
    print("Python rewrite by lifehackerhansol")
    scan_dldi_files()
