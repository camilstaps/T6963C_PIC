/**
 * C library for interfacing a T6963C display with a PIC microcontroller
 * Copyright (C) 2015 Camil Staps <info@camilstaps.nl>

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <p32xxxx.h>
#include "t6963c_specific.h"
#include "system_config.h"

inline void t6963c_initTimer(void) {
}

inline void t6963c_startTimer(void) {
    T2CON = NULL;
    PR2 = 0xffff;
    TMR2 = NULL;
    T2CONSET = 0xa000;
}

inline unsigned short t6963c_getTimeNs(void) {
    return TMR2 * t6963c_nspertick;
}

inline void t6963c_stopTimer(void) { 
    T2CONCLR = 0xa000;
}
