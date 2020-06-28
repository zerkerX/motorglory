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
#ifndef FONT_H
#define FONT_H

#include "tiles.h"

extern struct tiles defaultfont;
extern struct tiles tinyfont;

void font_load_default();

void font_unload_default();

void font_draw_string(struct tiles * font, const char * text, int delay_ms,
        unsigned short x, unsigned short y);

#endif
