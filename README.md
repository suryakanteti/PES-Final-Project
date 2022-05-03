# PES-Final-Project

Final Project of Principles of Embedded Software, ECEN 5813, Spring 2022

Developed by Surya Kanteti

# Project Description
The project contains the implementation of "ARMoinca" a command-line processor based musical instrument which runs on the KL25Z-FRDM board. It can take commands from the user and play a musical tone according to it.

# Files in the project:
Source files: ARMonica.c, AudioOut.c, cbfifo.c, CommandProcessor.c, fp_trig.c, UART_IO.c

Header files: AudioOut.h, cbfifo.h, CommandProcessor.h, fp_trig.h, UART_IO.h

# How to Run

Build the project in either Debug or Release mode. Indebug, extra tests will be run to verify authenticity of the modules.

Run the project.

On running the project the following, line appears on the screen.

![image](https://user-images.githubusercontent.com/81984166/166408324-7e3a9f34-e9d4-4146-9be3-1530b3ad1599.png)

The supported commands are as follows:

![image](https://user-images.githubusercontent.com/81984166/166408369-b79f30d2-29a6-4296-becf-88922a018938.png)

# Key Features

The UART is non-blocking and highly responsive, where the user can enter commands even while the tones are playing.

The DSP effect of an echo is introduced using the echo command, where the echo sound effect is created for a tone with diminishing volume.

# Error Handling

Error handling is done based on each command. For example, the play command does not accept more than 20 tones at once and it indicates the user the same.

"echo" command needs exactly two arguments, including the command name.
