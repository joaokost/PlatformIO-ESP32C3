
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "button.h"
//#include "wifi.h"
#include "wifiAP.h"
#include "pwm.h"
#include "adc.h"


void app_main(void)
{

    //init_wifi_ap();
    
    led_init();
    button_init();
    pwm_init();
    pwm_setDutyCycle(50);
    adc_init();

    uint16_t adcValue = 0;
    uint16_t pwm_dc = 50;

    //Apply Delay
    vTaskDelay(5000 / portTICK_RATE_MS);

    while (true) 
    {
        led_run();

        adcValue = adc_getValue();
        printf("Raw: %d\n", adcValue);
        pwm_dc = ((adcValue * 25) / 4095) + 50;
        pwm_setDutyCycle(pwm_dc);

        /*
        pwm_setDutyCycle(button_pwmValue());
        printf("Button PWM Value: %d\n", button_pwmValue());
        */
    } 
    

}