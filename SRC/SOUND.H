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
