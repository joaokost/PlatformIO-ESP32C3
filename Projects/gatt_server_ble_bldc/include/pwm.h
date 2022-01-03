

#include <stdint.h>
#include "driver/ledc.h"


#define PWM_TIMER       LEDC_TIMER_0
#define PWM_MODE        LEDC_LOW_SPEED_MODE
#define PWM_GPIO_PIN    (5)
#define PWM_CHANNEL     LEDC_CHANNEL_0
#define PWM_DUTY_RES    LEDC_TIMER_14_BIT
#define PWM_FREQ_HZ     (432)    


void pwm_init(void);
void pwm_setDutyCycle(uint8_t dc);