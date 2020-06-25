#include "stage.h"
#include "keys.h"
#include "font.h"
#include "game.h"
#include "sound.h"
#include "timer.h"
#include "moptions.h"
#include "msavload.h"
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <i86.h>
#include <math.h>

/* Stage itself is 32 x 20 tiles (8x8 tiles)
Actors have double this precision (move in 4 pixel increments)
MIN and MAX tuned to prevent partial overlap with outer edges, or
for spilling over the top boundary.
TODO Reconsider ranges or algorithm when implementing large enemies. */
#define MIN_X 0
#define MAX_X 62
#define MIN_Y 2
#define MAX_Y 38

/* Print keyboard info if defined */
#define KEYTEST

struct stage * curr_stage = NULL;
struct tiles tilesfg;
struct tiles tilesbg;
struct bitmap ak77;

/* Test if the given actor coordinates & dimensions contain a wall. */
bool stage_is_solid(struct stage * stage, int test_x, int test_y,
        int width, int height)
{
    bool result = false;
    int x, y, x1, x2, y1, y2;
    
                    x1 = test_x/2;
                    x2 = (test_x+width-1)/2;
                    y1 = test_y/2;
                    y2 = (test_y+height-1)/2;

    for (x = x1; x <= x2; x++)
        for (y = y1; y <= y2; y++)
            result |= stage->tiles[x][y] >= FG_TILE_OFFSET;

    return result;
}


void stage_add_actor(struct stage * stage, struct actor * actor)
{
    if (stage->first_actor == NULL)
    {
        stage->first_actor = actor;
        stage->last_actor = actor;
        actor->next = NULL;
        actor->prev = NULL;
    }
    else
    {
        /* Safe to assume last_actor is defined now too */
        stage->last_actor->next = actor;
        actor->prev = stage->last_actor;
        actor->next = NULL;
        stage->last_actor = actor;
    }
    if (actor->class == CLASS_PLAYER) curr_player = actor;
    stage->numactors++;
}

void stage_remove_actor(struct stage * stage, struct actor * actor)
{
    if (actor == stage->first_actor)
        stage->first_actor = actor->next;
    else
        actor->prev->next = actor->next;

    if (actor == stage->last_actor)
        stage->last_actor = actor->prev;
    else
        actor->next->prev = actor->prev;

    stage->numactors--;
    actor_free(actor);
}


struct actor * stage_add_actor_type(struct stage * stage, enum actor_types acttype,
    int16_t x, int16_t y, bool can_be_in_walls)
{
    /* New actors cannot spawn in walls or outside the playarea */
    if ( x < MIN_X || x > MAX_X
        || y < MIN_Y || y > MAX_Y
        || (!can_be_in_walls && stage_is_solid(stage, x, y, 2, 2)) )
    {
        return NULL;
    }
    else
    {
        struct actor * actor = malloc(sizeof(struct actor));
        *actor = standard_actors[acttype];

        actor->x = x;
        actor->y = y;

        stage_add_actor(stage, actor);
        return actor;
    }
}


void stage_default_actors(struct stage * stage)
{
    stage_add_actor_type(stage, SCARAB_STANDARD, 10, 10, false);
}


void stage_new(struct stage * stage)
{
    memset(stage, 0, sizeof(*stage));
    stage_default_actors(stage);
}


void stage_load(struct stage * stage, FILE * input)
{
    int i;
    struct actor * actor;
    struct actor * prev_actor = NULL;
    fread(stage, sizeof(*stage), 1, input);
    /* Don't trust pointers from the file. Actor pointer will
    be set by loading the actors; next/prev will be set by chapter
    loader. */
    stage->first_actor = NULL;
    stage->last_actor = NULL;
    stage->next = NULL;
    stage->prev = NULL;

    for (i = 0; i < stage->numactors; ++i )
    {
        actor = malloc(sizeof(*actor));
        actor_load(actor, input);
        if (prev_actor != NULL)
        {
            prev_actor->next = actor;
            actor->prev = prev_actor;
        }
        else
        {
            /* First actor */
            stage->first_actor = actor;
            actor->prev = NULL;
        }
        prev_actor = actor;
    }
    stage->last_actor = prev_actor;
}


void stage_save(struct stage * stage, FILE * output)
{
    struct actor * actor;
    fwrite(stage, sizeof(*stage), 1, output);

    for (actor = stage->first_actor; actor != NULL;
        actor = actor->next )
    {
        actor_save(actor, output);
    }
}


