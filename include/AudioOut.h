/*
 * AudioOut.h
 *
 *  Created on: May 1, 2022
 *      Author: Surya Kanteti
 */

#ifndef __AUDIO_OUT_H__
#define __AUDIO_OUT_H__


#include <stdbool.h>

/*
 * Initialize Audio Out module
 *
 * Contains the implementation to initialize the Audio Out module
 * by setting the clock and configuration settings.
 *
 * @input None
 * @return None
 *
 */
void AudioOut_Init();


/*
 * Start Audio Out module
 *
 * @input None
 * @return None
 *
 */
void AudioOut_Start();


/*
 * Compute samples based on tone frequencies and echo mode.
 *
 * Contains the implementation to compute the samples based on the tone
 * frequency as well as the echo mode flag.
 *
 * @input None
 * @return None
 *
 */
void ComputeSamples();


/*
 * Interface to set the echo mode flag
 *
 * Contains the implementation to set the echo mode flag.
 *
 * @input flag			Value to set for the flag
 * 		  scalingFactor Factor of amplitude reduction for echo
 * @return None
 *
 */
void SetEchoMode(bool flag, int scalingFactor);

#endif /* __AUDIO_OUT_H__ */
