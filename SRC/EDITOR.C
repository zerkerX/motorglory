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
#include "editor.h"
#include "chapter.h"
#include "keys.h"
#include "font.h"
#include "game.h"
#include "medit.h"
#include "moptions.h"
#include "sound.h"
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <i86.h>

/* Print keyboard info if defined */
#define KEYTEST

struct editor
{
    struct
    {
        /* Cursor coordinates are the same as for background */
        int x;
        int y;
        int dx;
        int dy;
    } cursor;
    int bgtile;
    int fgtile;
    int actor;
    struct stage * stage;
    int stagenum;
    char message[81];
    bool showmsg;
    bool dirty;
};

static struct editor editor;


void editor_draw_hud()
{
    video_clear();

    /* White border around playfield */
    video_draw_box(STAGE_X-1, STAGE_Y-1, STAGE_WIDTH*8+2, 1, COLOR_WHITE);
    video_draw_box(STAGE_X-1, STAGE_Y+STAGE_HEIGHT*8, STAGE_WIDTH*8+2, 1, COLOR_WHITE);
    video_draw_box(STAGE_X-1, STAGE_Y-1, 1, STAGE_HEIGHT*8+2, COLOR_WHITE);
    video_draw_box(STAGE_X+STAGE_WIDTH*8, STAGE_Y-1, 1, STAGE_HEIGHT*8+2, COLOR_WHITE);

    /* Boxes for Sidebar. First, fill with white */
    /* Actor box */
    video_draw_box(RIGHT_HUD_OFFSET+3, 5, 50, 70, COLOR_WHITE);
    video_clear_region(RIGHT_HUD_OFFSET+4, 6, 48, 68);

    /* Foreground */
    video_draw_box(RIGHT_HUD_OFFSET+3, 77, 50, 54, COLOR_WHITE);
    video_clear_region(RIGHT_HUD_OFFSET+4, 78, 48, 52);

    /* Background */
    video_draw_box(RIGHT_HUD_OFFSET+3, 133, 50, 54, COLOR_WHITE);
    video_clear_region(RIGHT_HUD_OFFSET+4, 134, 48, 52);

    /* Keyboard Ref */
    font_draw_string(&tinyfont,
        "F2: Save As, F3: Load, F6: Save, F10: Stage Info, F12: Test", 0, 8, 0);
    font_draw_string(&tinyfont,
        "/: Prev, *: Next, +: Add, -: Remove, ", 0, 8, 6);


    /* Labels for Sidebar */
    font_draw_string(&tinyfont, "Actor\r\n E:Edit\r\n D:Delete", 0,
        RIGHT_HUD_OFFSET+8, 8);
    font_draw_string(&tinyfont, "Ins    Del", 0,
        RIGHT_HUD_OFFSET+8, 64);
    font_draw_string(&defaultfont, "Z", 0,
        RIGHT_HUD_OFFSET+24, 56);

    font_draw_string(&tinyfont, "Foreground", 0,
        RIGHT_HUD_OFFSET+8, 80);
    font_draw_string(&tinyfont, "Home  Page\r\n"
                                "      Up", 0,
        RIGHT_HUD_OFFSET+8, 114);
    font_draw_string(&defaultfont, "X", 0,
        RIGHT_HUD_OFFSET+24, 112);

    font_draw_string(&tinyfont, "Background", 0,
        RIGHT_HUD_OFFSET+8, 136);
    font_draw_string(&tinyfont, "End   Page\r\n"
                                "      Down", 0,
        RIGHT_HUD_OFFSET+8, 168);
    font_draw_string(&defaultfont, "C", 0,
        RIGHT_HUD_OFFSET+24, 168);
}

void editor_draw_palette()
{
    int i, tile;

    video_clear_region(RIGHT_HUD_OFFSET + 20, 32, 16, 24);
    switch (standard_actors[editor.actor].size)
    {
        case SIZE_MEDIUM:
            tiles_replace(&sprites_m,
                    standard_actors[editor.actor].sprites[SOUTH][STANDING],
                    RIGHT_HUD_OFFSET + 24, 40);
            break;
        case SIZE_LARGE:
            tiles_replace(&sprites_l,
                    standard_actors[editor.actor].sprites[SOUTH][STANDING],
                    RIGHT_HUD_OFFSET + 20, 32);
            break;
    }
    
    for (i = 0; i < 5; ++i)
    {
        tile = editor.fgtile - 2 + i;
        
        if (tile < 0)
            tile += tilesfg.num;
        else if (tile >= tilesfg.num)
            tile -= tilesfg.num;
        tiles_replace(&tilesfg, tile,
            RIGHT_HUD_OFFSET + (i+1) * 8, 104);
    }
    for (i = 0; i < 5; ++i)
    {
        tile = editor.bgtile - 2 + i;
        
        if (tile < 0)
            tile += tilesbg.num;
        else if (tile >= tilesbg.num)
            tile -= tilesbg.num;
        tiles_replace(&tilesbg, tile,
            RIGHT_HUD_OFFSET + (i+1) * 8, 160);
    }
}


