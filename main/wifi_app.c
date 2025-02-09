#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_mac.h"

#include "rgb_led.h"
#include "tasks_common.h"
#include "wifi_app.h"

//Tag used for ESP serial console messages 
static const char TAG []= "wifi_app";

//Queue handle used to manipulate  the main queue of events 
static QueueHandle_t wifi_app_queue_handle ;

// netif objects for the station and access point
esp_netif_t* esp_netif_sta = NULL;
esp_netif_t* esp_netif_ap = NULL;

/* 
    main task for the wifi application 
    @param pvParameters which can be passes to the task 

 */
static void wifi_app_task(void *pvParameters)
{
    wifi_app_queue_message_t msg;

    // initialize the event handler
    wifi_app_event_handler_init();

    // initialize the TCP/IP stack and wifi config
    wifi_app_deafult_wifi_init();

    // SoftAP config
    wifi_app_soft_ap_config();

    // start wifi
    ESP_ERROR_CHECK(esp_wifi_start());

    //send first event message
    wifi_app_send_message(WIFI_APP_MSG_START_HTTP_SERVER);

    for(;;)
    {
        if (xQueueReceive(wifi_app_queue_handle,&msg, portMAX_DELAY ))
        {
            switch (msg.msgID)
            {
                case WIFI_APP_MSG_START_HTTP_SERVER:
                    ESP_LOGI(TAG, "WIFI_APP_MSG_START_HTTP_SERVER");

//                  http_server_start();
                    rgb_led_http_server_started();
                    break;

                case WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER:
                    ESP_LOGI(TAG,"WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER");
                    break;
                
                case WIFI_APP_MSG_STA_CONNECTED_GOT_IP:
                    ESP_LOGI(TAG,"WIFI_APP_MSG_STA_CONNECTED_GOT_IP");
                    rgb_led_wifi_connected();
                    break;
            
            default:
                break;
            }
        }
    }
}




BaseType_t wifi_app_send_message(wifi_app_message_e msgID)
{
    wifi_app_queue_message_t msg;
    msg.msgID = msgID;
    return xQueueSend(wifi_app_queue_handle, &msg, portMAX_DELAY);
}

void wifi_app_start(void)
{
    ESP_LOGI(TAG,"STARTING WIFI APPLICATION");
    //Start wifi started LED
    rgb_led_wifi_app_started();

    //disable default wifi logging messages
    esp_log_level_set("wifi",ESP_LOG_NONE);

    // create message queue 
    wifi_app_queue_handle = xQueueCreate(3,sizeof(wifi_app_queue_message_t));

    // Start the wifi application task
    xTaskCreatePinnedToCore(&wifi_app_task,"wifi_app_task",WIFI_APP_TASK_STACK_SIZE,NULL,WIFI_APP_TASK_PRIORITY,NULL,WIFI_APP_TASK_CORE_ID);
}