void stage_unload(struct stage * stage)
{
    struct actor * actor;
    struct actor * next_actor;

    /* Free all actors */
    for (actor = stage->first_actor; actor != NULL; actor = next_actor )
    {
        next_actor = actor->next;
        actor_free(actor);
    }
}


void stage_draw_hud(bool redraw)
{
    static int last_health = 0;
    static int last_ammo;
    char ammocount[6];
    
    /* TODO: New CSU? */
    if (redraw)
    {
        video_clear();

        /* White border around playfield */
        video_draw_box(STAGE_X-1, STAGE_Y-1, STAGE_WIDTH*8+2, 1, COLOR_WHITE);
        video_draw_box(STAGE_X-1, STAGE_Y+STAGE_HEIGHT*8, STAGE_WIDTH*8+2, 1, COLOR_WHITE);
        video_draw_box(STAGE_X-1, STAGE_Y-1, 1, STAGE_HEIGHT*8+2, COLOR_WHITE);
        video_draw_box(STAGE_X+STAGE_WIDTH*8, STAGE_Y-1, 1, STAGE_HEIGHT*8+2, COLOR_WHITE);


        /* Health box and label */
        font_draw_string(&tinyfont, "Scarab", 0, 16, 0);
        video_draw_box(14, 7, game_state.max_health *4 + 4, 1, COLOR_MAGENTA);
        video_draw_box(14, 8, 1, 4, COLOR_MAGENTA);
        video_draw_box(14, 12, game_state.max_health * 4 + 4, 1, COLOR_MAGENTA);
        video_draw_box(17 + game_state.max_health * 4, 8, 1, 4, COLOR_MAGENTA);

        /* Initial bar */
        video_draw_box(16, 9, game_state.health * 4, 2, COLOR_MAGENTA);

        /* Weapon and Ammo */
        bitmap_replace(&ak77, RIGHT_HUD_OFFSET + 4, 100);
        sprintf(ammocount, "%5d", game_state.ammo);
        font_draw_string(&tinyfont, ammocount, 0, RIGHT_HUD_OFFSET + 28, 116);
        font_draw_string(&tinyfont, "ak77", 0, RIGHT_HUD_OFFSET + 16, 108);

        /* Help text */
        font_draw_string(&tinyfont,
                "Arrows\xB3Move\r\n"
                "Space \xB3Shoot\r\n"
                "F2    \xB3Save\r\n"
                "Esc   \xB3Quit\r\n",
                0, RIGHT_HUD_OFFSET + 4,
                BOTTOM_HUD_OFFSET - 24);
    }
    else
    {
        if (game_state.health < last_health)
        {
            video_clear_region(16 + 4*game_state.health, 8,
                4*(last_health - game_state.health), 4);
        }
        else if (game_state.health > last_health)
        {
            video_draw_box(16 + 4*last_health, 9,
                4*(game_state.health - last_health), 2, COLOR_MAGENTA);
        }

        if (game_state.ammo != last_ammo)
        {
            video_clear_region(RIGHT_HUD_OFFSET + 28, 116, 20, 6);
            sprintf(ammocount, "%5d", game_state.ammo);
            font_draw_string(&tinyfont, ammocount, 0, RIGHT_HUD_OFFSET + 28, 116);
        }
    }

    last_health = game_state.health;
    last_ammo = game_state.ammo;
}


