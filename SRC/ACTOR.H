#ifndef ACTOR_H
#define ACTOR_H

#include "tiles.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

enum directions
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
    NUM_DIRECTIONS
};

enum sprites
{
    STANDING = 0,
    WALKING1,
    WALKING2,
    SHOOTING,
    NUM_SPRITES
};

enum actor_size
{
    SIZE_SMALL,
    SIZE_MEDIUM,
    SIZE_LARGE
};

enum alert_states
{
    ALERT_NA = 0,
    ALERT_IDLE,
    ALERT_ALERTED,
    ALERT_SEARCHING
};

enum actor_class
{
    CLASS_ENEMY = 0,
    CLASS_PLAYER,
    CLASS_ALLY,
    CLASS_PROJECTILE,
    CLASS_PICKUP,
    CLASS_OTHER
};

enum actions
{
    NO_ACTION = 0,
    MOVE_NORTH,
    MOVE_EAST,
    MOVE_SOUTH,
    MOVE_WEST,
    MOVE_NORTH_FAST,
    MOVE_EAST_FAST,
    MOVE_SOUTH_FAST,
    MOVE_WEST_FAST,
    MOVE_PLAYER,
    MOVE_PLAYER_FAST,
    MOVE_RANDOM,
    MOVE_RANDOM_FAST,
    MOVE_FACING,
    MOVE_FACING_FAST,
    LOOK_NORTH,
    LOOK_EAST,
    LOOK_SOUTH,
    LOOK_WEST,
    SHOOT_NORTH,
    SHOOT_EAST,
    SHOOT_SOUTH,
    SHOOT_WEST,
    SHOOT_FACING,
    SHOOT_PLAYER,
    SHOOT_RANDOM,
    STOP_SELF,
    STOP_OTHER,
    KILL_SELF,
    KILL_OTHER,
    HURT_ANY,
    HURT_PLAYER,
    END_CHAPTER,
    /* 1 param actions */
    CHANGE_SPRITES, 
    SHOW_SPRITE,
    SPAWN_NORTH,
    SPAWN_EAST,
    SPAWN_SOUTH,
    SPAWN_WEST,
    ADD_AMMO,
    ADD_HEALTH,
    /* 2 param actions */
    TELEPORT_OTHER,
    TELEPORT_SELF,
    NUM_ACTIONS
};

#define START_1PARAM_ACTIONS CHANGE_SPRITES
#define START_2PARAM_ACTIONS TELEPORT_OTHER


struct action_queue
{
    uint8_t * queue;
    bool custom;
    uint16_t num;
    uint16_t curr;
};

struct actor
{
    uint8_t class;
    int16_t sprites[NUM_DIRECTIONS][NUM_SPRITES];
    struct action_queue actions;
    struct action_queue coll_wall;
    struct action_queue coll_act;
    struct action_queue one_shot;
    struct action_queue alerted;
    struct action_queue searching;
    uint8_t size;
    /* Stage position, in 4 pixel chunks */
    int16_t x;
    int16_t y;
    int16_t dx;
    int16_t dy;
    /* Size of bounding box, in number of 4 pixel blocks */
    int16_t width;
    int16_t height;
    uint8_t facing;
    uint8_t sprite;
    bool dirty;
    int16_t health;
    bool dead;
    bool invisible;
    uint8_t alert;
    int16_t alert_timer;
    struct actor * next;
    struct actor * prev;
};


enum actor_types
{
    SCARAB_STANDARD,
    SCARAB_CLOAK,
    SCARAB_BLUE,
    GUARD_STANDARD,
    REBEL_STANDARD,
    BULLET,
    ALERTED_PROMPT,
    SEARCHING_PROMPT,
    IDLE_PROMPT,
    MECH,
    AMMO_BOX,
    RATION,
    CLOAK,
    EVENT_TRIGGER,
    NUM_DEFAULT_ACTORS
};


extern struct tiles sprites_m;
extern struct tiles sprites_l;
extern struct tiles mask_m;
extern struct tiles mask_l;

extern uint8_t player_queue_acts[];
extern struct actor standard_actors[];
extern struct actor * curr_player;

bool actor_could_collide(struct actor * actor1, struct actor * actor2);
void actor_do_actions(struct actor * actor1, struct action_queue * queue1,
        struct actor * actor2, bool do_all, bool no_loop);
void actor_do_alert_tests(struct actor * actor);
void actor_draw(struct actor * actor);

void actor_free(struct actor * actor);
void actor_load_sprites();
void actor_unload_sprites();

void actor_load(struct actor * actor, FILE * input);
void actor_save(struct actor * actor, FILE * output);

#endif
