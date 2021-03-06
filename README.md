# PES-Final-Project

Final Project of Principles of Embedded Software, ECEN 5813, Spring 2022

Developed by Surya Kanteti

# Project Description
The project contains the implementation of "ARMonica" a command-line processor based musical instrument which runs on the KL25Z-FRDM board. It can take commands from the user and play a musical tone according to it.

# Files in the project:
Source files: ARMonica.c, AudioOut.c, cbfifo.c, CommandProcessor.c, fp_trig.c, UART_IO.c

Header files: AudioOut.h, cbfifo.h, CommandProcessor.h, fp_trig.h, UART_IO.h

# How to Run

Build the project in either Debug or Release mode. Indebug, extra tests will be run to verify authenticity of the modules.

Connect a speaker to the FRDM-KL25Z board by connecting the PTE30 DAC output pin (J10 11) to the positive node of the speaker and connect the negative node to GND.

Run the project.

Use the UART command line interpretor using the following configuration (It could be on a different COM port):

![image](https://user-images.githubusercontent.com/81984166/161912221-563be975-3957-401a-97ca-3bd9fc3bd1aa.png)

On running the project the following, line appears on the screen.

![image](https://user-images.githubusercontent.com/81984166/166408324-7e3a9f34-e9d4-4146-9be3-1530b3ad1599.png)

The supported commands are as follows:

![image](https://user-images.githubusercontent.com/81984166/166408369-b79f30d2-29a6-4296-becf-88922a018938.png)

# Key Features

The UART is non-blocking and highly responsive, where the user can enter commands even while the tones are playing.


Please refer to the project proposal document for further details.

The DSP effect of an echo is introduced using the echo command, where the echo sound effect is created for a tone with diminishing volume.

The "play" command can take multiple tones at once, including the duration of each tone in seconds.

# Error Handling

Error handling is done based on each command. For example, the play command does not accept more than 20 tones at once and it indicates the user the same.

Please refer to the 

"echo" command needs exactly two arguments, including the command name.
