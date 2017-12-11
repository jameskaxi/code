################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ti/drv/pa/pa.obj: ../ti/drv/pa/pa.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="ti/drv/pa/pa.d_raw" --obj_directory="ti/drv/pa" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ti/drv/pa/painit.obj: ../ti/drv/pa/painit.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="ti/drv/pa/painit.d_raw" --obj_directory="ti/drv/pa" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


