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

#include "terminal.h"
#include <stdlib.h>
#include <string.h>

static Terminal* terminal_construct(unsigned int length) {
    char* content = calloc(1, length + 1);
    Terminal* terminal = calloc(1, sizeof(Terminal));
    if (content == NULL || terminal == NULL)
        return NULL;
    
    terminal->content = content;
    terminal->length = length;
    terminal->update = NULL;
    
    return terminal;
}

static void terminal_free(Terminal* terminal) {
    free(terminal->content);
    free(terminal);
}

static unsigned terminal_append_n(
		Terminal* terminal, char* string, unsigned short n) {
    if (strlen(terminal->content) + n > terminal->length) {
        unsigned int length = terminal->length + n * 2;
        char* content = calloc(1, length + 1);
        if (content == NULL)
            return 0;
        strcpy(content, terminal->content);
        free(terminal->content);
        terminal->content = content;
        terminal->length = length;
    }
    strncat(terminal->content, string, n);
    if (terminal->update)
        terminal->update(terminal);
    return 1;
}

static unsigned terminal_append(Terminal* terminal, char* string) {
	return terminal_append_n(terminal, string, strlen(string));
}

static unsigned terminal_appendChar(Terminal* terminal, char character) {
    if (strlen(terminal->content) + 1 > terminal->length) {
        unsigned int length = terminal->length * 2;
        char* content = calloc(1, length + 1);
        if (content == NULL)
            return 0;
        strcpy(content, terminal->content);
        free(terminal->content);
        terminal->content = content;
        terminal->length = length;
    }
    unsigned int length = strlen(terminal->content);
    terminal->content[length] = character;
    terminal->content[length + 1] = NULL;
    if (terminal->update)
        terminal->update(terminal);
    return 1;
} 

static void terminal_discard(Terminal* terminal, unsigned int n) {
	unsigned int len;
	len = strlen(terminal->content);
	if (len < n)
		n = len;
    *(terminal->content + strlen(terminal->content) - n) = NULL;
    if (terminal->update)
        terminal->update(terminal);
}

static unsigned int terminal_lines_needed(char* string, unsigned int row_length) {
    unsigned int i;
    unsigned int lines = 1, current_line = 0;
    for (i=0; string[i]; i++) {
        if (string[i] == '\n') {
            current_line = 0;
            lines++;
        } else if (current_line == row_length - 1) {
            current_line = 0;
            lines++;
        } else {
            current_line++;
        }
    }
    return lines;
}

static void terminal_discard_first_line(char* string, unsigned int row_length) {
    unsigned int i;
    for (i = 0; string[i] && string[i] != '\n' && i < row_length - 1; i++);
    strcpy(string, string + i + 1);
}

const Terminal_namespace terminal = {
    terminal_construct,
    terminal_free,
    terminal_append,
    terminal_append_n,
    terminal_appendChar,
    terminal_discard,
    terminal_lines_needed,
    terminal_discard_first_line
};
