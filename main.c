

#include "_include_header.h"

uint32_t sys_clock;


#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
    App_Heater_All_Off();
    dmsg(DL_ERR,"SYSTEM ERROR : %s = %d\r\n",pcFilename,ui32Line);
    while(1);
}
#endif


void Hal_Systick_IntHandler(void)
{
    Api_Systick_Update();
}


void Hal_Core_Init(void)    
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    sys_clock = SysCtlClockGet();
    
    FPULazyStackingEnable();

    IntMasterEnable();
}

int main(void)
{
    Hal_Core_Init();

    App_Init();
    
    while(1)
    { 
        App_System();
    }
}


