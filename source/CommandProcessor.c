/*
 * CommandProcessor.c
 *
 *  Created on: Apr 5, 2022
 *      Author: Surya Kanteti
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define ENTER_KEY (13)

// Argument parameters
#define MAX_NUM_OF_ARGUMENTS 5
#define MAX_LENGTH_OF_ARGUMENTS 10

typedef void (*command_handler_t)(int, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS]);

// Structure defining entries of the command table
typedef struct command_table_s{

  const char *name;
  command_handler_t handler;
  const char *help_string;

} command_table_t;

void Handler_Author(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS]);
void Handler_Play(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS]);
void Handler_Help(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS]);

// Command table containing all the supported commands.
static const command_table_t commands[] = {

		{"author", &Handler_Author, "\n\r\tPrint the author's name"},
		{"play"  , &Handler_Play  , "\n\r\tPlay the inputted tones based on the duration"},
		{"help"  , &Handler_Help  , "\n\r\tPrint this help message"},
};


// Number of commands in the table.
static const int num_commands = sizeof(commands) / sizeof(command_table_t);


/*
  * Handles the command "author".
  * Prints the name of the author.
  *
  * Parameters:
  *   argc		Number of arguments
  *   argv		Array of arguments
  *
  * Returns:
  *   None
  */
void Handler_Author(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS])
{
	printf("\n\rSurya Kanteti\r\n");
}


/*
  * Handles the command "play".
  *
  * Plays the tones for a certain duration based on the
  * input entered by the user .
  *
  * Parameters:
  *   argc		Number of arguments
  *   argv		Array of arguments
  *
  * Returns:
  *   None
  */
void Handler_Play(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS])
{
	printf("\n\rTones in progress...\r\n");
}


/*
  * Handles the command "help".
  * Prints all the existing commands along with their description.
  *
  * Parameters:
  *   argc		Number of arguments
  *   argv		Array of arguments
  *
  * Returns:
  *   None
  */
void Handler_Help(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS])
{
	printf("\r\n\nBreakfastSerial Command Processor\r\n\n");
	for(int i = 0; i < num_commands; i++)
	{
		printf("\r%s", commands[i].name);
		printf("%s\n\n", commands[i].help_string);
	}
	printf("\r");
}


/*
  * Handles the command inputted by the user.
  *
  * Parameters:
  *   input		Pointer to the input command
  *
  * Returns:
  *   None
  */
void HandleCommand(char* input)
{
	if (input == NULL)
		return;

	int index = 0;
	int argc = 0;
	char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS];
	bool validArg = false;

	char* ptr = input;
	while(*ptr == ' ') // Skip the starting spaces
	{
		ptr++;
	}

	while(*ptr != '\0') // Iterate through the whole command and parse it
	{
		validArg = false;
		index = 0;
		while(*ptr != ' ' && *ptr != '\0') // Spaces are removed all the time
		{
			argv[argc][index] = *ptr;
			validArg = true;
			index++;
			ptr++;
		}

		if(*ptr != '\0')
			ptr++;

		argv[argc][index] = '\0';

		if(validArg) // Increment argc only if the argument is non-empty
			argc++;
	}

	if(argc == 0) // No commands
		return;

	// Send the command to respective handler
	bool validCommand = false;
	for (int i=0; i < num_commands; i++)
	{
	    if (strcasecmp(argv[0], commands[i].name) == 0)
	    {
	      validCommand = true;
	      commands[i].handler(argc, argv);
	      break;
	    }
	}

	if(!validCommand) // Handle case where the command is unknown
		printf("\n\rUnknown command: %s\r\n", argv[0]);
}

/*
  * Reads the string entered by user on the UART based console.
  *
  * Parameters:
  *   None
  *
  * Returns:
  * 	A string (char*) which contains the input from the user
  */
char* ReadLine()
{
	char ch;
	char* linePtr;
	char line[100];

	int index = 0;
	while (ch != ENTER_KEY)
	{
		ch = getchar();
	  	if(ch != -1 && ch != '\b')
	  	{
	  		line[index] = ch;
	  	    index++;
	  	}
	  	if(ch == '\b' && index != 0) // Handle backspace
	  	{
	  		printf(" \b");
	  		index = index - 2;
	  	}
	}

	line[index - 1] = '\0';
	linePtr = line;
	return linePtr;
}
