#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "_header_core.h"
#include "hal_delay.h"


#define CPU_CYCLE   3

void Hal_Delay_us(uint32_t us) 
{
    SysCtlDelay(SysCtlClockGet()/1000000*us/CPU_CYCLE); // 1us
}


void Hal_Delay_ms(uint32_t ms) 
{
	SysCtlDelay(SysCtlClockGet()/1000*ms/CPU_CYCLE); // 1ms

}



