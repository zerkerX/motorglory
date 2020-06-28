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
#include "gallery.h"
#include "bitmap.h"
#include "keys.h"

#include <stdio.h>
#include <conio.h>

const int palettes[] =
{
    CGA_CYAN,
    CGA_CYAN,
    CGA_CYAN,
    CGA_BW,
    CGA_BW,

    CGA_CYAN,
    CGA_BW,
    CGA_CYAN,
    CGA_CYAN,
    CGA_BW,

    CGA_BW,
    CGA_BW,
    CGA_BW,
    CGA_YELLOW,
    CGA_CYAN,

    CGA_CYAN,
};

const char * names[] = { "001", "002", "003", "004", "005", "006", "007",
      "008", "009", "010", "011", "012", "013", "014", "015", "016"};

int gallery_show_image(enum gall_image image)
{
    int key;
    char filename[20];

    sprintf(filename, "data\\%s", names[image]);
    bitmap_display_file(filename, palettes[image]);

    key = getch();
    return (key != KEY_ESC && key != KEY_BACKSPACE);
}
