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
    unsigned char* content = calloc(1, length + 1);
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

static unsigned terminal_append(Terminal* terminal, unsigned char* string) {
    if (strlen(terminal->content) + strlen(string) > terminal->length) {
        unsigned int length = terminal->length + strlen(string) * 2;
        unsigned char* content = calloc(1, length + 1);
        if (content == NULL)
            return 0;
        strcpy(content, terminal->content);
        free(terminal->content);
        terminal->content = content;
        terminal->length = length;
    }
    strcat(terminal->content, string);
    if (terminal->update)
        terminal->update(terminal);
    return 1;
}

static unsigned terminal_appendChar(Terminal* terminal, unsigned char character) {
    if (strlen(terminal->content) + 1 > terminal->length) {
        unsigned int length = terminal->length * 2;
        unsigned char* content = calloc(1, length + 1);
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
    n = min(strlen(terminal->content), n);
    *(terminal->content + strlen(terminal->content) - n) = NULL;
    if (terminal->update)
        terminal->update(terminal);
}

static unsigned int terminal_lines_needed(unsigned char* string, unsigned int row_length) {
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

static void terminal_discard_first_line(unsigned char* string, unsigned int row_length) {
    unsigned int i;
    unsigned char* s = string;
    for (i = 0; s[i] && s[i] != '\n' && i < row_length; i++);
    strcpy(string, s + i);
}

const Terminal_namespace terminal = {
    terminal_construct,
    terminal_free,
    terminal_append,
    terminal_appendChar,
    terminal_discard,
    terminal_lines_needed,
    terminal_discard_first_line
};
