

#include "_header_core.h"

#include "utils/ustdlib.h"
#include "utils/uartstdio.h"

#include "app.h"

#include "app_ssr_ctrl.h"
#include "api_autotune.h"
#include "max31865.h"

#include "app_console.h"


#define AT_MAX_CHANNEL TEMP_CH_MAX

int _AtControlType[AT_MAX_CHANNEL]; // 0:PI, 1:PID
int _AtLookBackCnt[AT_MAX_CHANNEL];
int _AtStepUpCount[AT_MAX_CHANNEL];
float _AtOstep[AT_MAX_CHANNEL];
float _AtOutputStart[AT_MAX_CHANNEL];
float _AtSetpoint[AT_MAX_CHANNEL];
float _AtNoiseBand[AT_MAX_CHANNEL];
float _AtAbsMax[AT_MAX_CHANNEL];
float _AtAbsMin[AT_MAX_CHANNEL];
float _AtKu[AT_MAX_CHANNEL];
float _AtPu[AT_MAX_CHANNEL];
float _AtAmplitude[AT_MAX_CHANNEL];
float _AtPrevOutput[AT_MAX_CHANNEL];
float _AtPrevDiv1[AT_MAX_CHANNEL];	// debugging용 변수
float _AtPrevDiv2[AT_MAX_CHANNEL];	// debugging용 변수
uint32_t _AtSampleTime[AT_MAX_CHANNEL] = {0, };
uint32_t _AtLastPeak1ms[AT_MAX_CHANNEL];
uint32_t _AtLastTime1ms[AT_MAX_CHANNEL];
uint32_t _AtLastLowPeak1ms[AT_MAX_CHANNEL];
uint32_t _AtLastStepUpTime[AT_MAX_CHANNEL]; 
uint32_t _AtPrevStepUpTime[AT_MAX_CHANNEL];
int _AtRunning[AT_MAX_CHANNEL];
int _AtJustevaled[AT_MAX_CHANNEL];
int _AtJustchangedCnt[AT_MAX_CHANNEL];
int _AtIsMax[AT_MAX_CHANNEL];
int _AtIsMin[AT_MAX_CHANNEL];
int _AtMaxStepUpCount[AT_MAX_CHANNEL];

uint8_t _isStartAboveSP[AT_MAX_CHANNEL];



/*
 * ATuneInit()
 *    AutoTuning Parameter setting
 */
//void ATuneInit(int controlType, float outputStart, float outputStep, float noiseBand, unsigned int samplingTime, int maxStepupCount, int channel)
void ATuneInit(int controlType, unsigned int samplingTime, int maxStepupCount, int channel)
{
#if 0
    _AtControlType[channel] = controlType; // 0:PI, 1:PID
    _AtNoiseBand[channel] = noiseBand;
    _AtOutputStart[channel] = outputStart;
    _AtOstep[channel] = outputStep;
    _AtSampleTime[channel] = samplingTime;
    _AtMaxStepUpCount[channel] = maxStepupCount;

    // init
    _AtRunning[channel] = 0;
#else
    _AtControlType[channel] = ATUNE_CTRL_TYPE; // 0:PI, 1:PID
    _AtNoiseBand[channel] = ATUNE_NOISEBAND*1.0;
    _AtOutputStart[channel] = ATUNE_OUTPUT_START*1.0;
    _AtOstep[channel] = ATUNE_OUTPUT_STEP*1.0;
    _AtSampleTime[channel] = ATUNE_SAMPLING_TIME;
    _AtMaxStepUpCount[channel] = ATUNE_MAX_STEPUP_CNT;

    dmsg(DL_INF,"ATuneInit [%s] : _AtSampleTime   %d, _AtOutputStart %d.%d, _AtOstep %d.%d  \r\n",
        	  str_temp_ch[channel], _AtSampleTime[channel],
        	  (int)_AtOutputStart[channel], (int)(((int)(_AtOutputStart[channel]*10))%10),
        	  (int)_AtOstep[channel], (int)(((int)(_AtOstep[channel]*10))%10) );
        	  
    // init
    _AtRunning[channel] = 0;
#endif
}


/*
 * ATuneRun(float input, float *output)
 *
 *    AutoTuning Function
 *    Keep tuning until the return value is 1. 
 *    If the return value is 1, you can read the K value since AutoTuning is over.
 *    
 *    @setPoint    : Trigger Temp Value
 *    @input       : AutoTuning input value ( Temp )
 *    @*output     : AutoTuning output value
 *    @now100ms    : current time (100ms)
 *    @now1ms      : current time (1ms)
 *    @amxPeakCount : autotuning finish case
 *    @channel      : heater ch
 *    return : 1 (done),  0 (tuning),  -1 (did nothing)
 */

