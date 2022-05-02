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
  *   Pointer to the buffer which contains the input from the user
  *
  * Returns:
  * 	A string (char*) which contains the input from the user
  */
void ReadLine(char* commandStr);


#endif /* COMMANDPROCESSOR_H_ */
