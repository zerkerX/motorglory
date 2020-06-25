#include "medqueue.h"
#include "video.h"
#include "font.h"
#include "keys.h"
#include "menu.h"
#include <conio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

static const char actionnames[NUM_ACTIONS][20] =
{
    "NO_ACTION",
    "MOVE_NORTH",
    "MOVE_EAST",
    "MOVE_SOUTH",
    "MOVE_WEST",
    "MOVE_NORTH_FAST",
    "MOVE_EAST_FAST",
    "MOVE_SOUTH_FAST",
    "MOVE_WEST_FAST",
    "MOVE_PLAYER",
    "MOVE_PLAYER_FAST",
    "MOVE_RANDOM",
    "MOVE_RANDOM_FAST",
    "MOVE_FACING",
    "MOVE_FACING_FAST",
    "LOOK_NORTH",
    "LOOK_EAST",
    "LOOK_SOUTH",
    "LOOK_WEST",
    "SHOOT_NORTH",
    "SHOOT_EAST",
    "SHOOT_SOUTH",
    "SHOOT_WEST",
    "SHOOT_FACING",
    "SHOOT_PLAYER",
    "SHOOT_RANDOM",
    "STOP_SELF",
    "STOP_OTHER",
    "KILL_SELF",
    "KILL_OTHER",
    "HURT_ANY",
    "HURT_PLAYER",
    "END_CHAPTER",
    "CHANGE_SPRITES",
    "SHOW_SPRITE",
    "SPAWN_NORTH",
    "SPAWN_EAST",
    "SPAWN_SOUTH",
    "SPAWN_WEST",
    "ADD_AMMO",
    "ADD_HEALTH",
    "TELEPORT_OTHER",
    "TELEPORT_SELF"
};

static const char actionkeys[NUM_ACTIONS] =
{
    ' ', /* NO_ACTION, */
    'w', /* MOVE_NORTH, */
    'd', /* MOVE_EAST, */
    's', /* MOVE_SOUTH, */
    'a', /* MOVE_WEST, */
    'W', /* MOVE_NORTH_FAST, */
    'D', /* MOVE_EAST_FAST, */
    'S', /* MOVE_SOUTH_FAST, */
    'A', /* MOVE_WEST_FAST, */
    'p', /* MOVE_PLAYER, */
    'P', /* MOVE_PLAYER_FAST, */
    'r', /* MOVE_RANDOM, */
    'R', /* MOVE_RANDOM_FAST, */
    'f', /* MOVE_FACING, */
    'F', /* MOVE_FACING_FAST, */
    'i', /* LOOK_NORTH, */
    'l', /* LOOK_EAST, */
    'k', /* LOOK_SOUTH, */
    'j', /* LOOK_WEST, */
    'I', /* SHOOT_NORTH, */
    'L', /* SHOOT_EAST, */
    'K', /* SHOOT_SOUTH, */
    'J', /* SHOOT_WEST, */
    'o', /* SHOOT_FACING, */
    'O', /* SHOOT_PLAYER, */
    'u', /* SHOOT_RANDOM, */
    'q', /* STOP_SELF, */
    'Q', /* STOP_OTHER, */
    'z', /* KILL_SELF, */
    'Z', /* KILL_OTHER, */
    'x', /* HURT_ANY, */
    'X', /* HURT_PLAYER, */
    'e', /* END_CHAPTER, */
    'c', /* CHANGE_SPRITES, */
    'C', /* SHOW_SPRITE, */
    'g', /* SPAWN_NORTH, */
    'n', /* SPAWN_EAST, */
    'b', /* SPAWN_SOUTH, */
    'v', /* SPAWN_WEST, */
    'H', /* ADD_AMMO, */
    'h', /* ADD_HEALTH, */
    't', /* TELEPORT_OTHER, */
    'T'  /* TELEPORT_SELF, */
};


/* Act suffixes are for number of actions;
base parameters are for raw queue position */
struct medqueue
{
    int sel;
    int sel_act;
    int offset;
    int offset_act;
    bool changed;
    uint8_t queue[1024];
    bool is_param[1024];
    int num;
    int num_act;
};

static struct medqueue medqueue;


