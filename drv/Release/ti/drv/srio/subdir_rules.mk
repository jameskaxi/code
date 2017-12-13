################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ti/drv/srio/listlib.obj: ../ti/drv/srio/listlib.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-11-27/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-11-27/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --diag_warning=225 --display_error_number --mem_model:data=far --mem_model:const=far --preproc_with_compile --preproc_dependency="ti/drv/srio/listlib.d_raw" --obj_directory="ti/drv/srio" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ti/drv/srio/srio_drv.obj: ../ti/drv/srio/srio_drv.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-11-27/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-11-27/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --diag_warning=225 --display_error_number --mem_model:data=far --mem_model:const=far --preproc_with_compile --preproc_dependency="ti/drv/srio/srio_drv.d_raw" --obj_directory="ti/drv/srio" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


