/*
io_x9_card.h

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

#ifndef INCLUDED_io_x9_card_h
#define INCLUDED_io_x9_card_h

namespace x9sd {


//void X9CardPolledWrite(uint32 flags, const uint32* buffer, const uint8* command);
void X9CardIO(uint8 cmd, uint8 arg1, uint32 arg2, uint32 flags, uint32* buffer, uint32 words);
void X9CardWriteData(uint8 arg1, uint32 arg2, const void* buffer);
void cardIO_1();
void cardIO_2(uint32 arg);
void cardIO_3(uint8 arg0);
void cardIO_4(uint32 address);


} // namespace x9sd

#endif // INCLUDED_io_x9_card_h
