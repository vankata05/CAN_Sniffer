################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/IVI6SF/GitRepos/CAN_Sniffer/Seeed_LoRa_E5/Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.c \
C:/Users/IVI6SF/GitRepos/CAN_Sniffer/Seeed_LoRa_E5/Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.c 

OBJS += \
./Drivers/BSP/stm32wlxx_LoRa_E5_mini.o \
./Drivers/BSP/stm32wlxx_LoRa_E5_mini_radio.o 

C_DEPS += \
./Drivers/BSP/stm32wlxx_LoRa_E5_mini.d \
./Drivers/BSP/stm32wlxx_LoRa_E5_mini_radio.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/stm32wlxx_LoRa_E5_mini.o: C:/Users/IVI6SF/GitRepos/CAN_Sniffer/Seeed_LoRa_E5/Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -DDEBUG -c -I../Core/Inc -I../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../Drivers/CMSIS/Include -I../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler -I../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../../../../../Middlewares/Third_Party/SubGHz_Phy -I../../../../../Drivers/BSP/STM32WLxx_LoRa_E5_mini -I../../../../../Utilities/lpm/tiny_lpm -I../../../../../Utilities/misc -I../../../../../Utilities/sequencer -I../../../../../Utilities/timer -I../../../../../Utilities/trace/adv_trace -I../../../../../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../../../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../../../Projects/Applications/FreeRTOS/FreeRTOS_LoRaWAN_AT/LoRaWAN/App -I../../../../../Projects/Applications/FreeRTOS/FreeRTOS_LoRaWAN_AT/LoRaWAN/Target -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/BSP/stm32wlxx_LoRa_E5_mini_radio.o: C:/Users/IVI6SF/GitRepos/CAN_Sniffer/Seeed_LoRa_E5/Drivers/BSP/STM32WLxx_LoRa_E5_mini/stm32wlxx_LoRa_E5_mini_radio.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -DDEBUG -c -I../Core/Inc -I../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../Drivers/CMSIS/Include -I../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler -I../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../../../../../Middlewares/Third_Party/SubGHz_Phy -I../../../../../Drivers/BSP/STM32WLxx_LoRa_E5_mini -I../../../../../Utilities/lpm/tiny_lpm -I../../../../../Utilities/misc -I../../../../../Utilities/sequencer -I../../../../../Utilities/timer -I../../../../../Utilities/trace/adv_trace -I../../../../../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler/Packages -I../../../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../../../Projects/Applications/FreeRTOS/FreeRTOS_LoRaWAN_AT/LoRaWAN/App -I../../../../../Projects/Applications/FreeRTOS/FreeRTOS_LoRaWAN_AT/LoRaWAN/Target -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP

clean-Drivers-2f-BSP:
	-$(RM) ./Drivers/BSP/stm32wlxx_LoRa_E5_mini.cyclo ./Drivers/BSP/stm32wlxx_LoRa_E5_mini.d ./Drivers/BSP/stm32wlxx_LoRa_E5_mini.o ./Drivers/BSP/stm32wlxx_LoRa_E5_mini.su ./Drivers/BSP/stm32wlxx_LoRa_E5_mini_radio.cyclo ./Drivers/BSP/stm32wlxx_LoRa_E5_mini_radio.d ./Drivers/BSP/stm32wlxx_LoRa_E5_mini_radio.o ./Drivers/BSP/stm32wlxx_LoRa_E5_mini_radio.su

.PHONY: clean-Drivers-2f-BSP
