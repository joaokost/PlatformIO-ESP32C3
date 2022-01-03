

#include <stdint.h>
#include "soc/adc_channel.h"
#include "adc.h"


void adc_init(void){
    adc1_config_width(adc_width);
    adc1_config_channel_atten(adc_channel, adc_atten);
}

uint16_t adc_getValue(void){
    return (adc1_get_raw((adc1_channel_t)adc_channel));
}