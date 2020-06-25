#include "keys.h"
#include <dos.h>
#include <stdint.h>

#define KEYBOARD_INTERRUPT 0x09
#define KEY_BUFFER 0x60
#define KEY_CONTROL 0x61
#define INT_CONTROL 0x20
/* Note: reference is on page 493 of "Tricks of the Game Programming Gurus"*/


/* We are intentionally not supporting key codes above 128,
because they will be rather strange. I wouldn't even know
what release codes for those keys would look like. */
#define MAX_KEYS 0x80
#define RELEASE_CODE 0x80

bool key_pressed[MAX_KEYS] = {0}; 
bool key_pushed[MAX_KEYS] = {0}; 

void __interrupt new_key_isr()
{
    int16_t keycode = 0;

    _asm
    {
        ; Get Keycode from keyboard buffer
        in al, KEY_BUFFER
        mov ah,0
        mov keycode, ax

        ;Update the control register accordingly
        in al, KEY_CONTROL
        or al, 82h
        out KEY_CONTROL,al

        and al,7fh
        out KEY_CONTROL,al

        ; Interrupt cleanup
        mov al,20h
        out INT_CONTROL,al
        sti
    }

    if (keycode & RELEASE_CODE)
    {
        keycode = keycode ^ RELEASE_CODE;
        if (keycode < MAX_KEYS)
            key_pressed[keycode] = false;
    }
    else
    {
        if (keycode < MAX_KEYS)
        {
            key_pressed[keycode] = true;
            key_pushed[keycode] = true;
        }
    }
}

static void (__interrupt * old_isr)();

void key_replace_isr()
{
    int16_t key;

    for (key = 0; key < MAX_KEYS; key++)
    {
        key_pressed[key] = false;
        key_pushed[key] = false;
    }
    old_isr = _dos_getvect(KEYBOARD_INTERRUPT);
    _dos_setvect(KEYBOARD_INTERRUPT, new_key_isr);
}

void key_restore_isr()
{
    _dos_setvect(KEYBOARD_INTERRUPT, old_isr);
}


bool key_was_pushed(int keycode)
{
    bool result = key_pushed[keycode];
    key_pushed[keycode] = false;
    return result;
}
