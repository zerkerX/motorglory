#ifndef MEDIT_H
#define MEDIT_H

#include "menu.h"
#include "stage.h"
#include "actor.h"

extern struct menu edit_stage_menu;
extern struct menu load_stage_menu;
extern struct menu edit_actor_menu;

void medit_set_stage(struct stage * stage);
void medit_set_load_opts();
void medit_set_actor(struct actor * actor);

#endif