void stage_do_actions(struct stage * stage)
{
    struct actor * actor;
    struct actor * other;
    int destx, desty, otherx, othery;
    
    for (actor = stage->first_actor; actor != NULL; actor = actor->next)
    {
        actor->dx = 0;
        actor->dx = 0;

        if (actor->one_shot.curr < actor->one_shot.num)
        {
            actor_do_actions(actor, &actor->one_shot, NULL, false, true);
        }
        else
        {
            actor_do_alert_tests(actor);
            switch (actor->alert)
            {
                case ALERT_IDLE:
                case ALERT_NA:
                    actor_do_actions(actor, &actor->actions, NULL,
                            false, false);
                    break;
                case ALERT_ALERTED:
                    actor_do_actions(actor, &actor->alerted, NULL,
                            false, false);
                    break;
                case ALERT_SEARCHING:
                    actor_do_actions(actor, &actor->searching, NULL,
                            false, false);
                    break;
            }
        }

        /* Test for collisions with walls and stage boundary, then
         * activate corresponding logic*/
        if ( (actor->dx != 0 || actor->dy != 0) && !actor->dead)
        {
            destx = actor->x + actor->dx;
            desty = actor->y + actor->dy;

            /* Players must first test for stage transitions as well. */
            if (actor->class == CLASS_PLAYER)
            {
                if (destx < MIN_X && stage->adjstage[WEST] > 0)
                {
                    game_state.next_x = MAX_X;
                    game_state.next_y = actor->y;
                    game_state.mode = MODE_NEWSTAGE;
                    game_state.next_stage = stage->adjstage[WEST];
                }
                else if (destx > MAX_X && stage->adjstage[EAST] > 0)
                {
                    game_state.next_x = MIN_X;
                    game_state.next_y = actor->y;
                    game_state.mode = MODE_NEWSTAGE;
                    game_state.next_stage = stage->adjstage[EAST];
                }
                else if (desty < MIN_Y && stage->adjstage[NORTH] > 0)
                {
                    game_state.next_y = MAX_Y;
                    game_state.next_x = actor->x;
                    game_state.mode = MODE_NEWSTAGE;
                    game_state.next_stage = stage->adjstage[NORTH];
                }
                else if (desty > MAX_Y && stage->adjstage[SOUTH] > 0)
                {
                    game_state.next_y = MIN_Y;
                    game_state.next_x = actor->x;
                    game_state.mode = MODE_NEWSTAGE;
                    game_state.next_stage = stage->adjstage[SOUTH];
                }
            }

            if ( destx < MIN_X || destx > MAX_X
                || desty < MIN_Y || desty > MAX_Y
                || stage_is_solid(stage, destx, desty,
                        actor->width, actor->height) )
            {
                actor_do_actions(actor, &actor->coll_wall, NULL, true, false);
            }
        }
    }

    /* Test other actors for collisions after initial movement
    determination. */
    for (actor = stage->first_actor; actor != NULL; actor = actor->next)
    {
        for (other = stage->first_actor; other != NULL;
                other = other->next)
        {
            /* Stop when the actor is no longer moving, or dead */
            if (actor->dx == 0 && actor->dy == 0 || actor->dead)
            {
                break;
            }
            else if (actor_could_collide(actor, other))
            {
                destx = actor->x + actor->dx;
                desty = actor->y + actor->dy;
                otherx = other->x + other->dx;
                othery = other->y + other->dy;

                if (!other->dead
                    && destx + actor->width > otherx
                    && otherx + other->width > destx
                    && desty + actor->height > othery
                    && othery + other->height > desty)
                {
                    actor_do_actions(actor, &actor->coll_act, other, true, false);
                    actor_do_actions(other, &other->coll_act, actor, true, false);
                }
            }
        }
    }
}


void stage_draw_bgtile(struct stage * stage, int x, int y)
{
    /* TODO: Add safety checks for printing invalid tiles */
    if (stage->tiles[x][y] < FG_TILE_OFFSET)
        tiles_replace(&tilesbg,
                stage->tiles[x][y],
                STAGE_X + x * 8, STAGE_Y + y * 8);
    else
        tiles_replace(&tilesfg,
                stage->tiles[x][y]-FG_TILE_OFFSET,
                STAGE_X + x * 8, STAGE_Y + y * 8);
}


void stage_redraw_bg(struct stage * stage, bool drawall)
{
    struct actor * actor;
    int x, y;
    int x1, x2, y1, y2;
    if (drawall)
    {
        /* Draw every tile in the stage */
        for (x = 0; x < STAGE_WIDTH; ++x)
        {
            for (y = 0; y < STAGE_HEIGHT; ++y)
            {
                stage_draw_bgtile(stage, x, y);
            }
        }
    }
    else
    {
        for (actor = stage->first_actor; actor != NULL;
                actor = actor->next)
        {
            if (actor->dirty || actor->dead)
            {
                /* Reminder: coordinates are for the actor's feet. */
                    /* Compute bounding box to re-draw */
                    x1 = actor->x/2;
                    x2 = (actor->x+actor->width-1)/2;
                    y1 = actor->y/2-1;
                    y2 = (actor->y+actor->height-1)/2;

                    for (x = x1; x <= x2; x++)
                        for (y = y1; y <= y2; y++)
                            stage_draw_bgtile(stage, x, y);
            }
        }
    }
}


