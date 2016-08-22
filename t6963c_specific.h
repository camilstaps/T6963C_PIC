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
 *
 *******************************************************************************
 * 
 * File:   t6963c_specific.h
 * Author: Camil Staps
 *
 * Example of what you could put in a t6963c_specific.h.
 */

#ifndef T6963C_SPECIFIC_H
#define	T6963C_SPECIFIC_H

#include <p32xxxx.h>

#define t6963c_rst(x)    LATBbits.LATB12   = x // RESET line
#define t6963c_cd(x)     LATBbits.LATB8    = x // C/D line
#define t6963c_ce(x)     LATBbits.LATB9    = x // CE line
#define t6963c_rd(x)     LATBbits.LATB10   = x // RD line
#define t6963c_wr(x)     LATBbits.LATB11   = x // WR line
#define t6963c_t_rst(x)  TRISBbits.TRISB12 = x // TRIS bit of RESET pin
#define t6963c_t_cd(x)   TRISBbits.TRISB8  = x // TRIS bit of C/D pin
#define t6963c_t_ce(x)   TRISBbits.TRISB9  = x // TRIS bit of CE pin
#define t6963c_t_rd(x)   TRISBbits.TRISB10 = x // TRIS bit of RD pin
#define t6963c_t_wr(x)   TRISBbits.TRISB11 = x // TRIS bit of WR pin
#define t6963c_data(x)   LATB  = (LATB  & 0xff00) | x; // Data port (lowest 8 bits)
#define t6963c_t_data(x) TRISB = (TRISB & 0xff00) | x; // TRIS register of data port
    
#define t6963c_rows 16                  // Number of rows of the LCD
#define t6963c_columns 40               // Number of columns of the LCD

#ifdef	__cplusplus
extern "C" {
#endif

#define t6963c_nspertick 25
    
/**
 * Define the project-specific timer functions here
 * @see t6963c.h
 */
inline void t6963c_initTimer(void);
inline void t6963c_startTimer(void);
inline unsigned short t6963c_getTimeNs(void);
inline void t6963c_stopTimer(void);

#ifdef	__cplusplus
}
#endif

#endif	/* T6963C_SPECIFIC_H */

