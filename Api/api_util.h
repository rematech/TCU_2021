#ifndef __API_UTIL_H__
#define __API_UTIL_H__

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

#include "stdint.h"

/******************************************************************************/
/* define                                                                     */
/******************************************************************************/


/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/


uint8_t Api_CheckSum_Encoding(uint8_t *check_data,uint32_t data_len);
uint8_t Api_CheckSum_Decoding(uint8_t *check_data,uint32_t data_len,uint8_t check_sum);
float MAX_V(float* num,uint32_t cnt);
float MIN_V(float* num,uint32_t cnt);
float MAX_V_COUNT(uint8_t getcount, float* num,uint32_t cnt);
float MIN_V_COUNT(uint8_t getcount, float* num,uint32_t cnt);


float AVERAGE_V_FILTER(float* num,uint32_t cnt, float fExpectionThreshold);

// get a fraction part as an int
int32_t get_fraction(float val);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_UTIL_H__

