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
#include "timer.h"

#include <i86.h>
#include <conio.h>
#include <dos.h>

#define TIMER0_INTERRUPT 0x1C

/* Roughly 1 ms timer. This gives us about 49 days before rollover. */
#define TIMER_FREQ 1000

uint32_t countval;
uint32_t storedval;

void __interrupt new_timer_isr()
{
    countval++;
}

static void (__interrupt * old_timer_isr)();

void timer_replace_isr()
{
    uint32_t div;

    div = 1193180UL / TIMER_FREQ;

    /* Program PIT 0 */
    outp(0x43, 0x3C);
    outp(0x40, 0xFF & div);
    outp(0x40, 0xFF & (div >> 8) );

    /* Install ISR */
    countval = 0UL;
    old_timer_isr = _dos_getvect(TIMER0_INTERRUPT);
    _dos_setvect(TIMER0_INTERRUPT, new_timer_isr);

    /* TODO: Store old clock value */
}

void timer_restore_isr()
{
    _dos_setvect(TIMER0_INTERRUPT, old_timer_isr);
}


void timer_start()
{
    storedval = countval;
}


void timer_wait_for(uint32_t dur_ms)
{
    uint32_t endval = countval + dur_ms;

    while (countval < endval) {}
}


void timer_wait_until(uint32_t dur_ms)
{
    uint32_t endval = storedval + dur_ms;

    while (countval < endval) {}    
}

