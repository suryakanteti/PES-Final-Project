/*
 * UART_IO.c
 *
 *  Created on: May 1, 2022
 *      Author: Surya Kanteti
 */


#include "UART_IO.h"
#include "cbfifo.h"
#include "sysclock.h"
#include <MKL25Z4.h>


#define UART_OVERSAMPLE_RATE (16)

#define DATA_SIZE (8)
#define PARITY (0)
#define STOP_BITS (2)


/*
  * Reads one character from UART console.
  *
  * Parameters:
  *   None
  *
  * Returns:
  *   The character read in int.
  */
int __sys_readc()
{
	// Dequeue from the RXQ queue
	char ch;

	if(cbfifo_length(RXQ) == 0) // Return -1 if no character is present.
		return -1;

	if(cbfifo_dequeue(RXQ, &ch, sizeof(char)) == 0) // Some error
		return -1;

	return ch;
}


/*
  * Writes a string into UART console.
  *
  * Parameters:
  *   None
  *
  * Returns:
  *   0 if successful, -1 if failed.
  */
int __sys_write(int handle, char* buf, int size)
{

	if(buf == NULL || size <= 0)
		return -1;

	// Enqueue into the TXQ queue
	uint8_t bytesEnqueued;

	while(size > 0)
	{
		bytesEnqueued = cbfifo_enqueue(TXQ, buf, size);

		if( !(UART0->C2 & UART_C2_TIE_MASK))
			UART0->C2 |= UART_C2_TIE(1);

		buf = buf + bytesEnqueued;
		size = size - bytesEnqueued;
	}

	return 0;
}


/*
  * Initializes UART0 with the given baud rate.
  *
  * Parameters:
  *   baud_rate      Baud rate(bits per second) for UART communication
  *
  * Returns:
  *   None
  */
void Init_UART0(uint32_t baud_rate)
{
	uint16_t sbr;
	uint8_t temp;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYSCLOCK_FREQUENCY)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bits
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS - 1) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_SIZE - 8) | UART0_C1_PE(PARITY);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);


	// Enable interrupts. Listing 8.11 on p. 234

	NVIC_SetPriority(UART0_IRQn, 1); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}


// UART0 IRQ Handler. Listing 8.12 on p. 235

/*
 * Interrupt handler for UART0.
 * Performs enqueue-ing and dequeue-ing from the transmission and receiver buffer
 * corresponding to the UART based console.
 *
 * Parameters:
 * 		None
 *
 * Returns:
 * 		None
 */
void UART0_IRQHandler(void) {
	uint8_t ch;
	uint8_t tx_char;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			// read the data register to clear RDRF
			ch = UART0->D;
	}

	if (UART0->S1 & UART0_S1_RDRF_MASK)
	{
		// received a character
		ch = UART0->D;

		UART0->D = ch; // Reflect it back on the console

		if(!IsFull(RXQ))
		{
			cbfifo_enqueue(RXQ, &ch, sizeof(ch));
		}
		else
		{
			// error - queue full.
			// discard character
		}
	}

	if ( (UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK) ) // transmitter interrupt enabled and tx buffer empty
	{
		// can send another character
		if (!IsEmpty(TXQ))
		{
			cbfifo_dequeue(TXQ, &tx_char, sizeof(uint8_t));
			UART0->D = tx_char;
		}
		else
		{
			// queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}
