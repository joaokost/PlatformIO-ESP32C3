

#include <stdint.h>
#include "driver/gpio.h"
#include "driver/adc.h"
#include "soc/adc_channel.h"


#define DEFAULT_VREF    1100
#define NO_OF_SAMPLES   64

static const adc_channel_t adc_channel = ADC_CHANNEL_0;
static const adc_bits_width_t adc_width = ADC_WIDTH_BIT_12;
static const adc_atten_t adc_atten = ADC_ATTEN_DB_0;


void adc_init(void);
uint16_t adc_getValue(void);