#ifndef __API_FILTER_FIR_H__
#define __API_FILTER_FIR_H__

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
extern float _fir_avg_v[];


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

void InitFir(int channel);
//float GetFIR(float value, int channel);
int GetFIR(float *value, int channel);

void Fir_Data_Display_Flag_Set(uint8_t val);
void Fir_Data_Display(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_FILTER_FIR_H__
