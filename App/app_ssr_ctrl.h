#ifndef __APP_SSR_CTRL_H__
#define __APP_SSR_CTRL_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/
/* define                                                                     */
/******************************************************************************/


typedef enum
{
    MODE_STOP = 0,
    MODE_PID,
    MODE_ATUNE_100,
    MODE_ATUNE_40,
    MODE_MANUAL,
    MODE_PC,
    
    MODE_INVALID, // Do not Change.
}CONTROL_MODE_LIST;



// Error Code
#define ERR_EEPROM_CHKSUM	1
#define ERR_K_VALUE			2
#define ERR_OVER_TEMPLIMIT	3
#define ERR_AT_FAIL			4
#define ERR_CHANNEL_NUM		5



/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


extern const char* str_temp_ch[];
extern const char* str_ctrl_mode[];


extern uint8_t _ctrl_Mode[];
extern int32_t pv_integer[];
extern int32_t pv_fraction[];
extern uint8_t curr_ssr_status[];
extern int32_t pwm_percent_Integer[];
extern int max31865_fault[];

extern uint8_t alarm_status[];

extern float _kp[];
extern float _ki[];
extern int32_t _kp_integer[];
extern float _kd[];
extern int32_t _ki_integer[];
extern int32_t _kd_integer[];
extern float _outputUpWeight[];
extern float _outputDownWeight[];
extern int32_t _outputUpWeight_integer[];
extern int32_t _outputDownWeight_integer[];
extern int32_t _kp_fraction[];
extern int32_t _ki_fraction[];
extern int32_t _kd_fraction[];
extern int32_t _outputUpWeight_fraction[];
extern int32_t _outputDownWeight_fraction[];


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

void App_Heater_All_Off(void);
void App_Set_SSR_En(uint8_t en);
void App_Set_SSR_lock(uint8_t ch,uint8_t en);

void App_SSR_Ctrl(uint8_t ch, uint8_t onoff);

void App_Alarm_Ctrl(uint8_t ch, uint8_t onoff);
void App_Set_Ctrl_Mode(uint8_t ch,uint8_t val);
void App_Set_Ctrl_Mode_All(uint8_t val);

void App_Set_SP(uint8_t ch, uint8_t mode, uint8_t val);

void App_Run_Heater(uint8_t ch, float *temperature, float *setPoint, unsigned long Now_ms);

void App_Set_Data_Display_Flag(uint8_t val,uint8_t ch);

void App_Temp_Display(void);
void App_Temp_Alarm_Config(void);

void App_Temp_Status(void);

void App_Temp_Read(uint8_t ch);

void App_SSR_Ctrl_Init(void);

void App_Heater_Ctrl_Proc(void);

void App_Temp_Alarm_Check(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __APP_SSR_CTRL_H__

