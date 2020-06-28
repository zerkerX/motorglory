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
#ifndef CHAPTER_H
#define CHAPTER_H

#include "stage.h"
#include <stdint.h>
#include <stdio.h>

struct chapter {
    struct stage * first_stage;
    struct stage * last_stage;
    int16_t start_stage_num;
    int16_t num_stages;
    char filename[20];
};

extern struct chapter curr_chapter;

void chapter_run();

void chapter_new();

void chapter_add_new();

void chapter_remove(struct stage * remstage);

void chapter_load_from(FILE * input);

void chapter_load(const char * filename);

void chapter_save_into(FILE * output);

void chapter_save();

void chapter_save_as(const char * filename);

void chapter_unload();

struct stage * chapter_get_stage_num(int16_t num);

#endif
