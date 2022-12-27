#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "api_systick.h"


volatile uint32_t sys_tick_ms = 0;

volatile SWTIMER_T swtimer[SWTIMER_MAX]={0};

void Api_Systick_Update(void)
{
    sys_tick_ms ++;
    Api_SWTimer_Tick_Update();
}

uint32_t Api_Systick_Get(void)
{
    return sys_tick_ms;
}

uint8_t Api_Period_Check(uint32_t *tick,uint32_t period,uint8_t t_update)
{
    uint32_t sys_tick = Api_Systick_Get();
    uint32_t elapsed = sys_tick - *tick;
    
    if(elapsed >= period)
    {
        if(t_update)
        {
            if(elapsed > (period * TIME_RES_TH_PER))
                *tick = sys_tick;
            else
                *tick += period;
        }
        
        return true;
    }
    return false;
}

void Api_Delay_ms(uint32_t t)
{
    volatile uint32_t tick = Api_Systick_Get();
    
    while((Api_Systick_Get() - tick) < t);
}



void Api_SWTimer_Tick_Update(void)
{
    uint8_t i;
    
    for(i=0;i<SWTIMER_MAX;i++)
    {
        if(swtimer[i].handler)
        {
//            if(!swtimer[i].busy)
                swtimer[i].tick ++;
        }
    }
}

int8_t Api_SWTimer_Resister(uint32_t period, timer_handler handler)
{
    uint8_t i;
        
    for(i=0;i<SWTIMER_MAX;i++)
    {
        if(!swtimer[i].handler)
        {
            swtimer[i].tick = 0;
            swtimer[i].id = i;
            swtimer[i].period = period;
            swtimer[i].handler = handler;

            return i;
        }
    }
    return -1;
}

int8_t Api_SWTimer_Unresister(uint8_t timer_id)
{
    uint8_t i;
            
    for(i=0;i<SWTIMER_MAX;i++)
    {
        if(swtimer[i].id == timer_id)
        {
            swtimer[i].tick = 0;
            swtimer[i].id = 0;
            swtimer[i].period = 0;
            swtimer[i].handler = NULL;

            return i;
        }
    }
    return -1;
}

void Api_SWTimer_Clear(void)
{
    uint8_t i;
            
    for(i=0;i<SWTIMER_MAX;i++)
    {   
        swtimer[i].tick = 0;
        swtimer[i].id = 0;
        swtimer[i].period = 0;
        swtimer[i].handler = NULL;
    }
}

void Api_SWTimer_Proc(uint32_t param1,uint32_t param2)
{
    uint8_t i;
                
    for(i=0;i<SWTIMER_MAX;i++)
    {
        if(swtimer[i].handler)
        {
            if(swtimer[i].tick >= swtimer[i].period)
            {
                swtimer[i].busy = true;
                swtimer[i].tick = 0;
                swtimer[i].handler(swtimer[i].id, param2);
                swtimer[i].busy = false;
            }
        }
    }
}

