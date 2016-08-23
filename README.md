# T6963C
C library for interfacing a T6963C LCD display with a PIC microcontroller

This library was initially developed for the XC32 compiler, but I see no reason why it shouldn't work with XC16 or XC8.

Copyright &copy; 2015 [Camil Staps](mailto:info@camilstaps.nl). This project is licensed under the GPL v2.0 license. For more details, see the LICENSE file.

## Features

 * Basic routines for writing characters and commands to a T6963C LCD display
 * A unix-like Terminal structure

## Usage

### Just the T6963C library

You will not need `terminal.c` or `terminal.h`. 

Edit the header `t6963c_specific.h` to fill in the ports you will be using. The file given in this repository is merely an example. 

Edit the functions in `t6963c_specific.c`. These will be used to create the appropriate delays. On not so fast devices (5MIPS or less) you can leave them empty. On faster devices, you should use one of the timers. The `t6963c_getTimeNs()` function should return the number of nanoseconds that has elapsed since the last call to `t6963c_startTimer()`. The file in this repository shows an example for a PIC32MZ chip. 

If the pins you are using are by default used by other peripherals (oscillators, JTAG, ... are on by default) you will need to turn them off manually. The library will only take care of setting the appropriate TRIS bits.

Before anything else, you should call `t6963c_init()`, which will set the LCD in TEXT ATTRIBUTE mode and set the appropriate text and graphic areas.

After this, you will mainly need the following functions:

 * `t6963c_clear()` - clear the LCD and set the cursor and he character address to the top left
 * `t6963c_writeString(char* string)` - write a string to the LCD
 * `t6963c_set_address(unsigned char row, unsigned char column)` - set the character address
 * `t6963c_set_cursor_address(unsigned char row, unsigned char column)` - set the cursor address

You may also use `t6963c_writeCmd1()`, `t6963c_writeCmd2()` and `t6963c_writeByte()` for sending raw commands and bytes to the LCD.

See `t6963c.h` for a more detailed explanation on all the functions.

Example:

    t6963c_init();
    t6963c_writeString("Hello world!");
    t6963c_set_address(1,0);
    t6963c_writeString("This is the second row.");

### With the Terminal library

First set everything up as above.

A `Terminal` is a structure which holds among other things a data string and an update function. Characters may be appended to the end of the data string, and may be removed from the end of the data string. You can create a new `Terminal` object with:

    Terminal* term = terminal.construct(256); // Allocate 256 bytes

Then, data may be appended to and discarded from the terminal with the following functions:

    terminal.append(term, "Appending a string...");
    terminal.appendChar(term, 'X');     // Append a single character
    terminal.discard(term, 5);          // Discard 5 characters

After this, the `Terminal` will hold the string `Appending a strin`.

You can link an update function to a `Terminal` object. This function should have the type `void update(Terminal*)`. It will be called whenever data is appended to or discarded from the `Terminal` object. In `t6963c.c` we find a directly usable example of such an update function. You can set it with:

    term.update = t6963c_update_terminal;

From then on, the display will automatically be updated with data in the `Terminal`.

The `Terminal` library contains two useful functions on strings. The first, `terminal.lines_needed(char*, unsigned int row_length)`, calculates the number of lines needed to display a string on a display with a certain row length. The second, `terminal.discard_first_line(char*, unsigned int row_length)` discards the first line of a string as if displayed on a display with a certain row length. Both functions take into account both the row length and the new line character `\n`. A usage example may be found in `t6963c_update_terminal()` in `t6963c.c`.

See `terminal.h` for more information.

## To do

 * Graphic functions
 * Status checking as an option
 * More intuitive intialisation

