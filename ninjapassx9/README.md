X9SD DLDI Driver
Hardware driver to facilitate SD card access for Ninjapass X9

-

Changes:

16/09/2019

- remove DMA dependency, use a faster ARM memcpy. Improves stability.
- a bit faster performing writes to SD / less resource intensive
- fix/remove warnings


09/23/2016
coto: fixed proper DLDI support for most if not all LIBNDS homebrew, everything I have tried works

-

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

