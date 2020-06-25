#include "sound.h"
#include "moptions.h"
#include "timer.h"

#include <conio.h>

enum sound_types queued_sound = SND_SILENCE;

static void sound_output(unsigned long frequency)
{
    unsigned long div;
    unsigned char tmp;

    /* What is this number from? */
    div = 1193180 / frequency;

    /* Program PIT 2 */
    outp(0x43, 0xb6);
    outp( 0x42, 0xFF & div );
    outp( 0x42, 0xFF & (div >> 8) );

    /* Tie PC speaker to PIT */
    tmp = inp(0x61);
    outp(0x61, tmp | 0x03);
}

static void sound_off()
{
    unsigned char tmp = inp(0x61);
    outp(0x61, tmp & 0xFC);
}


static void sound_beep(unsigned long frequency, int time_ms)
{
    if (settings.sound)
    {
        sound_output(frequency);
        timer_wait_for(time_ms);
        sound_off();
    }
}


void sound_play(enum sound_types sound)
{
    switch (sound)
    {
        case SND_MENU_OK:
            sound_beep(2000, 100);
            sound_beep(2500, 100);
            break;
        case SND_MENU_BACK:
            sound_beep(2500, 100);
            sound_beep(2000, 100);
            break;
        case SND_MENU_UP:
            sound_beep(4000, 50);
            break;
        case SND_MENU_DOWN:
            sound_beep(4000, 20);
            break;
        case SND_MENU_INC:
            sound_beep(3000, 100);
            break;
        case SND_MENU_DEC:
            sound_beep(1000, 100);
            break;
        case SND_GAMEOVER:
            sound_beep(147, 167);
            timer_wait_for(145);
            sound_beep(147, 167);
            timer_wait_for(158);
            sound_beep(156, 167);
            timer_wait_for(44);
            sound_beep(147, 167);
            timer_wait_for(56);
            sound_beep(131, 167);
            timer_wait_for(470);
            sound_beep(233, 167);
            timer_wait_for(152);
            sound_beep(233, 167);
            timer_wait_for(156);
            sound_beep(261, 167);
            break;
        case SND_SHOT:
            sound_beep(800, 50);
            break;
        case SND_PICKUP:
            sound_beep(400, 30);
            sound_beep(500, 30);
            sound_beep(400, 30);
            break;
        case SND_HIT:
            sound_beep(500, 100);
            break;
        case SND_PLAYER_HIT:
            sound_beep(600, 100);
            break;
        case SND_ALERT:
            sound_beep(650, 100);
            sound_beep(700, 100);
            sound_beep(730, 100);
            sound_beep(750, 100);
            break;
        case SND_ERROR:
            sound_beep(1500, 1500);
            break;
        default:
            /* No sound. Also used for SND_SILENCE */
            break;
    }
}


void sound_queue(enum sound_types sound)
{
    if (sound > queued_sound)
        queued_sound = sound;
}


void sound_play_queue()
{
    sound_play(queued_sound);
    queued_sound = SND_SILENCE;
}

