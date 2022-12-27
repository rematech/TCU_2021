#ifndef __MAX31865_H__
#define __MAX31865_H__

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


#define TEMPERATURE_LIMIT_HI   80
#define TEMPERATURE_LINIT_MIN  0
#define TEMPERATURE_LOW_CUT    5

#define TEMPERATURE_STEP1    1.7
#define TEMPERATURE_STEP2    2.5
#define TEMPERATURE_STEP3    3


#define TEMPERATURE_ERROR   999
#define TEMP_ERR_CNT_MAX    10

/******************************************************************************/
/* variables                                                                  */
/******************************************************************************/


/******************************************************************************/
/* functions                                                                 */
/******************************************************************************/
void Drv_MAX31865_GPIO_Pin_Init(void);
void Drv_Max31865_sclk( uint8_t port, uint8_t val );
void Drv_Max31865_din( uint8_t port, uint8_t val );
int32_t Drv_Max31865_drdy( uint8_t port );
void Drv_Max31865_cs( uint8_t port, uint8_t val );
int32_t Drv_Max31865_dout( int port );
unsigned char Drv_Max31865_Reg_Read( int port, unsigned char addr );
unsigned char Drv_Max31865_Reg_Write( int port, unsigned char addr, unsigned char val );
void Drv_Max31865_Init_Config( int port );
unsigned char Drv_Max31865_Ready( int port );
void Drv_Max31865_Chips_Init(uint8_t ch);
int Drv_Max31865_Temp_Read( int port, float *temp );


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __MAX31865_H__

