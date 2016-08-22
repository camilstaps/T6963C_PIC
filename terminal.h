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
 * File:   terminal.h
 * Author: Camil Staps <info@camilstaps.nl>
 *
 * A Terminal is an abstract representation of a unix-like terminal. It holds a 
 * data string. Whenever the data is updated, a callback is called, to allow di-
 * rect updates on a display or elsewhere. 
 * Data may be added and removed at the end of the string, nowhere else.
 * 
 * The namespace_struct eases function names. Functions may now be called any-
 * where with, for example:
 * 
 *     terminal.construct(...);
 */

#ifndef TERMINAL_H
#define	TERMINAL_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct Terminal {
    char* content;                    // actual content
    unsigned int length;              // length of the allocated memory block
    void (*update)(struct Terminal*); // update callback
} Terminal;

typedef struct {
    /**
     * Construct a new Terminal with a specified size
     */
    Terminal* (*const construct)(unsigned int size);
    
    /**
     * Free a Terminal and all blocks allocated for it
     */
    void (*const free)(Terminal*);
    
    /**
     * Append a string to a Terminal's data, and call the update callback
     * A larger block of memory is allocated if needed.
     * @return 0 if allocating more memory failed, 1 otherwise
     */
    unsigned (*const append)(Terminal*, char*);
    
    /**
     * Append a character to a Terminal's data, and call the update callback
     * A larger block of memory is allocated if needed.
     * @return 0 if allocating more memory failed, 1 otherwise
     */
    unsigned (*const appendChar)(Terminal*, char);
    
    /**
     * Discard the last n characters of a Terminal's data, and call the update
     * callback.
     */
    void (*const discard)(Terminal*, unsigned int);
    
    /**
     * Calculate how many lines are needed to display a string on a display with
     * n columns, if words wrap occur at any place in a word and \n gives a new
     * line.
     */
    unsigned int (*const lines_needed)(char*, unsigned int row_length);
    
    /**
     * Discard the first line of a string, where a line has n columns - that is,
     * discard until the first \n or until n characters have passed, whichever
     * occurs first.
     */
    void (*const discard_first_line)(char*, unsigned int row_length);
} Terminal_namespace;

extern const Terminal_namespace terminal;

#ifdef	__cplusplus
}
#endif

#endif	/* TERMINAL_H */

