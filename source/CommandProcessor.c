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
#include <ctype.h>
#include <stdlib.h>
#include "cbfifo.h"

#define ENTER_KEY (13)

// Argument parameters
#define MAX_NUM_OF_ARGUMENTS 15
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
void Handler_Echo(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS]);
void Handler_Help(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS]);

// Command table containing all the supported commands.
static const command_table_t commands[] = {

		{"author", &Handler_Author, "\n\r\tPrint the author's name"},
		{"play"  , &Handler_Play  , "\n\r\tPlay the inputted tones based on the duration"},
		{"echo"  , &Handler_Echo  , "\n\r\tSet the echo mode on or off"},
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
	int tone = -1;
	int duration;

	for(int i = 1; i < argc; i++)
	{
		duration = 0;

		switch (toupper(argv[i][0]))
		{
		case 'A':
			tone = 0;
			break;
		case 'D':
			tone = 1;
			break;
		}

		for(int j = 1; j < strlen(argv[i]); j++)
		{
			duration = duration * 10;
			duration += (int)(argv[i][j]) - 48;
		}

		cbfifo_enqueue(TONES, &tone, 1);
		cbfifo_enqueue(TONES, &duration, 1);
	}
	printf("\n\rTones in progress...\r\n");
}

/*
  * Handles the command "echo".
  * Sets echo mode to on or off based on parameters.
  *
  * Parameters:
  *   argc		Number of arguments
  *   argv		Array of arguments
  *
  * Returns:
  *   None
  */
void Handler_Echo(int argc, char argv[MAX_NUM_OF_ARGUMENTS][MAX_LENGTH_OF_ARGUMENTS])
{
	printf("\n\rEcho mode...yet to do...\r\n");
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



bool ReadLine(char* commandStr, int* index)
{
	char ch;
	bool completed = false;

	ch = getchar();
	if(ch != -1 && ch != ENTER_KEY)
	{
		commandStr[*index] = ch;
		*index = *index + 1;
	}
	else if(ch == ENTER_KEY)
	{
		commandStr[*index] = '\0';
		*index = 0;
		completed = true;
	}

	return completed;
}
