
#include "_include_header.h"

static const uint32_t hal_uart_port[HAL_UART_CH_MAX] =
{
    SYSCTL_PERIPH_GPIOA, SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_GPIOD, SYSCTL_PERIPH_GPIOC, 
    SYSCTL_PERIPH_GPIOC, SYSCTL_PERIPH_GPIOE, SYSCTL_PERIPH_GPIOD, SYSCTL_PERIPH_GPIOE
};

static const uint32_t hal_uart_sysctl[HAL_UART_CH_MAX] =
{
    SYSCTL_PERIPH_UART0, SYSCTL_PERIPH_UART1, SYSCTL_PERIPH_UART2, SYSCTL_PERIPH_UART3, 
    SYSCTL_PERIPH_UART4, SYSCTL_PERIPH_UART5, SYSCTL_PERIPH_UART6, SYSCTL_PERIPH_UART7
};

static const uint32_t hal_uart_base[HAL_UART_CH_MAX] =
{
    UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE, UART4_BASE, UART5_BASE, UART6_BASE, UART7_BASE
};

static const uint32_t hal_uart_int_assign[HAL_UART_CH_MAX] =
{
    INT_UART0, INT_UART1, INT_UART2, INT_UART3, INT_UART4, INT_UART5, INT_UART6, INT_UART7
};


static volatile uint8_t uart_rx_wp[HAL_UART_CH_MAX]={0,}, uart_rx_rp[HAL_UART_CH_MAX]={0,}, uart_rx_cnt[HAL_UART_CH_MAX]={0,};
static volatile uint8_t uart_rx[HAL_UART_CH_MAX][UART_RX_BUF_SIZE]={0,};


uint8_t Hal_Uart_Rcv_Len(uint8_t ch)
{
    return uart_rx_cnt[ch];
}


uint8_t Hal_Uart_Get_Char(uint8_t ch)
{
    uint8_t val;

    while(uart_rx_cnt[ch] == 0);

    val = uart_rx[ch][uart_rx_rp[ch]];
    uart_rx_rp[ch] = (uart_rx_rp[ch] + 1) % UART_RX_BUF_SIZE;
    uart_rx_cnt[ch]--;
    
    return val;
}

uint8_t Hal_Uart_Data_Fill(uint8_t ch)
{
    int32_t str;
    
    if(uart_rx_cnt[ch] < UART_RX_BUF_SIZE)
    {
        str = UARTCharGetNonBlocking(hal_uart_base[ch]);
        if(str != -1)
        {
            uart_rx[ch][uart_rx_wp[ch]] = (uint8_t)(str & 0xFF);
            uart_rx_wp[ch] = (uart_rx_wp[ch] + 1) % UART_RX_BUF_SIZE;
            uart_rx_cnt[ch]++;
        }
        // else dmsg(DL_ERR,"CH:%d%x\r\n",ch,str);
    }
    
    return 0;
}

void Hal_Uart_Data_Rcv(uint8_t ch)
{
    uint32_t ui32Status;

    ui32Status = UARTIntStatus(hal_uart_base[ch], true);

    UARTIntClear(hal_uart_base[ch], ui32Status);

    while(UARTCharsAvail(hal_uart_base[ch]))
    {
        Hal_Uart_Data_Fill(ch);
    }

}


//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void Hal_Uart0_Int_Handler(void)
{
    uint8_t ch = HAL_UART_CH_0;

    Hal_Uart_Data_Rcv(ch);
}

void Hal_Uart1_Int_Handler(void)
{
    uint8_t ch = HAL_UART_CH_1;
    
    Hal_Uart_Data_Rcv(ch);
}

void Hal_Uart2_Int_Handler(void)
{
    uint8_t ch = HAL_UART_CH_2;
    
    Hal_Uart_Data_Rcv(ch);
}

void Hal_Uart3_Int_Handler(void)
{
    uint8_t ch = HAL_UART_CH_3;
    
    Hal_Uart_Data_Rcv(ch);
}

void Hal_Uart4_Int_Handler(void)
{
    uint8_t ch = HAL_UART_CH_4;
    
    Hal_Uart_Data_Rcv(ch);
}

void Hal_Uart5_Int_Handler(void)
{
    uint8_t ch = HAL_UART_CH_5;
    
    Hal_Uart_Data_Rcv(ch);
}

void Hal_Uart6_Int_Handler(void)
{
    uint8_t ch = HAL_UART_CH_6;
    
    Hal_Uart_Data_Rcv(ch);
}

void Hal_Uart7_Int_Handler(void)
{
    uint8_t ch = HAL_UART_CH_7;
    
    Hal_Uart_Data_Rcv(ch);
}




//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void Hal_Uart_Send(uint8_t ch,const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        UARTCharPut(hal_uart_base[ch], *pui8Buffer++);
    }
}


void Hal_Uart_Init(uint8_t ch,uint8_t priority,uint32_t baud,uint8_t parity)
{
    uint32_t config;
    
    SysCtlPeripheralEnable(hal_uart_port[ch]);
    SysCtlPeripheralEnable(hal_uart_sysctl[ch]);
    
    switch(ch)
    {
        case HAL_UART_CH_0: GPIOPinConfigure(GPIO_PA0_U0RX); GPIOPinConfigure(GPIO_PA1_U0TX); 
                            GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); break;
                        
        case HAL_UART_CH_1: GPIOPinConfigure(GPIO_PC4_U1RX); GPIOPinConfigure(GPIO_PC5_U1TX); 
                            GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5); break;
                        
        case HAL_UART_CH_2: GPIOPinConfigure(GPIO_PD6_U2RX); GPIOPinConfigure(GPIO_PD7_U2TX); 
                            GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7); break;
                        
        case HAL_UART_CH_3: GPIOPinConfigure(GPIO_PC6_U3RX); GPIOPinConfigure(GPIO_PC7_U3TX); 
                            GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7); break;
                        
        case HAL_UART_CH_4: GPIOPinConfigure(GPIO_PC4_U4RX); GPIOPinConfigure(GPIO_PC5_U4TX); 
                            GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5); break;
                        
        case HAL_UART_CH_5: GPIOPinConfigure(GPIO_PE4_U5RX); GPIOPinConfigure(GPIO_PE5_U5TX); 
                            GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5); break;
                        
        case HAL_UART_CH_6: GPIOPinConfigure(GPIO_PD4_U6RX); GPIOPinConfigure(GPIO_PD5_U6TX); 
                            GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5); break;
                        
        case HAL_UART_CH_7: GPIOPinConfigure(GPIO_PE0_U7RX); GPIOPinConfigure(GPIO_PE1_U7TX); 
                            GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1); break;
                        
        default : break;
        
    }
    
    config = (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE );
    
    switch(parity)
    {
        case PARITY_NONE: config |= UART_CONFIG_PAR_NONE; break;
        case PARITY_ODD:  config |= UART_CONFIG_PAR_ODD;  break;
        case PARITY_EVEN: config |= UART_CONFIG_PAR_EVEN; break;
        default : break;
    }
        
    UARTConfigSetExpClk(hal_uart_base[ch], SysCtlClockGet(), baud, config);
    IntPrioritySet(hal_uart_int_assign[ch], priority);
    IntEnable(hal_uart_int_assign[ch]);
    UARTIntEnable(hal_uart_base[ch], UART_INT_RX | UART_INT_RT);
}



