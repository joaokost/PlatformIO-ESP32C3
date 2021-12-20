#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_http_server.h"


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define MAX_RETRY 10
#define WIFI_SSID "esp32_ap1"
#define WIFI_PASS "wifi_password"

#define WIFI_CHANNEL 11
#define MAX_CONN_CNT 1


 
void init_wifi_ap(void);