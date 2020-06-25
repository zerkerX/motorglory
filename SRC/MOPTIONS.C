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

