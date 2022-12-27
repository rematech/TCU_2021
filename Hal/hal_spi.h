
#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

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

#define SPI_FIFO_NO     8

// PORTD
#define PIN_SPI_CS0     GPIO_PIN_0
#define PIN_SPI_CS1     GPIO_PIN_1
#define PIN_SPI_CS2     GPIO_PIN_2
#define PIN_SPI_CS3     GPIO_PIN_3
#define PIN_SPI_CS4     GPIO_PIN_4
#define PIN_SPI_CS5     GPIO_PIN_5
#define PIN_SPI_CS6     GPIO_PIN_6
// PORTB
#define PIN_SPI_CS7     GPIO_PIN_6

#define SPI_CS_ENA      1
#define SPI_CS_DIS      0

typedef enum
{
    SPI_CS0 = 0,
    SPI_CS1,
    SPI_CS2,
    SPI_CS3,
    SPI_CS4,
    SPI_CS5,
    SPI_CS6,
    SPI_CS7,
    
    SPI_CS_MAX // Do not Change.

}SPI_CS_L;



/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/

uint8_t Api_Spi_Transmit(uint8_t ch,uint8_t *txdata,uint8_t *rxdata,uint32_t len);
uint8_t Api_Spi_Comm(uint8_t ch,uint8_t *txdata,uint8_t *rxdata,uint32_t len);


void spi_test(void);

void Api_Spi_CS_Sel(uint8_t no,uint8_t en);
void spi_dummy(uint8_t ch);

void Api_Spi_Master_Init(void);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __HAL_SPI_H__

