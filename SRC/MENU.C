#include "menu.h"
#include "bitmap.h"
#include "font.h"
#include "keys.h"
#include "sound.h"


#include <i86.h>
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>

struct bitmap logo;

static const int MENU_OFFSET[] = {64, 64, 4};
static const int MENU_HEIGHT[] = {128, 128, 192};

void menu_init()
{
    font_load_default();
    bitmap_load(&logo, "data\\logo", 300, 54);
}


static void menu_draw_option(struct menu * menu, int i)
{
    char paramtext[7];
    struct parameter * param;
    bool * boolptr;
    int16_t * intptr;
    char * charptr;
    uint8_t * enumptr;

    video_clear_region(16, MENU_OFFSET[menu->style]+16*i, 288, 16);

    if (menu->selection == i)
    {
        video_draw_box(16, MENU_OFFSET[menu->style]+16*i, 288, 16, COLOR_MAGENTA);
    }
    font_draw_string(&defaultfont, menu->options[i].name, 0, 20,
        MENU_OFFSET[menu->style]+16*i);
    
    if (menu->options[i].param != NULL)
    {
        param = menu->options[i].param;
        paramtext[0] = '\0';
        switch (param->type)
        {
            case PARAM_BOOL:
                boolptr = (bool *)param->value;
                snprintf(paramtext, 7, "%s",
                    *boolptr ? param->trueval : param->falseval);
                break;
            case PARAM_INT:
                intptr = (int16_t *)param->value;
                snprintf(paramtext, 7, "%d", *intptr);
                break;
            case PARAM_STRING:
                charptr = (char *)param->value;
                snprintf(paramtext, 7, "%s", charptr);
                break;
            case PARAM_ENUM:
                enumptr = (uint8_t *)param->value;
                snprintf(paramtext, 7, "%s", param->enumdef[*enumptr]);
                break;
        }
        font_draw_string(&defaultfont, paramtext, 0, 252, 
                MENU_OFFSET[menu->style]+16*i);
    }
}


static void menu_increase(struct menu * menu)
{
    struct parameter * param = menu->options[menu->selection].param;
    bool * boolptr;
    int16_t * intptr;
    uint8_t * enumptr;

    if (param != NULL)
    {
        switch (param->type)
        {
            case PARAM_BOOL:
                boolptr = (bool *)param->value;
                *boolptr = !*boolptr;
                break;
            case PARAM_INT:
                intptr = (int16_t *)param->value;
                *intptr += 1;
                if (*intptr > param->maxval)
                    *intptr = param->maxval;
                break;
            case PARAM_ENUM:
                enumptr = (uint8_t *)param->value;
                *enumptr += 1;
                if (*enumptr > param->maxval)
                    *enumptr = param->maxval;
                break;
        }
    }
}


static void menu_decrease(struct menu * menu)
{
    struct parameter * param = menu->options[menu->selection].param;
    bool * boolptr;
    int16_t * intptr;
    uint8_t * enumptr;

    if (param != NULL)
    {
        switch (param->type)
        {
            case PARAM_BOOL:
                boolptr = (bool *)param->value;
                *boolptr = !*boolptr;
                break;
            case PARAM_INT:
                intptr = (int16_t *)param->value;
                *intptr -= 1;
                if (*intptr < param->minval)
                    *intptr = param->minval;
                break;
            case PARAM_ENUM:
                enumptr = (uint8_t *)param->value;
                *enumptr -= 1;
                /* Unsigned overflow will look large... */
                if (*enumptr > param->maxval)
                    *enumptr = 0;
                break;
        }
    }
}


static void menu_draw(struct menu * menu)
{
    int i;

    video_set_mode(CGA_CYAN);
    video_clear();
    if (menu->style != EDITMENU) bitmap_draw(&logo, 8, 4);
    video_draw_box(13, MENU_OFFSET[menu->style], 2,
        MENU_HEIGHT[menu->style], COLOR_WHITE);
    if (menu->style == SUBMENU)
    {
        video_draw_box(10, MENU_OFFSET[menu->style], 2,
            MENU_HEIGHT[menu->style], COLOR_WHITE);
    }
    for (i = 0; i < menu->num_options; i++)
    {
        menu_draw_option(menu, i);
    }
}


