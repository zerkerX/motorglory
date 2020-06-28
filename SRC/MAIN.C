/* This file is part of Motor Glory.

Motor Glory is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Motor Glory is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Motor Glory.  If not, see <https://www.gnu.org/licenses/>.
*/
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


