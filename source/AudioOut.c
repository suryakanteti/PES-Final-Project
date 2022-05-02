/*
 * AudioOut.c
 *
 *  Created on: May 1, 2022
 *      Author: Surya Kanteti
 */

#include "MKL25Z4.h"
#include <stdio.h>

#include "SysTick.h"
#include "cbfifo.h"

#define CLOCK_FREQUENCY (48000000)

// Sampling rate of DAC
#define DAC_SAMPLING_RATE (48000)

#define DAC_POS (30)

const static uint16_t tone_A[] =
			{2037, 2154, 2270, 2386, 2501, 2615, 2726, 2836, 2942, 3045, 3146, 3242, 3335, 3424, 3506, 3585,
			 3659, 3727, 3790, 3847, 3897, 3942, 3981, 4013, 4037, 4056, 4068, 4073, 4072, 4064, 4048, 4027,
			 3999, 3964, 3923, 3875, 3822, 3762, 3697, 3626, 3551, 3470, 3384, 3294, 3199, 3101, 3000, 2895,
			 2787, 2677, 2564, 2450, 2335, 2218, 2101, 1985, 1867, 1751, 1636, 1521, 1408, 1298, 1189, 1085,
			 983, 884, 790, 699, 612, 531, 455, 383, 318, 258, 204, 155, 114, 79, 50, 28, 11, 2, 1, 5, 17, 35,
			 58, 90, 127, 172, 222, 278, 340, 408, 481, 559, 643, 730, 822, 919, 1018, 1121, 1228, 1337, 1447,
			 1562, 1676, 1792, 1909};

const static uint16_t tone_D[] =
			{2037, 2193, 2348, 2501, 2653, 2800, 2943, 3080, 3211, 3336, 3452, 3560, 3660, 3749, 3829, 3898,
			 3957, 4003, 4037, 4061, 4072, 4072, 4059, 4035, 3998, 3950, 3891, 3820, 3740, 3649, 3549, 3439,
			 3322, 3196, 3065, 2927, 2784, 2636, 2485, 2331, 2176, 2020, 1864, 1709, 1556, 1405, 1258, 1116,
			 979, 849, 725, 609, 502, 404, 315, 237, 169, 112, 66, 34, 11, 2, 3, 17, 43, 80, 130, 190, 262,
			 343, 436, 536, 647, 766, 892, 1024, 1162, 1307, 1454, 1607, 1760};


const static uint16_t emptyBuff[] = {0};


uint16_t* tonesArray[] = {tone_A, tone_D};
uint8_t  toneCounts[] = {109, 81};

uint8_t duration = 0;
uint8_t toneIndex = -1;

// Variables to store playback source and count
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
 * Set placback values of DMA
 *
 * Contains the implementation to set the playback values
 * for the DMA module.
 *
 * @input None
 * @return None
 *
 */
void DMA_SetPlaybackValues(uint16_t * source, uint32_t count)
{
	// Save reload information
	Reload_DMA_Source = source;
	Reload_DMA_Byte_Count = count*2;
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

	if(get_timer() > 16 * duration)
	{
		reset_timer();
		if(cbfifo_dequeue(TONES, &toneIndex, 1) == 1)
		{
			cbfifo_dequeue(TONES, &duration, 1);
			Reload_DMA_Source = tonesArray[toneIndex];
			Reload_DMA_Byte_Count = toneCounts[toneIndex] * 2;
		}
		else
		{
			Reload_DMA_Source = emptyBuff;
			Reload_DMA_Byte_Count = 2;
			duration = 0;
		}
	}
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
	TPM0_Start();
	DMA_StartPlayback();
}
