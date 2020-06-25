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
