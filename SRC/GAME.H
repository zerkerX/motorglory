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
#ifndef GAME_H
#define GAME_H

#include <stdint.h>

enum game_mode
{
    MODE_PLAY,
    MODE_NEWSTAGE,
    MODE_GAMEOVER,
    MODE_QUIT,
    MODE_NEXT_CHAPTER
};

struct game_state
{
    enum game_mode mode;
    int16_t health;
    int16_t max_health;
    int16_t ammo;
    int16_t next_stage;
    int16_t next_x;
    int16_t next_y;
    int16_t statenum; /* overall game state machine */
};

extern struct game_state game_state;

void game_new();
void game_run();
void game_load(uint16_t position);
void game_save(uint16_t position);
void game_savename(uint16_t position, char * outname);

#endif
