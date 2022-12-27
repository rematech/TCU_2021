################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./tm4c1230h6pm_startup_ccs.obj" \
"./Api/api_autotune.obj" \
"./Api/api_cmd.obj" \
"./Api/api_filter_fir.obj" \
"./Api/api_msg.obj" \
"./Api/api_pid.obj" \
"./Api/api_systick.obj" \
"./Api/api_util.obj" \
"./App/app.obj" \
"./App/app_console.obj" \
"./App/app_gpio_config.obj" \
"./App/app_host_if.obj" \
"./App/app_ssr_ctrl.obj" \
"./App/app_test_func.obj" \
"./App/max31865.obj" \
"./Hal/hal_delay.obj" \
"./Hal/hal_eeprom_pb.obj" \
"./Hal/hal_flash_pb.obj" \
"./Hal/hal_gpio.obj" \
"./Hal/hal_i2c.obj" \
"./Hal/hal_spi.obj" \
"./Hal/hal_timer.obj" \
"./Hal/hal_uart.obj" \
"./utils/uartstdio.obj" \
"../tm4c1230h6pm.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/ti/tivaware_c_series_2_1_4_178/driverlib/ccs/Debug/driverlib.lib" \
-l"C:/ti/tivaware_c_series_2_1_4_178/grlib/ccs/Debug/grlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Api/subdir_vars.mk
-include App/subdir_vars.mk
-include Hal/subdir_vars.mk
-include utils/subdir_vars.mk
-include subdir_rules.mk
-include Api/subdir_rules.mk
-include App/subdir_rules.mk
-include Hal/subdir_rules.mk
-include utils/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
FR2V2_TCU.out \

EXE_OUTPUTS__QUOTED += \
"FR2V2_TCU.out" \

BIN_OUTPUTS += \
FR2V2_TCU.bin \

BIN_OUTPUTS__QUOTED += \
"FR2V2_TCU.bin" \


# All Target
all: FR2V2_TCU.out

# Tool invocations
FR2V2_TCU.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=0 --define=ccs="ccs" --define=DEBUG --define=TARGET_IS_TM4C123_RB1 --define=PART_TM4C1230H6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual -z -m"system.map" --heap_size=0 --stack_size=512 -i"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/lib" -i"C:/ti/tivaware_c_series_2_1_4_178" -i"C:/ti/tivaware_c_series_2_1_4_178/utils" -i"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="FR2V2_TCU_linkInfo.xml" --rom_model -o "FR2V2_TCU.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

FR2V2_TCU.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armhex"  -o "FR2V2_TCU.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "tm4c1230h6pm_startup_ccs.obj" "Api\api_autotune.obj" "Api\api_cmd.obj" "Api\api_filter_fir.obj" "Api\api_msg.obj" "Api\api_pid.obj" "Api\api_systick.obj" "Api\api_util.obj" "App\app.obj" "App\app_console.obj" "App\app_gpio_config.obj" "App\app_host_if.obj" "App\app_ssr_ctrl.obj" "App\app_test_func.obj" "App\max31865.obj" "Hal\hal_delay.obj" "Hal\hal_eeprom_pb.obj" "Hal\hal_flash_pb.obj" "Hal\hal_gpio.obj" "Hal\hal_i2c.obj" "Hal\hal_spi.obj" "Hal\hal_timer.obj" "Hal\hal_uart.obj" "utils\uartstdio.obj" 
	-$(RM) "main.d" "tm4c1230h6pm_startup_ccs.d" "Api\api_autotune.d" "Api\api_cmd.d" "Api\api_filter_fir.d" "Api\api_msg.d" "Api\api_pid.d" "Api\api_systick.d" "Api\api_util.d" "App\app.d" "App\app_console.d" "App\app_gpio_config.d" "App\app_host_if.d" "App\app_ssr_ctrl.d" "App\app_test_func.d" "App\max31865.d" "Hal\hal_delay.d" "Hal\hal_eeprom_pb.d" "Hal\hal_flash_pb.d" "Hal\hal_gpio.d" "Hal\hal_i2c.d" "Hal\hal_spi.d" "Hal\hal_timer.d" "Hal\hal_uart.d" "utils\uartstdio.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"C:/ti/ccs920/ccs/utils/tiobj2bin/tiobj2bin" "FR2V2_TCU.out" "FR2V2_TCU.bin" "C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armofd" "C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armhex" "C:/ti/ccs920/ccs/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

