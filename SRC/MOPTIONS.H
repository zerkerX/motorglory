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
#ifndef MOPTIONS_H
#define MOPTIONS_H

#include "menu.h"

#include <stdbool.h>

struct settings {
    bool sound;
    int16_t speed;
    bool dblbuff;
};

extern struct settings settings;

extern struct menu optmenu;

void options_save();

void options_load();

#endif
