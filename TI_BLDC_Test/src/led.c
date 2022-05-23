
#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/rmt.h"
#include "led_strip.h"
#include "led.h"


led_strip_t *strip;

uint8_t led_red_state = 0;
uint8_t led_green_state = 0;
uint8_t led_blue_state = 0;
uint8_t led_red = 0;
uint8_t led_green = 0;
uint8_t led_blue = 0;
uint16_t led_red_blink_interval = 0;
uint16_t led_green_blink_interval = 0;
uint16_t led_blue_blink_interval = 0;
TimerHandle_t ledTimers[6];
uint8_t led_red_toggle = 0;
uint8_t led_red_toggle_state = 0;
uint8_t led_green_toggle = 0;
uint8_t led_green_toggle_state = 0;
uint8_t led_blue_toggle = 0;
uint8_t led_blue_toggle_state = 0;


void vledTimersCallback(TimerHandle_t xTimer)
{
    if(pvTimerGetTimerID(xTimer) == pvTimerGetTimerID(ledTimers[RED]))
    {
        led_red_toggle = 1;
    }
    if(pvTimerGetTimerID(xTimer) == pvTimerGetTimerID(ledTimers[GREEN]))
    {
        led_green_toggle = 1;
    }
    if(pvTimerGetTimerID(xTimer) == pvTimerGetTimerID(ledTimers[BLUE]))
    {
        led_blue_toggle = 1;
    }
}

void led_init(void)
{
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_EXAMPLE_RMT_TX_GPIO, RMT_TX_CHANNEL);
    
    // set counter clock to 40MHz
    config.clk_div = 2;
    rmt_config(&config);
    rmt_driver_install(config.channel, 0, 0);

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(CONFIG_EXAMPLE_STRIP_LED_NUMBER, (led_strip_dev_t)config.channel);
    strip = led_strip_new_rmt_ws2812(&strip_config);

    // Clear LED strip (turn off all LEDs)
    strip->clear(strip, 100);

    //Initialize led Timers
    for(uint8_t i = 0; i < 6; i++)
    {
        ledTimers[i] = xTimerCreate("Timer", pdMS_TO_TICKS(1000), pdTRUE, (void *) 0, vledTimersCallback);
    }

}

void led_setColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
    led_red_state = 2;
    led_green_state = 2;
    led_blue_state = 2;
    led_red = r;
    led_green = g;
    led_blue = b;
    xTimerStop(ledTimers[RED], 0);
    xTimerStop(ledTimers[GREEN], 0);
    xTimerStop(ledTimers[BLUE], 0);
}

void led_clear(void)
{
    strip->clear(strip, 50);
}

void led_TurnOn(led_color_t color)
{
    switch(color)
    {
        case RED:
            led_red_state = 1;
            xTimerStop(ledTimers[RED], 0);
        break;

        case GREEN:
            led_green_state = 1;
            xTimerStop(ledTimers[GREEN], 0);
        break;

        case BLUE:
            led_blue_state = 1;
            xTimerStop(ledTimers[BLUE], 0);
        break;

        default:
        break;
    }
}

void led_TurnOff(led_color_t color)
{
    switch(color)
    {
        case RED:
            led_red_state = 0;
            xTimerStop(ledTimers[RED], 0);
        break;

        case GREEN:
            led_green_state = 0;
            xTimerStop(ledTimers[GREEN], 0);
        break;

        case BLUE:
            led_blue_state = 0;
            xTimerStop(ledTimers[BLUE], 0);
        break;

        default:
        break;
    }
}

void led_TurnOffAll(void)
{
    led_red_state = 0;
    led_green_state = 0;
    led_blue_state = 0;
    xTimerStop(ledTimers[RED], 0);
    xTimerStop(ledTimers[GREEN], 0);
    xTimerStop(ledTimers[BLUE], 0);
}

void led_Blink(led_color_t color, uint16_t interval_ms)
{
    switch(color)
    {
        case RED:
            led_red_state = 3;
            led_red_blink_interval = interval_ms;
            xTimerChangePeriod(ledTimers[RED], pdMS_TO_TICKS(interval_ms), 0);
            xTimerStart(ledTimers[RED], 0);
        break;

        case GREEN:
            led_green_state = 3;
            led_green_blink_interval = interval_ms;
            xTimerChangePeriod(ledTimers[GREEN], pdMS_TO_TICKS(interval_ms), 0);
            xTimerStart(ledTimers[GREEN], 0);
        break;

        case BLUE:
            led_blue_state = 3;
            led_blue_blink_interval = interval_ms;
            xTimerChangePeriod(ledTimers[BLUE], pdMS_TO_TICKS(interval_ms), 0);
            xTimerStart(ledTimers[BLUE], 0);
        break;

        default:
        break;
    }
}


void led_run(void)
{
    switch(led_red_state)
    {
        case 0: //Turn Off
            led_red = 0;
        break;
        
        case 1: //Turn On
            led_red = 255;
        break;
        
        case 2: //RGB
            ;
        break;

        case 3: //Blink
            if(led_red_toggle)
            {
                if(!led_red_toggle_state)
                {
                    led_red = 255;
                    led_red_toggle_state = 1;      
                }
                else
                {
                    led_red = 0;
                    led_red_toggle_state = 0;   
                } 
                led_red_toggle = 0;
            }
        break;
    }
    switch(led_green_state)
    {
        case 0: //Turn Off
            led_green = 0;
        break;
        
        case 1: //Turn On
            led_green = 255;
        break;
        
        case 2: //RGB
            ;
        break;

        case 3: //Blink
            if(led_green_toggle)
            {
                if(!led_green_toggle_state)
                {
                    led_green = 255;
                    led_green_toggle_state = 1;      
                }
                else
                {
                    led_green = 0;
                    led_green_toggle_state = 0;   
                } 
                led_green_toggle = 0;
            }
        break;
    }
    switch(led_blue_state)
    {
        case 0: //Turn Off
            led_blue = 0;
        break;
        
        case 1: //Turn On
            led_blue = 255;
        break;
        
        case 2: //RGB
            ;
        break;

        case 3: //Blink
            if(led_blue_toggle)
            {
                if(!led_blue_toggle_state)
                {
                    led_blue = 255;
                    led_blue_toggle_state = 1;      
                }
                else
                {
                    led_blue = 0;
                    led_blue_toggle_state = 0;   
                } 
                led_blue_toggle = 0;
            }
        break;
    }
    strip->set_pixel(strip, 0, led_red, led_green, led_blue);
    strip->refresh(strip, 100);
    vTaskDelay(pdMS_TO_TICKS(10));
}