#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

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
    GPIO_LVL_LO = 0,
    GPIO_LVL_HI,

    GPIO_LVL_MAX, // Do not Change.
}GPIO_LVL_L;

typedef struct
{
    uint32_t port;
    uint32_t pin;
    uint8_t  init_lvl;
    uint8_t  *str;
}GPIO_FIELD;



/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

void Hal_Gpo_Write(uint8_t ch,uint8_t lvl);
uint8_t Hal_Gpi_Read(uint8_t ch);

void Hal_Gpio_Pin_init(void);
void Hal_Gpio_Periph_En(void);
void Hal_Gpio_Init(void);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __HAL_GPIO_H__

