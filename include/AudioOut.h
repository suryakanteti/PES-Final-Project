/*
 * AudioOut.h
 *
 *  Created on: May 1, 2022
 *      Author: Surya Kanteti
 */

#ifndef __AUDIO_OUT_H__
#define __AUDIO_OUT_H__


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

#endif /* __AUDIO_OUT_H__ */
