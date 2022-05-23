#include <stdint.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "button.h"
//#include "wifi.h"
#include "wifiAP.h"
#include "Bonsaire_LEDS.h"


void app_main(void)
{
    init_wifi_ap();
    //button_init();
    Bonsaire_LEDS_Init();
    Bonsaire_LEDS_Bonsaire1(2, 100);

    while (true) 
    {
        Bonsaire_LEDS_Run();
    } 
    

}