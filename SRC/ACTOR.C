#include "actor.h"
#include "stage.h"
#include "game.h"
#include "sound.h"
#include <stddef.h>
#include <stdlib.h>

struct tiles sprites_m;
struct tiles sprites_l;
struct tiles mask_m;
struct tiles mask_l;

uint8_t player_queue_acts[] = { NO_ACTION };
struct actor * curr_player = NULL;

static uint8_t coll_wall_queue[] = { STOP_SELF };
static uint8_t coll_act_queue[] = { STOP_OTHER };
static uint8_t coll_wall_bullet[] = { KILL_SELF };
static uint8_t coll_act_bullet[] = { KILL_SELF, HURT_ANY };
static uint8_t empty_queue[] = { NO_ACTION };
static uint8_t mook_queue_acts[] = { MOVE_EAST, MOVE_EAST, MOVE_EAST, MOVE_EAST, 
        MOVE_SOUTH, MOVE_SOUTH, MOVE_SOUTH, MOVE_SOUTH, 
        MOVE_WEST, MOVE_WEST, MOVE_WEST, MOVE_WEST, 
        MOVE_NORTH, MOVE_NORTH, MOVE_NORTH, MOVE_NORTH  };
static uint8_t default_search[] = { MOVE_RANDOM, MOVE_FACING, MOVE_FACING };
static uint8_t default_alert[] = { MOVE_PLAYER,
        MOVE_PLAYER, NO_ACTION, SHOOT_PLAYER, MOVE_PLAYER };
static uint8_t alert_prompt[] = { NO_ACTION,
        NO_ACTION, KILL_SELF};
static uint8_t ammo_queue[] = { ADD_AMMO, 20, KILL_SELF };
static uint8_t ration_queue[] = { ADD_HEALTH, 5, KILL_SELF };
static uint8_t cloak_queue[] = {
    MOVE_WEST, MOVE_NORTH, MOVE_WEST, MOVE_NORTH, KILL_SELF };


