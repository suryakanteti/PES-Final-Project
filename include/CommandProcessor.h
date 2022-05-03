/*
 * CommandProcessor.h
 *
 *  Created on: Apr 5, 2022
 *      Author: Surya Kanteti
 */

#ifndef COMMANDPROCESSOR_H_
#define COMMANDPROCESSOR_H_

/*
  * Handles the command inputted by the user.
  *
  * Parameters:
  *   input		Pointer to the input command
  *
  * Returns:
  *   None
  */
void HandleCommand(char* input);


/*
  * Reads the string entered by user on the UART based console.
  *
  * Parameters:
  *   commandStr	Pointer to a buffer which can hold the command
  *   maxSize		Max capacity of the buffer
  *
  * Returns:
  * 	A string (char*) which contains the input from the user
  */
bool ReadLine(char* commandStr, int* index, uint8_t maxSize);


#endif /* COMMANDPROCESSOR_H_ */