int ATuneRun(float *setPoint, float *input, float *output, unsigned long now1ms,  int channel)
{
    uint32_t interval;   // now - lastTime 
    float refVal;


    _AtJustevaled[channel] = 0;

    if ((_AtStepUpCount[channel] >= _AtMaxStepUpCount[channel]) && (_AtRunning[channel] == 2))
    {
        _AtRunning[channel] = 0;
        	
        ATune_FinishUp(output, channel);

        if(_AtPu[channel] < ATUNE_PU_MIN ) //Setpoint < 1000ms ? Err.
        {
            dmsg(DL_WRN,"%s : %s : return -2\r\n",__func__,str_temp_ch[channel]);
            return -2;
        }

        return 1;
    }

    if(now1ms >= _AtLastTime1ms[channel])
        interval = (uint32_t)(now1ms - _AtLastTime1ms[channel]);    
    else
        interval = (uint32_t)((4294967295 - _AtLastTime1ms[channel]) + now1ms);  // now1ms was overflow


    // interval threhold 90%
    //if(interval < _AtSampleTime[channel] * 0.9)
    if(interval < ATUNE_SAMPLING_TIME * 0.9)
    {
        //dmsg(DL_WRN,"%s : %s : i : %d\r\n",__func__,str_temp_ch[channel],interval);
        return -1;
    }

    _AtLastTime1ms[channel] = now1ms;

    refVal = *input;
    _AtJustevaled[channel] = 1;

    if (_AtRunning[channel] == 0)
    { //initialize working variables the first time around
        _AtJustchangedCnt[channel] = 0;
        _AtLookBackCnt[channel]=0;

        _AtStepUpCount[channel] = 0;

        _AtAbsMax[channel] = *setPoint;
        _AtAbsMin[channel] = *setPoint;
        _AtSetpoint[channel] = *setPoint;
        _AtRunning[channel] = 1;

        _AtLastPeak1ms[channel] = 0;
        _AtLastTime1ms[channel] = 0;
        _AtLastLowPeak1ms[channel] = 0;
        _AtLastStepUpTime[channel] = 0;
        _AtPrevStepUpTime[channel] = 0;

        _AtKu[channel] = 0;
        _AtPu[channel] = 0;

        // Check whether the temperature starts above or below the SP the first time you start Autotuning.
        if(refVal <_AtSetpoint[channel])
        {
            _isStartAboveSP[channel] = 0;
        }else
        {
            _isStartAboveSP[channel] = 1;
        }
    }
    else
    {
        if(refVal>_AtAbsMax[channel])
        { 
            _AtAbsMax[channel] = refVal;
            _AtLastPeak1ms [channel] = now1ms;
        }	
        if(refVal<_AtAbsMin[channel])
        { 
            _AtAbsMin[channel] = refVal;
            _AtLastLowPeak1ms [channel] = now1ms;
        }
    }

    //oscillate the output base on the input's relation to the setpoint
    if(refVal > _AtSetpoint[channel])
    {
        *output = _AtOutputStart[channel] - _AtOstep[channel];

        if (_AtRunning[channel] == 1 && _isStartAboveSP[channel] == 0)
        {
            _AtRunning[channel] = 2;    // If crossed the SP once, value is "2"

            dmsg(DL_INF,"%s : %s output %d.%d, _AtRunning %d, _isStartAboveSP %d, _AtOutputStart %d.%d, _AtOstep %d.%d \r\n",
            	  __func__,str_temp_ch[channel],
            	  (int)*output, (int)(((int)(*output*10))%10),
            	  _AtRunning[channel],
            	  _isStartAboveSP[channel],
            	  (int)_AtOutputStart[channel], (int)(((int)(_AtOutputStart[channel]*10))%10),
            	  (int)_AtOstep[channel], (int)(((int)(_AtOstep[channel]*10))%10)  );
        }

        _AtPrevOutput[channel] = *output;
    }	
    else if (refVal < (_AtSetpoint[channel] - _AtNoiseBand[channel]) )
    {
        *output = _AtOutputStart[channel] + _AtOstep[channel];

        if (_AtRunning[channel] == 1) 
        {
            if (_isStartAboveSP[channel] == 1)
            {
                _AtRunning[channel] = 2;    // If crossed the SP once, value is "2"
                dmsg(DL_INF,"%s : %s output %d, _AtRunning %d, _isStartAboveSP %d \r\n",
                    __func__,str_temp_ch[channel], (int)*output, _AtRunning[channel], _isStartAboveSP[channel]);
            }
        }

      // Lookback time setup code
      // SP를 cross할 때(output + Ostep 하는 시점)를 기준으로 주기를 구하고 이값의 1/2을 lookback time으로 설정)
      // 정확성을 위해서 Autotuning 시작 후에 작동
      
      else if (_AtRunning[channel] == 2) 
      {

          // output이 처음 바뀐 시점
          if (_AtPrevOutput[channel] != *output) 
          {
              _AtStepUpCount[channel]++;

              _AtPrevStepUpTime[channel] = _AtLastStepUpTime[channel];
              _AtLastStepUpTime[channel] = now1ms;

              if(_AtStepUpCount[channel]==1)
              {
                // re-initializing
                _AtAbsMax[channel] = *setPoint;
                _AtAbsMin[channel] = *setPoint;
              }     
          }
      }

      _AtPrevOutput[channel] = *output;	// if문 밖으로 빼지 말것  (외부에서 output이 바뀌는 경우가 있으므로)
    }

#if 0
    dmsg(DL_INF,"  _AtLastTime1ms[%s] : %d, _AtOutputStart %d.%d(%c), _AtOstep %d.%d(%c), _AtRunning %d \r\n",
    	  str_temp_ch[channel],
    	  _AtLastTime1ms[channel],
    	  //(int)*output,
    	  (int)_AtOutputStart[channel], (int)(((int)(_AtOutputStart[channel]*10))%10), _AtOutputStart[channel]==0?'0':'x',
    	  (int)_AtOstep[channel], (int)(((int)(_AtOstep[channel]*10))%10), _AtOstep[channel]==0?'0':'x',
    	  _AtRunning[channel]);
#endif

    return 0;
} // int Run()


