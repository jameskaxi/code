################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/c6000_7.4.2

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./src/platform.obj" \
"./src/evmc66x_uart.obj" \
"./src/evmc66x_spi.obj" \
"./src/evmc66x_nor.obj" \
"./src/evmc66x_i2c_eeprom.obj" \
"./src/evmc66x_gpio.obj" \
"./src/evmc66x_fpga.obj" \
"./src/evmc6678_phy.obj" \
"./src/evmc6678.obj" \
"./src/evm66x_nand.obj" \
"./src/evm66x_i2c.obj" \
"./src/csl_gpioGetBaseAddress.obj" \
"./xmc_setup.obj" \
"./nand_flash.obj" \
"./main.obj" \
-l"libc.a" \
"../link.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: core0.out

# Tool invocations
core0.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.2/bin/cl6x" -mv6600 --abi=eabi -g --define=_PLATFORM_LITE_ --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:data=far -z -m"core0.map" -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.2/lib" -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.2/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="core0_linkInfo.xml" --rom_model -o "core0.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C6000_EXECUTABLE_OUTPUTS__QUOTED) "core0.out"
	-$(RM) "main.pp" "nand_flash.pp" "xmc_setup.pp" "src\csl_gpioGetBaseAddress.pp" "src\evm66x_i2c.pp" "src\evm66x_nand.pp" "src\evmc6678.pp" "src\evmc6678_phy.pp" "src\evmc66x_fpga.pp" "src\evmc66x_gpio.pp" "src\evmc66x_i2c_eeprom.pp" "src\evmc66x_nor.pp" "src\evmc66x_spi.pp" "src\evmc66x_uart.pp" "src\platform.pp" 
	-$(RM) "main.obj" "nand_flash.obj" "xmc_setup.obj" "src\csl_gpioGetBaseAddress.obj" "src\evm66x_i2c.obj" "src\evm66x_nand.obj" "src\evmc6678.obj" "src\evmc6678_phy.obj" "src\evmc66x_fpga.obj" "src\evmc66x_gpio.obj" "src\evmc66x_i2c_eeprom.obj" "src\evmc66x_nor.obj" "src\evmc66x_spi.obj" "src\evmc66x_uart.obj" "src\platform.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
