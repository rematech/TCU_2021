#include <math.h>

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"

#include "utils/ustdlib.h"
#include "utils/uartstdio.h"

#include "driverlib/pin_map.h"

#include "defines.h"
#include "hal_gpio.h"
#include "api_systick.h"
#include "api_autotune.h"
#include "api_pid.h"
#include "api_filter_fir.h"
#include "max31865.h"

#include "app.h"
#include "app_console.h"
#include "app_gpio_config.h"

#include "app_ssr_ctrl.h"

#define OFF 0
#define ON  1

#define SP_CORRECTION   0.5
#define SP_BASE_VAL     55.0
#define SP_INIT_VAL     (SP_BASE_VAL+SP_CORRECTION)

static volatile uint32_t SSROnCNT = 0;

uint8_t curr_ssr_status[TEMP_CH_MAX] = { OFF, OFF, OFF };

float _setPoint[TEMP_CH_MAX] = { SP_INIT_VAL, SP_INIT_VAL, SP_INIT_VAL};   // System(Heater) 嶺뚮ㅄ維싷쭗占� �뜝�럩沅붷뜝�럥利� �뤆�룊�삕
uint8_t _ctrl_Mode[TEMP_CH_MAX] = { MODE_STOP, MODE_STOP, MODE_STOP };   // 0 : Stop PID,  1: Pid On,  2, 3: AutoTuning On  4: PC control
uint8_t _ctrl_Mode_backup[TEMP_CH_MAX];
int32_t SSROnPeriod[TEMP_CH_MAX] = { 0, 0, 0 };

float _kp[TEMP_CH_MAX]={0,};
float _ki[TEMP_CH_MAX]={0,};
float _kd[TEMP_CH_MAX]={0,};

uint8_t temp_err_cnt = 0;

float _ctrlOutput[TEMP_CH_MAX]={0,}; // AutoTuning & PID Controller output
float g_fPWMPercentage[TEMP_CH_MAX] = { 100.0, 100.0, 100.0 }; //{ 50.7, 45.2, 33.4 };
int32_t pwm_percent_Integer[TEMP_CH_MAX] = { 0, 0, 0 };
int32_t pwm_percent_fraction[TEMP_CH_MAX] = { 0, 0, 0 };

// PID Control : App_Run_Heater
int _errorCode[TEMP_CH_MAX]={0,};	// 0 : no error
int _prevErrorCode[TEMP_CH_MAX]={0,};

float _outputUpWeight[TEMP_CH_MAX]={0,};
float _outputDownWeight[TEMP_CH_MAX]={0,};
float _atuneOutputStep[TEMP_CH_MAX]={0,};

float temp_Ambient[TEMP_CH_MAX];
float curr_Ambient[TEMP_CH_MAX];

int32_t pv_integer[TEMP_CH_MAX];
int32_t pv_fraction[TEMP_CH_MAX];
int max31865_fault[TEMP_CH_MAX] = {0,};

static uint32_t fail_tick[TEMP_CH_MAX];

uint32_t alarm_cnt_lo[TEMP_CH_MAX];
uint32_t alarm_cnt_hi[TEMP_CH_MAX];
uint8_t alarm_status[TEMP_CH_MAX];

uint8_t temp_ctrl_ready = 0;
uint8_t flag_data_display = 0;
uint8_t flag_data_display_ch = 0;

volatile uint8_t ssr_en = 1;
volatile uint8_t ssr_lock[TEMP_CH_MAX] ={0,};


const char* str_temp_ch[TEMP_CH_MAX]=
{
    "CH0", 
    "CH1",
    "CH2",
};

const char* str_ctrl_mode[]=
{
    "STOP", 
    "PID",
    "AT100",
    "AT40",
    "MAN",
    "Unknown",
};

void App_Set_Ctrl_Mode(uint8_t ch, uint8_t val)
{
    if(ch < TEMP_CH_MAX)
    {
        _ctrl_Mode[ch] = val;
    }
    else
    {
        dmsg(DL_ERR,"TEMP Setting Err.\r\n");
    }
}

void App_Set_Ctrl_Mode_All(uint8_t val)
{
    uint8_t ch;
    for(ch = 0 ; ch < TEMP_CH_MAX ; ch++)
    {
        _ctrl_Mode[ch] = val;
    }
}

void App_Heater_All_Off(void)
{
    uint8_t ch;
    for(ch = 0; ch < TEMP_CH_MAX; ch++)
        App_SSR_Ctrl(ch,OFF);
}

