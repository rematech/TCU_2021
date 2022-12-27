
#include "_include_header.h"



void Hal_Gpo_Write(uint8_t ch,uint8_t lvl)
{
    ASSERT_APP(ch < GPO_LIST_MAX);
    ASSERT_APP(lvl < GPIO_LVL_MAX);

    GPIOPinWrite(gpo_lists[ch].port, gpo_lists[ch].pin, (lvl==GPIO_LVL_LO) ? 0 : gpo_lists[ch].pin);
}

uint8_t Hal_Gpi_Read(uint8_t ch)
{
    ASSERT_APP(ch < GPI_LIST_MAX);

    return GPIOPinRead(gpi_lists[ch].port, gpi_lists[ch].pin) ? 1 : 0;
}

void Hal_Gpio_Pin_init(void)
{
    #if 1 // PD7 Pin Assign : NMI -> GPIO
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) |= GPIO_O_DIR;
    HWREG(GPIO_PORTD_BASE + GPIO_O_DEN) |= GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    #endif 
    
    #if 1 // PF0 Pin Assign : NMI -> GPIO
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_AFSEL) |= GPIO_O_DIR;
    HWREG(GPIO_PORTF_BASE + GPIO_O_DEN) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    #endif
    
    uint8_t ch;

    for(ch = 0; ch < GPI_LIST_MAX ; ch++)
    {
        GPIOPinTypeGPIOInput(gpi_lists[ch].port,gpi_lists[ch].pin);
    }
  
    for(ch = 0; ch < GPO_LIST_MAX ; ch++)
    {
        GPIOPinTypeGPIOOutput(gpo_lists[ch].port,gpo_lists[ch].pin);
        Hal_Gpo_Write(ch,gpo_lists[ch].init_lvl);
    }
}

void Hal_Gpio_Periph_En(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
}

void Hal_Gpio_Init(void)
{
    Hal_Gpio_Periph_En();
    
    Hal_Gpio_Pin_init();
}


