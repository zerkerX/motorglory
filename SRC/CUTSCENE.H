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