void App_SSR_Ctrl(uint8_t ch, uint8_t onoff)
{
    uint8_t gpo_lvl;

    if(!ssr_en) onoff = OFF;
    if(ssr_lock[ch]) onoff = OFF;
    
    if(onoff == OFF) gpo_lvl = GPIO_LVL_HI;
    else             gpo_lvl = GPIO_LVL_LO;


    switch(ch)
    {
        case TEMP_CH_1 :
            Hal_Gpo_Write(GPO_SSR1,gpo_lvl);
            break;
        
        case TEMP_CH_2 :
            Hal_Gpo_Write(GPO_SSR2,gpo_lvl);
            break;
        
        case TEMP_CH_3 :
            Hal_Gpo_Write(GPO_SSR3,gpo_lvl);
            break;
        
        default:
            break;
        
    }

	curr_ssr_status[ch] = onoff;
    
    // App_Alarm_Ctrl(ch,onoff);
}

void App_Alarm_Ctrl(uint8_t ch, uint8_t onoff)
{
    uint8_t gpo_lvl;

    if(onoff == OFF) gpo_lvl = GPIO_LVL_LO;
    else             gpo_lvl = GPIO_LVL_HI;
    
    switch(ch)
    {
        case TEMP_CH_1 : 
            Hal_Gpo_Write(GPO_TEMP_ALARM3,gpo_lvl);
            break;
        
        case TEMP_CH_2 : 
            Hal_Gpo_Write(GPO_TEMP_ALARM2,gpo_lvl);
            break;
        
        case TEMP_CH_3 : 
            Hal_Gpo_Write(GPO_TEMP_ALARM1,gpo_lvl);
            break;

        default : 
            break;
    }
}

uint8_t App_Check_K_Valid(uint8_t ch)
{
    if(_kp[ch] < Kx_MIN_VALUE || _kp[ch]> Kp_MAX_VALUE )
       return 0;

    if(_ki[ch] < Kx_MIN_VALUE || _ki[ch]> Ki_MAX_VALUE )
       return 0;

    if(_kd[ch] < Kx_MIN_VALUE || _kd[ch]> Kd_MAX_VALUE )
       return 0;

    return 1;
}

void App_KValues_Init(int ch)
{
	_kp[ch] = 0.0;
	_ki[ch] = 0.0;
	_kd[ch] = 0.0;
}

void App_SSR_PWM_Set(int ch, float *percentage)
{
	g_fPWMPercentage[ch] = *percentage;

	SSROnPeriod[ch] = (int)((*percentage)*10);
	//SSROnPeriod[ch] = (int)((*percentage)*loop_count/100.0);
	//SSROnPeriod[ch] = (int)(*percentage*1000/100.0);

	pwm_percent_Integer[ch] = (int32_t)g_fPWMPercentage[ch];
	pwm_percent_fraction[ch] = (int32_t)(g_fPWMPercentage[ch]*10.0f);
	pwm_percent_fraction[ch] = pwm_percent_fraction[ch] - (pwm_percent_Integer[ch]*10);
	if ( pwm_percent_Integer[ch] < 0 )
	{
		pwm_percent_fraction[ch] *= -1;
	}

#if 0
	//if ( _ctrl_Mode[ch] == MODE_MANUAL )
		UARTprintf("App_SSR_PWM_Set [%d(%c)]  %3d (%d)\r\n",
				ch, ch == CAT_CHANNEL ? 'C' : 'P',
				pwm_percent_Integer[ch], SSROnPeriod[ch]);
#endif

	return;
}


void App_Ctrl_Param_Init(int ch,uint8_t mode)
{
    float outLimitMin = 3.0;
	float outLimitMax = 100.0;
    
    // init Autotuning & PID
    _atuneOutputStep[ch] = ATUNE_OUTPUT_STEP;
    ATuneInit(ATUNE_CTRL_TYPE, ATUNE_SAMPLING_TIME, ATUNE_MAX_STEPUP_CNT, ch);   // controlType, outputStart, outputStep, noiseBand, samplingTime, lookBackSec, ch
    PidInit(PID_SAMPLING_TIME, &outLimitMin, &outLimitMax, ch); // sampleTime, outLimitMin, outLimitMax, ch

    _ctrl_Mode[ch] = mode;// MODE_ATUNE_100; //info_curr.pid_mode[ch];
    //_setPoint[ch] = info_curr.set_point[ch]; //55;
    
    // Fir Filter Init
    InitFir(ch);

    // init output
    _ctrlOutput[ch] = 0.0;

    // error reset
    _errorCode[ch] = 0.0;
    _prevErrorCode[ch] = 0.0;
    

}