void stage_draw_actors(struct stage * stage, bool drawinvs)
{
    struct actor * actor;
    struct actor * nextactor; /* to allow safe removal while still looping */
    for (actor = stage->first_actor; actor != NULL; actor = nextactor)
    {
        nextactor = actor->next;
        if (actor->dead)
        {
            stage_remove_actor(stage, actor);
        }
        else if (drawinvs || !actor->invisible)
        {
            /* Currently just redraw all actors. Too many
            problems with previous overlaps for otherwise-stationary
            actors */
            
            /* Actor needs to be redrawn.
            Apply movement now and clear dirty bit */
            actor->x += actor->dx;
            actor->y += actor->dy;
            actor->dx = 0;
            actor->dy = 0;
            actor->dirty = false;

            actor_draw(actor);
        }
    }
}


void stage_find_player(struct stage * stage)
{
    struct actor * actor;
    curr_player = NULL;

    for (actor = stage->first_actor; actor != NULL; actor = actor->next)
    {
        if (actor->class == CLASS_PLAYER)
        {
            curr_player = actor;
            break;
        }
    }
}

void stage_run(struct stage * stage)
{
    int16_t shot_delay = 0;
    uint32_t frame_period_ms = 1000/settings.speed;
    game_state.mode = MODE_PLAY;

    curr_stage = stage;
    stage_find_player(stage);
    
    if (curr_player != NULL && game_state.next_x >= 0 && game_state.next_y >= 0)
    {
        /* Stage transition has destination coordinates.
         * Move player accordingly */
        curr_player->x = game_state.next_x;
        curr_player->y = game_state.next_y;
        game_state.next_x = -1;
        game_state.next_y = -1;
    }

    stage_draw_hud(true);
    stage_redraw_bg(stage, true);
    stage_draw_actors(stage, false);
    key_replace_isr();

    while (game_state.mode == MODE_PLAY)
    {
        timer_start();
        
        if (key_was_pushed(KEY_SCAN_ESC))
        {
            game_state.mode = MODE_QUIT;
        }
        else if (key_was_pushed(KEY_F2))
        {
            key_restore_isr();
            msaveload_refresh();
            menu_loop(&savemenu);
            stage_draw_hud(true);
            stage_redraw_bg(stage, true);
            key_replace_isr();
        }
        else if (curr_player != NULL)
        {
            if (key_pressed[KEY_SCAN_SPACE] && shot_delay == 0)
            {
                 player_queue_acts[0] = SHOOT_FACING;
                 shot_delay = 2;
            }
            else if (key_pressed[KEY_UP])
                 player_queue_acts[0] = MOVE_NORTH;
            else if (key_pressed[KEY_DOWN])
                 player_queue_acts[0] = MOVE_SOUTH;
            else if (key_pressed[KEY_LEFT])
                 player_queue_acts[0] = MOVE_WEST;
            else if (key_pressed[KEY_RIGHT])
                 player_queue_acts[0] = MOVE_EAST;
            else
                 player_queue_acts[0] = NO_ACTION;
        }

        stage_do_actions(stage);
        stage_redraw_bg(stage, false);
        stage_draw_actors(stage, false);
        stage_draw_hud(false);
        video_flip_buffer();
        sound_play_queue();

        timer_wait_until(frame_period_ms);
        if (shot_delay > 0) shot_delay--;
        if ( game_state.health <= 0 )
        {
            game_state.mode = MODE_GAMEOVER;
        }
    }
    curr_stage = NULL;
    key_restore_isr();
}


void stage_free(struct stage * stage)
{
    struct actor * actor;
    struct actor * nextactor;

    for (actor = stage->first_actor; actor != NULL; actor = nextactor)
    {
        nextactor = actor->next;
        free(actor);
    }
    free(stage);
}


void stage_load_tiles()
{
    /* Tiles are all 8x8 pixels. FG is 8x8 tiles, BG is 4x4*/
    tiles_load(&tilesfg, "data\\tilesfg", 8, 8, 8, 8);
    tiles_load(&tilesbg, "data\\tilesbg", 8, 8, 4, 4);
    bitmap_load(&ak77, "data\\ak77", 48, 24);
}


void stage_unload_tiles()
{
    tiles_unload(&tilesfg);
    tiles_unload(&tilesbg);
    bitmap_unload(&ak77);
}

