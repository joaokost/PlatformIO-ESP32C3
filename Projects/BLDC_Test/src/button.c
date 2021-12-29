
#include <stddef.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "led.h"
#include "button.h"


uint8_t led_change = 1;
uint16_t button_pwm_value = 50;

static void button_handler(void *arg);

void button_init(void)
{
    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_DEF_INPUT;
    io_config.pin_bit_mask = GPIO_BUTTON_PIN_SEL;
    io_config.intr_type = GPIO_INTR_NEGEDGE;
    io_config.pull_up_en = 1;
    gpio_config(&io_config);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_BUTTON, button_handler, NULL);
}

static void IRAM_ATTR button_handler(void *arg)
{
    button_pwm_value++;
    if(button_pwm_value > 80){
        button_pwm_value = 50;
    }
    /*switch(led_change)
    {
        case 0:
            led_TurnOffAll();
        break;

        case 1:
            led_Blink(RED, 1000);
        break;

        case 2:
            led_TurnOff(RED);
            led_Blink(GREEN, 500);
        break;

        case 3:
            led_TurnOff(GREEN);
            led_Blink(BLUE, 100);
        break;
    }
    led_change++;
    if(led_change == 4)
    {
        led_change = 0;
    }
    */
}    

uint8_t button_pwmValue(void){
    return button_pwm_value;
}