void App_Set_SSR_lock(uint8_t ch,uint8_t en)
{
    uint8_t i;
    if(ch == TEMP_CH_MAX)
    {
        for(i=0 ; i<TEMP_CH_MAX ; i++)
        {
            ssr_lock[i] = en;
        }
    }
    else
        ssr_lock[ch] = en;
}



void App_Set_SSR_En(uint8_t en)
{
    ssr_en = en;
}


void App_Set_SP(uint8_t ch, uint8_t mode, uint8_t val)
{
    uint8_t i;

    if(ch > TEMP_CH_MAX) return;
    if(val >= 100) return;
    
    if(ch == TEMP_CH_MAX)
    {
        for(i=0 ; i<TEMP_CH_MAX ; i++)
        {
            _setPoint[i] = val;
            _setPoint[i] += SP_CORRECTION;
            App_Ctrl_Param_Init(i,mode);
            
        }
    }
    else
    {
        _setPoint[ch] = val;
        _setPoint[ch] += SP_CORRECTION;
        App_Ctrl_Param_Init(ch,mode);
    }
}

void App_Run_Heater(uint8_t ch, float *temperature, float *setPoint, unsigned long Now_ms)
{
	int i;//, index;
    int result = -1;

    //dmsg(DL_INF,"App_Run_Heater : ch %d : Current %d.%d  Limit %d\r\n", ch, pv_integer[ch], pv_fraction[ch], info_curr.set_point[ch]);


	// error check ->
	if(ch > (TEMP_CH_MAX))
	{
		 for(i = 0; i < TEMP_CH_MAX; i++)
		 {
            _ctrl_Mode[i]= MODE_STOP;
            _ctrlOutput[i] = 0.0;
            App_KValues_Init(i);
            App_SSR_PWM_Set(i, &_ctrlOutput[i]);
            App_SSR_Ctrl(ch,OFF);
            _prevErrorCode[i] = _errorCode[i];
            _errorCode[i] = ERR_CHANNEL_NUM;
		 }

		//dmsg(DL_ERR,"App_Run_Heater [%s] - Error Check.\r\n", str_temp_ch[ch]);
		return;
	}

	if((*temperature <= TEMPERATURE_LINIT_MIN) || (*temperature > TEMPERATURE_LIMIT_HI))
	{
        dmsg(DL_ERR,"%s : [%s] - Invalid Temp : %d \r\n", __func__,str_temp_ch[ch],*temperature);
        temp_err_cnt ++;
        if(temp_err_cnt > TEMP_ERR_CNT_MAX)
        {
            _ctrlOutput[ch] = 0.0;
            App_SSR_PWM_Set(ch, &_ctrlOutput[ch]);
            App_SSR_Ctrl(ch,OFF); // 20191121

            if(_ctrl_Mode[ch] == MODE_ATUNE_100 || _ctrl_Mode[ch] == MODE_ATUNE_40 )
            _ctrl_Mode[ch] = MODE_STOP;

            _prevErrorCode[ch] = _errorCode[ch];
            _errorCode[ch] = ERR_OVER_TEMPLIMIT;
        }
        return;
	}
    else
    {
        temp_err_cnt = 0;
    }
	// error check <-

    ///////////////////////////////////////////
    // AutoTuning & PID Controller
    switch(_ctrl_Mode[ch])
    {
        // Stop
        case MODE_STOP:
            _ctrlOutput[ch] = 0.0;
            App_SSR_Ctrl(ch,OFF); // 20191121
            result = 1;

            break;

        // Pid On
        case MODE_PID:

            if(App_Check_K_Valid(ch) == 0) 
            {
                _ctrl_Mode[ch] = MODE_STOP;
                App_KValues_Init(ch);
                _ctrlOutput[ch] = 0.0;
                App_SSR_PWM_Set(ch, &_ctrlOutput[ch]);
                App_SSR_Ctrl(ch,OFF); // 20191121

                _prevErrorCode[ch] = _errorCode[ch];
                _errorCode[ch] = ERR_K_VALUE;

                dmsg(DL_ERR,"%s : MODE_PID : [%s] invalid K. => MODE_STOP.\r\n",__func__, str_temp_ch[ch]);

                return;
            }

            // PID Operation.
            result = PidCompute(temperature, &_setPoint[ch], Now_ms, &(_ctrlOutput[ch]), ch);

            break;

        // AutoTuning On
        case MODE_ATUNE_100:
        case MODE_ATUNE_40:

            result = ATuneRun(&_setPoint[ch], temperature, &(_ctrlOutput[ch]), Now_ms, ch);

            //dmsg(DL_INF,"App_Run_Heater - Atune : [%d] result %d\r\n", ch, result);

            // AutoTuning finished
            if (result == 1)
            {
            	dmsg(DL_INF,"%s : [%s] AutoTuning finished.\r\n",__func__, str_temp_ch[ch]);

                _ctrl_Mode[ch] = MODE_PID; // PID on
                _ctrlOutput[ch] = 0; // 출력을 일단 0으로 
                App_SSR_Ctrl(ch,OFF); // 20191121

                ATuneGetKp(ch, &_kp[ch]);
                ATuneGetKi(ch, &_ki[ch]);
                ATuneGetKd(ch, &_kd[ch]);
                ATuneGetOutputWeight(&(_outputUpWeight[ch]), &(_outputDownWeight[ch]), ch);

                // PID setting
                PidSetTunings(&_kp[ch], &_ki[ch], &_kd[ch], &_outputUpWeight[ch], &_outputDownWeight[ch], ch);
            }

            break;

#if ENABLE_REMOTE_CONTROL == 1
        // Controled by PC
        case MODE_PC:

            result = 1;
            break;
#endif

        case MODE_MANUAL:
        	result = 1;
        	break;

		default:
			result = -2;
			break;
    }


    // Autotune Fail시 Stop Mode변경 및 Output = 0 [주기 길이가 500ms 이하일떄 Fail 발생]
    if(result == -2 )
    {

        _ctrl_Mode[ch]= MODE_STOP;
        _ctrlOutput[ch] = 0.0;
        App_KValues_Init(ch);
        App_SSR_PWM_Set(ch, &_ctrlOutput[ch]);
        App_SSR_Ctrl(ch,OFF); // 20191121

        _prevErrorCode[ch] = _errorCode[ch];
        _errorCode[ch] = ERR_AT_FAIL;

        App_Ctrl_Param_Init(ch,MODE_ATUNE_100);

        dmsg(DL_WRN,"%s : [%s] Autotune Fail, Retry \r\n",__func__, str_temp_ch[ch]);
    }


    // HeaterInput Setting
    if (result > -1 && !(_ctrl_Mode[ch] == MODE_STOP || _ctrl_Mode[ch] == MODE_MANUAL) )   
    //if (result > -1 )   
    {  // result == -1인 경우는 interval이 sampling time보다 작아서 output 업데이트 안한 경우
        ////////////////////////////
        // PWM Output Set
    	//if ( _ctrlOutput[ch] == 0. )

        //dmsg(DL_WRN,"%s : [%s] PWM output ... 0.0 \r\n",__func__, str_temp_ch[ch]);
        App_SSR_PWM_Set(ch, &_ctrlOutput[ch]);
        result = -1;
    }
}

