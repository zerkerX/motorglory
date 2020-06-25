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
