
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_gatt_defs.h"

#include "sdkconfig.h"

#include "button.h"
#include "led.h"
#include "pwm.h"
#include "adc.h"


//Free RTOS Tasks
static void led_run_task(void *arg);
static void pwm_adc_run_task(void *arg);

//Global Variables
uint16_t adcValue = 0;
uint16_t pwm_dc = 50;
uint8_t bldc_control_over_ble = 0;

//Functions
static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

#define TAG "app"
#define GATTS_SERVICE_UUID_TEST_A       0x00FF
#define GATTS_CHAR_UUID_TEST_A          0xFF01
#define GATTS_DESCR_UUID_TEST_A         0x3333
#define GATTS_NUM_HANDLE_TEST_A         4
#define GATTS_SERVICE_UUID_TEST_B       0x00EE
#define GATTS_CHAR_UUID_TEST_B          0xEE01
#define GATTS_DESCR_UUID_TEST_B         0x2222
#define GATTS_NUM_HANDLE_TEST_B         4
#define TEST_DEVICE_NAME                "ESP32-BLE-BLDC"
#define TEST_MANUFACTURER_DATA_LEN      17
#define GATTS_DEMO_CHAR_VAL_LEN_MAX     0x40
#define PREPARE_BUF_MAX_SIZE            1024
#define adv_config_flag                 (1 << 0)
#define scan_rsp_config_flag            (1 << 1)
#define PROFILE_NUM                     2
#define PROFILE_A_APP_ID                0
#define PROFILE_B_APP_ID                1

//#define CONFIG_SET_RAW_ADV_DATA         1



static uint8_t char1_str[] = {0x11, 0x22, 0x33};
static esp_gatt_char_prop_t a_property = 0;
static esp_gatt_char_prop_t b_property = 0;

static esp_attr_value_t gatts_demo_char1_val = 
{
    .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
    .attr_len = sizeof(char1_str),
    .attr_value = char1_str,
};

static uint8_t adv_config_done = 0;

#ifdef CONFIG_SET_RAW_ADV_DATA
    static uint8_t raw_adv_data[] = {
        0x02, 0x01, 0x06,
        0x02, 0x0A, 0xEB, 0x03, 0x03, 0xAB, 0xCD
    };
    static uint8_t raw_scan_rsp_data[] = {
        0x0F, 0x09, 0x45, 0x53, 0x50, 0x5F, 0x47, 0x41, 0x54, 0x53, 0x5F, 0x44,
        0x45, 0x4D, 0x4F
    };
#else
    static uint8_t adv_service_uuid128[32] = {
        0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xEE, 0x00, 0x00, 0x00,
        0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
    };

    //adv data
    static esp_ble_adv_data_t adv_data = {
        .set_scan_rsp = false,
        .include_name = true,
        .include_txpower = false,
        .min_interval = 0x0006,
        .max_interval = 0x0010,
        .appearance = 0x00,
        .manufacturer_len = 0,
        .p_manufacturer_data = NULL,
        .service_data_len = 0,
        .p_service_data = NULL,
        .service_uuid_len = sizeof(adv_service_uuid128),
        .p_service_uuid = adv_service_uuid128,
        .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
    };

    //scan response data
    static esp_ble_adv_data_t scan_rsp_data = {
        .set_scan_rsp = true,
        .include_name = true,
        .include_txpower = true,
        //.min_interval = 0x0006,
        //.max_interval = 0x0010,
        .appearance = 0x00,
        .manufacturer_len = 0,
        .p_manufacturer_data = NULL,
        .service_data_len = 0,
        .p_service_data = NULL,
        .service_uuid_len = sizeof(adv_service_uuid128),
        .p_service_uuid = adv_service_uuid128,
        .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
    };

#endif

static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr = 
    //.peer_addr_type =
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] = {
    [PROFILE_A_APP_ID] = {
        .gatts_cb = gatts_profile_a_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,
    },
    [PROFILE_B_APP_ID] = {
        .gatts_cb = gatts_profile_b_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,
    },
};

