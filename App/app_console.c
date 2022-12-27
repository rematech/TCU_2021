

#include "_include_header.h"


#include "api_cmd.h"
#include "app_console.h"



static uint8_t cmd_buf[UART_RX_BUF_SIZE] = {0,}, console_char_cnt=0;
static uint8_t console_ch;

static uint8_t debug_lvl = DEFAULT_DEBUG_LVL;

int cmd_host_if_echo_set(int argc, char** argv)
{
    uint8_t en;

    if(argc != 2)
    {
        return CMDLINE_INVALID_ARG;
    }
    
    en = atoi(argv[1]);

    App_HI_Data_Echo(en);
    
    return CMDLINE_SUC;
};




console_cmds_t cmd_list[] =
{
    { "help",   Cmd_help,               " Display list of commands" },
    { "?",      Cmd_help,               " Display list of commands" },
    { "dl",     cmd_debug_lvl,          " Set Debug Level" },        
    { "info",   Cmd_info,               " Display System info" },
    { "show",   Cmd_show,               " Display System status" },

    { "reset",  Cmd_reset,              " System reset" },

    { "ht",     cmd_heater_ctrl,        " ht ch onoff. ex: < ht 0 1 .. ht 2 0> Must Set -> pid 4" },        
    { "sp",     cmd_set_sp,             " Set SP. ex: < sp 55 >" },
    { "pid",    cmd_pid_mode,           " Pid Mode ch Setting. ex: <pid 0 0 > <0 : Stop, 1 : PID, 2 : AT100, 3 : AT40, 4 : Manual>" },
    { "pida",   cmd_pid_mode_all,       " Pid Mode ch all Setting. ex: <pida 0 > <0 : Stop, 1 : PID, 2 : AT100, 3 : AT40, 4 : Manual>" },        
    { "dis",    cmd_data_display,       " Data Display. ex: <0 : off, 1: on>" },
    
    { "echo",   cmd_host_if_echo_set,   " Host IF RCV Data Echo En/Dis ex: echo 1, echo 0" },
    
    { NULL, NULL, NULL}
};

const char* debug_log_msg[DL_MSG_NO_MAX]=
{
    "NUL", // Unused    
    "NON",
    "INF",
    "DBG",
    "RAW",
    "   ",
    "WRN",
    "ERR",
};

int dmsg(DEBUG_LEVEL_T deb_lvl, const char *fmt, ...)
{
    if(debug_lvl & _BIT(deb_lvl))
    {
        va_list ap;
        va_start(ap, fmt);

        #if 0
        if((deb_lvl  == DL_ERR) || (deb_lvl == DL_WRN))
            UARTprintf("[%s:%d] ",debug_log_msg[deb_lvl],Api_Systick_Get());
        else
        #endif
        if(deb_lvl != DL_NON)
            UARTprintf("\b[%s] ",debug_log_msg[deb_lvl]);

        UARTvprintf(fmt, ap);

        va_end(ap);
    }

    return 0;
}

uint8_t Debug_Lv_Get(void)
{
    return debug_lvl;
}

int cmd_debug_lvl(int argc, char** argv)
{
    uint32_t level;
    uint8_t i;
    
    if(argc != 2)
    {
        dmsg(DL_NON,"Set Debug level, now setting is %x\r\n",debug_lvl);
        for(i=0;i<DL_MSG_NO_MAX;i++)
            dmsg(DL_NON,"%s = %d\r\n",debug_log_msg[i],i);
        
        return CMDLINE_INVALID_ARG;
    }

    level = atoi(argv[1]);

    if(level == DL_NUL || level == DL_NON || level == 0xF) 
    {
        debug_lvl = DEFAULT_DEBUG_LVL;
        dmsg(DL_NON,"Set Default Debug Level[%02x]\r\n", debug_lvl);    
    }
    else
    {
        debug_lvl |= _BIT(level);        
        dmsg(DL_NON,"Set Debug Level[%02x]\r\n", debug_lvl);    
        dmsg(DL_NON,"Added MSG[%s]\r\n", debug_log_msg[level]);
    }

    return CMDLINE_SUC;
}

//////////////////////////////////////////////////////////////////////
int cmd_set_sp(int argc, char** argv)
{
    uint8_t val;
    
    if(argc != 2)
    {
        return CMDLINE_INVALID_ARG;
    }

    val = atoi(argv[1]);

    dmsg(DL_NON,"SET POINT : %d\r\n",val);

    App_Set_SP(TEMP_CH_MAX,MODE_ATUNE_100,val);
    
    return CMDLINE_SUC;
}


int cmd_pid_mode(int argc, char** argv)
{
    uint8_t ch;
    uint8_t val;
    
    if(argc != 3)
    {
        return CMDLINE_INVALID_ARG;
    }

    ch = atoi(argv[1]);

    val = atoi(argv[2]);

    dmsg(DL_NON,"Pid mode : %s -> %s\r\n",str_temp_ch[ch], str_ctrl_mode[val]);

    App_Set_Ctrl_Mode(ch,val);
    
    return CMDLINE_SUC;
}

