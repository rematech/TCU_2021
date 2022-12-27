#ifndef __INLCUDE_HEADER__
#define __INLCUDE_HEADER__

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

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "_header_core.h"
#include "defines.h"

#include "utils/uartstdio.h"

#include "hal_delay.h"
#include "hal_interrupt.h"
#include "hal_gpio.h"
#include "hal_timer.h"
#include "hal_uart.h"
#include "hal_spi.h"
#include "hal_i2c.h"
#include "hal_flash_pb.h"
#include "hal_eeprom_pb.h"


#include "main.h"
#include "api_systick.h"
#include "app_config.h"
#include "app_gpio_config.h"


#include "api_util.h"

#include "app.h"


#include "app_host_if.h"

#include "api_cmd.h"
#include "app_console.h"

#include "max31865.h"
#include "api_filter_fir.h"
#include "api_pid.h"
#include "api_autotune.h"
#include "app_ssr_ctrl.h"

/******************************************************************************/
/* define                                                                     */
/******************************************************************************/



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

#endif // __INLCUDE_HEADER__


