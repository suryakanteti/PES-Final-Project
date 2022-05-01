/*
 * UART_IO.h
 *
 *  Created on: Apr 5, 2022
 *      Author: Surya Kanteti
 */

#ifndef __UART_IO_H__
#define __UART_IO_H__

#include <stdint.h>


/*
  * Reads one character from UART console.
  *
  * Parameters:
  *   None
  *
  * Returns:
  *   The character read in int.
  */
int __sys_readc();


/*
  * Writes a string into UART console.
  *
  * Parameters:
  *   None
  *
  * Returns:
  *   0 if successful, -1 if failed.
  */
int __sys_write(int handle, char* buf, int size);


/*
  * Initializes UART0 with the given baud rate.
  *
  * Parameters:
  *   baud_rate      Baud rate(bits per second) for UART communication
  *
  * Returns:
  *   None
  */
void Init_UART0(uint32_t baud_rate);


#endif /* __UART_IO_H__ */