void editor_draw_stats(int key, bool extkey)
{
    char status_text[320];
    char chapname[20];
    char keytype = extkey ? 'e' : ' ';
    strcpy(chapname, curr_chapter.filename);
    if (editor.dirty) strcat(chapname, "*");
    
    video_clear_region(0, BOTTOM_HUD_OFFSET+8, RIGHT_HUD_OFFSET, 200-BOTTOM_HUD_OFFSET-8);
    if (editor.showmsg)
    {
        strcpy(status_text, editor.message);
    }
    else
    {
        sprintf(status_text,
            "Chapter: %s; Stage: %d/%d; Key:%c%d; Cursor: %d, %d",
            chapname,
            editor.stagenum, curr_chapter.num_stages,
            keytype, key,
            editor.cursor.x, editor.cursor.y);
    }
    font_draw_string(&tinyfont, status_text, 0, 8, BOTTOM_HUD_OFFSET+8);
    if (editor.showmsg)
    {
        /* Beep AFTER displaying so the user can read while waiting */
        sound_play(SND_ERROR);
        editor.showmsg = false;
    }
}


void editor_draw_cursor()
{
    /* Consider better cursor options in the future? */
    video_draw_box(STAGE_X + editor.cursor.x * 8 + 3,
        STAGE_Y + editor.cursor.y * 8 + 3, 2, 2, COLOR_WHITE);
}


void editor_draw(bool drawall)
{
    if (drawall)
    {
        editor_draw_hud();
        stage_redraw_bg(editor.stage, true);
    }
    stage_draw_actors(editor.stage, true);
    editor_draw_cursor();
    editor_draw_palette();
}


void editor_switch_stages()
{
    if (editor.stagenum < 1)
        editor.stagenum = 1;
    else if (editor.stagenum > curr_chapter.num_stages)
        editor.stagenum = curr_chapter.num_stages;

    editor.stage = chapter_get_stage_num(editor.stagenum);
    editor_draw(true);
}


struct actor * editor_find_actor()
{
    /* Finds an actor at the current cursor coordinates */
    struct actor * result = NULL;
    struct actor * search;
    int cursx = editor.cursor.x * 2;
    int cursy = editor.cursor.y * 2;
    const int CURS_SIZE = 2;
    for (search = editor.stage->first_actor; search != NULL;
                search = search->next)
    {
        /* Note: normal actor start position is based on collidable
        area. Subtract 2 from Y to also include top overlay region
        for purposes of cursor selection. */
        if (cursx + CURS_SIZE > search->x
           && search->x + search->width > cursx
           && cursy + CURS_SIZE > search->y - 2
           && search->y + search->height > cursy)
        {
            result = search;
            break;
        }
    }
    return result;
}


void editor_message(const char * msg)
{
    strcpy(editor.message, msg);
    editor.showmsg = true;
}


