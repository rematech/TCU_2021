#ifndef __APP_CONSOLE_H__
#define __APP_CONSOLE_H__

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
    DL_NUL  = 0,
    DL_NON  ,
    DL_INF  ,
    DL_DBG  ,
    DL_RAW  ,
    
    DL_WRN  = 6,
    DL_ERR  = 7,
    DL_MSG_NO_MAX
}DEBUG_LEVEL_T;


#define TEST_DEBUG_LVL      (_BIT(DL_NON)|_BIT(DL_INF)|_BIT(DL_DBG)|_BIT(DL_WRN)|_BIT(DL_ERR)|_BIT(DL_RAW))
#define NORMAL_DEBUG_LVL    (_BIT(DL_NON)|_BIT(DL_INF)|_BIT(DL_WRN)|_BIT(DL_ERR))


#define DEFAULT_DEBUG_LVL   NORMAL_DEBUG_LVL


#define MACRO_DEBUG_RAW_PRINT   (Debug_Lv_Get() & _BIT(DL_RAW))
#define MACRO_DEBUG_WRN_PRINT   (Debug_Lv_Get() & _BIT(DL_WRN))
#define MACRO_DEBUG_ERR_PRINT   (Debug_Lv_Get() & _BIT(DL_DBG))


#define ASCII_CR    0x0D
#define ASCII_LF    0x0A
#define ASCII_BS    0x08


#define ASSERT_APP(expr) { if(!(expr)) { dmsg(DL_ERR,"ASSERT FAILED [%s:%d] \r\n",__func__,__LINE__); while(1); } }

/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/


int dmsg(DEBUG_LEVEL_T deb_lvl, const char *fmt, ...);

uint8_t Debug_Lv_Get(void);
int cmd_debug_lvl(int argc, char* argv[]);
int Cmd_info(int argc, char *argv[]);
int Cmd_show(int argc, char *argv[]);
int Cmd_reset(int argc, char *argv[]);
int Cmd_help(int argc, char *argv[]);

void App_Console_Cmd_Check(void);
void App_Console_Data_Rcv(void);

void App_Console_Init(uint8_t ch,uint8_t priority,uint32_t baudrate,uint8_t parity);



// Added New CMD

int cmd_set_sp(int argc, char** argv);

int cmd_data_display(int argc, char** argv);

int cmd_pid_mode(int argc, char** argv);
int cmd_pid_mode_all(int argc, char** argv);

int cmd_heater_ctrl(int argc, char** argv);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __APP_CONSOLE_H__

