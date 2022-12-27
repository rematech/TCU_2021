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
#include "app.h"

#include "app_ssr_ctrl.h"
#include "api_pid.h"
#include "max31865.h"


#define PID_MAX_CHANNEL TEMP_CH_MAX

#define _ValidRange(val, min, max)  (((val)<(min))?(min):(((val)>(max))?(max):(val)))

unsigned int _pidSampleTime[PID_MAX_CHANNEL];
unsigned long _pidLastTime1ms[PID_MAX_CHANNEL];
float _pidLastInput[PID_MAX_CHANNEL];
float _pidIntegralTerm[PID_MAX_CHANNEL];
float _pidKp[PID_MAX_CHANNEL];                  // * (P)roportional Tuning Parameter
float _pidKi[PID_MAX_CHANNEL];                  // * (I)ntegral Tuning Parameter
float _pidKd[PID_MAX_CHANNEL];                  // * (D)erivative Tuning Parameter
float _pidOutMin[PID_MAX_CHANNEL];
float _pidOutMax[PID_MAX_CHANNEL];
float _pidUpWeight[PID_MAX_CHANNEL];
float _pidDownWeight[PID_MAX_CHANNEL];

// test
extern float _orgOutput[];

void PidSetOutputLimits(float *Min, float *Max, int channel);


/*
 * void PidInit(...)
 * 
 */
void PidInit(unsigned int sampleTime, float *outLimitMin, float *outLimitMax, int channel)
{
    _pidSampleTime[channel] = sampleTime;
    PidSetOutputLimits(outLimitMin, outLimitMax, channel);
    _pidIntegralTerm[channel] = 0.0;
    _pidLastInput[channel] = 0.0;
}
 
/*
 * int PidCompute(...)
 *      PID ���
 *      
 *      float input     (in)    : input 값 (예, 현재 온도)
 *      float setPoint  (in)    : 목표 값 
 *      float now       (in)    : 현재 시간(ms)
 *      float *pOutput  (out)   : 결과 값 (Factory System input으로 사용,  예, 0 ~ 100% pwm input)
 *      return                  : 1(success), 0(fail), -1(interval is not enough)

 */
