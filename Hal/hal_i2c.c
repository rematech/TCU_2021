
#include "_include_header.h"

#define HIH_8000_ADDRESS    0x27 // !!! Current Address, Default : 0x27

void app_i2c_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false); // 100K
    
    
}


void app_i2c_write(void)
{
    dmsg(DL_INF,"app_i2c_write start\r\n");
    I2CMasterSlaveAddrSet(I2C0_BASE, HIH_8000_ADDRESS, false);
    
//    I2CMasterDataPut(I2C0_BASE, 0);

    //
    // Initiate send of data from the master.  Since the loopback
    // mode is enabled, the master and slave units are connected
    // allowing us to receive the same data that we sent out.
    //
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    //
    // Wait until the slave has received and acknowledged the data.
    //
    #if 0
    while(!(I2CSlaveStatus(I2C0_BASE) & I2C_SLAVE_ACT_RREQ))
    {
    }
    #endif
    while(I2CMasterBusy(I2C0_BASE)) {} // delay until complete
    dmsg(DL_INF,"app_i2c_write end\r\n");

}


void app_i2c_read(void)
{
    uint8_t rx_buf[5]={0};
    uint16_t humi;
    uint16_t temp;
    float hum, tmp;
    dmsg(DL_INF,"app_i2c_write start\r\n");

    I2CMasterSlaveAddrSet( I2C0_BASE, HIH_8000_ADDRESS, true);   // false = write, true = read

    /** Single Read
    */
    I2CMasterControl( I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    rx_buf[0] = I2CMasterDataGet(I2C0_BASE);
    I2CMasterControl( I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    rx_buf[1] = I2CMasterDataGet(I2C0_BASE);
    I2CMasterControl( I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    rx_buf[2] = I2CMasterDataGet(I2C0_BASE);
    I2CMasterControl( I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    rx_buf[3] = I2CMasterDataGet(I2C0_BASE);

    humi = (((rx_buf[0] & 0x3F) << 8) | rx_buf[1]); // Humidity 14bit Data
    temp = ((rx_buf[2] << 8) | rx_buf[3]) >> 2; // Temperature 14bit Data
    temp &= 0x3FFF;
    					
    hum = (float)humi/16382*100;
    tmp = (float)temp/16382*165-40;

    dmsg(DL_INF,"I2C Data [%02x][%02x][%02x][%02x] %f%% %fC\r\n",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],hum,tmp);
}

