#ifndef __API_PID_h__
#define __API_PID_h__

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


/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

void PidInit(unsigned int sampleTime, float *outLimitMin, float *outLimitMax, int channel);
void PidSetTunings(float *Kp, float *Ki, float *Kd, float *upWeight, float *downWeight, int channel);
int PidCompute(float *input, float *setPoint, unsigned long now1ms, float *pOutput, int channel);
float PidGetKp(int channel);
float PidGetKi(int channel);
float PidGetKd(int channel);
void IntCombine(float *value, int intPart, int fractionalPart, int fractionalDigits);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_PID_h__

