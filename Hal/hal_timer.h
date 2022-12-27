#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

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
    HAL_TIMER_CH_0 = 0,
    HAL_TIMER_CH_1,
    HAL_TIMER_CH_2,
    HAL_TIMER_CH_3,
    HAL_TIMER_CH_4,
    HAL_TIMER_CH_5,
    HAL_TIMER_CH_MAX,
}HAL_TIMER_LIST;

#define HAL_TIMER_REF_MS   1000L
#define HAL_TIMER_REF_US   1000000L

#define TIMER_PERIOD_MS(X)          (HAL_TIMER_REF_MS/X)
#define TIMER_PERIOD_US(X)          (HAL_TIMER_REF_US/X)

//*****************************************************************************
//
// The number of SysTick interrupts to generate per second.
//
//*****************************************************************************

void Hal_Timer_Init(uint8_t ch,uint8_t priority,uint32_t period);

void Hal_Systick_Init(void);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __HAL_TIMER_H__

