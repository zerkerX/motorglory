#include "medit.h"
#include "chapter.h"
#include "medqueue.h"
#include "editor.h"
#include <stddef.h>
#include <direct.h>
#include <string.h>

static struct parameter param_dir[] =
{
    {PARAM_INT, NULL, NULL, NULL, 0, 10, NULL},
    {PARAM_INT, NULL, NULL, NULL, 0, 10, NULL},
    {PARAM_INT, NULL, NULL, NULL, 0, 10, NULL},
    {PARAM_INT, NULL, NULL, NULL, 0, 10, NULL}    
};

static struct option edit_stage_opt[] =
{
    {"North Stage", NULL, NULL, &param_dir[NORTH]},
    {"South Stage", NULL, NULL, &param_dir[SOUTH]},
    {"East Stage", NULL, NULL, &param_dir[EAST]},
    {"West Stage", NULL, NULL, &param_dir[WEST]}
};

struct menu edit_stage_menu = {4, 0, false, EDITMENU, edit_stage_opt};

void medit_set_stage(struct stage * stage)
{
    enum directions dir;

    for (dir = NORTH; dir < NUM_DIRECTIONS; dir++)
    {
        param_dir[dir].value = &stage->adjstage[dir];
        param_dir[dir].maxval = curr_chapter.num_stages;
    }
}


void load_stage_act(void * stage_raw)
{
    char * stagename = (char *)stage_raw;
    editor_load(stagename);
}


static char mapnames[12][15] =
{
    "MAP0.MAP", 
    "MAP1.MAP", 
    "MAP2.MAP",
    "MAP3.MAP",
    "MAP4.MAP", 
    "MAP5.MAP", 
    "MAP6.MAP",
    "MAP7.MAP",
    "MAP8.MAP", 
    "MAP9.MAP", 
    "MAP10.MAP",
    "MAP11.MAP"
};

static struct option load_stage_opt[] =
{
    {"MAP0.MAP", &load_stage_act, mapnames[0], NULL},
    {"MAP1.MAP", &load_stage_act, mapnames[1], NULL},
    {"MAP2.MAP", &load_stage_act, mapnames[2], NULL},
    {"MAP3.MAP", &load_stage_act, mapnames[3], NULL},
    {"MAP4.MAP", &load_stage_act, mapnames[4], NULL},
    {"MAP5.MAP", &load_stage_act, mapnames[5], NULL},
    {"MAP6.MAP", &load_stage_act, mapnames[6], NULL},
    {"MAP7.MAP", &load_stage_act, mapnames[7], NULL},
    {"MAP8.MAP", &load_stage_act, mapnames[8], NULL},
    {"MAP9.MAP", &load_stage_act, mapnames[9], NULL},
    {"MAP10.MAP", &load_stage_act, mapnames[10], NULL},
    {"MAP11.MAP", &load_stage_act, mapnames[11], NULL},
};

struct menu load_stage_menu = {12, 0, true, EDITMENU, load_stage_opt};

void medit_set_load_opts()
{
    DIR * leveldir;
    struct dirent *mapfile;

    leveldir = opendir("levels");
    load_stage_menu.num_options = 0;
    
    if( leveldir != NULL )
    {
        for(mapfile = readdir(leveldir); mapfile != NULL; mapfile = readdir(leveldir))
        {
            if (strcmp("..", mapfile->d_name) != 0
                && strcmp(".", mapfile->d_name) != 0)
            {
                strcpy(mapnames[load_stage_menu.num_options], mapfile->d_name);
                strcpy(load_stage_opt[load_stage_menu.num_options].name,
                    mapfile->d_name);
                load_stage_menu.num_options++;
            }
        }
        closedir( leveldir );
    }
}


static void edit_actor_queue_act(void * queue_raw)
{
    struct action_queue * queue = (struct action_queue *)queue_raw;
    medqueue_run(queue);
}


static const char * actor_classes[] =
{
    "ENEMY",
    "PLAYER",
    "ALLY",
    "PROJECTILE",
    "PICKUP",
    "OTHER"
};


static const char * actor_alerts[] =
{
    "NA",
    "IDLE",
    "ALERTED",
    "SEARCHING"
};


static struct parameter param_actor[] =
{
    {PARAM_ENUM, NULL, NULL, NULL, 0, 5, actor_classes},
    {PARAM_INT, NULL, NULL, NULL, -1, 100, NULL},
    {PARAM_BOOL, NULL, "Yes", "No", 0, 0, NULL},
    {PARAM_ENUM, NULL, NULL, NULL, 0, 3, actor_alerts}
};

static struct option edit_actor_opt[] =
{
    {"Actor Class", NULL, NULL, &param_actor[0]},
    {"Health", NULL, &param_actor[1], &param_actor[1]},
    {"Invisible", NULL, NULL, &param_actor[2]},
    {"Alert State", NULL, NULL, &param_actor[3]},
    {"Edit Action Queue", &edit_actor_queue_act, NULL, NULL},
    {"Edit One-time Action Queue", &edit_actor_queue_act, NULL, NULL},
    {"Edit Alerted Action Queue", &edit_actor_queue_act, NULL, NULL},
    {"Edit Searching Action Queue", &edit_actor_queue_act, NULL, NULL},
    {"Edit Wall Collision Action Queue", &edit_actor_queue_act, NULL, NULL},
    {"Edit Actor Collision Action Queue", &edit_actor_queue_act, NULL, NULL}
};

struct menu edit_actor_menu = {10, 0, false, EDITMENU, edit_actor_opt};

void medit_set_actor(struct actor * actor)
{
    param_actor[0].value = &actor->class;
    param_actor[1].value = &actor->health;
    param_actor[2].value = &actor->invisible;
    param_actor[3].value = &actor->alert;
    edit_actor_opt[4].f_param = &actor->actions;
    edit_actor_opt[5].f_param = &actor->one_shot;
    edit_actor_opt[6].f_param = &actor->alerted;
    edit_actor_opt[7].f_param = &actor->searching;
    edit_actor_opt[8].f_param = &actor->coll_wall;
    edit_actor_opt[9].f_param = &actor->coll_act;
}

