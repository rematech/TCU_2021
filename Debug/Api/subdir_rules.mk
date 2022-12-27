################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Api/%.obj: ../Api/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --include_path="C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --include_path="C:/ti/tivaware_c_series_2_1_4_178/examples/boards/dk-tm4c123g" --include_path="C:/ti/tivaware_c_series_2_1_4_178" --include_path="C:/ti/tivaware_c_series_2_1_4_178/utils" --include_path="D:/Workspace_TI/P_FFS2000_R2V2/FR2V2_TCU" --include_path="D:/Workspace_TI/P_FFS2000_R2V2/FR2V2_TCU/Hal" --include_path="D:/Workspace_TI/P_FFS2000_R2V2/FR2V2_TCU/Api" --include_path="D:/Workspace_TI/P_FFS2000_R2V2/FR2V2_TCU/App" --include_path="D:/Workspace_TI/P_FFS2000_R2V2/FR2V2_TCU/Drivers" --define=ccs="ccs" --define=DEBUG --define=TARGET_IS_TM4C123_RB1 --define=PART_TM4C1230H6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="Api/$(basename $(<F)).d_raw" --obj_directory="Api" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


