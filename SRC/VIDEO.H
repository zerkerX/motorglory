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
#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

enum video_mode { TEXT_MODE, CGA_BW, CGA_CYAN, CGA_YELLOW };
#define COLOR_BLACK (0x0)
#define COLOR_CYAN (0x1)
#define COLOR_MAGENTA (0x2)
#define COLOR_WHITE (0x3)
#define VIDEO_BUFFER_SIZE (8000)

extern uint8_t * CGA1;
extern uint8_t * CGA2;

void video_set_mode(enum video_mode mode);

void video_clear();

/* NOTE: X values and widths must be divisible by 4.
   Y values must be divisible by 2 */
void video_clear_region(uint16_t x, uint16_t y,
        uint16_t width, uint16_t height);

/* Unlike above, this function can operate on individual pixel dimensions */
void video_draw_box(uint16_t x, uint16_t y,
        uint16_t width, uint16_t height, uint8_t color);

void video_enable_double_buffer();
void video_disable_double_buffer();
void video_flip_buffer();

#endif
