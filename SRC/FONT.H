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
