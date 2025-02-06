//rgb_led.h
//Created on     26/01/2025
#ifndef MAIN_RGB_LED_H_
#define MAIN_RGB_LED_H_

//RGB LED GPIOs
#define RGB_LED_RED_GPIO  21
#define RGB_LED_GREEN_GPIO  22
#define RGB_LED_BLUE_GPIO  23

//RGB led color mix channels
#define RGB_LED_CHANNEL_NUM  3
//RGB led config
typedef struct 
{
    int channel;
    int gpio;
    int mode;
    int timer_index;
}ledc_info_t;

// Creates an array of the struct type with the same size as number of channels

/* 
    *Color to indicate Wifi application has started 
 */
void rgb_led_wifi_app_started(void);

/* 
    *Color to indicate HTTP Server has started 
 */
void rgb_led_http_server_started(void);

/* 
    *Color to indicate ESP#@ has connected to an access point 
 */
void rgb_led_wifi_connected(void);
#endif