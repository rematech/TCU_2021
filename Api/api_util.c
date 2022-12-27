

#include "api_util.h"

uint8_t Api_CheckSum_Encoding(uint8_t *check_data,uint32_t data_len)
{
    uint8_t data_sum = 0;
 
    for (;data_len > 0;data_len--)  data_sum += *check_data++;
    
    data_sum = ~data_sum;
    data_sum +=1;
    
    return data_sum;
}

uint8_t Api_CheckSum_Decoding(uint8_t *check_data,uint32_t data_len,uint8_t check_sum)
{
    uint8_t data_sum = 0;
 
    for (;data_len > 0;data_len--)  data_sum += *check_data++;
    
    return ((check_sum + data_sum) & 0x00ff); // 0 is Success.
}

float MAX_V(float* num,uint32_t cnt)
{
    float max_v;
    uint32_t i;

    max_v=num[0];
    for(i=1;i<cnt;i++)
    {
        if(max_v<num[i])
        max_v=num[i];
    }
    return max_v;
}

float MIN_V(float* num,uint32_t cnt)
{
    float min_v;
    uint32_t i;

    min_v=num[0];
    for(i=1;i<cnt;i++)
    {
        if(min_v>num[i])
        min_v=num[i];
    }
    return min_v;
}


