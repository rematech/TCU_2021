#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "_header_core.h"
#include "app_console.h"
#include "hal_timer.h"


#define TIMER0_INT_INCREASE_DURING_USE    0
#define TIMER1_INT_INCREASE_DURING_USE    1
#define TIMER2_INT_INCREASE_DURING_USE    1
#define TIMER3_INT_INCREASE_DURING_USE    1
#define TIMER4_INT_INCREASE_DURING_USE    1


volatile uint32_t hal_timer_clk = 0;


static const uint32_t hal_timer_sysctl[HAL_TIMER_CH_MAX] =
{
    SYSCTL_PERIPH_TIMER0, SYSCTL_PERIPH_TIMER1, SYSCTL_PERIPH_TIMER2, SYSCTL_PERIPH_TIMER3, 
    SYSCTL_PERIPH_TIMER4, SYSCTL_PERIPH_TIMER5
};

static const uint32_t hal_timer_base[HAL_TIMER_CH_MAX] =
{
    TIMER0_BASE, TIMER1_BASE, TIMER2_BASE, TIMER3_BASE, TIMER4_BASE, TIMER5_BASE
};

static const uint32_t hal_timer_assign[HAL_TIMER_CH_MAX] =
{
    INT_TIMER0A, INT_TIMER1A, INT_TIMER2A, INT_TIMER3A, INT_TIMER4A, INT_TIMER5A
};


void __attribute__((weak))App_Timer0A_IntHandler(void)
{

}

void __attribute__((weak))App_Timer1A_IntHandler(void)
{

}

void __attribute__((weak))App_Timer2A_IntHandler(void)
{

}

void __attribute__((weak))App_Timer3A_IntHandler(void)
{

}

void __attribute__((weak))App_Timer4A_IntHandler(void)
{

}

void Hal_Timer0A_IntHandler(void)
{
    TimerIntClear(hal_timer_base[HAL_TIMER_CH_0], TIMER_TIMA_TIMEOUT);
    #if TIMER0_INT_INCREASE_DURING_USE
    TimerDisable(hal_timer_base[HAL_TIMER_CH_0], TIMER_A);
    #endif
    App_Timer0A_IntHandler();
    #if TIMER0_INT_INCREASE_DURING_USE
    TimerEnable(hal_timer_base[HAL_TIMER_CH_0], TIMER_A);
    #endif
}

void Hal_Timer1A_IntHandler(void)
{
    TimerIntClear(hal_timer_base[HAL_TIMER_CH_1], TIMER_TIMA_TIMEOUT);
    #if TIMER1_INT_INCREASE_DURING_USE
    TimerDisable(hal_timer_base[HAL_TIMER_CH_1], TIMER_A);
    #endif
    App_Timer1A_IntHandler();
    #if TIMER1_INT_INCREASE_DURING_USE
    TimerEnable(hal_timer_base[HAL_TIMER_CH_1], TIMER_A);
    #endif
}

void Hal_Timer2A_IntHandler(void)
{
    TimerIntClear(hal_timer_base[HAL_TIMER_CH_2], TIMER_TIMA_TIMEOUT);
    #if TIMER2_INT_INCREASE_DURING_USE
    TimerDisable(hal_timer_base[HAL_TIMER_CH_2], TIMER_A);
    #endif
    App_Timer2A_IntHandler();
    #if TIMER2_INT_INCREASE_DURING_USE
    TimerEnable(hal_timer_base[HAL_TIMER_CH_2], TIMER_A);
    #endif
}

void Hal_Timer3A_IntHandler(void)
{
    TimerIntClear(hal_timer_base[HAL_TIMER_CH_3], TIMER_TIMA_TIMEOUT);
    #if TIMER3_INT_INCREASE_DURING_USE
    TimerDisable(hal_timer_base[HAL_TIMER_CH_3], TIMER_A);
    #endif
    App_Timer3A_IntHandler();
    #if TIMER3_INT_INCREASE_DURING_USE
    TimerEnable(hal_timer_base[HAL_TIMER_CH_3], TIMER_A);
    #endif
}

void Hal_Timer4A_IntHandler(void)
{
    TimerIntClear(hal_timer_base[HAL_TIMER_CH_4], TIMER_TIMA_TIMEOUT);
    #if TIMER4_INT_INCREASE_DURING_USE
    TimerDisable(hal_timer_base[HAL_TIMER_CH_4], TIMER_A);
    #endif
    App_Timer4A_IntHandler();
    #if TIMER4_INT_INCREASE_DURING_USE
    TimerEnable(hal_timer_base[HAL_TIMER_CH_4], TIMER_A);
    #endif
}

void Hal_Timer_Init(uint8_t ch,uint8_t priority,uint32_t period)
{      
    SysCtlPeripheralEnable(hal_timer_sysctl[ch]);
    TimerLoadSet(hal_timer_base[ch], TIMER_A, SysCtlClockGet() / period);
    TimerConfigure(hal_timer_base[ch], TIMER_CFG_PERIODIC);
    IntEnable(hal_timer_assign[ch]);
    TimerIntEnable(hal_timer_base[ch], TIMER_TIMA_TIMEOUT);
    TimerEnable(hal_timer_base[ch], TIMER_A);
	IntPrioritySet(hal_timer_assign[ch], priority);
}

void Hal_Systick_Init(void)
{  
    SysTickPeriodSet(SysCtlClockGet() / HAL_TIMER_REF_MS); // 1ms
    SysTickEnable();
    SysTickIntEnable();
}


