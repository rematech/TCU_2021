
#include "_include_header.h"

#define NUM_SSI_DATA 10

uint8_t ulDataTx[NUM_SSI_DATA]={0};
uint8_t ulDataRx[NUM_SSI_DATA]={0};
uint32_t ulindex;

void spi_dummy(uint8_t ch)
{
    uint8_t dummy_tx[2];
    uint8_t dummy_rx[2]; 
    
    Api_Spi_CS_Sel(ch,SPI_CS_ENA);
  
    while(SSIDataGetNonBlocking(SSI0_BASE, (uint32_t*)dummy_rx))
    {       
    }

    SSIDataPut(SSI0_BASE, (uint32_t)dummy_tx);


    while(SSIBusy(SSI0_BASE))
    {
    }

//    SSIDataGet(SSI0_BASE, (uint32_t*)dummy_rx);
    
 //   Api_Spi_CS_Sel(ch,SPI_CS_DIS);

}

uint8_t Api_Spi_Transmit(uint8_t ch,uint8_t *txdata,uint8_t *rxdata,uint32_t len)
{  
    uint32_t cnt=0;
    uint32_t tx_bf[SPI_FIFO_NO]={0},rx_bf[SPI_FIFO_NO]={0};
    uint8_t i=0;

    //dmsg(DL_DBG,"SPI %d ch transmit \r\n",ch);
    //dmsg(DL_DBG,"SPI Sent: "); for(cnt = 0; cnt < len; cnt++) UARTprintf("%x ", txdata[cnt]); UARTprintf("\r\n");

    Api_Spi_CS_Sel(ch,SPI_CS_ENA);
    SysCtlDelay(100);

    for(i=0;i<len;i++) tx_bf[i] = txdata[i];
 
    while(SSIDataGetNonBlocking(SSI0_BASE, tx_bf)) ;
   
    for(cnt = 0; cnt < len; cnt++) SSIDataPut(SSI0_BASE, tx_bf[cnt]);
  
    while(SSIBusy(SSI0_BASE)) ;
  
    for(cnt = 0; cnt < len; cnt++) SSIDataGet(SSI0_BASE, &rx_bf[cnt]);
 
    SysCtlDelay(10);
    Api_Spi_CS_Sel(ch,SPI_CS_DIS);

    for(i=0;i<len;i++)
    {
        rxdata[i] = rx_bf[i] & 0x00FF;
    }
    
//    dmsg(DL_INF,"SPI RCV: "); for(cnt = 0; cnt < len; cnt++) UARTprintf("%x ", rx_bf[cnt]); UARTprintf("\r\n");
    
    return 0;
}

uint8_t Api_Spi_Comm(uint8_t ch,uint8_t *txdata,uint8_t *rxdata,uint32_t len)
{  
    uint32_t cnt=0;
    uint32_t i=0;
    uint32_t cycle;
    uint8_t tx_bf[SPI_FIFO_NO]={0},rx_bf[SPI_FIFO_NO]={0};

    for(i=0;i<SPI_FIFO_NO;i++) tx_bf[i] = txdata[i];
 
    if(len <= SPI_FIFO_NO)
    {
        cycle = 1;
    }
    else
    {
        cycle = len / SPI_FIFO_NO;
        cycle ++;  
    }

    Api_Spi_Transmit(ch,tx_bf,rx_bf,SPI_FIFO_NO);

    Api_Delay_ms(2);
    
    for(cnt = 0 ; cnt < cycle ; cnt++)
    {
        switch(tx_bf[0])
        {
            

            default:
                break;
        }
       
        SysCtlDelay(4000); // slave data set up delay.
        Api_Spi_Transmit(ch,tx_bf,&rxdata[cnt*SPI_FIFO_NO],SPI_FIFO_NO);
    }

    return 0;
}

