/*
 * cbfifo.c - Implementation of a fixed size FIFO
 *			  based on circular buffer.
 *
 * Author: Surya Kanteti
 */

#include "cbfifo.h"
#include "MKL25Z4.h"

#define LOWER8_MASK 0xFF


// Structure to hold the metadata of the circular buffer
typedef struct cbfifo_s
{
	uint16_t readIndex; // First element to read
	uint16_t writeIndex; // Next available position to write
	uint16_t length; // Number of elements occupied in the buffer

	bool isFull; // Indicates if the circular buffer is full.

	uint8_t cbFifo[CBFIFO_SIZE];
} cbfifo_t;


// Structures for UART TX, UART RX and TONES queues
cbfifo_t queues[3];


/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   bufSelect 	Select TX/RX buffer
 *   buf      	Pointer to the data
 *   nbyte    	Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns (size_t) -1.
 */
size_t cbfifo_enqueue(int bufSelect, void* buf, size_t nbyte)
{
	if (buf == NULL)
		return -1;

	if (queues[bufSelect].isFull)
		return 0; // Cannot write anything because the buffer is full.

	uint32_t maskingState;
	maskingState = __get_PRIMASK();
	__disable_irq();

	if (nbyte > CBFIFO_SIZE - queues[bufSelect].length)
		nbyte = CBFIFO_SIZE - queues[bufSelect].length; // Write only into the available number of bytes.

	uint8_t* byteStream = (uint8_t*)(buf);
	for (uint8_t i = 0; i < nbyte; i++)
	{
		queues[bufSelect].cbFifo[queues[bufSelect].writeIndex] = byteStream[i];
		queues[bufSelect].writeIndex = (queues[bufSelect].writeIndex + 1) & LOWER8_MASK;
	}

	queues[bufSelect].length += nbyte;

	if (queues[bufSelect].writeIndex == queues[bufSelect].readIndex)
		if (queues[bufSelect].length != 0)
			queues[bufSelect].isFull = true;

	__set_PRIMASK(maskingState);
	return nbyte;
}


/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 * 	 bufSelect 	Select TX/RX buffer
 *   buf      	Destination for the dequeued data
 *   nbyte    	Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
size_t cbfifo_dequeue(int bufSelect, void* buf, size_t nbyte)
{
	if (queues[bufSelect].length == 0)
		return 0; // Cannot dequeue anything if the buffer is empty.

	uint32_t maskingState;
	maskingState = __get_PRIMASK();
	__disable_irq();

	if (nbyte > queues[bufSelect].length)
		nbyte = queues[bufSelect].length; // Cannot dequeue more bytes than existing.

	uint8_t* outputBuf = (uint8_t*)(buf);

	for (uint8_t i = 0; i < nbyte; i++)
	{
		*outputBuf = queues[bufSelect].cbFifo[queues[bufSelect].readIndex];
		outputBuf++;
		queues[bufSelect].readIndex = (queues[bufSelect].readIndex + 1) & LOWER8_MASK;
	}

	queues[bufSelect].length -= nbyte;
	queues[bufSelect].isFull = false; // Since bytes were removed, buffer is not full anymore.

	__set_PRIMASK(maskingState);
	return nbyte;
}


/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   bufSelect 	Select TX/RX buffer
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(int bufSelect)
{
	return queues[bufSelect].length;
}


/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   bufSelect 	Select TX/RX buffer
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
size_t cbfifo_capacity()
{
	return CBFIFO_SIZE;
}


/*
 * Checks if FIFO is full
 *
 * Parameters:
 *   bufSelect 	Select TX/RX buffer
 *
 * Returns:
 *   True if buffer is full, else False
 */
bool IsFull(int bufSelect)
{
	return (queues[bufSelect].length == CBFIFO_SIZE);
}


/*
 * Checks if FIFO is empty
 *
 * Parameters:
 *   bufSelect 	Select TX/RX buffer
 *
 * Returns:
 *   True if buffer is empty, else False
 */
bool IsEmpty(int bufSelect)
{
	return (queues[bufSelect].length == 0);
}