typedef struct {
    uint8_t *prepare_buf;
    int     prepare_len;
} prepare_type_env_t;

static prepare_type_env_t a_prepare_write_env;
static prepare_type_env_t b_prepare_write_env;

void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);
void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param){
    if(event == ESP_GATTS_REG_EVT){
        if(param->reg.status == ESP_GATT_OK){
            gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
        }
        else{
            ESP_LOGI(TAG, "Reg app failed, app_id %04x, status %f", param->reg.app_id, param->reg.status);
            return;
        }
    }

    do{
        int idx;
        for(idx = 0; idx < PROFILE_NUM; idx++){
            if(gatts_if == ESP_GATT_IF_NONE || gatts_if == gl_profile_tab[idx].gatts_if){
                if(gl_profile_tab[idx].gatts_cb){
                    gl_profile_tab[idx].gatts_cb(event, gatts_if, param);
                }
            }
        }
    }while(0);
}

static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
        case ESP_GATTS_REG_EVT:
            ESP_LOGI(TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
            gl_profile_tab[PROFILE_A_APP_ID].service_id.is_primary = true;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.inst_id = 0x00;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_A;

            esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(TEST_DEVICE_NAME);
            if (set_dev_name_ret){
                ESP_LOGE(TAG, "set device name failed, error code = %x", set_dev_name_ret);
            }

#ifdef CONFIG_SET_RAW_ADV_DATA
            esp_err_t raw_adv_ret = esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
            if (raw_adv_ret){
                ESP_LOGE(TAG, "config raw adv data failed, error code = %x ", raw_adv_ret);
            }
            adv_config_done |= adv_config_flag;
            esp_err_t raw_scan_ret = esp_ble_gap_config_scan_rsp_data_raw(raw_scan_rsp_data, sizeof(raw_scan_rsp_data));
            if (raw_scan_ret){
                ESP_LOGE(TAG, "config raw scan rsp data failed, error code = %x", raw_scan_ret);
            }
            adv_config_done |= scan_rsp_config_flag;
#else
            //config adv data
            esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
            if (ret){
                ESP_LOGE(TAG, "config adv data failed, error code = %x", ret);
            }
            adv_config_done |= adv_config_flag;
            //config scan response data
            ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
            if (ret){
                ESP_LOGE(TAG, "config scan response data failed, error code = %x", ret);
            }
            adv_config_done |= scan_rsp_config_flag;

#endif
            esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_A_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_A);
            break;

        case ESP_GATTS_READ_EVT: {
            ESP_LOGI(TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
            esp_gatt_rsp_t rsp;
            memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
            rsp.attr_value.handle = param->read.handle;
            rsp.attr_value.len = 4;
            rsp.attr_value.value[0] = 0xde;
            rsp.attr_value.value[1] = 0xed;
            rsp.attr_value.value[2] = 0xbe;
            rsp.attr_value.value[3] = 0xef;
            esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                        ESP_GATT_OK, &rsp);
            break;
        }

        case ESP_GATTS_WRITE_EVT: {
            ESP_LOGI(TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);
            if (!param->write.is_prep){
                ESP_LOGI(TAG, "GATT_WRITE_EVT, value len %d, value :", param->write.len);
                esp_log_buffer_hex(TAG, param->write.value, param->write.len);  
                uint8_t* received_value = param->write.value;
                if((*received_value) > 0){
                    ESP_LOGI(TAG, "BLDC Control over BLE enabled!");
                    bldc_control_over_ble = 1;
                }
                else{
                    ESP_LOGI(TAG, "BLDC Control over BLE disabled!");  
                    bldc_control_over_ble = 0;    
                }
                if (gl_profile_tab[PROFILE_A_APP_ID].descr_handle == param->write.handle && param->write.len == 2){
                    uint16_t descr_value = param->write.value[1]<<8 | param->write.value[0];
                    if (descr_value == 0x0001){
                        if (a_property & ESP_GATT_CHAR_PROP_BIT_NOTIFY){
                            ESP_LOGI(TAG, "notify enable");
                            uint8_t notify_data[15];
                            for (int i = 0; i < sizeof(notify_data); ++i)
                            {
                                notify_data[i] = i%0xff;
                            }
                            //the size of notify_data[] need less than MTU size
                            esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gl_profile_tab[PROFILE_A_APP_ID].char_handle,
                                                    sizeof(notify_data), notify_data, false);
                        }
                    }else if (descr_value == 0x0002){
                        if (a_property & ESP_GATT_CHAR_PROP_BIT_INDICATE){
                            ESP_LOGI(TAG, "indicate enable");
                            uint8_t indicate_data[15];
                            for (int i = 0; i < sizeof(indicate_data); ++i)
                            {
                                indicate_data[i] = i%0xff;
                            }
                            //the size of indicate_data[] need less than MTU size
                            esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gl_profile_tab[PROFILE_A_APP_ID].char_handle,
                                                    sizeof(indicate_data), indicate_data, true);
                        }
                    }
                    else if (descr_value == 0x0000){
                        ESP_LOGI(TAG, "notify/indicate disable ");
                    }else{
                        ESP_LOGE(TAG, "unknown descr value");
                        esp_log_buffer_hex(TAG, param->write.value, param->write.len);
                    }

                }
            }
            example_write_event_env(gatts_if, &a_prepare_write_env, param);
            break;
        }

        case ESP_GATTS_EXEC_WRITE_EVT:
            ESP_LOGI(TAG,"ESP_GATTS_EXEC_WRITE_EVT");
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
            example_exec_write_event_env(&a_prepare_write_env, param);
            break;

        case ESP_GATTS_MTU_EVT:
            ESP_LOGI(TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
            break;

        case ESP_GATTS_UNREG_EVT:
            break;

        case ESP_GATTS_CREATE_EVT:
            ESP_LOGI(TAG, "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
            gl_profile_tab[PROFILE_A_APP_ID].service_handle = param->create.service_handle;
            gl_profile_tab[PROFILE_A_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_A_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_A;

            esp_ble_gatts_start_service(gl_profile_tab[PROFILE_A_APP_ID].service_handle);
            a_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
            esp_err_t add_char_ret = esp_ble_gatts_add_char(gl_profile_tab[PROFILE_A_APP_ID].service_handle, &gl_profile_tab[PROFILE_A_APP_ID].char_uuid,
                                                            ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                            a_property,
                                                            &gatts_demo_char1_val, NULL);
            if (add_char_ret){
                ESP_LOGE(TAG, "add char failed, error code =%x",add_char_ret);
            }
            break;

        case ESP_GATTS_ADD_INCL_SRVC_EVT:
            break;

        case ESP_GATTS_ADD_CHAR_EVT: {
            uint16_t length = 0;
            const uint8_t *prf_char;

            ESP_LOGI(TAG, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                    param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
            gl_profile_tab[PROFILE_A_APP_ID].char_handle = param->add_char.attr_handle;
            gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
            esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(param->add_char.attr_handle,  &length, &prf_char);
            if (get_attr_ret == ESP_FAIL){
                ESP_LOGE(TAG, "ILLEGAL HANDLE");
            }

            ESP_LOGI(TAG, "the gatts demo char length = %x\n", length);
            for(int i = 0; i < length; i++){
                ESP_LOGI(TAG, "prf_char[%x] =%x\n",i,prf_char[i]);
            }
            esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(gl_profile_tab[PROFILE_A_APP_ID].service_handle, &gl_profile_tab[PROFILE_A_APP_ID].descr_uuid,
                                                                    ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
            if (add_descr_ret){
                ESP_LOGE(TAG, "add char descr failed, error code =%x", add_descr_ret);
            }
            break;
        }

        case ESP_GATTS_ADD_CHAR_DESCR_EVT:
            gl_profile_tab[PROFILE_A_APP_ID].descr_handle = param->add_char_descr.attr_handle;
            ESP_LOGI(TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                    param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
            break;

        case ESP_GATTS_DELETE_EVT:
            break;

        case ESP_GATTS_START_EVT:
            ESP_LOGI(TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
                    param->start.status, param->start.service_handle);
            break;

        case ESP_GATTS_STOP_EVT:
            break;

        case ESP_GATTS_CONNECT_EVT: {
            esp_ble_conn_update_params_t conn_params = {0};
            memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
            /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
            conn_params.latency = 0;
            conn_params.max_int = 0x20;    // max_int = 0x20*1.25ms = 40ms
            conn_params.min_int = 0x10;    // min_int = 0x10*1.25ms = 20ms
            conn_params.timeout = 400;    // timeout = 400*10ms = 4000ms
            ESP_LOGI(TAG, "ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
                    param->connect.conn_id,
                    param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                    param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
            gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
            //start sent the update connection parameters to the peer device.
            esp_ble_gap_update_conn_params(&conn_params);
            break;
        }

        case ESP_GATTS_DISCONNECT_EVT:
            ESP_LOGI(TAG, "ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x", param->disconnect.reason);
            esp_ble_gap_start_advertising(&adv_params);
            break;

        case ESP_GATTS_CONF_EVT:
            ESP_LOGI(TAG, "ESP_GATTS_CONF_EVT, status %d attr_handle %d", param->conf.status, param->conf.handle);
            if (param->conf.status != ESP_GATT_OK){
                esp_log_buffer_hex(TAG, param->conf.value, param->conf.len);
            }
            break;

        case ESP_GATTS_OPEN_EVT:
            break;

        case ESP_GATTS_CANCEL_OPEN_EVT:
            break;

        case ESP_GATTS_CLOSE_EVT:
            break;

        case ESP_GATTS_LISTEN_EVT:
            break;

        case ESP_GATTS_CONGEST_EVT:
            break;

        default:
            break;
    }
}
        
static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
        case ESP_GATTS_REG_EVT:
            ESP_LOGI(TAG, "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
            gl_profile_tab[PROFILE_B_APP_ID].service_id.is_primary = true;
            gl_profile_tab[PROFILE_B_APP_ID].service_id.id.inst_id = 0x00;
            gl_profile_tab[PROFILE_B_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_B_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_B;

            esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_B_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_B);
            break;

        case ESP_GATTS_READ_EVT: {
            ESP_LOGI(TAG, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
            esp_gatt_rsp_t rsp;
            memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
            rsp.attr_value.handle = param->read.handle;
            rsp.attr_value.len = 4;
            rsp.attr_value.value[0] = 0xde;
            rsp.attr_value.value[1] = 0xed;
            rsp.attr_value.value[2] = 0xbe;
            rsp.attr_value.value[3] = 0xef;
            esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                        ESP_GATT_OK, &rsp);
            break;
        }

        case ESP_GATTS_WRITE_EVT: {
            ESP_LOGI(TAG, "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\n", param->write.conn_id, param->write.trans_id, param->write.handle);
            if (!param->write.is_prep){
                ESP_LOGI(TAG, "GATT_WRITE_EVT, value len %d, value :", param->write.len);
                esp_log_buffer_hex(TAG, param->write.value, param->write.len);

                //Change BLDC Speed if control over ble is enabled
                if(bldc_control_over_ble){
                    uint8_t new_pwm = *(param->write.value);
                    if(new_pwm > 100){
                        new_pwm = 100;
                    }
                    pwm_dc = ((new_pwm * 25) / 100) + 50;
                    pwm_setDutyCycle(pwm_dc);
                    ESP_LOGI(TAG, "BLDC Control over BLE enabled!");      
                }

                if (gl_profile_tab[PROFILE_B_APP_ID].descr_handle == param->write.handle && param->write.len == 2){
                    uint16_t descr_value= param->write.value[1]<<8 | param->write.value[0];
                    if (descr_value == 0x0001){
                        if (b_property & ESP_GATT_CHAR_PROP_BIT_NOTIFY){
                            ESP_LOGI(TAG, "notify enable");
                            uint8_t notify_data[15];
                            for (int i = 0; i < sizeof(notify_data); ++i)
                            {
                                notify_data[i] = i%0xff;
                            }
                            //the size of notify_data[] need less than MTU size
                            esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gl_profile_tab[PROFILE_B_APP_ID].char_handle,
                                                    sizeof(notify_data), notify_data, false);
                        }
                    }else if (descr_value == 0x0002){
                        if (b_property & ESP_GATT_CHAR_PROP_BIT_INDICATE){
                            ESP_LOGI(TAG, "indicate enable");
                            uint8_t indicate_data[15];
                            for (int i = 0; i < sizeof(indicate_data); ++i)
                            {
                                indicate_data[i] = i%0xff;
                            }
                            //the size of indicate_data[] need less than MTU size
                            esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gl_profile_tab[PROFILE_B_APP_ID].char_handle,
                                                    sizeof(indicate_data), indicate_data, true);
                        }
                    }
                    else if (descr_value == 0x0000){
                        ESP_LOGI(TAG, "notify/indicate disable ");
                    }else{
                        ESP_LOGE(TAG, "unknown value");
                    }

                }
            }
            example_write_event_env(gatts_if, &b_prepare_write_env, param);
            break;
        }

        case ESP_GATTS_EXEC_WRITE_EVT:
            ESP_LOGI(TAG,"ESP_GATTS_EXEC_WRITE_EVT");
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
            example_exec_write_event_env(&b_prepare_write_env, param);
            break;

        case ESP_GATTS_MTU_EVT:
            ESP_LOGI(TAG, "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
            break;

        case ESP_GATTS_UNREG_EVT:
            break;

        case ESP_GATTS_CREATE_EVT:
            ESP_LOGI(TAG, "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
            gl_profile_tab[PROFILE_B_APP_ID].service_handle = param->create.service_handle;
            gl_profile_tab[PROFILE_B_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_B_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_B;

            esp_ble_gatts_start_service(gl_profile_tab[PROFILE_B_APP_ID].service_handle);
            b_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
            esp_err_t add_char_ret =esp_ble_gatts_add_char( gl_profile_tab[PROFILE_B_APP_ID].service_handle, &gl_profile_tab[PROFILE_B_APP_ID].char_uuid,
                                                            ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                            b_property,
                                                            NULL, NULL);
            if (add_char_ret){
                ESP_LOGE(TAG, "add char failed, error code =%x",add_char_ret);
            }
            break;

        case ESP_GATTS_ADD_INCL_SRVC_EVT:
            break;

        case ESP_GATTS_ADD_CHAR_EVT:
            ESP_LOGI(TAG, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                    param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);

            gl_profile_tab[PROFILE_B_APP_ID].char_handle = param->add_char.attr_handle;
            gl_profile_tab[PROFILE_B_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
            gl_profile_tab[PROFILE_B_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
            esp_ble_gatts_add_char_descr(gl_profile_tab[PROFILE_B_APP_ID].service_handle, &gl_profile_tab[PROFILE_B_APP_ID].descr_uuid,
                                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                        NULL, NULL);
            break;

        case ESP_GATTS_ADD_CHAR_DESCR_EVT:
            gl_profile_tab[PROFILE_B_APP_ID].descr_handle = param->add_char_descr.attr_handle;
            ESP_LOGI(TAG, "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                    param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
            break;

        case ESP_GATTS_DELETE_EVT:
            break;

        case ESP_GATTS_START_EVT:
            ESP_LOGI(TAG, "SERVICE_START_EVT, status %d, service_handle %d\n",
                    param->start.status, param->start.service_handle);
            break;

        case ESP_GATTS_STOP_EVT:
            break;

        case ESP_GATTS_CONNECT_EVT:
            ESP_LOGI(TAG, "CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
                    param->connect.conn_id,
                    param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                    param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
            gl_profile_tab[PROFILE_B_APP_ID].conn_id = param->connect.conn_id;
            break;

        case ESP_GATTS_CONF_EVT:
            ESP_LOGI(TAG, "ESP_GATTS_CONF_EVT status %d attr_handle %d", param->conf.status, param->conf.handle);
            if (param->conf.status != ESP_GATT_OK){
                esp_log_buffer_hex(TAG, param->conf.value, param->conf.len);
            }
        break;

        case ESP_GATTS_DISCONNECT_EVT:
            break;

        case ESP_GATTS_OPEN_EVT:
            break;
        
        case ESP_GATTS_CANCEL_OPEN_EVT:
            break;
        
        case ESP_GATTS_CLOSE_EVT:
            break;
        
        case ESP_GATTS_LISTEN_EVT:
            break;
        
        case ESP_GATTS_CONGEST_EVT:
            break;

        default:
            break;
    }
}

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param){
    switch(event){
#ifdef CONFIG_SET_RAW_ADV_DATA
        case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT:
            adv_config_done &= (~adv_config_flag);
            if(adv_config_done == 0){
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;

        case ESP_GAP_BLE_SCAN_RSP_DATA_RAW_SET_COMPLETE_EVT:
            adv_config_done &= (~scan_rsp_config_flag);
            if(adv_config_done == 0){
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;

#else
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            adv_config_done &= (~adv_config_flag);
            if(adv_config_done == 0){
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;

        case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
            adv_config_done &= (~scan_rsp_config_flag);
            if(adv_config_done == 0){
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;

#endif 

        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            if(param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS){
                ESP_LOGE(TAG, "Advertising start failed\n");
            }
            break;

        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            if(param->adv_stop_cmpl.status != ESP_BT_STATUS_SUCCESS){
                ESP_LOGE(TAG, "Advertising stop failed");
            }
            else{
                ESP_LOGI(TAG, "Stop adv successfully");
            }
            break;

        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            ESP_LOGI(TAG, "update connection params status = %d, min_int = %d, max_int = %dconn_int = %d, latency = %d, timeout = %d",
                            param->update_conn_params.status,
                            param->update_conn_params.min_int,
                            param->update_conn_params.max_int,
                            param->update_conn_params.conn_int,
                            param->update_conn_params.latency,
                            param->update_conn_params.timeout);
            break;

        default:
            break;
    }
}

void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param){
    esp_gatt_status_t status = ESP_GATT_OK;
    if(param->write.need_rsp){
        if(param->write.is_prep){
            if(prepare_write_env->prepare_buf == NULL){
                prepare_write_env->prepare_buf = (uint8_t *)malloc(PREPARE_BUF_MAX_SIZE*sizeof(uint8_t));
                prepare_write_env->prepare_len = 0;
                if(prepare_write_env->prepare_buf == NULL){
                    ESP_LOGE(TAG, "Gatt_server prep no mem\n");
                    status = ESP_GATT_NO_RESOURCES;
                }
            }
            else{
                if(param->write.offset > PREPARE_BUF_MAX_SIZE){
                    status = ESP_GATT_INVALID_OFFSET;
                }
                else{
                    status = ESP_GATT_INVALID_ATTR_LEN;
                }
            }

            esp_gatt_rsp_t *gatt_rsp = (esp_gatt_rsp_t *)malloc(sizeof(esp_gatt_rsp_t));
            gatt_rsp->attr_value.len = param->write.len;
            gatt_rsp->attr_value.handle = param->write.handle;
            gatt_rsp->attr_value.offset = param->write.offset;
            gatt_rsp->attr_value.auth_req = ESP_GATT_AUTH_REQ_NONE;
            memcpy(gatt_rsp->attr_value.value, param->write.value, param->write.len);
            esp_err_t response_err = esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, gatt_rsp);
            if(response_err != ESP_OK){
                ESP_LOGE(TAG, "Send response error\n");
            }
            free(gatt_rsp);
            if(status != ESP_GATT_OK){
                return;
            }
            memcpy(prepare_write_env->prepare_buf + param->write.offset, param->write.value, param->write.len);
            prepare_write_env->prepare_len += param->write.len;
        }
        else{
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, NULL);
        }
    }
}

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param){
    if(param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC){
        esp_log_buffer_hex(TAG, prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
    }
    else{
        ESP_LOGI(TAG, "ESP_GATT_PREP_WRITE_CANCEL");
    }
    if(prepare_write_env->prepare_buf){
        free(prepare_write_env->prepare_buf);
        prepare_write_env->prepare_buf = NULL;
    }
    prepare_write_env->prepare_len = 0;
}


void app_main()
{
    //Init Hardware
    led_init();
    button_init();
    pwm_init();
    adc_init();

    //BLE Stuff
    esp_err_t ret;

    //Initialize NVS
    ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //BT Controller Configuration
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if(ret){
        ESP_LOGE(TAG, "%s initialize controller failed", __func__);
    }
    else{
        //ENABLE Controller
        ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
        if(ret){
            ESP_LOGE(TAG, "%s enable controller failed", __func__);
        }
        else{
             //Enable Bluedroid Stack
            ret = esp_bluedroid_init();
            if(ret){
                ESP_LOGE(TAG, "%s initialize bluedroic failed", __func__);
            }
            else{
                //Enable Bluedroid Stack
                ret = esp_bluedroid_enable();
                if(ret){
                    ESP_LOGE(TAG, "%s enable bluedroid failed", __func__);
                }
                else{
                    //Register GATTS Callback
                    ret = esp_ble_gatts_register_callback(gatts_event_handler);
                    if(ret){
                        ESP_LOGE(TAG, "gatts register error, error code = %x", ret);
                    }
                    else{
                        //Register GAP Callback
                        ret = esp_ble_gap_register_callback(gap_event_handler);
                        if(ret){
                            ESP_LOGE(TAG, "gap register error, error code: %x", ret);
                        }
                        else{
                            //Register Gatts APP A
                            ret = esp_ble_gatts_app_register(PROFILE_A_APP_ID);
                            if(ret){
                                ESP_LOGE(TAG, "gatts app a register error, error code = %x", ret);
                            }
                            else{
                                //Register Gatts APP B
                                ret = esp_ble_gatts_app_register(PROFILE_B_APP_ID);
                                if(ret){
                                    ESP_LOGE(TAG, "gatts app b register error, error code = %x", ret);
                                }
                                else{
                                    //Config Local MTU
                                    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(512);
                                    if(local_mtu_ret){
                                        ESP_LOGE(TAG, "set local MTU failed, erro code = %x", local_mtu_ret);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    //Create Tasks
    xTaskCreate(led_run_task, "led", configMINIMAL_STACK_SIZE * 3, NULL, 6, NULL);
    xTaskCreate(pwm_adc_run_task, "pwm_adc", configMINIMAL_STACK_SIZE * 3, NULL, 7, NULL);
    
}


static void led_run_task(void *arg)
{
    while(1){
        led_run();
    }
}

static void pwm_adc_run_task(void *arg)
{
    while(1){
        if(!bldc_control_over_ble){
            adcValue = adc_getValue();
            pwm_dc = ((adcValue * 25) / 4095) + 50;
            pwm_setDutyCycle(pwm_dc);
        }
    }
}

