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
