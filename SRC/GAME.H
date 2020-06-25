#ifndef GAME_H
#define GAME_H

#include <stdint.h>

enum game_mode
{
    MODE_PLAY,
    MODE_NEWSTAGE,
    MODE_GAMEOVER,
    MODE_QUIT,
    MODE_NEXT_CHAPTER
};

struct game_state
{
    enum game_mode mode;
    int16_t health;
    int16_t max_health;
    int16_t ammo;
    int16_t next_stage;
    int16_t next_x;
    int16_t next_y;
    int16_t statenum; /* overall game state machine */
};

extern struct game_state game_state;

void game_new();
void game_run();
void game_load(uint16_t position);
void game_save(uint16_t position);
void game_savename(uint16_t position, char * outname);

#endif
