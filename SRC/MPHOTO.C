#include "mphoto.h"
#include "gallery.h"
#include <stddef.h>

void photo_image(void * image_raw)
{
    enum gall_image * image = (enum gall_image *)image_raw;
    gallery_show_image(*image);
}

/* Order is a little off from original intent.
  Easier to just use differnet values for each
  position than to edit the menu structs themselves */
static enum gall_image photo1 = IMAGE_PHOTO1;
static enum gall_image photo2 = IMAGE_PHOTO3;
static enum gall_image photo3 = IMAGE_PHOTO4;
static enum gall_image photo4 = IMAGE_PHOTO5;
static enum gall_image photo5 = IMAGE_PHOTO6;
static enum gall_image photo6 = IMAGE_PHOTO7;
static enum gall_image photo7 = IMAGE_PHOTO8;
static enum gall_image photo8 = IMAGE_PHOTO9;
static enum gall_image photo9 = IMAGE_PHOTO10;
static enum gall_image photo10 = IMAGE_PHOTO11;
static enum gall_image photo11 = IMAGE_PHOTO12;
static enum gall_image photo12 = IMAGE_PHOTO13;
static enum gall_image photo13 = IMAGE_PHOTO14;
static enum gall_image photo14 = IMAGE_PHOTO15;
static enum gall_image photo15 = IMAGE_PHOTO16;
static enum gall_image photo16 = IMAGE_PHOTO2;

static struct option photomenu_opt1[] =
{
    {"Scarab", &photo_image, &photo1, NULL},
    {"Water Control", &photo_image, &photo2, NULL},
    {"Refinery", &photo_image, &photo3, NULL},
    {"Walkways", &photo_image, &photo4, NULL},
    {"Power Reg.", &photo_image, &photo5, NULL},
    {"Power Dist.", &photo_image, &photo6, NULL},
    {"Power Gen.", &photo_image, &photo8, NULL},
    {"Ravine", &photo_image, &photo7, NULL}
};

static struct option photomenu_opt2[] =
{
    {"Tunnels", &photo_image, &photo9, NULL},
    {"Platform", &photo_image, &photo10, NULL},
    {"Aquaduct", &photo_image, &photo11, NULL},
    {"Bolts", &photo_image, &photo12, NULL},
    {"Sunrise", &photo_image, &photo13, NULL},
    {"Entrance", &photo_image, &photo14, NULL},
    {"Comm Tower", &photo_image, &photo15, NULL},
    {"Entertainment", &photo_image, &photo16, NULL}
};

struct menu photomenu1 = {8, 0, false, SUBMENU, photomenu_opt1};
struct menu photomenu2 = {8, 0, false, SUBMENU, photomenu_opt2};
