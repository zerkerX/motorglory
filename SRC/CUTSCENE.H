#ifndef INTRO_H
#define INTRO_H

#include "sound.h"

#include <stdint.h>

void scene_intro();

void scene_intro_conv();

/* Returns the key pressed by the user */
int scene_text(const char * text, uint16_t x, uint16_t y,
    uint32_t delay_ms, enum sound_types sound);

#endif
