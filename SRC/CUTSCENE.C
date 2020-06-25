#include "cutscene.h"
#include "bitmap.h"
#include "menu.h"
#include "font.h"
#include "video.h"
#include "keys.h"

#include <conio.h>
#include <string.h>

static const char * intro_crawl[] =
{
    /*2345678901234567890123456789012345678*/
    "What am I doing here?\r\n",
    
    "Another war, another battlefield.\r\n"
    "I don't understand why I keep taking\r\n"
    "these missions.",
    
    "Retirement is looking really good \r\n"
    "right now.",

    ""
};

enum cast
{
    CAST_SCARAB,
    CAST_TECH,
    CAST_END
};

static struct bitmap faces[CAST_END];

struct conv_line {
    enum cast speaker;
    const char * message;
};

static struct conv_line graveyard_conv[] = {
    {CAST_SCARAB,  "Great. Mechs."},
    {CAST_TECH, "I love mechs."},
    {CAST_TECH, "You should really be more \r\n"
                  "accepting."},
    {CAST_SCARAB,  "Come out here and say \r\n"
                  "hello then."},
                  /*****************************/
    {CAST_TECH, "I'm quite cozy in my lab,\r\n"
                  "thanks. This hot chocolate \r\n"
                  "is delicious."},
    {CAST_SCARAB,  "You don't have to rub it in."},
    {CAST_SCARAB,  "I expect some on my next \r\n"
                  "supply drop."},
                  /*****************************/
    {CAST_END, ""}
};


void scene_intro()
{
    int i;
    int key = 0;
    
    for (i = 0; strlen(intro_crawl[i]) > 0 && key != KEY_ESC; i++)
    {
        key = scene_text(intro_crawl[i], 8, 16, 5, SND_SILENCE);
    }
}


static void intro_conversation(struct conv_line conv[])
{
    int i;
    int key = 0;
    int speakpos;
    enum cast speakers[] = {CAST_END, CAST_END};
    int last_speaker = -1;

    for (i = 0; conv[i].speaker != CAST_END && key != KEY_ESC; i++)
    {
        if (conv[i].speaker == speakers[0] || speakers[0] == CAST_END)
        {
            /* No first speaker, or same as current */
            speakpos = 0;
        }
        else if (conv[i].speaker == speakers[1] || speakers[1] == CAST_END)
        {
            /* No second speaker, or same as current */
            speakpos = 1;
        }
        else
        {
            /* Opposite position from last speaker */
            speakpos = (last_speaker + 1) % 2;
        }

        if (speakers[speakpos] == CAST_END)
        {
            /* No speaker. Draw initial box */
            video_draw_box(2, 2+100*speakpos, 316, 1, COLOR_WHITE);
            video_draw_box(2, 97+100*speakpos, 316, 1, COLOR_WHITE);
            video_draw_box(2, 2+100*speakpos, 1, 96, COLOR_WHITE);
            video_draw_box(317, 2+100*speakpos, 1, 96, COLOR_WHITE);
        }

        if (speakers[speakpos] != conv[i].speaker)
        {
            /* Different speaker. Erase box and draw face */
            video_clear_region(4, 4+100*speakpos, 312, 92);
            bitmap_draw(&faces[conv[i].speaker], 8, 18+100*speakpos);
        }
        else
        {
            /* Same speaker. Erase text region only */
            video_clear_region(72, 4+100*speakpos, 240, 92);
        }

        /* Update conversation state to match */
        speakers[speakpos] = conv[i].speaker;
        last_speaker = speakpos;

        /* Draw text and wait for user response */
        font_draw_string(&defaultfont, conv[i].message, 5, 80, 8+100*speakpos);
        tiles_draw(&defaultfont, 31, 304, 80+100*speakpos);
        key = getch();
        if (key == 0) getch();
        video_clear_region(304, 80+100*speakpos, 8, 16);
    }
}


void scene_intro_conv()
{
    bitmap_load(&faces[CAST_SCARAB], "data\\scarab", 64, 64);
    bitmap_load(&faces[CAST_TECH], "data\\tech", 64, 64);

    video_set_mode(CGA_CYAN);
    scene_text("Incoming Call...", 88, 92, 20, SND_SILENCE);
    
    video_clear();
    intro_conversation(graveyard_conv);

    bitmap_unload(&faces[CAST_SCARAB]);
    bitmap_unload(&faces[CAST_TECH]);
}


int scene_text(const char * text, uint16_t x, uint16_t y,
    uint32_t delay_ms, enum sound_types sound)
{
    int key = 0;
    video_clear();
    font_draw_string(&defaultfont, text, delay_ms, x, y);
    sound_play(sound);
    
    key = getch();
    if (key == 0) getch();

    return key;
}


/*

*/