int PidCompute(float *input, float *setPoint, unsigned long now1ms, float *pOutput, int channel)
{
   float error = 0.0;
   float dInput = 0.0;
   float output = 0.0;
   float proportionalTerm = 0.0;
   float integralTerm = 0.0;
   float derivativeTerm = 0.0;
   unsigned long timeChange = 0;
   
   if(now1ms >= _pidLastTime1ms[channel])
      timeChange = now1ms - _pidLastTime1ms[channel];
   else
      timeChange = (4294967295 - _pidLastTime1ms[channel]) + now1ms;  // now1ms overflow

   // 맨 처음 실행시 _lastInput이 0 이므로 input값으로 초기화
   if (_pidLastInput[channel] == 0)
     _pidLastInput[channel] = *input;
   
   
   // interval이 sampling time 보다 일정 % 작아도 허용
   if(timeChange < _pidSampleTime[channel] * 0.95)
   {
	 //UARTprintf("PidCompute - [%d] input %d, timeChange %d, _pidSampleTime %d, now1ms %d : result -1 \n\r", channel, (int)*input, timeChange, _pidSampleTime[channel], now1ms);
     return -1;
   }

   //UARTprintf("PidCompute - [%d] input %d, setPoint %d, timeChange %d, _pidSampleTime %d, now1ms %d \n\r", channel, (int)*input, (int)*setPoint, timeChange, _pidSampleTime[channel], now1ms);

    /*Compute all the working error variables*/
    error = *setPoint - *input;
    _pidIntegralTerm[channel]+= (_pidKi[channel] * error);
    if(_pidIntegralTerm[channel] > _pidOutMax[channel]) _pidIntegralTerm[channel] = _pidOutMax[channel];
    else if(_pidIntegralTerm[channel] < _pidOutMin[channel]) _pidIntegralTerm[channel] = _pidOutMin[channel];
    dInput = (*input - _pidLastInput[channel]);

    // 온도를 상승 시키는 구간에서는 IntegralTerm을 0으로 해서 Overshooting 최소화
    // PTerm이 일정 %보다 작아지면 IntegralTerm 가동
    proportionalTerm = _pidKp[channel] * error;
    if (proportionalTerm > 60)
        _pidIntegralTerm[channel] = 0;
    
    integralTerm = _pidIntegralTerm[channel];
    derivativeTerm = -_pidKd[channel] * dInput; // - 부호 주의
    
    // test
//    _orgOutput[channel] = proportionalTerm + integralTerm + derivativeTerm;    

    // output 보정 (Autotuning시 up/down 기울기 값에 의한 보정)
    if(proportionalTerm < 0)
      proportionalTerm *= _pidDownWeight[channel];
    else
      proportionalTerm *= _pidUpWeight[channel];
    
    if(integralTerm < 0)
      integralTerm *= _pidDownWeight[channel];
    else
      integralTerm *= _pidUpWeight[channel];
    
    if(derivativeTerm < 0)
      derivativeTerm *= _pidDownWeight[channel];
    else
      derivativeTerm *= _pidUpWeight[channel];

    
    /*Compute PID Output*/ 
    output = proportionalTerm + integralTerm + derivativeTerm;

    //#define _ValidRange(val, min, max)  (((val)<(min))?(min):(((val)>(max))?(max):(val)))
    //*pOutput = _ValidRange(output, _pidOutMin[channel], _pidOutMax[channel]);
    if ( output < _pidOutMin[channel] ) *pOutput = _pidOutMin[channel];
    else if ( output > _pidOutMax[channel] ) *pOutput = _pidOutMax[channel];
    else *pOutput = output;

        
    /*Remember some variables for next time*/
    _pidLastInput[channel] = *input;
    _pidLastTime1ms[channel] = now1ms;
    
    return 1;
}

/*
 * void SetTunings(...)
 *      K value setting
 */
void PidSetTunings(float *Kp, float *Ki, float *Kd, float *upWeight, float *downWeight, int channel)
{
   float sampleTimeInSec;
   if (*Kp<0 || *Ki<0 || Kd<0) return;
 
   sampleTimeInSec = (float)_pidSampleTime[channel] / 1000.0;  
   _pidKp[channel] = *Kp;
   _pidKi[channel] = *Ki * sampleTimeInSec;
   _pidKd[channel] = *Kd / sampleTimeInSec;
   _pidUpWeight[channel] = *upWeight;
   _pidDownWeight[channel] = *downWeight;
}
 

/*
 * void SetOutputLimits(...)
 *      output range setting
 */
void PidSetOutputLimits(float *Min, float *Max, int channel)
{
   if(*Min >= *Max) return;
   _pidOutMin[channel] = *Min;
   _pidOutMax[channel] = *Max;
 
}

float PidGetKp(int channel)
{
    return _pidKp[channel];
}

float PidGetKi(int channel)
{
   float sampleTimeInSec;
   
   sampleTimeInSec = (float)_pidSampleTime[channel] / 1000.0;
   return _pidKi[channel] / sampleTimeInSec;
}

float PidGetKd(int channel)
{
   float sampleTimeInSec;
 
   sampleTimeInSec = (float)_pidSampleTime[channel] / 1000.0;    
  
   return _pidKd[channel] * sampleTimeInSec;
}


void IntCombine(float *value, int intPart, int fractionalPart, int fractionalDigits)
{
  float fDigits = 0;
  
  if(fractionalDigits <= 3)
    fDigits = (float)(pow(10,fractionalDigits));
  else
    fDigits = 10; // default 
  
 *value = intPart + (float)((float)fractionalPart/fDigits); 
 
}