/*
 * float GetKp()
 *    
 */
void ATuneGetKp(int channel, float *out)
{
    *out = _AtControlType[channel] == 1 ? (0.6 * _AtKu[channel]) : (0.4 * _AtKu[channel]);
}

/*
 * float GetKi()
 *   
 */
void ATuneGetKi(int channel, float *out)
{
    *out = _AtControlType[channel] == 1 ? (1.2 * _AtKu[channel] / _AtPu[channel]) : (0.48 * _AtKu[channel] / _AtPu[channel]);  // Ki = Kc/Ti
}

/*
 * float GetKd()
 *  
 */
void ATuneGetKd(int channel, float *out)
{
    *out = _AtControlType[channel] == 1 ? (0.075 * _AtKu[channel] * _AtPu[channel]) : 0;  //Kd = Kc * Td
}

/*
 *  GetOutputWeight()
 *   output * weight value
 */
void ATuneGetOutputWeight(float *pUpWeight, float *pDownWeight, int channel)
{
	float atPrevDiv = 0.0;

#if SEND_DIAGONOSIS == 1
	_AtPrevDiv1[channel] = 0;
	_AtPrevDiv2[channel] = 0;
#endif

	if (_AtLastPeak1ms[channel] >= _AtLastLowPeak1ms[channel])
	{
		atPrevDiv = (float)((_AtLastPeak1ms[channel] - _AtLastLowPeak1ms[channel]) / 1000.0);

		// test 코드 (디버깅용) ???
#if SEND_DIAGONOSIS == 1
		_AtPrevDiv1[channel] = atPrevDiv;
#endif
	}	
	else
	{
		// atPrevDiv < 0인 경우는 마지막 low peak가 이전 low peak보다 낮은 경우 발생하며, 이때는 high peak보다 low peak가 시간 상 나중에 측정된 값이 됨  
		// => 따라서 atPrevDiv는 Up 그래프의 x축이 아니라 Down 그래프의 x축이 됨 
		// > 이것을 다시 Up그래프의 x축으로 바꿔주는 작업
		
		atPrevDiv = (float)((_AtLastLowPeak1ms[channel] - _AtLastPeak1ms[channel]) / 1000.0);
		atPrevDiv = _AtPu[channel] - atPrevDiv;

		// 만약 측정 오차로 인해 음수가 나오는 경우에 대한 처리
		if (atPrevDiv < 0)
			atPrevDiv = 0;

		// test 코드 (디버깅용) ???
#if SEND_DIAGONOSIS == 1
		_AtPrevDiv2[channel] = atPrevDiv;
#endif
	}

    *pUpWeight = atPrevDiv / _AtPu[channel] * 2.0;   // atPrevDiv / _AtPu[channel] == 0.5일때 Weight = 1 이 됨  (default)
  
    if(*pUpWeight < ATUNE_UP_WEIGHT_MIN)
    {
        *pUpWeight = ATUNE_UP_WEIGHT_MIN;
    }
    else if(*pUpWeight > ATUNE_UP_WEIGHT_MAX)
    {
        *pUpWeight = ATUNE_UP_WEIGHT_MAX;
    } 

    *pDownWeight = 2.0 - *pUpWeight;

    return;
}

/*
 * void SetControlType(int type)
 *    Control Type (PI, PID) 설정
 *    
 *    @int type : 0 = PI, 1 = PID
 */
void ATuneSetControlType(int type, int channel)
{
    _AtControlType[channel] = type;
}

/*
 * void SetNoiseBand(float band)
 */
void ATuneSetNoiseBand(float *band, int channel)
{
    _AtNoiseBand[channel] = *band;
}

void ATune_FinishUp(float *output, int channel)
{
    _AtAmplitude[channel] = _AtAbsMax[channel] - _AtAbsMin[channel];
                                    
    *output = _AtOutputStart[channel];

    //we can generate tuning parameters!
    _AtKu[channel] = 4.0 * (2.0 * _AtOstep[channel]) / (_AtAmplitude[channel] * 3.141592);
    _AtPu[channel] =(float)(( _AtLastStepUpTime[channel] - _AtPrevStepUpTime[channel])/1000.0);
}

