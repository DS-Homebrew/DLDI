/*
io_x9sd.dldi.h

Hardware routines for interfacing with SD cards on a Ninjapass X9

Copyright (C) 2007 by CJ Bell
siegebell at gmail dot com


This software is provided 'as-is', without any express or implied warranty.  In
no event will the author be held liable for any damages arising from the use of
this software.  Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not claim
    that you wrote the original software. If you use this software in a
    product, an acknowledgment in the product documentation would be
    appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.

*/


#ifdef __cplusplus
extern "C" {
#endif

extern bool _X9SD_isInserted(void);
extern bool _X9SD_clearStatus (void);
extern bool _X9SD_shutdown(void);
extern bool _X9SD_startup(void);
extern bool _X9SD_writeSectors(u32 sector, u32 sectors, const u8* buffer);
extern bool _X9SD_readSectors(u32 sector, u32 sectorCount, u8* buffer);

#ifdef __cplusplus
} // extern "C"
#endif
