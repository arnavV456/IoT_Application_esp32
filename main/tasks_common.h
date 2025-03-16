#ifndef MAIN_TASKS_COMMON_H
#define MAIN_TASKS_COMMON_H

//Wifi application task
#define WIFI_APP_TASK_STACK_SIZE            4096
#define WIFI_APP_TASK_PRIORITY              5
#define WIFI_APP_TASK_CORE_ID               0

// HTTP server task info
#define HTTP_SERVER_TASK_STACK_SIZE         8192
#define HTTP_SERVER_TASK_PRIORITY           4
#define HTTP_SERVER_TASK_CORE_ID            0

// HTTP server monitor task 
#define HTTP_SERVER_MONITOR_STACK_SIZE      4096
#define HTTP_SERVER_MONITOR_PRIORITY        3
#define HTTP_SERVER_MONITOR_CORE_ID         0

#endif