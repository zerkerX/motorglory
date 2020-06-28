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
#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stdint.h>

enum param_type
{
    PARAM_BOOL,
    PARAM_INT,
    PARAM_STRING,
    PARAM_ENUM
};

struct parameter
{
    enum param_type type;
    void * value;
    const char * trueval;
    const char * falseval;
    int16_t minval;
    int16_t maxval;
    const char ** enumdef;
};

struct option
{
    char name[40];
    void (* function)(void *);
    void * f_param;
    struct parameter * param;
};

enum menustyle
{
    MAINMENU,
    SUBMENU,
    EDITMENU
};

struct menu
{
    int num_options;
    int selection;
    bool close_on_sel;
    enum menustyle style;
    struct option * options;
};

void menu_init();

void menu_loop(struct menu * menu);

void menu_end();

/* Returns true if string confirmed, false on cancel */
bool menu_get_string(const char * prompt, char * output, int bufsize);
bool menu_get_int(const char * prompt, int16_t * output);
bool menu_get_coords(const char * prompt, int16_t * x, int16_t * y);

#endif
