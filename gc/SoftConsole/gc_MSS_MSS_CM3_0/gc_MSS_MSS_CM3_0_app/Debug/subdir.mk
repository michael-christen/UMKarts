################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../controller.c \
../lcd.c \
../main.c \
../mytimer.c 

OBJS += \
./controller.o \
./lcd.o \
./main.o \
./mytimer.o 

C_DEPS += \
./controller.d \
./lcd.d \
./main.d \
./mytimer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\CMSIS -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_ace -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers_config -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers_config\mss_ace -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\hal -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IC:\Users\knekritz\UMKarts\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


