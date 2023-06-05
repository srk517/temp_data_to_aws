#ifndef ADC_TEMPERATURE_H
#define ADC_TEMPERATURE_H


#include <math.h>

/*#define DEFAULT_VREF    3300  // Default ADC reference voltage in mV
#define ADC_WIDTH       ADC_WIDTH_BIT_12
#define ADC_CHANNEL     ADC_CHANNEL_3 // Replace with the correct ADC channel
*/
float calculate_Temp(uint32_t adc_reading) {
    uint32_t avg=0,avg2=0;
    float resistance,Vout,t1,t2,t3,td;
    for(int i=0;i<10;i++)
    {
        avg+=adc_reading;     
    }
    avg=avg/10;
    
    adc_reading=avg;
    
    avg=0;
    Vout=adc_reading * 3.3/4095;
    resistance = (10000*5)/Vout;
    resistance=resistance-10000;
   

    t1=(log(resistance/10000));
    td=t1/3950;
    t2=1/298.15+td;
    t3=1/t2-275;
    return t3; 
}
#endif