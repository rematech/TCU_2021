/*
 * max31865.c
 *
 *  Created on: 2018. 11. 26.
 *      Author: bsy
 */

/*
 * RTD-to-Digital Convertor : MAX31865
 */

#include <stdint.h>
#include <stdbool.h>

#include "hal_gpio.h"
#include "app_gpio_config.h"
#include "app_console.h"

#include "app.h"
#include "app_ssr_ctrl.h"
#include "max31865.h"

#define TEMP_LOW    GPIO_LVL_LO
#define TEMP_HIGH   GPIO_LVL_HI

#define CLK_DELAY   1

extern void Hal_Delay_us(uint32_t us);

void Drv_MAX31865_GPIO_Pin_Init(void)
{

}

void Drv_Max31865_sclk( uint8_t port, uint8_t val )
{
    switch(port)
    {
        case TEMP_CH_1 :
            Hal_Gpo_Write(GPO_SCK1, val);
            break;

        case TEMP_CH_2 :
            Hal_Gpo_Write(GPO_SCK2, val);
            break;

        case TEMP_CH_3 :
            Hal_Gpo_Write(GPO_SCK3, val);
            break;

        default:

            break;
    }
}

void Drv_Max31865_din( uint8_t port, uint8_t val )
{
    switch(port)
    {
        case TEMP_CH_1 :
            Hal_Gpo_Write(GPO_SDI1, val);
            break;

        case TEMP_CH_2 :
            Hal_Gpo_Write(GPO_SDI2, val);
            break;

        case TEMP_CH_3 :
            Hal_Gpo_Write(GPO_SDI3, val);
            break;

        default:

            break;
    }
}

int32_t Drv_Max31865_drdy( uint8_t port )
{
    int32_t val = -1;
    
    switch(port)
    {
        case TEMP_CH_1 :
            val = Hal_Gpi_Read(GPI_DRDY1);
            break;

        case TEMP_CH_2 :
            val = Hal_Gpi_Read(GPI_DRDY2);
            break;

        case TEMP_CH_3 :
            val = Hal_Gpi_Read(GPI_DRDY3);
            break;

        default:

        break;
    }

    return val;
}

void Drv_Max31865_cs( uint8_t port, uint8_t val )
{
    switch(port)
    {
        case TEMP_CH_1 :
            Hal_Gpo_Write(GPO_CS1, val);
            break;

        case TEMP_CH_2 :
            Hal_Gpo_Write(GPO_CS2, val);
            break;

        case TEMP_CH_3 :
            Hal_Gpo_Write(GPO_CS3, val);
            break;

        default:

            break;
    }

}

int32_t Drv_Max31865_dout( int port )
{
    int32_t val = -1;
    
    switch(port)
    {
        case TEMP_CH_1 :
            val = Hal_Gpi_Read(GPI_SDO1);
            break;

        case TEMP_CH_2 :
            val = Hal_Gpi_Read(GPI_SDO2);
            break;

        case TEMP_CH_3 :
            val = Hal_Gpi_Read(GPI_SDO3);
            break;

        default:

        break;
    }

    return val;
}

unsigned char Drv_Max31865_Reg_Read( int port, unsigned char addr )
{
	int i;
	unsigned char data = 0;

	Drv_Max31865_cs( port, TEMP_HIGH );  // CS High
	// initialization

    Hal_Delay_us(CLK_DELAY);
    Drv_Max31865_cs( port, TEMP_LOW );
    Drv_Max31865_din( port, TEMP_LOW );
    Drv_Max31865_sclk( port, TEMP_LOW );        // CLK Low

	Hal_Delay_us(CLK_DELAY);

	for ( i = 7; i >= 0; i-- )
	{
    	Drv_Max31865_sclk( port, TEMP_HIGH );
		(addr&(1<<i)) ? Drv_Max31865_din( port, TEMP_HIGH ) : Drv_Max31865_din( port, TEMP_LOW );
		Hal_Delay_us(CLK_DELAY);
    	Drv_Max31865_sclk( port, TEMP_LOW );
		Hal_Delay_us(CLK_DELAY);
	}
	for ( i = 7; i >= 0; i-- )
	{
    	Drv_Max31865_sclk( port, TEMP_HIGH );
		Hal_Delay_us(CLK_DELAY);
    	Drv_Max31865_sclk( port, TEMP_LOW );
		data <<= 1;
		data |= ( Drv_Max31865_dout( port ) ? 1 : 0 );
		Hal_Delay_us(CLK_DELAY);
	}

	Hal_Delay_us(CLK_DELAY);
	Drv_Max31865_cs( port, TEMP_HIGH );  // CS High
	//Hal_Delay_us(CLK_DELAY);

	return data;
}

