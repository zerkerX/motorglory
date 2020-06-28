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
#include "mmain.h"
#include "mphoto.h"
#include "moptions.h"
#include "msavload.h"
#include "sound.h"
#include "font.h"
#include "chapter.h"
#include "game.h"
#include "cutscene.h"
#include "editor.h"

#include <stddef.h>
#include <conio.h>
#include <i86.h>

void mmain_submenu(void * input)
{
    struct menu * submenu = (struct menu *)input;
    menu_loop(submenu);
}


void mmain_newgame(void * unused)
{
    game_new();
    game_run();
}


void mmain_loadgame(void * unused)
{
    msaveload_refresh();
    menu_loop(&loadmenu);
}


void mmain_editor(void * unused)
{
    editor_do();
}


static struct option mainmenu_opt[] =
{
    {"New Game", &mmain_newgame, NULL, NULL},
    {"Load Game", &mmain_loadgame, NULL, NULL},
    {"Options", &mmain_submenu, &optmenu, NULL},
    {"Gallery 1", &mmain_submenu, &photomenu1, NULL},
    {"Gallery 2", &mmain_submenu, &photomenu2, NULL},
    {"Editor", &mmain_editor, NULL, NULL}
};

struct menu mainmenu = {6, 0, false, MAINMENU, mainmenu_opt};
