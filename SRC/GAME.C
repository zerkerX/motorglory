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
#include "game.h"
#include "chapter.h"
#include "cutscene.h"

#include <stdio.h>

struct game_state game_state;

void game_new()
{
    game_state.mode = MODE_NEXT_CHAPTER;
    game_state.statenum = 0;
}

void game_run()
{
    while(game_state.mode != MODE_GAMEOVER
        && game_state.mode != MODE_QUIT
        && game_state.statenum < 4)
    {
        switch (game_state.statenum)
        {
            case 0:
                scene_intro();
                break;
            case 1:
                chapter_load("stage1.map");
                game_state.health = 15;
                game_state.max_health = 15;
                game_state.ammo = 30;
                break;
            case 2:
                /* Separate load from run to support save
                game restores instead. */
                chapter_run();
                chapter_unload();
                break;
            case 3:
                scene_intro_conv();
                break;
        }

        game_state.statenum++;
    }
    
    if (game_state.mode == MODE_GAMEOVER)
    {
        scene_text("G A M E  O V E R", 88, 92, 50, SND_GAMEOVER);
    }
}


void game_load(uint16_t position)
{
    char fullpath[30];
    FILE * input;
    game_savename(position, fullpath);

    input = fopen(fullpath, "rb");
    fread(&game_state, sizeof(game_state), 1, input);

    if (game_state.statenum == 2)
    {
        chapter_load_from(input);
    }

    fclose(input);
    game_run();
}


void game_save(uint16_t position)
{
    char fullpath[30];
    FILE * output;
    game_savename(position, fullpath);

    output = fopen(fullpath, "wb");
    fwrite(&game_state, sizeof(game_state), 1, output);

    if (game_state.statenum == 2)
    {
        curr_chapter.start_stage_num = game_state.next_stage;
        chapter_save_into(output);
    }

    fclose(output);
}


void game_savename(uint16_t position, char * outname)
{
    sprintf(outname, "saves\\game%u.sav", position);
}