static void medqueue_draw_option(int i, int pos)
{
    char temptext[7];

    video_clear_region(0, 6*(pos-medqueue.offset_act), 100, 6);

    if (medqueue.sel == i)
    {
        video_draw_box(20, 6*(pos-medqueue.offset_act), 80, 6, COLOR_MAGENTA);
    }

    /* Number each entry so large queues can be easily navigated */
    sprintf(temptext, "%4d", i);
    font_draw_string(&tinyfont, temptext, 0, 0, (pos-medqueue.offset_act)*6);

    /* Only draw text that is *IN* range. However, allow OOR selection
    to handle empty queues, or add a new item at the end */
    if (i < medqueue.num)
    {
        font_draw_string(&tinyfont, actionnames[medqueue.queue[i]],
                0, 20, (pos-medqueue.offset_act)*6);

        /* Check and print parameters */
        if (medqueue.queue[i] >= START_2PARAM_ACTIONS)
        {
            sprintf(temptext, "%2d,%2d",
                    (int)medqueue.queue[i+1],
                    (int)medqueue.queue[i+2]);
            font_draw_string(&tinyfont, temptext,
                    0, 80, (pos-medqueue.offset_act)*6);
        }
        else if (medqueue.queue[i] >= START_1PARAM_ACTIONS)
        {
            sprintf(temptext, "%3d",
                    (int)medqueue.queue[i+1]);
            font_draw_string(&tinyfont, temptext,
                    0, 88, (pos-medqueue.offset_act)*6);
        }
    }
}


static void medqueue_draw_legend()
{
    int row, col;
    enum actions act;
    char legtext[22];
    
    for (act = NO_ACTION; act < NUM_ACTIONS; act++)
    {
        row = act % 32;
        col = act / 32;

        sprintf(legtext, "%c:%s", actionkeys[act], actionnames[act]);
        font_draw_string(&tinyfont, legtext, 0, 100*(col+1), row*6);
    }
}


static void medqueue_draw()
{
    int i, pos, end;
    end = medqueue.offset_act + 32;
    if (end > medqueue.num_act) end = medqueue.num_act;

    video_clear();
    medqueue_draw_legend();

    i = medqueue.offset;
    for (pos = medqueue.offset_act; pos <= end; pos++)
    {
        while( medqueue.is_param[i] ) i++;

        medqueue_draw_option(i, pos);
        i++;
    }
}


static void medqueue_move(int amount)
{
    medqueue.sel += amount;
    medqueue.sel_act += amount;

    if (medqueue.sel > medqueue.num)
    {
        medqueue.sel = 0;
        medqueue.sel_act = 0;
    }
    else if (medqueue.sel < 0)
    {
        medqueue.sel_act = medqueue.num_act;
        medqueue.sel = medqueue.num;
    }

    /* Shift past parameters */
    if (amount > 0)
    {
        while (medqueue.sel < medqueue.num
                && medqueue.is_param[medqueue.sel])
            medqueue.sel++;
    }
    else
    {
        while (medqueue.is_param[medqueue.sel])
            medqueue.sel--;
    }
}


static void medqueue_find_offset()
{
    int pos, pos_act;

    pos = 0;
    pos_act = 0;

    /* Find the offset position from the start of the queue. */
    while (pos_act < medqueue.offset_act)
    {
        pos++;
        pos_act++;

        while (pos < medqueue.num && medqueue.is_param[pos]) pos++;
    }
    medqueue.offset = pos;
}


static void medqueue_add(enum actions action)
{
    int i;
    int16_t param[2];
    bool paramok;
    int paramcount;
    
    if (medqueue.num < 1022)
    {
        if (action >= START_2PARAM_ACTIONS)
        {
            paramok = menu_get_coords(
                    "Please enter two parameters (x, y)", &param[0], &param[1]);
            paramcount = 2;
        }
        else if (action >= START_1PARAM_ACTIONS)
        {
            paramok = menu_get_int("Please enter parameter value", &param[0]);
            paramcount = 1;
        }
        else
        {
            paramok = true;
            paramcount = 0;
        }

        if (paramok)
        {
            for (i = medqueue.num+paramcount;
                    i > medqueue.sel+paramcount; i--)
            {
                medqueue.queue[i] = medqueue.queue[i-1-paramcount];
                medqueue.is_param[i] = medqueue.is_param[i-1-paramcount];
            }
            medqueue.queue[medqueue.sel] = action;
            medqueue.is_param[medqueue.sel] = false;
            medqueue.sel++;
            medqueue.sel_act++;
            medqueue.num++;
            medqueue.num_act++;
            for (i = 0; i < paramcount; i++)
            {
                medqueue.queue[medqueue.sel] = param[i];
                medqueue.is_param[medqueue.sel] = true;
                medqueue.sel++;
                medqueue.num++;
            }
        }
    }
    medqueue.changed = true;
    medqueue_draw();
}


static void medqueue_remove()
{
    int i;
    int paramcount = 0;

    if (medqueue.queue[medqueue.sel] >= START_2PARAM_ACTIONS)
    {
        paramcount = 2;
    }
    else if (medqueue.queue[medqueue.sel] >= START_1PARAM_ACTIONS)
    {
        paramcount = 1;
    }

    if (medqueue.num > 0 && medqueue.sel < medqueue.num)
    {
        for (i = medqueue.sel; i < medqueue.num - 1 - paramcount; i++)
        {
            medqueue.queue[i] = medqueue.queue[i + 1 + paramcount];
            medqueue.is_param[i] = medqueue.is_param[i + 1 + paramcount];
        }
        medqueue.num -= 1 + paramcount;
        medqueue.num_act--;
        medqueue_move(-1);
        medqueue.changed = true;
        medqueue_draw();
    }
}