void App_Heater_Ctrl(void)
{
    uint8_t ch;
    
    for (ch = 0; ch < TEMP_CH_MAX ; ch++ )
    {
        switch(_ctrl_Mode[ch])
        {
            case MODE_PID:
            case MODE_ATUNE_100:
            case MODE_ATUNE_40:
                ( (SSROnPeriod[ch] != 0) && (SSROnCNT < SSROnPeriod[ch]) ) ? App_SSR_Ctrl(ch,ON) : App_SSR_Ctrl(ch,OFF);

            break;

            case MODE_STOP:
                App_SSR_Ctrl(ch,OFF);

            break;
            case MODE_MANUAL:

            break;
            
            default:
                App_SSR_Ctrl(ch,OFF);
            break;

        }
    }
}

#define ALARM_TIME  TICK_MS(500)

void App_Temp_Alarm_Check(void)
{
    uint8_t ch;

    if(dev.alarm.lo_en == TRUE)
    {
        for(ch=0;ch<TEMP_CH_MAX;ch++)
        {
           if(pv_integer[ch] < dev.alarm.lo_val)
           {
               if(alarm_cnt_lo[ch] <= ALARM_TIME)
               {
                   alarm_cnt_lo[ch] += TEMP_READ_TIME_MS;
               }
           }
           else
           {
               alarm_cnt_lo[ch] = 0;
           }
        }
    }
    else
    {
        for(ch=0;ch<TEMP_CH_MAX;ch++)
        {
            alarm_cnt_lo[ch] = 0;
            alarm_status[ch] = OFF;
            App_Alarm_Ctrl(ch,OFF);
        }
    }
    
    if(dev.alarm.hi_en == TRUE)
    {
        for(ch=0;ch<TEMP_CH_MAX;ch++)
        {
            if(pv_integer[ch] > dev.alarm.hi_val)
            {
                if(alarm_cnt_hi[ch] <= ALARM_TIME)
                {
                    alarm_cnt_hi[ch] += TEMP_READ_TIME_MS;
                }
            }
            else
            {
                alarm_cnt_hi[ch] = 0;
            }
        }
    }
    else
    {
        for(ch=0;ch<TEMP_CH_MAX;ch++)
        {
            alarm_cnt_hi[ch] = 0;
            alarm_status[ch] = OFF;
            App_Alarm_Ctrl(ch,OFF);
        }
    }

    for(ch=0;ch<TEMP_CH_MAX;ch++)
    {
        if((alarm_cnt_lo[ch] >= ALARM_TIME) || (alarm_cnt_hi[ch] >= ALARM_TIME))
        {
           alarm_status[ch] = ON;
           App_Alarm_Ctrl(ch,ON);
        }
        else
        {
           alarm_status[ch] = OFF;
           App_Alarm_Ctrl(ch,OFF);
        }
    }
}

