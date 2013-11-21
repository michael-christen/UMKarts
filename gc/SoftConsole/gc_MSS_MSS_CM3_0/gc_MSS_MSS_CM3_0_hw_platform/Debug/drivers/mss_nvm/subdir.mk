################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/mss_nvm/mss_nvm.c 

OBJS += \
./drivers/mss_nvm/mss_nvm.o 

C_DEPS += \
./drivers/mss_nvm/mss_nvm.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/mss_nvm/%.o: ../drivers/mss_nvm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -DACTEL_STDIO_THRU_UART -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\CMSIS -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_ace -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers_config -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\drivers_config\mss_ace -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\hal -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IC:\Users\mjschul\Documents\project\gc\SoftConsole\gc_MSS_MSS_CM3_0\gc_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


