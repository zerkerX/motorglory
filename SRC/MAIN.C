#include "bitmap.h"
#include "video.h"
#include "menu.h"
#include "mmain.h"
#include "moptions.h"
#include "timer.h"

#include <stdio.h>
#include <conio.h>

int main()
{
    timer_replace_isr();
    options_load();
    menu_init();
    menu_loop(&mainmenu);
    menu_end();

    video_set_mode(TEXT_MODE);
    options_save();
    timer_restore_isr();

    return 0;
}


