#ifndef MAIN_WIFI_APP_H_
#define MAIN_WIFI_APP_H

#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
//Wifi application setting
#define WIFI_AP_SSID            "ESP32_AP"      // Wifi Name
#define WIFI_AP_PASSWORD        "password"      //AP password
#define WIFI_AP_CHANNEL         1               //AP channel
#define WIFI_AP_SSID_HIDDEN     0               //AP visibility 0= visible 
#define WIFI_AP_MAX_CONNECTIONS 5               //Max clients 
#define WIFI_AP_BEACON_INTERVAL 100             // AP beacon interval set to 100 mills
#define WIFI_AP_IP              "192.168.0.1"   // AP Default IP
#define WIFI_AP_GATEWAY         "192.168.0.1"   //Default gateway
#define WIFI_AP_NETMASK         "255.255.255.0" // AP Netmask
#define WIFI_AP_BANDWIDTH       WIFI_BW_HT20    // AP bandwidth is 20Mhz(40 Mhz is the other option)
#define WIFI_STA_POWER_SAVE     WIFI_PS_NONE    // POWER save not used 
#define WIFI_SSID_LENGTH        32              // IEEE standard maximum
#define MAX_PASSWORD_LENGTH     64              //IEEE standard maximum
#define MAX_CONNECTION_RETRIES  5

//netif object for the station and access point
extern esp_netif_t* esp_netif_sta;
extern esp_netif_t* esp_netif_ap;

/* 
    MEssage IDS for the wifi application task
 */
typedef enum wifi_ap_message
{
    WIFI_APP_MSG_START_HTTP_SERVER = 0,
    WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
    WIFI_APP_MSG_STA_CONNECTED_GOT_IP,
}wifi_app_message_e;

/* 
    Structure for the message queue
 */
typedef struct wifi_app_queue_message
{
    wifi_app_message_e msgID;

}wifi_app_queue_message_t;

/* 
    Sends a message to the queue 
    @param msgID message id from the wifi_app_message_e enum
    @return pdTRUE if an item was succcessfuly sent to the queue , other wise pdFALSE
 */
BaseType_t wifi_app_send_message(wifi_app_message_e msgID);

/* 
    Starts wifi RTOS task 
 */
void wifi_app_start(void);
#endif 
