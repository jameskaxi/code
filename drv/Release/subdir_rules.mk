################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
board_init.obj: ../board_init.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="board_init.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

edma.obj: ../edma.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="edma.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ipc.obj: ../ipc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="ipc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

osal.obj: ../osal.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="osal.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

resourcemgr.obj: ../resourcemgr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/bin/cl6x" -mv6600 -O2 --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/ndk_2_25_01_11/packages" --include_path="C:/ti/bios_6_46_05_55/packages" --include_path="D:/workspace/6678/v7-12-08/drv" --include_path="C:/ti/xdctools_3_32_02_25_core/packages" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c6000_8.1.4/include" --define=_INLINE --define=_INCLUDE_NIMU_CODE --diag_wrap=off --display_error_number --diag_warning=225 --mem_model:const=far --mem_model:data=far --preproc_with_compile --preproc_dependency="resourcemgr.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


