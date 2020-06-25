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
