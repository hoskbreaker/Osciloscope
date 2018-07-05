################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
commands.obj: ../commands.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="commands.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

configADC.obj: ../configADC.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="configADC.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

crc.obj: ../crc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="crc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

protocol.obj: ../protocol.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="protocol.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

remote.obj: ../remote.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="remote.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_dev_serial.obj: ../usb_dev_serial.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="usb_dev_serial.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_serial_structs.obj: ../usb_serial_structs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/include" --include_path="C:/Users/carlos/Desktop/superproyecto/base-practica_TIVA-v2/FreeRTOS/Source/portable/CCS/ARM_CM4F" -g --gcc --define=ccs="ccs" --define=TARGET_IS_BLIZZARD_RB1 --define=UART_BUFFERED --define=WANT_CMDLINE_HISTORY --define=WANT_FREERTOS_SUPPORT --define=PART_TM4C123GH6PM --diag_wrap=off --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="usb_serial_structs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


