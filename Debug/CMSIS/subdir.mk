################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL25Z4.c 

OBJS += \
./CMSIS/system_MKL25Z4.o 

C_DEPS += \
./CMSIS/system_MKL25Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\board" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\source" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\startup" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\CMSIS" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\drivers" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\utilities" -I"C:\Users\Teju\Documents\MCUXpressoIDE_11.5.0_7232\workspace\ARMonica\include" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

