

#ifndef __API_CMD_H__
#define __API_CMD_H__


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CMDLINE_MAX_ARGS
#define CMDLINE_MAX_ARGS        5
#endif


#define CMDLINE_SUC            (0)
#define CMDLINE_BAD_CMD        (-1)
#define CMDLINE_TOO_MANY_ARGS  (-2)
#define CMDLINE_TOO_FEW_ARGS   (-3)
#define CMDLINE_INVALID_ARG    (-4)


typedef int (*cmd_func_t)(int argc, char *argv[]);

typedef struct
{   
    const char *pcCmd;  
    cmd_func_t pfnCmd;
    const char *phelp_msg;
}console_cmds_t;



extern console_cmds_t cmd_list[];

extern int CmdLineProcess(char *pcCmdLine);



#ifdef __cplusplus
}
#endif

#endif // __API_CMD_H__
