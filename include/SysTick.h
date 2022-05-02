/**
 * @file    SysTick.h
 * @brief   Interface to the functions used to initialize and get timing.
 * @author  Surya Kanteti
 *
 */

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>

/*
 * Type to accommodate number of ticks, where each tick is 1/16 of a second
 */
typedef uint32_t ticktime_t;


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
void SysTick_Init();


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
ticktime_t now();


/*
 * Reset the timer
 *
 * Function resets the timer when invoked.
 *
 * @input None
 * @return None
 *
 */
void reset_timer();


/*
 * Returns time since last reset
 *
 * Function returns the time in ticks (1/16 of a second) since the timer was last reset.
 *
 * @input None
 * @return uint32_t, ticks since last reset
 *
 */
ticktime_t get_timer();

#endif /* __SYSTICK_H__ */
