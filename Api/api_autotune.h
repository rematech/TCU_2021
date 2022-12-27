#ifndef __API_AUTOTUNE_H__
#define __API_AUTOTUNE_H__


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


#define PID_SAMPLING_TIME           1000
#define ATUNE_SAMPLING_TIME         70
#define ATUNE_MAX_STEPUP_CNT        2 // for autotuning
#define ATUNE_CTRL_TYPE             1 // 0:PI, 1:PID
#define ATUNE_NOISEBAND             0.7
#define ATUNE_OUTPUT_STEP           50  // 25 ~ 50
#define ATUNE_OUTPUT_START          ATUNE_OUTPUT_STEP
#define ATUNE_PU_MIN                1.0 // sec  주기값이 1sec 이하이면 Error 간주.
#define ATUNE_MAX_LOOKBACK_TIME     20000	// msec.
#define ATUNE_INIT_LOOKBACK_TIME    ATUNE_MAX_LOOKBACK_TIME // msec.
#define ATUNE_UP_WEIGHT_MIN         0.0   // 온도 올릴 때 Output 가중치 값 Limit
#define ATUNE_UP_WEIGHT_MAX         2.0   // 2보다는 작아야함.
#define ATUNE_MAX_LOOKBACK_CNT      (ATUNE_MAX_LOOKBACK_TIME / ATUNE_SAMPLING_TIME)



#define Pu_max                      1800    //  (sec).  최대 처리 가능한 주기 설정할 것
#define Ku_max                      (400 / (ATUNE_NOISEBAND * 3.141952)) // 1795.3857...
#define Kx_MIN_VALUE                0.0
#define Kp_MAX_VALUE                (0.6 * Ku_max) * 2  // *2 = margin, 2154.4817....
#define Ki_MAX_VALUE                (1.2 * Ku_max / ATUNE_PU_MIN) * 2  // *2 = margin, 2154.4628...
#define Kd_MAX_VALUE                (0.075 * Ku_max * Pu_max) * 2  // *2 = margin, 484754.1428...


/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

/////////////////////////////
// Interfaces

// Run
//void ATuneInit(int controlType, float outputStart, float outputStep, float noiseBand, unsigned int samplingTime, int maxStepupCount, int channel);
void ATuneInit(int controlType, unsigned int samplingTime, int maxStepupCount, int channel);
int ATuneRun(float *setPoint, float *input, float *output, unsigned long now1ms, int channel);
void ATune_FinishUp(float *output, int channel);

// setting
void ATuneSetControlType(int type, int channel);  // 0:PI, 1:PID
void ATuneSetNoiseBand(float *band, int channel);

// Results
void ATuneGetKp(int channel, float* out);
void ATuneGetKi(int channel, float* out);
void ATuneGetKd(int channel, float* out);
void ATuneGetOutputWeight(float *pUpWeight, float *pDownWeight, int channel);

// test
int ATuneGetPeakCount(int channel);
/////////////////////////////


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_AUTOTUNE_H__