unsigned char Drv_Max31865_Reg_Write( int port, unsigned char addr, unsigned char val )
{
	int i;
	unsigned char data = 0;

	addr |= 0x80;

	Drv_Max31865_cs( port, TEMP_HIGH );  // CS High
	// initialization

	Hal_Delay_us(CLK_DELAY);

	Drv_Max31865_cs( port, TEMP_LOW );  // CS High
	Drv_Max31865_din( port, TEMP_LOW );
    Drv_Max31865_sclk( port, TEMP_LOW );        // CLK Low

	Hal_Delay_us(CLK_DELAY);

	for ( i = 7; i >= 0; i-- )
	{
    	Drv_Max31865_sclk( port, TEMP_HIGH );
		(addr&(1<<i)) ? Drv_Max31865_din( port, TEMP_HIGH ) : Drv_Max31865_din( port, TEMP_LOW );
		Hal_Delay_us(CLK_DELAY);
    	Drv_Max31865_sclk( port, TEMP_LOW );
		Hal_Delay_us(CLK_DELAY);
	}
	for ( i = 7; i >= 0; i-- )
	{
    	Drv_Max31865_sclk( port, TEMP_HIGH );
		(val&(1<<i)) ? Drv_Max31865_din( port, TEMP_HIGH ) : Drv_Max31865_din( port, TEMP_LOW );
		Hal_Delay_us(CLK_DELAY);
    	Drv_Max31865_sclk( port, TEMP_LOW );
		Hal_Delay_us(CLK_DELAY);
	}

	//Hal_Delay_us(CLK_DELAY);
	Drv_Max31865_cs( port, TEMP_HIGH );  // CS High
	//Hal_Delay_us(CLK_DELAY);

	return data;
}

void Drv_Max31865_Init_Config( int port )
{
	//unsigned char data;

	Drv_Max31865_Reg_Write( port, 0, 0xD2 ); // 0xD0

	Drv_Max31865_Reg_Read( port, 0 );
	//data = Drv_Max31865_Reg_Read( port, 0 );
	//UARTprintf("Drv_Max31865_Init_Config : Port %d - Config Reg 0x%02x \n\r", port, data);
}

unsigned char Drv_Max31865_Ready( int port )
{
	return (Drv_Max31865_drdy( port ) ? GPIO_LVL_LO : GPIO_LVL_HI);
}


/*
 * port : 1~
 * return
 *   -1 : fault
 *    0 : not ready
 *    1 : success
 */
 #if 0
int Drv_Max31865_Temp_Read( int port, float *temp )
{
	uint8_t data;
	uint16_t rtd;
	
	uint32_t loop = 0;

	while ( !Drv_Max31865_Ready( port ) ) { Hal_Delay_us(CLK_DELAY); if( loop++ > 100 ) return -1; }

	loop = 0;

	rtd = Drv_Max31865_Reg_Read( port, 1 );



    rtd <<= 8;
    while ( !Drv_Max31865_Ready( port ) ) { Hal_Delay_us(CLK_DELAY); if( loop++ > 10000 ) return -1; }

    rtd |= Drv_Max31865_Reg_Read( port, 2 );
    if ( rtd & 0x01 ) // Fault
    {
        data = Drv_Max31865_Reg_Read( port, 7 );
        UARTprintf("MAX31865 #%d Fault Status : 0x%02x\n\r", port, data);

        if ( data != 0 )
        return -1;
    }
    
    #if 0
    float adc;    
    adc = (float)rtd/2.0;
	*temp = adc/32.0 - 256.0;
    #else
    rtd >>= 1;
    *temp = (rtd/32.0) - 256.0;

    #endif

	//UARTprintf("RTD[%d]:0x%02x\n\r", port, rtd);

	return 1;
}
#else
int Drv_Max31865_Temp_Read( int port, float *temp )
{
	uint8_t data;
	uint16_t rtd;
	
	uint32_t loop = 0;

	while ( !Drv_Max31865_Ready( port ) ) { Hal_Delay_us(CLK_DELAY); if( loop++ > 100 ) return -1; }

	rtd = Drv_Max31865_Reg_Read( port, 1 );

    rtd <<= 8;
//    while ( !Drv_Max31865_Ready( port ) ) { Hal_Delay_us(CLK_DELAY); if( loop++ > 10000 ) return -1; }

    rtd |= Drv_Max31865_Reg_Read( port, 2 );
    if ( rtd & 0x01 ) // Fault
    {
        data = Drv_Max31865_Reg_Read( port, 7 );
//        UARTprintf("MAX31865 #%d Fault Status : 0x%02x\n\r", port, data);

        if ( data != 0 )
        return -1;
    }
    
    #if 0
    float adc;    
    adc = (float)rtd/2.0;
	*temp = adc/32.0 - 256.0;
    #else
    rtd >>= 1;
    *temp = (rtd/32.0) - 256.0;

    #endif

	//UARTprintf("RTD[%d]:0x%02x\n\r", port, rtd);

	return 1;
}
#endif
void Drv_Max31865_Chips_Init(uint8_t ch)
{
	int i;
//	unsigned char data;

	dmsg(DL_NUL,"MAX31865 %d Ch Init ----------------\r\n",ch);
	Drv_MAX31865_GPIO_Pin_Init();

	for ( i = 0; i < 8; i++ )
	{
        Drv_Max31865_Reg_Read( ch, i );
		//data = Drv_Max31865_Reg_Read( port, i );
		//UARTprintf("0x%02x ", data);
	}
	//UARTprintf("\n\r");

	Drv_Max31865_Init_Config(ch);
   
	
}



