

#include <stdint.h>
#include "driver/ledc.h"
#include "pwm.h"


void pwm_init(void){
    
    //Set PWM Configuration 
    ledc_timer_config_t pwm_timer = {
        .speed_mode = PWM_MODE,
        .timer_num = PWM_TIMER,
        .duty_resolution = PWM_DUTY_RES,
        .freq_hz = PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&pwm_timer));

    //Set Channel Configuration
    ledc_channel_config_t pwm_channel = {
        .speed_mode = PWM_MODE,
        .channel = PWM_CHANNEL,
        .timer_sel = PWM_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PWM_GPIO_PIN,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&pwm_channel));

}

void pwm_setDutyCycle(uint8_t dc){
    int max_dc;
    int duty_cycle;
    //Calculate duty cycle
    if(dc > 100){
        dc = 100;
    }
    //maximum duty cycle 
    max_dc = (1 << PWM_DUTY_RES) - 1;
    duty_cycle = (dc * max_dc) / 100;
    ESP_ERROR_CHECK(ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(PWM_MODE, PWM_CHANNEL));
}

