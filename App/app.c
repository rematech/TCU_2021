

#include "_include_header.h"

DEV_T dev;
GLOBAL_DATA g_Data;


void App_Sys_Logo(void)
{
    dmsg(DL_INF,"-----------------------------------------------------------------\r\n");
    dmsg(DL_INF,"%s - %s\r\n",COP_NAME,DEV_NAME);
    dmsg(DL_INF,"SW VER       : %d.%d.%d [build : %s]\r\n",SW_VER_MAJOR,SW_VER_MINOR,SW_VER_BUILD,SW_DATE);
    dmsg(DL_INF,"Sys Clock    : %d Mhz\r\n",SysCtlClockGet()/UNIT_MHZ);
    dmsg(DL_INF,"-----------------------------------------------------------------\r\n");
}

void App_System_Info(void)
{
    dmsg(DL_NON,"\r\n");
    
    App_Sys_Logo();
}


void App_System_status_show(void)
{
    dmsg(DL_NON,"\r\n");

    App_Temp_Alarm_Config();

    App_Temp_Status();    
}


void App_NV_Data_Save(void)
{
    FLASH_INFO_T fi;
    
    //memcpy(&fi.dev.guc,&guc,sizeof(guc));
    FlashPBSave((uint8_t*)&fi);

    dmsg(DL_INF,"FLASH : Data Save Ok.\r\n");
}

void App_NV_Data_Default_Set(void)
{
    
}

void App_Load_Config(void)
{    
    FlashPBInit(FLASH_PB_START, FLASH_PB_END, FLASH_PB_SIZE);
    
    FLASH_INFO_T* ret = (FLASH_INFO_T*)FlashPBGet();
    FLASH_INFO_T fi;
    
    if(ret)
    {
        dmsg(DL_INF,"FLASH : Data Load Ok.\r\n");
        fi = *ret;
        //memcpy(&guc,&fi.dev.guc,sizeof(guc));
    }
    else
    {
        dmsg(DL_INF,"FLASH : No Data. Saved Default Data.\r\n");
        
        App_NV_Data_Default_Set();
        App_NV_Data_Save();
    }
    
}


void App_Temp_Ctrl_Proc(void)
{
    uint32_t i;
    
    //dmsg(DL_INF,"%s %06d\r\n",__func__,Api_Systick_Get());
    for(i=0;i<TEMP_CH_MAX;i++)
    {
        App_Temp_Read(i);
    }

}

void App_SWTimer_Temp_Reading(uint8_t timer_id, uint32_t param1)
{
    dmsg(DL_NUL,"=>%s %06d\r\n",__func__,Api_Systick_Get());

    #ifndef _TEMP_USE_HAL_TIMER_
    App_Temp_Ctrl_Proc();
    #endif
    App_Temp_Alarm_Check();
}

void App_SWTimer_Comm_Proc(uint8_t timer_id, uint32_t param1)
{
    dmsg(DL_NUL,"=>%s %06d\r\n",__func__,Api_Systick_Get());

    switch(App_Host_Comm_Seq_Get())
    {
        case HCS_BOOT_MSG:
            App_Host_IF_Send_Boot_Msg();
            break;

        default:
            break;
    }

}

void App_SWTimer_Misc_Proc(uint8_t timer_id, uint32_t param1)
{
    dmsg(DL_NUL,"=>%s %06d\r\n",__func__,Api_Systick_Get());

    App_Temp_Display();
}


void App_SWTimer_Proc_init(void)
{
    Api_SWTimer_Resister(TEMP_READ_TIME_MS,App_SWTimer_Temp_Reading);
    Api_SWTimer_Resister(TICK_MS(1000),App_SWTimer_Comm_Proc);
    Api_SWTimer_Resister(TICK_MS(1000),App_SWTimer_Misc_Proc);
}

void App_Timer0A_IntHandler(void)
{
    App_Heater_Ctrl_Proc();
}

void App_Timer1A_IntHandler(void)
{
    App_Temp_Ctrl_Proc();
}

void App_Timer_Init_SSR_Ctrl(void)
{
    Hal_Timer_Init(HAL_TIMER_CH_0,HAL_INT_PRIORITY_LVL1,TIMER_PERIOD_MS(1));
}

void App_Timer_Init_Temp_Reading(void)
{
    Hal_Timer_Init(HAL_TIMER_CH_1,HAL_INT_PRIORITY_LVL2,TIMER_PERIOD_MS(TEMP_READ_TIME_MS));
}


void App_Temp_Alarm_Init(void)
{
    dev.alarm.lo_en = FALSE;
    dev.alarm.hi_en = FALSE;
    dev.alarm.lo_val = 30;
    dev.alarm.hi_val = 70;
}

void App_Init_Temp_Ctrl(void)
{
    App_Temp_Alarm_Init();
    
    App_SSR_Ctrl_Init();
    
    App_Timer_Init_SSR_Ctrl();
    #if _TEMP_USE_HAL_TIMER_
    App_Timer_Init_Temp_Reading(); // Hal Timer
    #endif
}

void App_Init_GData()
{
	g_Data.iReadCount = 0;
	g_Data.iExcuteCount = 0;
}


void App_Init(void)
{
    App_Console_Init(CONSOLE_PORT,HAL_INT_PRIORITY_LVL2,CONSOLE_BAUD,PARITY_NONE);
    App_System_Info();
    
    Hal_Gpio_Init();

    Hal_Systick_Init();
    
    App_Host_IF_Init(HIL_MCU,HOST_IF_PORT,HAL_INT_PRIORITY_LVL2,HOST_IF_BAUD,PARITY_NONE); 

    App_Load_Config();

    App_SWTimer_Proc_init();

    App_Init_Temp_Ctrl();

	App_Init_GData();

}

void App_SWTimer_Proc(void)
{
    Api_SWTimer_Proc(0,0);    
}

void App_System(void)
{
    App_SWTimer_Proc();

    App_Host_IF_Proc();

    App_Console_Data_Rcv();
}


