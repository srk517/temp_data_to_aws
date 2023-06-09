/* MQTT Mutual Authentication Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)
working
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "driver/adc.h"
#include "ADC_Temperature.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "json_parser.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#define ADC_WIDTH       ADC_WIDTH_BIT_12
#define NTC2     ADC_CHANNEL_3
#define NTC1     ADC2_CHANNEL_5   

float temp1,temp2;

esp_mqtt_client_handle_t client;

static const char *TAG = "MQTTS_EXAMPLE";

int msg_id;

int raw_temp_reading_1;
static const char *message ="HERE";
const char *jsonmessage = "{\"temperature\":\"}";//-000\"}";

static const char *PUBLISH = "$aws/rules/r1/630f3f41376e9/l/47cf63a";
static const char *SUBSCRIBE = "630f3f41376e9/sub/l/47cf63a";
static const char *TEST ="test/630f3f41376e9/l/47cf63a";
static void _data_handler(void *handler_args, esp_event_base_t base, int32_t event_id,void *event_data);

extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
extern const uint8_t server_cert_pem_start[] asm("_binary_thingsty_ca_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_thingsty_ca_crt_end");
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */

static void _data_handler(void *handler_args, esp_event_base_t base, int32_t event_id,void *event_data){
bool once =true;
char data_arr[40];
int count=0;
esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
client= event->client;
/*while(once){
snprintf(data_arr,sizeof(data_arr),"Sending Temperature Reading Now\n");
esp_mqtt_client_publish(event->client,TOPIC_TEST,data_arr,strlen(data_arr),1,0);
printf("Count = %d\n",count);
count++;
once=false;sjhgdjhdgjsdgjsfgj
}*/
     const char* para="temperature 1";
     const char* para2="temperature 2";

    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(NTC2, ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH);
    adc2_config_channel_atten(NTC1, ADC_ATTEN_DB_11);
    while(1)
    {
         adc2_get_raw(NTC1,ADC_WIDTH, &raw_temp_reading_1);
        uint32_t raw_temp_reading_2 = adc1_get_raw(NTC2);
                temp1 = calculate_Temp(raw_temp_reading_1);
                temp2= calculate_Temp(raw_temp_reading_2);
                printf("Temperature NTC1\t%.2f\n",temp1);
                printf("Temperature NTC2\t%.2f\n",temp2);
               
                
                int temp_int_1 = (int)temp1;
                int temp_int_2 = (int)temp2;
               
               
              //snprintf(data_arr, sizeof(data_arr), "{\"temperature\":\"%d\"}", temp_int);
             //snprintf(data_arr, sizeof(data_arr), "{\"%s\":\"%d\"}", para,temp_int);
              
              // printf("Concatenated String: %s\n", data_arr);
             //  esp_mqtt_client_publish(event->client,TOPIC_TEST,data_arr,strlen(data_arr),1,0);
            // esp_mqtt_client_publish(client, PUBLISH ,data_arr, strlen(data_arr), 1, 0);
             //esp_mqtt_client_publish(client, PUBLISH ,data_arr, strlen(data_arr), 1, 0);
              char *jsonString =  parseIntegerToJSON(para,para2, temp_int_1,temp_int_2);
              printf("JSON String: %s\n", jsonString);

               esp_mqtt_client_publish(client, PUBLISH ,jsonString, strlen(jsonString), 1, 0);

                vTaskDelay(1000/portTICK_PERIOD_MS);
    }


esp_mqtt_client_unsubscribe(client, SUBSCRIBE);
}

////////////////////////////sending data code ends///////////////////////////////////
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

        //msg_id = esp_mqtt_client_subscribe(client, "test/630f3f41376e9/l/47cf63a", 1);
        //esp_mqtt_client_publish(client, "test/630f3f41376e9/l/47cf63a" ,message,strlen(message), 1, 0);

        msg_id = esp_mqtt_client_subscribe(client, SUBSCRIBE, 1);
        //esp_mqtt_client_publish(client, "$aws/rules/r1/630f3f41376e9/l/47cf63a" ,jsonmessage,strlen(jsonmessage), 1, 0);

        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        //msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        //ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        //msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        //ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

        // msg_id = esp_mqtt_client_subscribe(client, SUBSCRIBE, 1);
         msg_id = esp_mqtt_client_publish(client, PUBLISH ,"{\"temperature\":\"-169\"}", strlen("{\"temperature\":\"-169\",}"), 1, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        //msg_id = esp_mqtt_client_publish(client, PUBLISH ,message,strlen(message), 1, 0);
       // msg_id = esp_mqtt_client_publish(client, "$aws/rules/r1/630f3f41376e9/l/47cf63a/qos1" ,"{"message":"hello"}", 0, 1, 0);
       // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
         _data_handler(NULL,NULL,0,event);
       // msg_id = esp_mqtt_client_publish(client, PUBLISH ,message,strlen(message), 1, 0);
        //msg_id = esp_mqtt_client_publish(client, PUBLISH ,"{\"temperature\":\"-179\"}", strlen("{\"temperature\":\"-169\",}"), 1, 0);
        // ESP_LOGI(TAG, "SENT PUBLISH successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
      //  _data_handler(NULL,NULL,0,event);
        
        //my_mqtt_data_handler
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

static void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        //.uri = "mqtts://iot.thingsty.com:8883/mqtt",
        .host = "iot.thingsty.com",
        .path = "/mqtt",
        .port = 8883,
        .client_id = "630f3f41376e947cf63a",
        .transport =MQTT_TRANSPORT_OVER_SSL,//MQTT_TRANSPORT_OVER_SSL,//MQTT_TRANSPORT_UNKNOWN,// MQTT_TRANSPORT_OVER_TCP,//MQTT_TRANSPORT_OVER_SSL,
        .client_cert_pem = (const char *)client_cert_pem_start,
        .client_key_pem = (const char *)client_key_pem_start,
        .cert_pem = (const char *)server_cert_pem_start,
        .lwt_qos=2,
    };

    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    mqtt_app_start();
}