void editor_loop()
{
    int key = 0;
    struct actor * actor;
    bool extkey = false;
    bool done = false;

    editor_draw(true);
    editor_draw_stats(0, false);

    while (!done)
    {
        video_flip_buffer();
        key = getch();
        extkey = false;

        if (key == 0)
        {
            /* Extended keys */
            extkey = true;
            key = getch();

            switch (key)
            {
                case KEY_UP:
                    editor.cursor.dy = -1;
                    break;
                case KEY_DOWN:
                    editor.cursor.dy = 1;
                    break;
                case KEY_LEFT:
                    editor.cursor.dx = -1;
                    break;
                case KEY_RIGHT:
                    editor.cursor.dx = 1;
                    break;
                case KEY_INSERT:
                    --editor.actor;
                    if (editor.actor < 0)
                        editor.actor += NUM_DEFAULT_ACTORS;
                    break;
                case KEY_DELETE:
                    ++editor.actor;
                    if (editor.actor >= NUM_DEFAULT_ACTORS)
                        editor.actor -= NUM_DEFAULT_ACTORS;
                    break;
                case KEY_HOME:
                    --editor.fgtile;
                    if (editor.fgtile < 0)
                        editor.fgtile += tilesfg.num;
                    break;
                case KEY_PAGE_UP:
                    ++editor.fgtile;
                    if (editor.fgtile >= tilesfg.num)
                        editor.fgtile -= tilesfg.num;
                    break;
                case KEY_END:
                    --editor.bgtile;
                    if (editor.bgtile < 0)
                        editor.bgtile += tilesbg.num;
                    break;
                case KEY_PAGE_DOWN:
                    ++editor.bgtile;
                    if (editor.bgtile >= tilesbg.num)
                        editor.bgtile -= tilesbg.num;
                    break;
                case KEY_F2:
                    {
                        char filename[14];
                        memset(filename, 0, 14);
                        if (menu_get_string("File Name:", filename, 14))
                        {
                            chapter_save_as(filename);
                            editor.dirty = false;
                        }
                    }
                    editor_draw(true);
                    break;
                case KEY_F3:
                    medit_set_load_opts();
                    menu_loop(&load_stage_menu);
                    editor_draw(true);
                    editor.dirty = false;
                    break;
                case KEY_F6:
                    chapter_save();
                    editor.dirty = false;
                    break;
                case KEY_F12:
                    if (editor.dirty)
                    {
                        editor_message("Chapter is not saved! "
                                "Please save before testing.");
                    }
                    else
                    {
                        game_state.health = 25;
                        game_state.max_health = 25;
                        game_state.ammo = 30;
                        game_state.next_x = -1;
                        game_state.next_y = -1;
                        stage_run(editor.stage);
                        
                        /* Re-load after test to restore state */
                        editor_load(curr_chapter.filename);
                        editor_draw(true);
                    }
                    break;
                case KEY_F10:
                    medit_set_stage(editor.stage);
                    menu_loop(&edit_stage_menu);
                    editor_draw(true);
                    break;
            }
        }
        else
        {
            /* Non-extended keys (e.g. basic ASCII results) */
            switch (key)
            {
                case KEY_ESC:
                    done = true;
                    break;
                case 'z':
                    stage_add_actor_type(editor.stage, editor.actor,
                        editor.cursor.x*2, editor.cursor.y*2, true);
                    editor.dirty = true;
                    break;
                case 'c':
                    editor.stage->tiles[editor.cursor.x][editor.cursor.y] = editor.bgtile;
                    editor.dirty = true;
                    break;
                case 'x':
                    editor.stage->tiles[editor.cursor.x][editor.cursor.y] = editor.fgtile + FG_TILE_OFFSET;
                    editor.dirty = true;
                    break;
                case 'e':
                    actor = editor_find_actor();
                    if (actor == NULL)
                    {
                        editor_message("No actor to edit "
                                "at cursor position!");
                    }
                    else
                    {
                        medit_set_actor(actor);
                        menu_loop(&edit_actor_menu);
                        editor.dirty = true;
                        editor_draw(true);
                    }
                    break;
                case 'd':
                    actor = editor_find_actor();
                    if (actor == NULL)
                    {
                        editor_message("No actor to delete "
                                "at cursor position!");
                    }
                    else
                    {
                        stage_remove_actor(editor.stage, actor);
                        editor.dirty = true;
                        editor_draw(true);
                    }
                    break;
                case '/':
                    editor.stagenum--;
                    editor_switch_stages();
                    editor.dirty = true;
                    break;
                case '*':
                    editor.stagenum++;
                    editor_switch_stages();
                    editor.dirty = true;
                    break;
                case '+':
                    chapter_add_new();
                    break;
                case '-':
                    chapter_remove(editor.stage);
                    editor_switch_stages();
                    break;
                default:
                     break;
            }
        }

        /* Redraw the selected tile always */
        stage_draw_bgtile(editor.stage, editor.cursor.x, editor.cursor.y);

        /* Cursor is moving */
        if (editor.cursor.dx != 0 || editor.cursor.dy != 0)
        {
            /* Check for cursor moving out of bounds */
            if (editor.cursor.x + editor.cursor.dx < 0
                    || editor.cursor.x + editor.cursor.dx >= STAGE_WIDTH)
                editor.cursor.dx = 0;
            if (editor.cursor.y + editor.cursor.dy < 0
                    || editor.cursor.y + editor.cursor.dy >= STAGE_HEIGHT)
                editor.cursor.dy = 0;
            
            /* Apply the movement */
            editor.cursor.x += editor.cursor.dx;
            editor.cursor.y += editor.cursor.dy;
            editor.cursor.dx = 0;
            editor.cursor.dy = 0;
        }

        editor_draw(false);
        editor_draw_stats(key, extkey);
    }
}


/* Basic initialization only. Common to first startup and new map load */
static void editor_init()
{
    memset(&editor, sizeof(editor), 0);
    editor.cursor.x = STAGE_WIDTH / 2;
    editor.cursor.y = STAGE_HEIGHT / 2;
    editor.stagenum = curr_chapter.start_stage_num;
    editor.stage = chapter_get_stage_num(editor.stagenum);
    
    /* Right now, editor starts with a blank map. The user will
     * need to save this before they can test. */
    editor.dirty = true;
}


void editor_load(const char * filename)
{
    /* This function assumes the editor is already initialized */
    chapter_load(filename);
    editor_init();
}


void editor_do()
{
    if (settings.dblbuff) video_enable_double_buffer();
    actor_load_sprites();
    stage_load_tiles();
    chapter_new();
    editor_init();
    editor_loop();
    chapter_unload();
    actor_unload_sprites();
    stage_unload_tiles();
    if (settings.dblbuff) video_disable_double_buffer();
}