void App_Heater_Ctrl_Proc(void)
{
    uint8_t ch;

    SSROnCNT++;
    if( SSROnCNT >= 1000 )
    {
    	SSROnCNT = 0;
    }
    
    if ( temp_ctrl_ready == 1 )
    {
        App_Heater_Ctrl();
    }
    else
    {
        for (ch = 0; ch < TEMP_CH_MAX; ch++ )
        {
            App_SSR_Ctrl(ch,OFF);
        }
    }

}

void App_Set_Data_Display_Flag(uint8_t val,uint8_t ch)
{
    flag_data_display = val;
    flag_data_display_ch = ch;
}

void App_Temp_Display(void)
{
    uint8_t ch = 0;
    
    if(flag_data_display)
    {
        dmsg(DL_NON,"PV ");
        for(ch = 0; ch < TEMP_CH_MAX ; ch++)
        {
            dmsg(DL_NON,"| %s %d.%d PWM %03d ",str_temp_ch[ch],pv_integer[ch],pv_fraction[ch]/100, pwm_percent_Integer[ch]);
        }
        dmsg(DL_NON,"\r\n");
    }
}

void App_Temp_Alarm_Config(void)
{
    dmsg(DL_NON," Temp Alarm Config\r\n");
    dmsg(DL_NON," \t-> Lo En/Dis : %s, Val : %d \r\n",dev.alarm.lo_en?"Ena":"Dis",dev.alarm.lo_val);
    dmsg(DL_NON," \t-> Hi En/Dis : %s, Val : %d \r\n",dev.alarm.hi_en?"Ena":"Dis",dev.alarm.hi_val); 
    dmsg(DL_NON,"\r\n");
}


void App_Temp_Status(void)
{
    uint8_t ch=0;

    for(ch = 0; ch < TEMP_CH_MAX ; ch++)
    {
        dmsg(DL_NON," Temp Ch %d : %s \r\n",ch,str_temp_ch[ch]);
        dmsg(DL_NON," \t-> SP : %d / PV : %d.%d \r\n",(uint8_t)_setPoint[ch],pv_integer[ch],pv_fraction[ch]);
        dmsg(DL_NON," \t-> Control Mode : %s \r\n", str_ctrl_mode[_ctrl_Mode[ch]]);
        dmsg(DL_NON," \t-> PWM : %d%% \r\n", pwm_percent_Integer[ch]);
        dmsg(DL_NON," \t-> SSR Status   : %s\r\n",curr_ssr_status[ch]?"ON":"OFF");
        dmsg(DL_NON," \t-> SSROnPeriod  : %d\r\n", SSROnPeriod[ch]);
        dmsg(DL_NON," \t-> Alarm Status : %d\r\n", alarm_status[ch]);
    }
    dmsg(DL_NON,"\r\n");
}

int32_t pv_test[TEMP_CH_MAX];