void menu_loop(struct menu * menu)
{
    int key = 0;
    bool close = false;
    int lastsel = menu->selection;
    struct option * option;
    bool forcedraw = false;

    menu_draw(menu);

    while (!close)
    {
        video_flip_buffer();
        key = getch();

        switch (key)
        {
            case KEY_ESC:
            case KEY_BACKSPACE:
                 close = true;
                 sound_play(SND_MENU_BACK);
                 break;
            case KEY_UP:
                 menu->selection -= 1;
                 sound_play(SND_MENU_UP);
                 break;
            case KEY_DOWN:
                 menu->selection += 1;
                 sound_play(SND_MENU_DOWN);
                 break;
            case KEY_RIGHT:
                menu_increase(menu);
                forcedraw = true;
                sound_play(SND_MENU_INC);
                break;
            case KEY_LEFT:
                menu_decrease(menu);
                forcedraw = true;
                sound_play(SND_MENU_DEC);
                break;
            case KEY_ENTER:
            case KEY_SPACE:
                sound_play(SND_MENU_OK);
                option = &menu->options[menu->selection];
                if (option->function != NULL)
                {
                    option->function(option->f_param);
                    if (menu->close_on_sel)
                        close = true;
                    else
                        menu_draw(menu);
                }
                break;
        }

        if (lastsel != menu->selection)
        {
            if (menu->selection >= menu->num_options)
                menu->selection = 0;
            else if (menu->selection < 0)
                menu->selection = menu->num_options - 1;

            menu_draw_option(menu, menu->selection);
            menu_draw_option(menu, lastsel);
        }
        else if (forcedraw)
        {
            menu_draw_option(menu, menu->selection);
            forcedraw = false;
        }

        lastsel = menu->selection;
    }
}


void menu_end()
{
    font_unload_default();
    bitmap_unload(&logo);
}


bool menu_get_string(const char * prompt, char * output, int bufsize)
{
    int charnum = 0;
    int key = 0;
    bool result = false;
    bool done = false;

    charnum = strlen(output);
    video_draw_box(7, 75, 306, 46, COLOR_WHITE);
    video_clear_region(8, 76, 304, 44);
    font_draw_string(&defaultfont, prompt, 0, 12, 80);

    while (!done)
    {
        video_clear_region(12, 100, 296, 16);
        font_draw_string(&defaultfont, output, 0, 12, 100);
        font_draw_string(&defaultfont, "_", 0, 12+8*charnum, 100);
        video_flip_buffer();

        key = getch();

        if (key == 0)
        {
            /* Extended character. Not really used here. */
            key = getch();
        }
        else
        {
            /* Normal ASCII character */
            switch (key)
            {
                case KEY_ESC:
                    done = true;
                    result = false;
                    break;
                case KEY_ENTER:
                    done = true;
                    result = true;
                    break;
                case KEY_BACKSPACE:
                    if (charnum > 0)
                    {
                        charnum--;
                        output[charnum] = 0;
                    }
                    break;
                default:
                    /* Normal printable ASCII character (including space) */
                    if (key >= 32 && key < 127)
                    {
                        output[charnum] = (char)key;
                        charnum++;
                    }
                    break;
            }
        }

        if (charnum >= bufsize - 1)
        {
            /* Instantly end if we fill up the buffer */
            done = true;
            result = true;
        }
    }

    return result;
}


bool menu_get_int(const char * prompt, int16_t * output)
{
    char buffer[10];
    bool result;
    memset(buffer, 0, 10);
    
    result = menu_get_string(prompt, buffer, 10);
    if (result)
    {
        if (sscanf(buffer, "%d", output) != 1)
        {
            result = false;
        }
    }

    return result;
}


bool menu_get_coords(const char * prompt, int16_t * x, int16_t * y)
{
    char buffer[20];
    bool result;
    memset(buffer, 0, 10);
    
    result = menu_get_string(prompt, buffer, 10);
    if (result)
    {
        if (sscanf(buffer, "%d, %d", x, y) != 2)
        {
            result = false;
        }
    }

    return result;
}

