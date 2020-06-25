#ifndef SOUND_H
#define SOUND_H

/* Note: Enum order is by priority (later items are more
important than earlier ones). At least for those that
can happen during gameplay. */
enum sound_types
{
    SND_SILENCE,
    SND_MENU_OK,
    SND_MENU_BACK,
    SND_MENU_UP,
    SND_MENU_DOWN,
    SND_MENU_INC,
    SND_MENU_DEC,
    SND_GAMEOVER,
    SND_SHOT,
    SND_PICKUP,
    SND_HIT,
    SND_PLAYER_HIT,
    SND_ALERT,
    SND_ERROR
};

void sound_play(enum sound_types sound);

void sound_queue(enum sound_types sound);

void sound_play_queue();

#endif
