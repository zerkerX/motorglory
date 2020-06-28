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
#ifndef STAGE_H
#define STAGE_H

#include "actor.h"
#include <stdint.h>
#include <stdio.h>

/* Stage itself is 32 x 20 tiles (8x8 tiles)
Actors have double this precision (move in 4 pixel increments) */
#define STAGE_WIDTH 32
#define STAGE_HEIGHT 20

/* HUD offset for drawing the stage (in pixels) */
#define STAGE_X 8
#define STAGE_Y 16

/* HUD Sidebar and bottom bar offsets */
#define RIGHT_HUD_OFFSET 264
#define BOTTOM_HUD_OFFSET 176

/* Offset between background and foreground tile indexes */
#define FG_TILE_OFFSET 128

struct stage {
       struct actor * first_actor;
       uint8_t tiles[STAGE_WIDTH][STAGE_HEIGHT];
       struct stage * next;
       struct stage * prev;
       struct actor * last_actor;
       int16_t adjstage[NUM_DIRECTIONS];
       int numactors;
};

/* For use when other units (e.g. actors) need to refer to the stage */
extern struct stage * curr_stage;

/* Adds a new generic actor to the stage (e.g. for shooting action,
    or editor) */
struct actor * stage_add_actor_type(struct stage * stage, enum actor_types acttype,
    int16_t x, int16_t y, bool can_be_in_walls);

void stage_remove_actor(struct stage * stage, struct actor * actor);

/* Test if the given actor coordinates contain a wall. */
bool stage_is_solid(struct stage * stage, int x, int y,
        int width, int height);

void stage_new(struct stage * stage);

void stage_load(struct stage * stage, FILE * input);

void stage_save(struct stage * stage, FILE * output);

void stage_unload(struct stage * stage);

void stage_run(struct stage * stage);

void stage_free(struct stage * stage);

/* Hooks to load or unload the graphical tiles used by any stage.
Only need to be called once per session */
void stage_load_tiles();

void stage_unload_tiles();

/* The following are only visible for editor use */
extern struct tiles tilesfg;
extern struct tiles tilesbg;
void stage_draw_bgtile(struct stage * stage, int x, int y);
void stage_redraw_bg(struct stage * stage, bool drawall);
void stage_draw_actors(struct stage * stage, bool drawall);

#endif
