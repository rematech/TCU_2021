#ifndef __APP_H__
#define __APP_H__

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

// #define _TEMP_USE_HAL_TIMER_    1

#define TEMP_READ_TIME_MS       TICK_MS(50)

// FLASH
#define FLASH_PB_SIZE           0x100

#define FLASH_PB_START          0x3F800
#define FLASH_PB_END            FLASH_PB_START + 0x400



typedef enum
{
    TEMP_CH_1 = 0, // PAD
    TEMP_CH_2,     // CAT
    TEMP_CH_3,     // RSV
    
    TEMP_CH_MAX, // Do not Change.

}TEMP_CH_LIST;


typedef struct
{   
    uint8_t inf;
}__attribute__((packed)) DEV_INF_T;


typedef struct
{   
    uint8_t reserved[4];
    DEV_INF_T dev;
}__attribute__((packed)) FLASH_INFO_T;


typedef struct 
{
    uint8_t ctrl_mode;
    uint8_t pv;
    uint8_t ssr_on;
    uint8_t pwm;
    uint8_t alarm;
}__attribute__((packed)) TEMP_STATUS_T;

typedef struct 
{
    uint8_t ctrl_mode;
    uint8_t sp;
}__attribute__((packed)) SET_SP_T;

typedef struct 
{
    uint8_t lo_en;
    uint8_t lo_val;
    uint8_t hi_en;
    uint8_t hi_val;
}__attribute__((packed)) SET_ALARM_T;

typedef struct
{   
    TEMP_STATUS_T   status[TEMP_CH_MAX];
    SET_SP_T        set[TEMP_CH_MAX];
    SET_ALARM_T     alarm;
}__attribute__((packed)) DEV_T;

typedef enum
{
    HCS_BOOT_MSG  = 0,
    HCS_NORMAL,

    
    HCS_MAX
}HOST_COMM_STATE;

#define _TEMP_LPF_COUNT			20
typedef struct
{   
	bool bMax31865Print		:1;
	bool bCheckIFStatePrint	:1;
	bool bGetTCUData		:1;
	
	struct
	{
    	uint16_t iRtdRegisterValue[_TEMP_LPF_COUNT];
		int iTemperature[_TEMP_LPF_COUNT];
		int iLessThanZeroPart[_TEMP_LPF_COUNT];

		int iIndex;
	} tempdata[TEMP_CH_MAX];

	uint16_t iReadCount;
	uint8_t iExcuteCount;

	uint16_t iLPFMax31865Reg[TEMP_CH_MAX];
	int iLPFTemperature[TEMP_CH_MAX];
	float fTempFirfilter[TEMP_CH_MAX]; 

	//for analog device cal
	uint16_t iRTD[TEMP_CH_MAX];
	float fAnalogDeviceCal[TEMP_CH_MAX];

	uint8_t iMax31865Func_temp[TEMP_CH_MAX];
	uint8_t iMax31865Func_temp_lesszero[TEMP_CH_MAX];

}__attribute__((packed)) GLOBAL_DATA;
extern GLOBAL_DATA g_Data;


/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/
extern DEV_T dev;

/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

void App_Sys_Logo(void);
void App_System_Info(void);
void App_System_status_show(void);
void App_NV_Data_Save(void);

void App_Init(void);
void App_Init_GData();


void App_System(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __APP_H__

