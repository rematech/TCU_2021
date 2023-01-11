/*
 *  FilterFIR.c
 *
 *    FIR Filter
 */

//#include <math.h>
//#include "defines.h"

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
#include "api_util.h"
#include "app.h"
#include "app_ssr_ctrl.h"
#include "api_filter_fir.h"
#include "max31865.h"


#define FIR_MAX_CHANNEL TEMP_CH_MAX
#define SAMPLING_NUM  			26//10 // FIR 필터 적용시 몇개의  sampling data로 평균 낼것인지 결정

float _firCurrentSum[FIR_MAX_CHANNEL];//계산하기 위한 중간 값
float _firPrevValue[FIR_MAX_CHANNEL][SAMPLING_NUM];
uint32_t _firIndex[FIR_MAX_CHANNEL];
uint32_t _firIndex_set[FIR_MAX_CHANNEL];


float _fir_avg_v[FIR_MAX_CHANNEL];

void InitFir(int channel)
{
    int i = 0;
    
    _firCurrentSum[channel] = 0.0;
    _firIndex[channel] = 0;
    _firIndex_set[channel] = 0;
    for(i = 0; i < SAMPLING_NUM; i++)
      _firPrevValue[channel][i] = 0.0;
}

#if 0
float GetFIR(float value, int channel)
{
    int i;
    float result, sum, in, out;
    float *pFirPrevValue;
    
    // error check
    if(channel >= FIR_MAX_CHANNEL)
      return -1;
    
    pFirPrevValue = _firPrevValue[channel];  // 속도향상을 위해 이차원배열을 쓰지 않고 포인터에 할당해서 일차원배열로 연산
    
    if(_firIndex[channel] < SAMPLING_NUM)
    {
        pFirPrevValue[_firIndex[channel]++] = value;
        sum = 0;
        for(i = 0; i < _firIndex[channel]; i++)
        {
            sum += pFirPrevValue[i];
        }
        
        result = sum / _firIndex[channel];
        _firCurrentSum[channel] = sum;
    }
    else
    {
        in = value;
        out = pFirPrevValue[0];
        
        for(i = 0; i < SAMPLING_NUM - 1; i++)
        {
            pFirPrevValue[i] = pFirPrevValue[i + 1];
        }
        pFirPrevValue[SAMPLING_NUM - 1] = value;
        _firCurrentSum[channel] += in - out;
        result = _firCurrentSum[channel] / (float)SAMPLING_NUM;
    }
    
    // 소수 첫째자리까지 사용
//    result = Round(result, 1);
    
    return result;
}
#else
int GetFIR(float *value, int channel)
{
    float out;
    float calc;
    float *pFirPrevValue;
    char i;
    // error check
    if(channel >= FIR_MAX_CHANNEL) return -1;
      
    pFirPrevValue = _firPrevValue[channel];
    out = pFirPrevValue[ _firIndex[channel]%SAMPLING_NUM ];
    pFirPrevValue[ _firIndex[channel]%SAMPLING_NUM ] = *value;
    
    _firCurrentSum[channel] += (*value - out);

    _firIndex[channel]++;
    
    if(_firIndex_set[channel])
    {

        calc =_firCurrentSum[channel] - MAX_V(_firPrevValue[channel], SAMPLING_NUM);
        *value = (calc - MIN_V(_firPrevValue[channel], SAMPLING_NUM)) / (SAMPLING_NUM-2);
        
        if(_firIndex[channel] >= SAMPLING_NUM ) _firIndex[channel] = 0;
    }
    else
    {
        *value = _firCurrentSum[channel]/_firIndex[channel];
        
        if(_firIndex[channel] >= SAMPLING_NUM) _firIndex_set[channel] = 1;
    }

	g_Data.fTempFirfilter[channel] = *value;
 

    return 0;
}

#define _RANGE_FILTER__VALUE_				1.8
int GetFIR_New(float *value, int channel)
{
    float out;
    float calc;
    float *pFirPrevValue;
    char i;

	float fAverage;
	
    // error check
    if(channel >= FIR_MAX_CHANNEL) return -1;
      
    pFirPrevValue = _firPrevValue[channel];

	*value = AVERAGE_V_FILTER(pFirPrevValue, SAMPLING_NUM, _RANGE_FILTER__VALUE_);
	g_Data.fTempFirfilter[channel] = *value;

	return 0;
}
#endif

uint8_t fir_data_display_flag = 0;

void Fir_Data_Display_Flag_Set(uint8_t val)
{
    fir_data_display_flag = val;
}

void Fir_Data_Display(void)
{
    char i;
    if(fir_data_display_flag)
    {
        
        _fir_avg_v[2] = 0;
        for(i=0;i<10;i++)
        {
            UARTprintf("%d\n\r",(int)(_firPrevValue[2][i]*10));
            _fir_avg_v[2] += _firPrevValue[2][i];
        }
        UARTprintf("avg %d ",(int)_fir_avg_v[2]);
        _fir_avg_v[2] = _firCurrentSum[2] - MAX_V(_firPrevValue[2], 10);
        _fir_avg_v[2] = _fir_avg_v[2] - MIN_V(_firPrevValue[2], 10);
        
        UARTprintf("%d\n\r",(int)((_fir_avg_v[2]*10)/8));
    }
}

