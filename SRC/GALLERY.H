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
#ifndef GALLERY_H
#define GALLERY_H

enum gall_image {
     IMAGE_PHOTO1 = 0,
     IMAGE_PHOTO2,
     IMAGE_PHOTO3,
     IMAGE_PHOTO4,
     IMAGE_PHOTO5,
     IMAGE_PHOTO6,
     IMAGE_PHOTO7,
     IMAGE_PHOTO8,
     IMAGE_PHOTO9,
     IMAGE_PHOTO10,
     IMAGE_PHOTO11,
     IMAGE_PHOTO12,
     IMAGE_PHOTO13,
     IMAGE_PHOTO14,
     IMAGE_PHOTO15,
     IMAGE_PHOTO16,
};

/* returns 1 to show the next image; 0 if esc/backspace were pressed */
int gallery_show_image(enum gall_image image);

#endif
