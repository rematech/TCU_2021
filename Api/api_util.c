

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


float MAX_V_COUNT(uint8_t getcount, float* num,uint32_t cnt)
{
    float max_v[getcount], fTotal;
    uint32_t i, j, tempIndex;
	float fTemp[cnt];

	memcpy( fTemp, num, cnt);

	for( j=0; j<getcount; j++)
	{
    	max_v[j]=fTemp[0];
	    for(i=1;i<cnt;i++)
	    {
	        if(max_v[j]<fTemp[i])
	        {
	        	max_v[j]=fTemp[i];
				tempIndex = i;
	        }
	    }

		fTemp[tempIndex] = 0;
	}

	for(i=0; i<getcount; i++)
		fTotal += max_v[i];
	
    return fTotal;
}

float MIN_V_COUNT(uint8_t getcount, float* num,uint32_t cnt)
{
    float min_v[getcount], fTotal;
    uint32_t i, j, tempIndex;
	float fTemp[cnt];

	memcpy( fTemp, num, cnt);

	for( j=0; j<getcount; j++)
	{
    	min_v[j]=fTemp[0];
	    for(i=1;i<cnt;i++)
	    {
	        if(min_v[j]>fTemp[i])
	        {
	        	min_v[j]=fTemp[i];
				tempIndex = i;
	        }
	    }

		fTemp[tempIndex] = 0;
	}

	for(i=0; i<getcount; i++)
		fTotal += min_v[i];
	
    return fTotal;
}

float AVERAGE_V_FILTER(float* num,uint32_t cnt, float fExpectionThreshold)
{
    float av;
	float threshold_high, threshold_low;
	float sum = 0;
    uint32_t i;
	uint8_t u8_count = 0;


    for(i=0;i<cnt;i++)
    {
        sum += num[i];
    }
	
	av = sum / cnt;

	if( fExpectionThreshold == 0 )
		return av;	

	//Filter
	sum = 0;
	threshold_high = av + fExpectionThreshold;
	threshold_low = av - fExpectionThreshold;
	for(i=0;i<cnt;i++)
    {
    	if( num[i] >= threshold_high )
			continue;

		if( num[i] <= threshold_low )
			continue;
		
        sum += num[i];
		u8_count++;
    }

	av = sum / u8_count;

	return av;


	
}


// get a fraction part as an int
int32_t get_fraction(float val)
{
#if 1
	int32_t integer;

	if(val >= 0)
	{
		integer = val * 100;
		integer = integer%100;
	}
	else
	{
		integer = -val * 100;
		integer = (integer%100);
	}

	return integer;

#else
    int32_t integer, fraction;

    integer = (int32_t)val;
    fraction = (int32_t)(val*100.0f);
    fraction = fraction - (integer*100);
    if ( fraction < 0 )
        fraction *= -1;

    return fraction;
#endif

}