int cmd_pid_mode_all(int argc, char** argv)
{
    uint8_t val;
    
    if(argc != 2)
    {
        return CMDLINE_INVALID_ARG;
    }

    val = atoi(argv[1]);

    dmsg(DL_NON,"Pid mode : All -> %s\r\n", str_ctrl_mode[val]);

    App_Set_Ctrl_Mode_All(val);
    
    return CMDLINE_SUC;
}

int cmd_data_display(int argc, char** argv)
{
    uint8_t val;
    uint8_t ch;
    
    if(argc == 3)
    {
        ch = atoi(argv[2]);
    }
    else if(argc == 2)
    {
        ch = TEMP_CH_MAX;
    }
    else
    {
        return CMDLINE_INVALID_ARG;
    }
    val = atoi(argv[1]);
    
    dmsg(DL_NON,"Data Display %s \r\n", (val ? "ON" : "OFF"));

    App_Set_Data_Display_Flag(val,ch);
    
    return CMDLINE_SUC;
}

int cmd_heater_ctrl(int argc, char** argv)
{
    uint8_t ch,onoff;
    
    if(argc != 3)
    {
        return CMDLINE_INVALID_ARG;
    }

    ch = atoi(argv[1]);
    onoff = atoi(argv[2]);

    dmsg(DL_NON,"Heater Ctrl %s : %s\r\n",str_temp_ch[ch],onoff ? "ON" : "OFF");

    App_SSR_Ctrl(ch,onoff);
    
    return CMDLINE_SUC;
}

int Cmd_reset(int argc, char *argv[])
{

    dmsg(DL_NON,"\r\n");
    dmsg(DL_NON,"System Reset!\r\n\r\n");
    Api_Delay_ms(1000);
    SysCtlReset();


    return CMDLINE_SUC;
}

int Cmd_info(int argc, char *argv[])
{
    App_System_Info();
    
    return CMDLINE_SUC;
}

int Cmd_show(int argc, char *argv[])
{
    App_System_status_show();
    
    return CMDLINE_SUC;
}


int Cmd_help(int argc, char *argv[])
{
    console_cmds_t *cmd_entry;

    dmsg(DL_NON,"\r\n");
    dmsg(DL_NON,"System commands list\r\n");
    dmsg(DL_NON,"--------------------\r\n");

    cmd_entry = cmd_list;

    while(cmd_entry->pcCmd)
    {
        dmsg(DL_NON,"%7s : %s\r\n", cmd_entry->pcCmd, cmd_entry->phelp_msg);
        cmd_entry++;
    }
    dmsg(DL_NON,"\r\n");
    return CMDLINE_SUC;
}


void Api_Console_Data_Clr(void)
{
    int cnt;
    
    for(cnt=0;cnt<sizeof(cmd_buf);cnt++) cmd_buf[cnt] = 0;
    console_char_cnt = 0;
}

void App_Console_Cmd_Check(void)
{
    int iStatus;

//    strcpy(CMD_Buffer, cmd_buf, uart_rx_cnt);
    
    iStatus = CmdLineProcess((char*)cmd_buf);
    //
    // Handle the case of bad command.
    //
    if(iStatus == CMDLINE_BAD_CMD)
    {
        dmsg(DL_NON,"Bad command!\n\r");
    }

    //
    // Handle the case of too many arguments.
    //
    else if(iStatus == CMDLINE_TOO_MANY_ARGS)
    {
        dmsg(DL_NON,"Too many arguments for command processor!\n\r");
    }

    else if(iStatus == CMDLINE_TOO_FEW_ARGS)
    {
        dmsg(DL_NON,"Too few arguments for command processor!\n\r");
    }

    else if(iStatus == CMDLINE_INVALID_ARG)
    {
        dmsg(DL_NON,"Invalid arguments for command processor!\n\r");
    }

}


void App_Console_Data_Rcv(void)
{
    uint8_t ch;
    uint8_t i;

    i = Hal_Uart_Rcv_Len(console_ch);
    
    while(i)
    {
        i --;
        ch  = Hal_Uart_Get_Char(console_ch);
        
        if(console_char_cnt > UART_RX_BUF_SIZE) console_char_cnt = 0;
        
        if(ch == ASCII_CR)
        {    
            if(console_char_cnt == 0)
            {
				dmsg(DL_NON,"\r\n#");
            }
            else
            {
                dmsg(DL_NON,"\r\n");
				App_Console_Cmd_Check();    
                Api_Console_Data_Clr();
                dmsg(DL_NON,"\r\n#");
            }
        }
        else if(ch == ASCII_BS)
        {    
            if(!console_char_cnt == 0)
            {
                console_char_cnt = console_char_cnt-1;
                dmsg(DL_NON,"\b \b");
            }
        }
        else
        {
            dmsg(DL_NON,"%c",ch);
            cmd_buf[console_char_cnt] = ch;
            console_char_cnt++;
        }
    }
}

void App_Console_Init(uint8_t ch,uint8_t priority,uint32_t baudrate,uint8_t parity)
{
    console_ch = ch;
    Hal_Uart_Init(ch,priority,baudrate,parity);

    UARTStdioConfig(ch, baudrate, SysCtlClockGet());
    
    dmsg(DL_NON,"\r\n\r\n");
}

