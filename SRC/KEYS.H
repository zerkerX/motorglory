/* This file is part of Motor Glory.

Motor Glory is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Motor Glory is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Motor Glory.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef KEYS_H
#define KEYS_H

#include <stdbool.h>

/* Definitions for ASCII key equivalents */
#define KEY_ENTER 13
#define KEY_SPACE 32
#define KEY_BACKSPACE 8
#define KEY_ESC 27

/* Definitions for keyboard scan codes */
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_F1 59
#define KEY_F2 60
#define KEY_F3 61
#define KEY_F4 62
#define KEY_F5 63
#define KEY_F6 64
#define KEY_F7 65
#define KEY_F8 66
#define KEY_F9 67
#define KEY_F10 68
#define KEY_F11 133
#define KEY_F12 134
#define KEY_INSERT 82
#define KEY_DELETE 83
#define KEY_HOME 71
#define KEY_END 79
#define KEY_PAGE_UP 73
#define KEY_PAGE_DOWN 81
#define KEY_SCAN_SPACE 57
#define KEY_SCAN_ESC 1

extern bool key_pressed[];

void key_replace_isr();
void key_restore_isr();
bool key_was_pushed(int keycode);

#endif
