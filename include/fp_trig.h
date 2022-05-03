/*
 * fp_trig.h
 *
 *  Created on: Apr 16, 2022
 *      Author: Surya Kanteti
 */

#ifndef FP_TRIG_H_
#define FP_TRIG_H_

#define TRIG_SCALE_FACTOR  (2037)
#define HALF_PI            (3200)  // pi * TRIG_SCALE_FACTOR / 2
#define PI                 (6399)  // pi * TRIG_SCALE_FACTOR
#define TWO_PI            (12799)  // 2 * pi * TRIG_SCALE_FACTOR

#include <stdint.h>

#define PERIODS_PER_BUFFER (1)

/*
 * Calculate sine values
 *
 * Contains the implementation to calculate sine values based on the input.
 * The input is scaled based on the values HALF_PI, PI, TWO_PI
 * The output is in the range [-TRIG_SCALE_FACTOR, TRIG_SCALE_FACTOR]
 *
 *
 * @input x		Input value to the sine function
 * @return		Sine value of the input scaled to the output range.
 *
 */
int fp_sin(int x);


/*
 * Calculate sine table
 *
 * Contains the implementation to populate a buffer with sine values
 * based on the tone frequency.
 *
 * @input table		Pointer to the buffer to be populated
 * 		  toneFreq	Input frequency deciding the samples
 * @return Number of elements written into the buffer.
 *
 */
int tone_to_samples(uint16_t* table, int toneFreq);

#endif /* FP_TRIG_H_ */
