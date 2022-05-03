/*
 * fp_trig.c
 *
 *  Created on: Apr 16, 2022
 *      Author: Surya Kanteti
 */

#include <stdbool.h>

#include <stdio.h>

#include "fp_trig.h"

#define DAC_SAMPLING_RATE 	 (48000)

// Number of steps in the lookup table.
#define TRIG_TABLE_STEPS     (32)

#define TRIG_TABLE_STEP_SIZE (HALF_PI/TRIG_TABLE_STEPS)


/*
 * Lookup table of sine values according to the scale.
 * Generated using a Python script.
 */
static const int16_t sin_lookup[TRIG_TABLE_STEPS+1] =
	{	0, 100, 200, 299, 398, 495, 592, 687,
		780, 872, 961, 1048, 1132, 1214, 1293, 1369,
		1441, 1510, 1575, 1637, 1694, 1748, 1797, 1842,
		1883, 1919, 1950, 1977, 1998, 2015, 2028, 2035,
		2037
  };


/*
 * Calculate interpolated values
 *
 * Contains the implementation to calculate sine values which are not
 * part of the steps we present in the lookup table.
 *
 *
 * @input x		Input value
 * @return		Interpolated value of the input.
 *
 */
uint16_t InterpolateValue(int x)
{
	uint16_t retValue;

	//Based on the checked index, interpolate values within range
	int checkIndex = x / 100;
	uint16_t sineValueUpper = sin_lookup[checkIndex + 1];
	uint16_t sineValueLower = sin_lookup[checkIndex];
	retValue = ((sineValueUpper - sineValueLower) * (x % 100))/100 + sineValueLower;
	return retValue;
}


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
int fp_sin(int x)
{
	bool lowerHalf = false;
	int16_t retValue;

	// Get values within the [-PI, PI] range
	while(x > PI)
	{
		x -= TWO_PI;
	}
	while(x < -PI)
	{
		x += TWO_PI;
	}

	// Handle the four quadrants
	if(x > HALF_PI && x <= PI)
	{
		x = PI - x;
	}
	else if(x >= -HALF_PI && x < 0)
	{
		lowerHalf = true;
		x = -x;
	}
	else if(x >= -PI && x <= -HALF_PI)
	{
		lowerHalf = true;
		x = PI + x;
	}

	retValue = InterpolateValue(x);

	if(lowerHalf)
		retValue = -retValue;

	return retValue;
}


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
int tone_to_samples(uint16_t* table, int toneFreq)
{
	int bufferSize = 0;
	int numOfSamples = (PERIODS_PER_BUFFER * DAC_SAMPLING_RATE)/toneFreq;

	for(int i = 0; i < numOfSamples; i++)
	{
		table[i] = fp_sin((i * TWO_PI * toneFreq)/DAC_SAMPLING_RATE) + TRIG_SCALE_FACTOR;
		bufferSize++;
	}
	return bufferSize;
}
