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
#include "msavload.h"
#include "game.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>
#include <string.h>

static void menu_save(void * pos_raw)
{
    uint16_t * pos = (uint16_t *)pos_raw;
    game_save(*pos);
}


static void menu_load(void * pos_raw)
{
    uint16_t * pos = (uint16_t *)pos_raw;
    game_load(*pos);
    game_run();
}

static uint16_t savepos[] = {0, 1, 2, 3, 4, 5, 6, 7}; 

static struct option savemenu_opt[] =
{
    {"Empty", &menu_save, &savepos[0], NULL},
    {"Empty", &menu_save, &savepos[1], NULL},
    {"Empty", &menu_save, &savepos[2], NULL},
    {"Empty", &menu_save, &savepos[3], NULL},
    {"Empty", &menu_save, &savepos[4], NULL},
    {"Empty", &menu_save, &savepos[5], NULL},
    {"Empty", &menu_save, &savepos[6], NULL},
    {"Empty", &menu_save, &savepos[7], NULL}
};

static struct option loadmenu_opt[] =
{
    {"Empty", &menu_load, &savepos[0], NULL},
    {"Empty", &menu_load, &savepos[1], NULL},
    {"Empty", &menu_load, &savepos[2], NULL},
    {"Empty", &menu_load, &savepos[3], NULL},
    {"Empty", &menu_load, &savepos[4], NULL},
    {"Empty", &menu_load, &savepos[5], NULL},
    {"Empty", &menu_load, &savepos[6], NULL},
    {"Empty", &menu_load, &savepos[7], NULL}
};

struct menu savemenu = {8, 0, true, SUBMENU, savemenu_opt};
struct menu loadmenu = {8, 0, true, SUBMENU, loadmenu_opt};



void msaveload_refresh()
{
    int i, handle;
    struct stat info;
    struct tm * modtime;
    char savepath[30];
    fstat;

    /* Assume 8 saves until we find less later */
    savemenu.num_options = 8;
    loadmenu.num_options = 8;

    for (i = 0; i < 8; i++)
    {
        game_savename(i, savepath);
        handle = open(savepath, O_RDONLY);
        if (handle != -1)
        {
            /* Get the save modify date to populate the menu */
            fstat(handle, &info);
            close(handle);

            modtime = localtime(&info.st_atime);
            strftime(savemenu_opt[i].name, 40,
                "%d-%h-%Y %I:%M:%S %p", modtime);
            strcpy(loadmenu_opt[i].name, savemenu_opt[i].name);
        }
        else
        {
            /* No save available. End the menu */
            strcpy(savemenu_opt[i].name, "New Save");
            savemenu.num_options = i+1;
            loadmenu.num_options = i;
            break;
        }
    }
}

