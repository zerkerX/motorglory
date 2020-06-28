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
#include "moptions.h"
#include "gallery.h"
#include "chapter.h"
#include "editor.h"
#include <stddef.h>
#include <stdio.h>

struct settings settings = {true, 10, true};

static struct parameter param_sound = {PARAM_BOOL, &settings.sound,
    "Yes", "No", 0, 0, NULL};

static struct parameter param_speed = {PARAM_INT, &settings.speed,
    NULL, NULL, 5, 30, NULL};

static struct parameter param_buffered = {PARAM_BOOL, &settings.dblbuff,
    "Double", "Single", 0, 0, NULL};


static struct option optmenu_opt[] =
{
    {"Sound", NULL, NULL, &param_sound},
    {"Game Speed (fps)", NULL, NULL, &param_speed},
    {"Video Buffers", NULL, NULL, &param_buffered}
};

struct menu optmenu = {3, 0, false, SUBMENU, optmenu_opt};

void options_save()
{
    FILE * optfile = fopen("options.dat", "wb");
    if (optfile != NULL)
    {
        fwrite(&settings, sizeof(settings), 1, optfile);
        fclose(optfile);
    }
}

void options_load()
{
    FILE * optfile = fopen("options.dat", "rb");
    if (optfile != NULL)
    {
        fread(&settings, sizeof(settings), 1, optfile);
        fclose(optfile);
    }
}

