/*
 * AudioOut.c
 *
 *  Created on: May 1, 2022
 *      Author: Surya Kanteti
 */

#include "MKL25Z4.h"
#include <stdio.h>
#include <stdbool.h>

#include "SysTick.h"
#include "cbfifo.h"
#include "fp_trig.h"

// Frequency of clock used
#define CLOCK_FREQUENCY (48000000)

// Sampling rate of DAC
#define DAC_SAMPLING_RATE (48000)

// Pin of DAC output
#define DAC_POS (30)

// Number of ticks in one second
#define TICKS_PER_SECOND (16)

// Frequencies of the supported tones
uint16_t toneFrequencies[] = {440, 494, 523, 587, 659, 698, 784};

// Common variables (play)
#define ALTERNATING_BUFFERS (2)
#define MAX_BUFFER_SIZE (150)

int currentBuffer = 0; // Index to keep track of samplesBuffers
uint8_t toneDuration = 0;
uint16_t samplesBuffers[ALTERNATING_BUFFERS][MAX_BUFFER_SIZE]; // Buffers to alternate between
const static uint16_t emptyBuff[] = {0};

// Echo parameters
#define ECHO_BUFFER_SIZE (6000)
#define ECHO_BUFFER_PARTITION (3500)
#define GAIN (0.6)
bool echoEnabled = false; // Flag to check if echo mode is enabled
uint16_t echoBuffer[ECHO_BUFFER_SIZE]; // Buffer containing echo samples

// Variables to store play-back source and count
static uint16_t * Reload_DMA_Source = emptyBuff;
static uint32_t Reload_DMA_Byte_Count = 2;


/*
 * Initialize TPM0
 *
 * Contains the implementation to initialize the timer-PWM 0 module
 * by setting the clock and configuration settings.
 *
 * @input None
 * @return None
 *
 */
void TPM0_Init(uint32_t period_us)
{
	//turn on clock to TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

	//set clock source for tpm
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	// disable TPM
	TPM0->SC = 0;

	//load the counter and mod
	TPM0->MOD = TPM_MOD_MOD(CLOCK_FREQUENCY / period_us);

	//set TPM to count up and divide by 1 prescaler and clock mode
	TPM0->SC = (TPM_SC_DMA_MASK | TPM_SC_PS(0));

	// Enable TPM interrupts for debugging
	TPM0->SC |= TPM_SC_TOIE_MASK;

	// Configure NVIC
	NVIC_SetPriority(TPM0_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);
}


/*
 * Start TPM0
 *
 * Starts the timer module TPM0.
 *
 * @input None
 * @return None
 *
 */
void TPM0_Start(void)
{
	// Enable counter
	TPM0->SC |= TPM_SC_CMOD(1);
}


/*
 * ISR for TPM0
 *
 * Interrupt service routine is called for TPM0 overflow.
 *
 * @input None
 * @return None
 *
 */
void TPM0_IRQHandler()
{
	//clear pending IRQ
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	TPM0->SC |= TPM_SC_TOIE_MASK; // reset overflow flag
}


/*
 * Initialize DMA
 *
 * Contains the implementation to initialize the DMA module
 * by setting the source and configuration settings.
 *
 * @input None
 * @return None
 *
 */
void DMA_Init()
{
	// Gate clocks to DMA and DMAMUX
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	// Disable DMA channel to allow configuration
	DMAMUX0->CHCFG[0] = 0;

	// Generate DMA interrupt when done
	// Increment source, transfer words (16 bits)
	// Enable peripheral request
	DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK |
											DMA_DCR_SSIZE(2) | DMA_DCR_DSIZE(2) |
											DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK;

	// Configure NVIC for DMA ISR
	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	// Enable DMA MUX channel with TPM0 overflow as trigger
	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(54);
}


/*
 * Start DMA placback.
 *
 * Contains the implementation to start the DMA playback.
 *
 * @input None
 * @return None
 *
 */
void DMA_StartPlayback()
{
	// initialize source and destination pointers
	DMA0->DMA[0].SAR = DMA_SAR_SAR((uint32_t) Reload_DMA_Source);
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t) (&(DAC0->DAT[0])));
	// byte count
	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(Reload_DMA_Byte_Count);
	// clear done flag
	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;
	// set enable flag
	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
}


/*
 * ISR for DMA0
 *
 * Interrupt service routine is called after DMA completion.
 *
 * @input None
 * @return None
 *
 */
void DMA0_IRQHandler(void)
{
	// Clear done flag
	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;

	// Start the next DMA playback cycle
	DMA_StartPlayback();
}


/*
 * Initialize DAC
 *
 * Contains the implementation to initialize the digital-to-analog converter module by setting
 * the clock and configuration settings.
 *
 * @input None
 * @return None
 *
 */
void DAC_Init()
{
  // Init DAC output

	SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; // DAC0_OUT is by deafult on PTE30

	PORTE->PCR[DAC_POS] &= ~(PORT_PCR_MUX(7));	// Select analog

	// Disable buffer mode
	DAC0->C1 = 0;
	DAC0->C2 = 0;

	// Enable DAC, select VDDA as reference voltage
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
}


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
void AudioOut_Init()
{
	TPM0_Init(DAC_SAMPLING_RATE); // Initialize TPM0 to trigger DAC0
	DAC_Init();
	DMA_Init();
}


/*
 * Start Audio Out module
 *
 * @input None
 * @return None
 *
 */
void AudioOut_Start()
{
	TPM0_Start(); // Start the individual modules
	DMA_StartPlayback();
}


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
void ComputeSamples()
{
	uint8_t tone;
	int count;

	if(echoEnabled)
	{
		for(int i = 0; i < ECHO_BUFFER_SIZE; i++)
		{
			echoBuffer[i] *= 0.97;
		}
	}

	if(get_timer() > TICKS_PER_SECOND * toneDuration)
	{
		reset_timer();

		if(cbfifo_dequeue(TONES, &tone, 1) == 1)
		{
			cbfifo_dequeue(TONES, &toneDuration, 1);
			count = tone_to_samples(samplesBuffers[currentBuffer], toneFrequencies[tone]);

			if(echoEnabled)
			{
				// Initialize the echo buffer
				for(int i = 0; i < ECHO_BUFFER_PARTITION; i++)
				{
					echoBuffer[i] = samplesBuffers[currentBuffer][i % count];
				}
				for(int i = ECHO_BUFFER_PARTITION; i < ECHO_BUFFER_SIZE; i++)
				{
					echoBuffer[i] = samplesBuffers[currentBuffer][i % count] + samplesBuffers[currentBuffer][(i - ECHO_BUFFER_PARTITION) % count] * GAIN;
				}
				Reload_DMA_Source = echoBuffer;
				Reload_DMA_Byte_Count = 2 * ECHO_BUFFER_SIZE;
			}
			else
			{
				Reload_DMA_Source = samplesBuffers[currentBuffer];
				Reload_DMA_Byte_Count = count * 2;
			}

			currentBuffer = 1 - currentBuffer; // Switch between the buffers
		}
		else
		{
			Reload_DMA_Source = emptyBuff;
			Reload_DMA_Byte_Count = 2;
			toneDuration = 0;
		}

	}
}


/*
 * Interface to set the echo mode flag
 *
 * Contains the implementation to set the echo mode flag.
 *
 * @input flag	Value to set for the flag
 * @return None
 *
 */
void SetEchoMode(bool flag)
{
	echoEnabled = flag;
}
