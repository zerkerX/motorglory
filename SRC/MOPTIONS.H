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