struct actor standard_actors[NUM_DEFAULT_ACTORS] =
{
    { /* SCARAB_STANDARD */
        CLASS_PLAYER, /* class */
        { /* Sprites */
            {30, 31, 32, 33}, /* North */
            {18, 19, 20, 21}, /* East */
            {26, 27, 28, 29}, /* South */
            {22, 23, 24, 25}  /* West */
        },
        /* Actions */
        {player_queue_acts, false, 1, 0},
        {coll_wall_queue, false, 1, 0},
        {coll_act_queue, false, 1, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, invs and alert stuff */
        /* Health does not apply to player actor; it uses
        the game state instead */
        SOUTH, STANDING, true, 0, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* SCARAB_CLOAK */
        CLASS_PLAYER, /* class */
        { /* Sprites */
            {12, 13, 14, 15}, /* North */
            {0, 1, 2, 3}, /* East */
            {8, 9, 10, 11}, /* South */
            {4, 5, 6, 7}  /* West */
        },
        /* Actions */
        {player_queue_acts, false, 1, 0},
        {coll_wall_queue, false, 1, 0},
        {coll_act_queue, false, 1, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, invs and alert stuff */
        /* Health does not apply to player actor; it uses
        the game state instead */
        SOUTH, STANDING, true, 0, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* SCARAB_BLUE */
        CLASS_PLAYER, /* class */
        { /* Sprites */
            {48, 49, 50, 51}, /* North */
            {36, 37, 38, 39}, /* East */
            {44, 45, 46, 47}, /* South */
            {40, 41, 42, 43}  /* West */
        },
        /* Actions */
        {player_queue_acts, false, 1, 0},
        {coll_wall_queue, false, 1, 0},
        {coll_act_queue, false, 1, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, invs and alert stuff */
        /* Health does not apply to player actor; it uses
        the game state instead */
        SOUTH, STANDING, true, 0, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* GUARD_STANDARD */
        CLASS_ENEMY, /* class */
        { /* Sprites */
            {84, 85, 86, 87}, /* North */
            {72, 73, 74, 75}, /* East */
            {80, 81, 82, 83}, /* South */
            {76, 77, 78, 79}  /* West */
        },
        /* Actions */
        {mook_queue_acts, false, 16, 0},
        {coll_wall_queue, false, 1, 0},
        {coll_act_queue, false, 1, 0},
        {NULL, false, 0, 0},
        {default_alert, false, 5, 0},
        {default_search, false, 3, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, invs and alert stuff */
        SOUTH, STANDING, true, 2, false, false, ALERT_IDLE, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* REBEL_STANDARD */
        CLASS_ALLY, /* class */
        { /* Sprites */
            {66, 67, 68, 69}, /* North */
            {54, 55, 56, 57}, /* East */
            {62, 63, 64, 65}, /* South */
            {58, 59, 60, 61}  /* West */
        },
        /* Actions */
        {mook_queue_acts, false, 16, 0},
        {coll_wall_queue, false, 1, 0},
        {coll_act_queue, false, 1, 0},
        {NULL, false, 0, 0},
        {default_alert, false, 5, 0},
        {default_search, false, 3, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, invs and alert stuff */
        SOUTH, STANDING, true, 2, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* BULLET */
        CLASS_PROJECTILE, /* class */
        { /* Sprites */
            {53, 53, 53, 53}, /* North */
            {53, 53, 53, 53}, /* East */
            {53, 53, 53, 53}, /* South */
            {53, 53, 53, 53}  /* West */
        },
        /* Actions */
        {empty_queue, false, 1, 0},
        {coll_wall_bullet, false, 1, 0},
        {coll_act_bullet, false, 2, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, inv and alert stuff */
        SOUTH, STANDING, true, 1, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /*ALERTED_PROMPT,*/
        CLASS_OTHER, /* class */
        { /* Sprites */
            {70, 70, 70, 70}, /* North */
            {70, 70, 70, 70}, /* East */
            {70, 70, 70, 70}, /* South */
            {70, 70, 70, 70}  /* West */
        },
        /* Actions */
        {alert_prompt, false, 3, 0},
        {empty_queue, false, 1, 0},
        {empty_queue, false, 1, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, inv and alert stuff */
        SOUTH, STANDING, true, 99, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /*SEARCHING_PROMPT,*/
        CLASS_OTHER, /* class */
        { /* Sprites */
            {71, 71, 71, 71}, /* North */
            {71, 71, 71, 71}, /* East */
            {71, 71, 71, 71}, /* South */
            {71, 71, 71, 71}  /* West */
        },
        /* Actions */
        {alert_prompt, false, 3, 0},
        {empty_queue, false, 1, 0},
        {empty_queue, false, 1, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, inv and alert stuff */
        SOUTH, STANDING, true, 99, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /*IDLE_PROMPT,*/
        CLASS_OTHER, /* class */
        { /* Sprites */
            {88, 88, 88, 88}, /* North */
            {88, 88, 88, 88}, /* East */
            {88, 88, 88, 88}, /* South */
            {88, 88, 88, 88}  /* West */
        },
        /* Actions */
        {alert_prompt, false, 3, 0},
        {empty_queue, false, 1, 0},
        {empty_queue, false, 1, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, inv and alert stuff */
        SOUTH, STANDING, true, 99, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* MECH */
        CLASS_ENEMY, /* class */
        { /* Sprites */
            {4, 5, 6, 7}, /* North */
            {12, 13, 14, 15}, /* East */
            {0, 1, 2, 3}, /* South */
            {8, 9, 10, 11}  /* West */
        },
        /* Actions */
        {mook_queue_acts, false, 16, 0},
        {coll_wall_queue, false, 1, 0},
        {coll_act_queue, false, 1, 0},
        {NULL, false, 0, 0},
        {default_alert, false, 5, 0},
        {default_search, false, 3, 0},

        /* Size and position */
        SIZE_LARGE, 10, 10, 0, 0, 4, 4,

        /* Facing, sprite, dirty, health, dead, invs and alert stuff */
        SOUTH, STANDING, true, 20, false, false, ALERT_IDLE, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* AMMO_BOX */
        CLASS_PICKUP, /* class */
        { /* Sprites */
            {16, 16, 16, 16}, /* North */
            {16, 16, 16, 16}, /* East */
            {16, 16, 16, 16}, /* South */
            {16, 16, 16, 16}  /* West */
        },
        /* Actions */
        {empty_queue, false, 1, 0},
        {empty_queue, false, 1, 0},
        {ammo_queue, false, 3, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, inv and alert stuff */
        SOUTH, STANDING, true, 99, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* RATION */
        CLASS_PICKUP, /* class */
        { /* Sprites */
            {17, 17, 17, 17}, /* North */
            {17, 17, 17, 17}, /* East */
            {17, 17, 17, 17}, /* South */
            {17, 17, 17, 17}  /* West */
        },
        /* Actions */
        {empty_queue, false, 1, 0},
        {empty_queue, false, 1, 0},
        {ration_queue, false, 3, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, inv and alert stuff */
        SOUTH, STANDING, true, 99, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* CLOAK */
        CLASS_OTHER, /* class */
        { /* Sprites */
            {35, 35, 35, 35}, /* North */
            {35, 35, 35, 35}, /* East */
            {35, 35, 35, 35}, /* South */
            {35, 35, 35, 35}  /* West */
        },
        /* Actions */
        {cloak_queue, false, 5, 0},
        {empty_queue, false, 1, 0},
        {empty_queue, false, 1, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, inv and alert stuff */
        SOUTH, STANDING, true, 99, false, false, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    },
    { /* EVENT_TRIGGER */
        CLASS_OTHER, /* class */
        { /* Sprites */
            {89, 89, 89, 89}, /* North */
            {89, 89, 89, 89}, /* East */
            {89, 89, 89, 89}, /* South */
            {89, 89, 89, 89}  /* West */
        },
        /* Actions */
        {empty_queue, false, 1, 0},
        {empty_queue, false, 1, 0},
        {empty_queue, false, 1, 0},
        /* Alert and one-shot actions don't apply */
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},
        {NULL, false, 0, 0},

        /* Size and position */
        SIZE_MEDIUM, 10, 10, 0, 0, 2, 2,

        /* Facing, sprite, dirty, health, dead, inv and alert stuff */
        SOUTH, STANDING, true, 99, false, true, ALERT_NA, 0,

        /* Linked list pointers */
        NULL, NULL
    }
};


void actor_new_actions(struct action_queue * actions, uint16_t numact)
{
    if (actions->custom) free(actions->queue);
    actions->queue = malloc(numact);
    actions->num = numact;
    actions->curr = 0;
    actions->custom = true;
}


/* Determine which direction the specified actor should
face towards the player */
enum directions get_player_dir(struct actor * actor)
{
    int16_t deltax, deltay;
    enum directions result = SOUTH;
    
    if (curr_player != NULL)
    {
        deltax = curr_player->x - actor->x;
        deltay = curr_player->y - actor->y;
        if (abs(deltax) > abs(deltay))
        {
            if (deltax > 0) result = EAST;
            else result = WEST;
        }
        else
        {
            if (deltay > 0) result = SOUTH;
            else result = NORTH;
        }
    }
    
    return result;
}


inline enum directions get_random_dir()
{
    return rand() % 4;
}


static void actor_do_shoot(struct actor * actor, enum directions dir)
{
    /* NOTE: Actor will get a chance to act the same frame it
    is created, so it is safe to spawn directly on top of the
    originator. For 2x2 actors anyways. 4x4 will need slight offsets. */
    struct actor * newactor = NULL;
    if (actor->class != CLASS_PLAYER || game_state.ammo > 0)
    {
        sound_queue(SND_SHOT);
        newactor = stage_add_actor_type(curr_stage, BULLET,
            actor->x, actor->y, false);
    }

    if (actor->class == CLASS_PLAYER && game_state.ammo > 0)
    {
        game_state.ammo--;
    }
    
    if (newactor != NULL)
    {
        actor_new_actions(&newactor->actions, 1);
        switch (dir)
        {
            case NORTH:
                newactor->actions.queue[0] = MOVE_NORTH_FAST;
                if (actor->size == SIZE_LARGE) newactor->x += 1;
                break;
            case SOUTH:
                newactor->actions.queue[0] = MOVE_SOUTH_FAST;
                if (actor->size == SIZE_LARGE)
                {
                    newactor->x += 1;
                    newactor->y += 2;
                }
                break;
            case EAST:
                newactor->actions.queue[0] = MOVE_EAST_FAST;
                if (actor->size == SIZE_LARGE)
                {
                    newactor->x += 2;
                }
                break;
            case WEST:
                newactor->actions.queue[0] = MOVE_WEST_FAST;
                break;
        }
    }

    actor->facing = dir;
    actor->sprite = SHOOTING;
    actor->dirty = true;
}


static void actor_do_spawn(struct actor * actor, enum directions dir,
        enum actor_types acttype)
{
    struct actor * newactor = stage_add_actor_type(curr_stage, acttype,
            actor->x, actor->y, false);

    if (newactor != NULL)
    {
        switch (dir)
        {
            case NORTH:
                newactor->y -= 2;
                if (actor->size == SIZE_LARGE) newactor->x += 1;
                break;
            case SOUTH:
                newactor->y += 2;
                if (actor->size == SIZE_LARGE)
                {
                    newactor->x += 1;
                    newactor->y += 2;
                }
                break;
            case EAST:
                newactor->x += 2;
                if (actor->size == SIZE_LARGE)
                {
                    newactor->x += 2;
                }
                break;
            case WEST:
                newactor->x -= 2;
                break;
        }
    }
}


static void actor_do_move(struct actor * actor, enum directions dir, int speed)
{
    switch (dir)
    {
        case NORTH:
            actor->dy = -speed;
            break;
        case SOUTH:
            actor->dy = speed;
            break;
        case EAST:
            actor->dx = speed;
            break;
        case WEST:
            actor->dx = -speed;
            break;
    }
    actor->facing = dir;

    if (actor->sprite == WALKING1)
        actor->sprite = WALKING2;
    else
        actor->sprite = WALKING1;
    actor->dirty = true;
}


static void actor_do_stand(struct actor * actor, enum directions dir)
{
    if (actor->sprite != STANDING || actor->facing != dir)
    {
        actor->facing = dir;
        actor->sprite = STANDING;
        actor->dirty = true;
    }
}


static void actor_change_sprites(struct actor * actor,
        enum actor_types acttype)
{
    enum directions dir;
    enum sprites spr;

    for (dir = NORTH; dir < NUM_DIRECTIONS; dir++)
    { 
        for (spr = STANDING; spr < NUM_SPRITES; spr++)
        {
            actor->sprites[dir][spr] =
                    standard_actors[acttype].sprites[dir][spr];
        }
    }
}


bool actor_could_collide(struct actor * actor1, struct actor * actor2)
{
    bool could_collide = false;

    if (actor1 != actor2)
    {
        switch (actor1->class)
        {
            case CLASS_ENEMY:
                could_collide = (actor2->class != CLASS_PICKUP);
                break;
            case CLASS_PLAYER:
                could_collide = true;
                break;
            case CLASS_ALLY:
                could_collide = (actor2->class != CLASS_PICKUP);
                break;
            case CLASS_PICKUP:
                could_collide = (actor2->class == CLASS_PLAYER);
                break;
            default:
                /* Catch-all, CLASS_PROJETILE and CLASS_OTHER */
                could_collide = (actor2->class == CLASS_PLAYER)
                    || (actor2->class == CLASS_ALLY)
                    || (actor2->class == CLASS_ENEMY);
                break;
        }
    }

    return could_collide;
}


void actor_do_actions(struct actor * actor1, struct action_queue * queue1,
        struct actor * actor2, bool do_all, bool no_loop)
{
    unsigned int numloops = 1;
    unsigned int i;

    if (do_all) numloops = queue1->num;
        
    for (i = 0; i < numloops; i++)
    {
        /* Protect against running the queue past its end */
        if (no_loop && queue1->curr >= queue1->num) return;

        switch(queue1->queue[queue1->curr])
        {
            case NO_ACTION:
                actor_do_stand(actor1, actor1->facing);
                break;
            case MOVE_NORTH:
                actor_do_move(actor1, NORTH, 1);
                break;
            case MOVE_SOUTH:
                actor_do_move(actor1, SOUTH, 1);
                break;
            case MOVE_EAST:
                actor_do_move(actor1, EAST, 1);
                break;
            case MOVE_WEST:
                actor_do_move(actor1, WEST, 1);
                break;
            case MOVE_NORTH_FAST:
                actor_do_move(actor1, NORTH, 2);
                break;
            case MOVE_SOUTH_FAST:
                actor_do_move(actor1, SOUTH, 2);
                break;
            case MOVE_EAST_FAST:
                actor_do_move(actor1, EAST, 2);
                break;
            case MOVE_WEST_FAST:
                actor_do_move(actor1, WEST, 2);
                break;
            case MOVE_PLAYER:
                actor_do_move(actor1, get_player_dir(actor1), 1);
                break;
            case MOVE_PLAYER_FAST:
                actor_do_move(actor1, get_player_dir(actor1), 2);
                break;
            case MOVE_RANDOM:
                actor_do_move(actor1, get_random_dir(), 1);
                break;
            case MOVE_RANDOM_FAST:
                actor_do_move(actor1, get_random_dir(), 2);
                break;
            case MOVE_FACING:
                actor_do_move(actor1, actor1->facing, 1);
                break;
            case MOVE_FACING_FAST:
                actor_do_move(actor1, actor1->facing, 2);
                break;
            case LOOK_NORTH:
                actor_do_stand(actor1, NORTH);
                break;
            case LOOK_EAST:
                actor_do_stand(actor1, EAST);
                break;
            case LOOK_SOUTH:
                actor_do_stand(actor1, SOUTH);
                break;
            case LOOK_WEST:
                actor_do_stand(actor1, WEST);
                break;
            case SHOOT_NORTH:
                actor_do_shoot(actor1, NORTH);
                break;
            case SHOOT_SOUTH:
                actor_do_shoot(actor1, SOUTH);
                break;
            case SHOOT_EAST:
                actor_do_shoot(actor1, EAST);
                break;
            case SHOOT_WEST:
                actor_do_shoot(actor1, WEST);
                break;
            case SHOOT_FACING:
                actor_do_shoot(actor1, actor1->facing);
                break;
            case SHOOT_PLAYER:
                actor_do_shoot(actor1, get_player_dir(actor1));
                break;
            case SHOOT_RANDOM:
                actor_do_shoot(actor1, get_random_dir());
                break;
            case STOP_OTHER:
                if (actor2 != NULL)
                {
                    actor2->dx = 0;
                    actor2->dy = 0;
                    actor_do_stand(actor2, actor2->facing);
                }
                break;
            case STOP_SELF:
                actor1->dx = 0;
                actor1->dy = 0;
                actor_do_stand(actor1, actor1->facing);
                break;
            case KILL_SELF:
                actor1->dead = true;
                break;
            case KILL_OTHER:
                if (actor2 != NULL) actor2->dead = true;
                break;
            case HURT_ANY:
                if (actor2 != NULL)
                {
                    if (actor2->class == CLASS_PLAYER)
                    {
                        --game_state.health;
                        sound_queue(SND_PLAYER_HIT);
                    }
                    else
                    {
                        sound_queue(SND_HIT);
                        --actor2->health;
                        if (actor2->health <= 0)
                        {
                            actor2->dead = true;
                        }
                    }
                }
                break;
            case HURT_PLAYER:
                if (actor2 != NULL && actor2->class == CLASS_PLAYER)
                {
                    sound_queue(SND_PLAYER_HIT);
                    --game_state.health;
                }
                break;
            case END_CHAPTER:
                game_state.mode = MODE_NEXT_CHAPTER;
                break;
                /* Start of one param */
            case CHANGE_SPRITES:
                actor_change_sprites(actor1, queue1->queue[queue1->curr+1]);
                ++queue1->curr;
                i++;
                break;
            case SHOW_SPRITE:
                actor1->sprite = NUM_SPRITES + queue1->queue[queue1->curr+1];
                ++queue1->curr;
                i++;
                actor1->dirty = true;
                break;
            case SPAWN_NORTH:
                actor_do_spawn(actor1, NORTH, queue1->queue[queue1->curr+1]);
                ++queue1->curr;
                i++;
                break;
            case SPAWN_EAST:
                actor_do_spawn(actor1, EAST, queue1->queue[queue1->curr+1]);
                ++queue1->curr;
                i++;
                break;
            case SPAWN_SOUTH:
                actor_do_spawn(actor1, SOUTH, queue1->queue[queue1->curr+1]);
                ++queue1->curr;
                i++;
                break;
            case SPAWN_WEST:
                actor_do_spawn(actor1, WEST, queue1->queue[queue1->curr+1]);
                ++queue1->curr;
                i++;
                break;
            case ADD_AMMO:
                game_state.ammo += queue1->queue[queue1->curr+1];
                sound_queue(SND_PICKUP);
                ++queue1->curr;
                i++;
                /* TODO: Limit? */
                break;
            case ADD_HEALTH:
                game_state.health += queue1->queue[queue1->curr+1];
                sound_queue(SND_PICKUP);
                ++queue1->curr;
                i++;
                if (game_state.health > game_state.max_health)
                    game_state.health = game_state.max_health;
                break;
                /* Start of two param */
            case TELEPORT_OTHER:
                actor2->dx = 2*queue1->queue[queue1->curr+1] - actor2->x;
                actor2->dy = 2*queue1->queue[queue1->curr+2] - actor2->y;
                actor2->dirty = true;
                queue1->curr += 2;
                i += 2;
                break;
            case TELEPORT_SELF:
                actor1->dx = 2*queue1->queue[queue1->curr+1] - actor1->x;
                actor1->dy = 2*queue1->queue[queue1->curr+2] - actor1->y;
                actor1->dirty = true;
                queue1->curr += 2;
                i += 2;
                break;
        }

        ++queue1->curr;
        if (!no_loop) queue1->curr = queue1->curr % queue1->num;
    }
}


static bool test_line_for_walls_by_x(int16_t startx, int16_t starty,
        int16_t endx, float slope)
{
    int16_t dir = 1, i, delta;
    bool result = false;

    delta = endx - startx;
    if (delta < 0)
    {
        delta = -delta;
        dir = -1;
    }

    for (i = 0; i < delta; ++i)
    {
        if (stage_is_solid(curr_stage, startx + dir * i,
                starty + dir * i * slope, 2, 2))
        {
            result = true;
            break;
        }
    }

    return result;
}


static bool test_line_for_walls_by_y(int16_t startx, int16_t starty,
        int16_t endy, float slope)
{
    int16_t dir = 1, i, delta;
    bool result = false;

    delta = endy - starty;
    if (delta < 0)
    {
        delta = -delta;
        dir = -1;
    }

    for (i = 0; i < delta; ++i)
    {
        if (stage_is_solid(curr_stage, startx + dir * i * slope,
                starty + dir * i, 2, 2))
        {
            result = true;
            break;
        }
    }

    return result;
}


static bool actor_can_see_player(struct actor * actor)
{
    int16_t deltax, deltay;
    float ratiox, ratioy;
    bool result = false;

    /* Below algorithm intentionally avoids using potentially
    slow sin/cos equations in favour of simplistic slope and
    direction detection algorithm. We only need to support four
    directions anyways. */
    deltax = curr_player->x - actor->x;
    deltay = curr_player->y - actor->y;
    ratiox = (float)deltay/deltax;
    ratioy = (float)deltax/deltay;

    switch (actor->facing)
    {
        case NORTH:
            result = (deltay < 0 && ratioy < 1.0f && ratioy > -1.0f);
            break;
        case SOUTH:
            result = (deltay > 0 && ratioy < 1.0f && ratioy > -1.0f);
            break;
        case EAST:
            result = (deltax > 0 && ratiox < 1.0f && ratiox > -1.0f);
            break;
        case WEST:
            result = (deltax < 0 && ratiox < 1.0f && ratiox > -1.0f);
            break;
    }

    /* TODO: Follow ratio line to look for obsticles (using curr stage)*/
    if (result)
    {
        if (abs(deltax) > abs(deltay))
        {
            /* Walk primarily by X */
            result = !test_line_for_walls_by_x(actor->x, actor->y,
                    curr_player->x, ratiox);
        }
        else
        {
            /* Walk primarily by Y */
            result = !test_line_for_walls_by_y(actor->x, actor->y,
                    curr_player->y, ratioy);
        }
    }

    return result;
}



void actor_do_alert_tests(struct actor * actor)
{
    bool visible = false;
    /* Skip tests if actor does not care about alert status */
    if (actor->alert == ALERT_NA) return;

    /* Every other test is interested if the player is visible */
    visible = actor_can_see_player(actor);

    switch (actor->alert)
    {
        case ALERT_IDLE:
            if (visible)
            {
                actor->alert = ALERT_ALERTED;
                sound_queue(SND_ALERT);
                actor->alert_timer = 20;
                stage_add_actor_type(curr_stage, ALERTED_PROMPT,
                        actor->x, actor->y-actor->height*2, true);
            }
            break;
        case ALERT_ALERTED:
            if (visible)
            {
                actor->alert_timer = 20;
            }
            else if (actor->alert_timer <= 0)
            {
                actor->alert = ALERT_SEARCHING;
                actor->alert_timer = 20;
                stage_add_actor_type(curr_stage, SEARCHING_PROMPT,
                        actor->x, actor->y-actor->height*2, true);
            }
            else
            {
                --actor->alert_timer;
            }
            break;
        case ALERT_SEARCHING:
            if (visible)
            {
                sound_queue(SND_ALERT);
                actor->alert = ALERT_ALERTED;
                actor->alert_timer = 20;
                stage_add_actor_type(curr_stage, ALERTED_PROMPT,
                        actor->x, actor->y-actor->height*2, true);
            }
            else if (actor->alert_timer <= 0)
            {
                actor->alert = ALERT_IDLE;
                stage_add_actor_type(curr_stage, IDLE_PROMPT,
                        actor->x, actor->y-actor->height*2, true);
            }
            else
            {
                --actor->alert_timer;
            }
            break;
    }
}


void actor_draw(struct actor * actor)
{
    int tilenum;
    
            if (actor->sprite >= NUM_SPRITES)
            {
                /* Overridden special sprite */
                tilenum = actor->sprite - NUM_SPRITES;
            }
            else
            {
                /* Normal sprite with facing and lookup */
                tilenum = actor->sprites[actor->facing][actor->sprite];
            }

            /* Reminder: coordinates are for the top-left
            of the actor's feet. */
            switch (actor->size)
            {
                case SIZE_MEDIUM:
                    tiles_mask(&mask_m, tilenum,
                        STAGE_X + actor->x * 4, STAGE_Y + actor->y * 4 - 8);
                    tiles_draw(&sprites_m, tilenum,
                        STAGE_X + actor->x * 4, STAGE_Y + actor->y * 4 - 8);
                    break;
                case SIZE_LARGE:
                    tiles_mask(&mask_l, tilenum,
                        STAGE_X + actor->x * 4, STAGE_Y + actor->y * 4 - 8);
                    tiles_draw(&sprites_l, tilenum,
                        STAGE_X + actor->x * 4, STAGE_Y + actor->y * 4 - 8);
                    break;
            }
}


void actor_free(struct actor * actor)
{
    if (actor->actions.custom) free(actor->actions.queue);
    if (actor->coll_wall.custom) free(actor->coll_wall.queue);
    if (actor->coll_act.custom) free(actor->coll_act.queue);
    free(actor);
}


void actor_load_sprites()
{
    /* 8 x 16 medium sprites, 18 columns and 5 rows */
    tiles_load(&sprites_m, "data\\sprite_m", 8, 16, 18, 5);
    tiles_load(&mask_m, "data\\mask_m", 8, 16, 18, 5);

    /* 16 x 24 large sprites, 16 sprites and 1 row */
    tiles_load(&sprites_l, "data\\sprite_l", 16, 24, 16, 1);
    tiles_load(&mask_l, "data\\mask_l", 16, 24, 16, 1);
}

void actor_unload_sprites()
{
    tiles_unload(&sprites_m);
    tiles_unload(&sprites_l);
    tiles_unload(&mask_m);
    tiles_unload(&mask_l);
}


void actor_load_actions(struct action_queue * actions, FILE * input)
{
    if (actions->num > 0)
    {
        actions->queue = malloc(actions->num);
        actions->custom = true;
        fread(actions->queue, sizeof(enum actions), actions->num, input);
    }
    else
    {
        /* Probably already saved this way, but may as well be safe */
        actions->queue = NULL;
        actions->custom = false;
    }
}


void actor_load(struct actor * actor, FILE * input)
{
    fread(actor, sizeof(*actor), 1, input);
    /* Don't trust pointers from the file. Actor pointers will
    be re-written by parent. */
    actor->next = NULL;
    actor->prev = NULL;

    actor_load_actions(&actor->actions, input);

    /* Players always point to the explicit player queue. But we still
    need to load the queue from file first, then unload it and replace
    it. */
    if (actor->class == CLASS_PLAYER)
    {
        free(actor->actions.queue);
        actor->actions.queue = player_queue_acts;
        actor->actions.custom = false;
        actor->actions.num = 1;
    }
    actor_load_actions(&actor->coll_wall, input);
    actor_load_actions(&actor->coll_act, input);
    actor_load_actions(&actor->one_shot, input);
    actor_load_actions(&actor->alerted, input);
    actor_load_actions(&actor->searching, input);            
}


void actor_save_actions(struct action_queue * actions, FILE * output)
{
    if (actions->num > 0)
    {
        fwrite(actions->queue, sizeof(enum actions), actions->num, output);
    }
}


void actor_save(struct actor * actor, FILE * output)
{
    fwrite(actor, sizeof(*actor), 1, output);

    actor_save_actions(&actor->actions, output);
    actor_save_actions(&actor->coll_wall, output);
    actor_save_actions(&actor->coll_act, output);
    actor_save_actions(&actor->one_shot, output);
    actor_save_actions(&actor->alerted, output);
    actor_save_actions(&actor->searching, output);            
}
