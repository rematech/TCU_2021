#ifndef __API_SYSTICK_H__
#define __API_SYSTICK_H__

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
#define TIME_RES_TH_PER     (1.1) // Time Response Threshold Percent 

#define SYSTICK_PERIOD_MS   1L
#define TICK_MS(X)          ((X+SYSTICK_PERIOD_MS-1L)/SYSTICK_PERIOD_MS)
#define TICK_S(X)           (X*TICK_MS(1000))

//*****************************************************************************
//
// The number of SysTick interrupts to generate per second.
//
//*****************************************************************************
// Tick Funtion
void Api_Systick_Update(void);
uint32_t Api_Systick_Get(void);
uint8_t Api_Period_Check(uint32_t *tick,uint32_t period,uint8_t t_update);
void Api_Delay_ms(uint32_t t);



// Timer Funtion

#define SWTIMER_MAX     10

typedef void (*timer_handler)(uint8_t timer_id, uint32_t param1);

typedef struct
{
    uint8_t id;
    uint8_t busy;
    uint32_t tick;
    uint32_t period;
    timer_handler handler;
}SWTIMER_T;

int8_t Api_SWTimer_Resister(uint32_t period, timer_handler handler);
int8_t Api_SWTimer_Unresister(uint8_t timer_id);
void Api_SWTimer_Clear(void);
void Api_SWTimer_Tick_Update(void);
void Api_SWTimer_Proc(uint32_t param1,uint32_t param2);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __API_SYSTICK_H__

