#include "font.h"
#include "timer.h"
#include <stdio.h>

struct tiles defaultfont;
struct tiles tinyfont;

void font_load_default()
{
    /* 8 x 16 glyphs, 64 columns and 4 rows */
    tiles_load(&defaultfont, "data\\font", 8, 16, 64, 4);

    /* 4 x 6 glyphs, 64 columns and 4 rows */
    tiles_load(&tinyfont, "data\\tinyfont", 4, 6, 64, 4);
}

void font_unload_default()
{
    tiles_unload(&defaultfont);
    tiles_unload(&tinyfont);
}


void font_draw_string(struct tiles * font, const char * text, int delay_ms,
        unsigned short x, unsigned short y)
{
    int i;
    unsigned short xpos=x, ypos=y;

    for (i = 0; text[i] != 0; i++)
    {
        switch (text[i])
        {
            case '\n':
                ypos += font->tile[0].height;
                break;
            case '\r':
                xpos = x;
                break;
            default:
                bitmap_draw(&font->tile[(int)text[i]], xpos, ypos);
                xpos += font->tile[0].width;
                break;
        }
        timer_wait_for(delay_ms);
    }
}
