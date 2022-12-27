#ifndef __APP_GPIO_CONFIG_H__
#define __APP_GPIO_CONFIG_H__

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

#include "hal_gpio.h"

/******************************************************************************/
/* define                                                                     */
/******************************************************************************/

#if 0
// PORTA
#define PIN_I_DRDY3         GPIO_PIN_2
#define PIN_I_SDO3          GPIO_PIN_3
#define PIN_O_CS3           GPIO_PIN_4
#define PIN_O_SCK3          GPIO_PIN_5
#define PIN_O_SDI3          GPIO_PIN_6

// PORTB

// PORTC

// PORTD
#define PIN_O_TEMP_ALARM3   GPIO_PIN_0

#define PIN_O_SSR1          GPIO_PIN_1
#define PIN_O_SSR2          GPIO_PIN_2
#define PIN_O_SSR3          GPIO_PIN_3

// PORTE
#define PIN_O_TEMP_ALARM1   GPIO_PIN_4
#define PIN_O_TEMP_ALARM2   GPIO_PIN_5

// PORTF
#define PIN_I_DRDY2         GPIO_PIN_0
#define PIN_I_SDO2          GPIO_PIN_1
#define PIN_O_CS2           GPIO_PIN_2
#define PIN_O_SCK2          GPIO_PIN_3


// PORTG
#define PIN_O_SDI1          GPIO_PIN_0
#define PIN_O_SCK1          GPIO_PIN_1
#define PIN_O_CS1           GPIO_PIN_2
#define PIN_I_SDO1          GPIO_PIN_3
#define PIN_I_DRDY1         GPIO_PIN_4
#define PIN_O_SDI2          GPIO_PIN_5
#endif
typedef enum
{
    GPO_CS3 = 0, 
    GPO_SCK3,
    GPO_SDI3,
    
    GPO_TEMP_ALARM3,
    GPO_SSR1,
    GPO_SSR2,
    GPO_SSR3,
    
    GPO_TEMP_ALARM1,
    GPO_TEMP_ALARM2,
    
    GPO_CS2,
    GPO_SCK2,
    
    GPO_SDI1,
    GPO_SCK1,
    GPO_CS1,
    GPO_SDI2,

    GPO_LIST_MAX, // Do not Change.
}GPO_L;

typedef enum
{
    GPI_DRDY3 = 0,
    GPI_SDO3,
    GPI_DRDY2,
    GPI_SDO2,
    GPI_DRDY1,
    GPI_SDO1,
    
    GPI_LIST_MAX, // Do not Change.
}GPI_L;

/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/

extern const GPIO_FIELD gpo_lists[];
extern const GPIO_FIELD gpi_lists[];

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

#endif // __APP_GPIO_CONFIG_H__