void spi_test(void)
{  
    #define TEST_CNT    4
    
    dmsg(DL_INF,"spi_test\r\n");
    Api_Spi_CS_Sel(SPI_CS0,SPI_CS_ENA);
     
    while(SSIDataGetNonBlocking(SSI0_BASE,(uint32_t*)&ulDataRx[0]))
    {
    }
    strncpy((char*)ulDataTx,"test",strlen("test"));
    dmsg(DL_INF,"SPI Sent:\r\n  ");


    UARTprintf("\r\n");
    
    for(ulindex = 0; ulindex < TEST_CNT; ulindex++)
        UARTprintf("'%x' ", ulDataTx[ulindex]);
    UARTprintf("\r\n");
    
    for(ulindex = 0; ulindex < TEST_CNT; ulindex++)
    {
        SSIDataPut(SSI0_BASE, ulDataTx[ulindex]);
    }
    
    while(SSIBusy(SSI0_BASE))
    {

    }
    
    dmsg(DL_INF,"SPI rcv:\r\n  ");

    for(ulindex = 0; ulindex < TEST_CNT; ulindex++)
    {
        SSIDataGet(SSI0_BASE, (uint32_t*)&ulDataRx[ulindex]);
        ulDataRx[ulindex] &= 0x00FF;
    }
    for(ulindex = 0; ulindex < TEST_CNT; ulindex++)    
        UARTprintf("'%x' ", ulDataRx[ulindex]);
    UARTprintf("\r\n");
    Api_Spi_CS_Sel(SPI_CS0,SPI_CS_DIS);

}


void Api_Spi_CS_Sel(uint8_t no,uint8_t en)
{
    uint32_t port;
    uint8_t pin,val;

    switch(no)
    {
        case SPI_CS0:
            port = GPIO_PORTD_BASE;
            pin = PIN_SPI_CS0;
            break;
        case SPI_CS1:
            port = GPIO_PORTD_BASE;
            pin = PIN_SPI_CS1;
            break;
        case SPI_CS2:
            port = GPIO_PORTD_BASE;
            pin = PIN_SPI_CS2;
            break;
        case SPI_CS3:
            port = GPIO_PORTD_BASE;
            pin = PIN_SPI_CS3;
            break;
        case SPI_CS4:
            port = GPIO_PORTD_BASE;
            pin = PIN_SPI_CS4;
            break;
        case SPI_CS5:
            port = GPIO_PORTD_BASE;
            pin = PIN_SPI_CS5;
            break;
        case SPI_CS6:
            port = GPIO_PORTD_BASE;
            pin = PIN_SPI_CS6;
            break;
        case SPI_CS7:
            port = GPIO_PORTB_BASE;
            pin = PIN_SPI_CS7;
            break;

        default:
            dmsg(DL_ERR,"SPI CS SETTING ERR\r\n");
            return;
    }

    switch(en)
    {
        case SPI_CS_ENA: // ENABLE
            val = 0;
            break;

        case SPI_CS_DIS: // DISABLE
            val = pin;
            break;

        default:
            dmsg(DL_ERR,"SPI CS SETTING ERR\r\n");
            return;
    }

    GPIOPinWrite(port, pin, val);    
    SysCtlDelay(1);
}


void Api_Spi_Master_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
//    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, PIN_SPI_CS0 | PIN_SPI_CS1 | PIN_SPI_CS2 | PIN_SPI_CS3 |
                                           PIN_SPI_CS4 | PIN_SPI_CS5 | PIN_SPI_CS6 | PIN_SPI_CS7 );

    uint8_t i;
    for(i=0;i<SPI_CS_MAX;i++)
    {
        Api_Spi_CS_Sel(i,SPI_CS_DIS);
    }

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2);

    //
    // Configure and enable the SSI port for SPI master mode.  Use SSI0,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    // For SPI mode, you can set the polarity of the SSI clock when the SSI
    // unit is idle.  You can also configure what clock edge you want to
    // capture data on.  Please reference the datasheet for more information on
    // the different SPI modes.
    //
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, 1000000, 8);

    
    //
    // Enable the SSI0 module.
    //
    SSIEnable(SSI0_BASE);

}


