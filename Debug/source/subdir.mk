################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ARMonica.c \
../source/AudioOut.c \
../source/CommandProcessor.c \
../source/UART_IO.c \
../source/cbfifo.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sysclock.c 

OBJS += \
./source/ARMonica.o \
./source/AudioOut.o \
./source/CommandProcessor.o \
./source/UART_IO.o \
./source/cbfifo.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sysclock.o 

C_DEPS += \
./source/ARMonica.d \
./source/AudioOut.d \
./source/CommandProcessor.d \
./source/UART_IO.d \
./source/cbfifo.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sysclock.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\board" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\source" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\startup" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\CMSIS" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\drivers" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\utilities" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\include" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


