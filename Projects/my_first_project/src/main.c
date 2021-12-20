
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "button.h"
//#include "wifi.h"
#include "wifiAP.h"


void app_main(void)
{

    init_wifi_ap();
    /*
    led_init();
    button_init();
    while (true) 
    {
        led_run();
    } 
    */

}