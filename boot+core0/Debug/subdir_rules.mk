################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.2/bin/cl6x" -mv6600 --abi=eabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.2/include" --include_path="D:/workspace/6678/boot-1115/core0" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --include_path="D:/workspace/6678/boot-1115/core0/include" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --define=_PLATFORM_LITE_ --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:data=far --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nand_flash.obj: ../nand_flash.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.2/bin/cl6x" -mv6600 --abi=eabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.2/include" --include_path="D:/workspace/6678/boot-1115/core0" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --include_path="D:/workspace/6678/boot-1115/core0/include" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --define=_PLATFORM_LITE_ --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:data=far --preproc_with_compile --preproc_dependency="nand_flash.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

xmc_setup.obj: ../xmc_setup.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.2/bin/cl6x" -mv6600 --abi=eabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.2/include" --include_path="D:/workspace/6678/boot-1115/core0" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --include_path="D:/workspace/6678/boot-1115/core0/include" --include_path="C:/ti/pdk_C6678_1_1_2_6/packages" --define=_PLATFORM_LITE_ --display_error_number --diag_warning=225 --diag_wrap=off --mem_model:data=far --preproc_with_compile --preproc_dependency="xmc_setup.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


