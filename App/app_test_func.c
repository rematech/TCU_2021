

#include "_include_header.h"

void flaot_test(void)
{
    uint8_t buff[4]={0};
    uint8_t buff1[4]={0};
    uint16_t b_16 = 300;
    
    float f_buf;
    float f_buf1;

    memcpy(buff,(uint8_t*)(&b_16),2);

    dmsg(DL_INF,"buff0 :%x\r\n",buff[0]);
    dmsg(DL_INF,"buff1 :%x\r\n",buff[1]);

    
    memcpy(&b_16,buff,2);
    
    dmsg(DL_INF,"buff :%d\r\n",b_16);

    buff[0] = *((uint8_t*)(&b_16)+0);
    buff[1] = *((uint8_t*)(&b_16)+1);
   
    dmsg(DL_INF,"buff0 :%x\r\n",buff[0]);
    dmsg(DL_INF,"buff1 :%x\r\n",buff[1]);

    *((uint8_t*)(&f_buf)+3) = buff[0];
    *((uint8_t*)(&f_buf)+2) = buff[1];
    *((uint8_t*)(&f_buf)+1) = buff[2];
    *((uint8_t*)(&f_buf)+0) = buff[3];

    f_buf = 0.0;
    buff[0] = *((uint8_t*)(&f_buf)+0);
    buff[1] = *((uint8_t*)(&f_buf)+1);
    buff[2] = *((uint8_t*)(&f_buf)+2);
    buff[3] = *((uint8_t*)(&f_buf)+3);

    dmsg(DL_INF,"buff :%x\r\n",buff[0]);
    dmsg(DL_INF,"buff :%x\r\n",buff[1]);
    dmsg(DL_INF,"buff :%x\r\n",buff[2]);
    dmsg(DL_INF,"buff :%x\r\n",buff[3]);
    
    memcpy(buff1,(uint8_t*)(&f_buf),4);
    dmsg(DL_INF,"buff :%x\r\n",buff1[0]);
    dmsg(DL_INF,"buff :%x\r\n",buff1[1]);
    dmsg(DL_INF,"buff :%x\r\n",buff1[2]);
    dmsg(DL_INF,"buff :%x\r\n",buff1[3]);

    memcpy((uint8_t*)(&f_buf1),buff1,4);
    if(f_buf1 == f_buf) dmsg(DL_INF,"Same\r\n");


    f_buf = 0.0;
    buff[0] = *((uint8_t*)(&f_buf)+0);
    buff[1] = *((uint8_t*)(&f_buf)+1);
    buff[2] = *((uint8_t*)(&f_buf)+2);
    buff[3] = *((uint8_t*)(&f_buf)+3);

    dmsg(DL_INF,"buff :%x\r\n",buff[0]);
    dmsg(DL_INF,"buff :%x\r\n",buff[1]);
    dmsg(DL_INF,"buff :%x\r\n",buff[2]);
    dmsg(DL_INF,"buff :%x\r\n",buff[3]);

    while(1);
}


#define CPU_CLOCK   16000000
#define TIM_FRSCL   1024.0


void pwm_calc(void)
{
    unsigned char hz = 60;
    float period;
    float resolution;
    unsigned int top;


    period = 1.0 / hz;
    resolution = TIM_FRSCL / CPU_CLOCK;
    top = (unsigned int)(period / resolution);

    dmsg(DL_INF,"Top :%d\r\n",top);
}