void App_Temp_Read(uint8_t ch)
{
    int ret = 0;
    float pv_offset = 0;
    float pv = 0;
    
    if ( max31865_fault[ch] == 0 )
    {
        // ret = Drv_Max31865_Temp_Read( ch, &temp_Ambient[ch] );
        ret = Drv_Max31865_Temp_Read( ch, &pv );
        
        if ( ret > 0 )
        {
            App_Set_SSR_lock(ch,0);
        
            if ( pv < TEMPERATURE_LINIT_MIN )
            {
                dmsg(DL_ERR,"RunTempControl ch %d : TEMPERATURE_ERROR \r\n", ch);
            }
            else
            {
                if(pv < TEMPERATURE_LOW_CUT)
                {
                    temp_Ambient[ch] = curr_Ambient[ch];
                }
                else
                {
                    temp_Ambient[ch] = pv;
                    curr_Ambient[ch] = pv;
                }
                // FIR Filter
                ret = GetFIR(&temp_Ambient[ch], ch);
                // TEMP FILTER 
                pv_offset = temp_Ambient[ch] - _setPoint[ch];

                if( (pv_offset > -(TEMPERATURE_STEP1)) && (pv_offset < (TEMPERATURE_STEP1)) )
                {
                    pv_integer[ch] = (int32_t)_setPoint[ch];
                }
                else if( (pv_offset > -(TEMPERATURE_STEP2)) && (pv_offset < (TEMPERATURE_STEP2)) )
                {
                    if(pv_offset < 0)   pv_integer[ch] = ((int32_t)_setPoint[ch]) - 1;                   
                    else                pv_integer[ch] = ((int32_t)_setPoint[ch]) + 1;                    
                }
                else if( (pv_offset > -(TEMPERATURE_STEP3)) && (pv_offset < (TEMPERATURE_STEP3)) )
                {
                    if(pv_offset < 0)   pv_integer[ch] = ((int32_t)_setPoint[ch]) - 2;                   
                    else                pv_integer[ch] = ((int32_t)_setPoint[ch]) + 2;  

                }
                else
                {
                    pv_integer[ch] = (int32_t)temp_Ambient[ch];
                    #if 0
                    pv_fraction[ch] = (int32_t)(temp_Ambient[ch]*1000.0f);
                    pv_fraction[ch] = pv_fraction[ch] - (pv_integer[ch]*1000);
                    if ( pv_fraction[ch] < 0 )
                    {
                        pv_fraction[ch] *= -1;
                    }
                    #endif
                }

                #if 0
                if(pv_test[ch] != pv_integer[ch])
                {
                    pv_test[ch] = pv_integer[ch];
                    dmsg(DL_INF,"PV%d : %d \r\n", ch,pv_test[ch]);
                }
                #endif
                // dmsg(DL_INF," CH %d : %d.%d \r\n",ch,pv_integer[ch],pv_fraction[ch]);
                // Run Heater
                App_Run_Heater(ch, &temp_Ambient[ch], &_setPoint[ch], Api_Systick_Get());
            }
        }
        else
        {
            if(ch != TEMP_CH_3)
                dmsg(DL_ERR,"MAX31865 ch %d : %s fault %d \r\n", ch, str_temp_ch[ch],ret);
            max31865_fault[ch] = ret;
            App_Set_SSR_lock(ch,1);
        }
    }
    else
    {
        if(Api_Period_Check(&fail_tick[ch],TICK_S(1),true))
        {
            Drv_Max31865_Chips_Init(ch);
            max31865_fault[ch] = 0;
            App_Set_SSR_lock(ch,1);
        }

        //Api_Delay_ms(50);
    }

}

void App_SSR_PWM_Init(void)
{
	uint8_t ch;
	float percentage = 0.0;

	for ( ch = 0; ch < TEMP_CH_MAX; ch++ )
	{
		App_SSR_PWM_Set(ch, &percentage);
	}
}

void App_SSR_Ctrl_Init(void)
{
    uint8_t ch;
    
    for(ch=0 ; ch < TEMP_CH_MAX ; ch++)
    {
        Drv_Max31865_Chips_Init(ch);
        max31865_fault[ch] = 0;
        
        App_Ctrl_Param_Init(ch,MODE_STOP);
        
        alarm_cnt_lo[ch] = 0;
        alarm_cnt_hi[ch] = 0;
        alarm_status[ch] = OFF;
    }
    
    App_SSR_PWM_Init();
    
    temp_ctrl_ready = 1;
}


