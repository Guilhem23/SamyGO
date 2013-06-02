/*
 * (C) Copyright 2008
 * Texas Instruments
 *
 * Richard Woodruff <r-woodruff2@ti.com>
 * Syed Moahmmed Khasim <khasim@ti.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <types.h>
#include <echo-reg.h>
 
static ulong timestamp;
static ulong lastdec;


#define TIMER_LOAD_VAL	0xFFFF
#define TIMER_PRESCALER 124


extern unsigned long get_clock_apb(void);
void reset_timer_masked(void);
unsigned long get_timer_masked(void);

/* use lower 32 bit of 64bit timer */
static inline volatile ulong READ_TIMER(void)
{
	return (volatile ulong)(0x0000FFFFUL & R_TMCNT0);
}


int timer_init(void)
{
	unsigned int prescaler, prescaler1, prescaler2;
	
	prescaler = (unsigned int) (get_clock_apb() / 100000);
	if((prescaler / 256) < 4)
		prescaler2 = 0;
	else if((prescaler / 256) < 8)
		prescaler2 = 1;
	else if((prescaler / 256) < 16)
		prescaler2 = 2;
	else 
		prescaler2 = 3;

	prescaler1 = prescaler / (4 << prescaler2);

	R_TMDMASEL = 0;
	R_TMCON0 =  0x10 | (prescaler2 << 2);	/* 16bit mode */
	R_TMDATA0 =  (prescaler1<<16) | (TIMER_LOAD_VAL & 0xFFFF);
	
	timestamp = 0;
	lastdec = TIMER_LOAD_VAL;

	/* timer start */
	R_TMCON0 =  0x11 | (prescaler2 << 2);

	return (0);
}

/*
 * timer without interrupts
 */
void reset_timer(void)
{
	reset_timer_masked ();

}

ulong get_timer(ulong base)
{
	return (get_timer_masked () / 1000) - base;  // change from usec to msec

}

void set_timer(ulong t)
{
	timestamp = t;

}

inline ulong get_delay_time(const unsigned long usec)
{
	/* 1 downcount takes 10 usec */
	return usec / 10;
}


/* delay x useconds */
void __udelay(unsigned long usec)
{
	ulong tmo;
		
	tmo = get_delay_time(usec);
	//	tmo += get_timer (0);			/* from current time to +tmo */
	tmo += get_timer_masked (); 		/* from current time to +tmo */
	
	while (get_timer_masked () < tmo)
		/*NOP*/;

}

void reset_timer_masked(void)
{
	lastdec = READ_TIMER();
	timestamp = 0;

}

ulong get_timer_masked(void)
{
	volatile ulong now = READ_TIMER();

	/* add elapsed time to timestamp */
	if( lastdec >= now )
		timestamp += lastdec - now;
	else
		// ij.jang 26.Feb.2006 : fixed the critical bug
		timestamp += (0xFFFF) & ((TIMER_LOAD_VAL - now) + lastdec);

	lastdec = now;

	return timestamp; 
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);		// msec tick
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return 0;
}
