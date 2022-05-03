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



bool ReadLine(char* commandStr, int* index, uint8_t maxSize);


#endif /* COMMANDPROCESSOR_H_ */
