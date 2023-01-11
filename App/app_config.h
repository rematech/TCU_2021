
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

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

#define COP_NAME   "PICOANDTERA"
#define DEV_NAME   "FFS2000-TCU"

#define SW_DATE    "221220" 

#define SW_VER_MAJOR    1
#define SW_VER_MINOR    0
#define SW_VER_BUILD    3



#define CONSOLE_PORT    HAL_UART_CH_0
#define CONSOLE_BAUD    115200

#define HOST_IF_PORT    HAL_UART_CH_3
#define HOST_IF_BAUD    38400


/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/



//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __APP_CONFIG_H__