void medqueue_paramupd()
{
    int i;
    int16_t param[2];
    bool paramok = false;
    int paramcount = 0;
    
    if (medqueue.queue[medqueue.sel] >= START_2PARAM_ACTIONS)
    {
        paramok = menu_get_coords(
                 "Please enter two parameters (x, y)", &param[0], &param[1]);
        paramcount = 2;
    }
    else if (medqueue.queue[medqueue.sel] >= START_1PARAM_ACTIONS)
    {
        paramok = menu_get_int("Please enter parameter value", &param[0]);
        paramcount = 1;
    }

    if (paramok)
    {
        for (i = 0; i < paramcount; i++)
            medqueue.queue[medqueue.sel+i+1] = param[i];

        medqueue.changed = true;    
    }

    /* Redraw for either case above to remove the prompt */
    if (paramcount > 0) medqueue_draw();
}


void medqueue_run(struct action_queue * queue)
{
    int key = 0;
    bool close = false;
    int lastsel = 0, lastsel_act = 0, lastoffs = 0;
    int i;
    bool forcedraw = false;
    enum actions newact;

    medqueue.sel = 0;
    medqueue.sel_act = 0;
    medqueue.offset = 0;
    medqueue.offset_act = 0;
    medqueue.num = queue->num;
    medqueue.num_act = queue->num;
    medqueue.changed = false;
    memcpy(medqueue.queue, queue->queue, queue->num);

    /* Locate and flag parameters. */
    for (i = 0; i < medqueue.num; i++)
    {
        /* We always start with explicit actions */
        medqueue.is_param[i] = false;

        /* Check action type to locate subsequent parameters */
        if (medqueue.queue[i] >= START_2PARAM_ACTIONS)
        {
            medqueue.is_param[i+1] = true;
            medqueue.is_param[i+2] = true;
            i += 2;
            medqueue.num_act -= 2;
        }
        else if (medqueue.queue[i] >= START_1PARAM_ACTIONS)
        {
            medqueue.is_param[i+1] = true;
            i++;
            medqueue.num_act -= 1;
        }
    }

    medqueue_draw();

    while (!close)
    {
        video_flip_buffer();
        key = getch();

        if (key == 0)
        {
            /* Extended character. */
            key = getch();

            switch (key)
            {
                /* TODO: Complex actions should skip parameters */
                case KEY_UP:
                    medqueue_move(-1);
                    break;
                case KEY_DOWN:
                    medqueue_move(1);
                    break;
                case KEY_PAGE_UP:
                    medqueue_move(-24);
                    break;
                case KEY_PAGE_DOWN:
                    medqueue_move(24);
                    break;
                case KEY_DELETE:
                    medqueue_remove();
                    break;
            }
        }
        else
        {
            /* Normal ASCII character */
            switch (key)
            {
                case KEY_ESC:
                    close = true;
                break;
                case KEY_ENTER:
                    medqueue_paramupd();
                    break;
                default:
                    for (newact = NO_ACTION; newact < NUM_ACTIONS; newact++)
                    {
                        if (actionkeys[newact] == key)
                        {
                            medqueue_add(newact);
                        }
                    }
                    break;
            }
        }

        if (lastsel != medqueue.sel)
        {
            medqueue.offset_act = (medqueue.sel_act / 24) * 24 - 4;
            if (medqueue.offset_act < 0) medqueue.offset_act = 0;
            
            if ( lastoffs != medqueue.offset_act )
            {
                medqueue_find_offset();
                medqueue_draw();
            }
            else
            {
                medqueue_draw_option(medqueue.sel, medqueue.sel_act);
                medqueue_draw_option(lastsel, lastsel_act);
            }
        }
        else if (forcedraw)
        {
            medqueue_draw_option(medqueue.sel, medqueue.sel_act);
            forcedraw = false;
        }

        lastsel = medqueue.sel;
        lastsel_act = medqueue.sel_act;
        lastoffs = medqueue.offset;
    }

    if (medqueue.changed)
    {
        if (queue->custom && queue->num > 0)
        {
            free (queue->queue);
        }

        queue->num = medqueue.num;
        queue->curr = 0;
        if (medqueue.num > 0)
        {
            queue->queue = malloc(medqueue.num);
            memcpy(queue->queue, medqueue.queue, medqueue.num);
            queue->custom = true;
        }
        else
        {
            queue->queue = NULL;
            queue->custom = false;
        }
    }
}

