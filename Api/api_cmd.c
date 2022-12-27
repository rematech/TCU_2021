
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "api_cmd.h"



static char *g_ppcArgv[CMDLINE_MAX_ARGS + 1];


int CmdLineProcess(char *pcCmdLine)
{
    char *pcChar;
    uint_fast8_t ui8Argc;
    bool bFindArg = true;
    console_cmds_t *psCmdEntry;

    ui8Argc = 0;
    pcChar = pcCmdLine;

    while(*pcChar)
    {
        if(*pcChar == ' ')
        {
            *pcChar = 0;
            bFindArg = true;
        }
        else
        {
            if(bFindArg)
            {
                if(ui8Argc < CMDLINE_MAX_ARGS)
                {
                    g_ppcArgv[ui8Argc] = pcChar;
                    ui8Argc++;
                    bFindArg = false;
                }
                else
                {
                    return(CMDLINE_TOO_MANY_ARGS);
                }
            }
        }
        pcChar++;
    }
    if(ui8Argc)
    {
        psCmdEntry = cmd_list;

        while(psCmdEntry->pcCmd)
        {
            if(!strcmp(g_ppcArgv[0], psCmdEntry->pcCmd))
            {
                return(psCmdEntry->pfnCmd(ui8Argc, g_ppcArgv));
            }
            psCmdEntry++;
        }
    }
    return(CMDLINE_BAD_CMD);
}


