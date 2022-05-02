/*
 * SysTick.c
 *
 *  Created on: Apr 19, 2022
 *      Author: Surya Kanteti
 */


#include <stdio.h>
#include "SysTick.h"
#include "MKL25Z4.h"

static ticktime_t timeSinceReset = 0;
static ticktime_t timeSinceStartup = 0;


/*
 * Initialize systick
 *
 * Contains the implementation to initialize the systick module by setting
 * the clock, load and interrupt priority.
 *
 * @input None
 * @return None
 *
 */
void SysTick_Init()
{
	SysTick->LOAD = (48000000L/256); // Set reload to get 1/16 th second interrupt
	NVIC_SetPriority(SysTick_IRQn,3); // Set interrupt priority
	SysTick->VAL = 0; // Force load of reload value
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | // Enable interrupt, alternate clock source
					SysTick_CTRL_ENABLE_Msk; // Enable SysTick timer
}


/*
 * Returns current time
 *
 * Function returns the time in ticks (1/16 of a second) since the
 * systick module has been initialized.
 *
 * @input None
 * @return uint32_t, ticks since startup
 *
 */
ticktime_t now()
{
	return timeSinceStartup;
}


/*
 * Reset the timer
 *
 * Function resets the timer when invoked.
 *
 * @input None
 * @return None
 *
 */
void reset_timer()
{
	timeSinceReset = 0;
}


/*
 * Returns time since last reset
 *
 * Function returns the time in ticks (1/16 of a second) since the timer was last reset.
 *
 * @input None
 * @return uint32_t, ticks since last reset
 *
 */
ticktime_t get_timer()
{
	return timeSinceReset;
}


/*
 * ISR for systick
 *
 * Interrupt service routine is called for every tick.
 *
 * @input None
 * @return None
 *
 */
void SysTick_Handler()
{
	timeSinceReset++;
	timeSinceStartup++;
}
