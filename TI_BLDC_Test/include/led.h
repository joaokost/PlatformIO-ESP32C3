

#pragma once

#include <stdint.h>
#include <stdio.h>
#include "driver/rmt.h"
#include "led_strip.h"


#define RMT_TX_CHANNEL RMT_CHANNEL_0
#define CONFIG_EXAMPLE_RMT_TX_GPIO 8
#define CONFIG_EXAMPLE_STRIP_LED_NUMBER 1

typedef enum {
    RED, 
    GREEN, 
    BLUE, 
    RED2,
    GREEN2,
    BLUE2
}led_color_t;

void led_init(void);
void led_setColorRGB(uint8_t r, uint8_t g, uint8_t b);
void led_clear(void);
void led_TurnOn(led_color_t color);
void led_TurnOff(led_color_t color);
void led_TurnOffAll(void);
void led_Blink(led_color_t color, uint16_t interval_ms);
void led_run(void);