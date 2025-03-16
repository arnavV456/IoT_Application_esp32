#include "esp_http_server.h"
#include "esp_log.h"

#include "http_server.h"
#include "tasks_common.h"
#include "wifi_app.h"

// Tag used for ESP serial console messages 
static const char TAG[]= "http_server";

//HTTP server task handle 
static httpd_handle_t http_server_handle = NULL;

//embedded files 
extern const uint8_t jquery_3_3_1_min_js_start[]    asm("_binary_jquery_3_3_1_min_js_start");
extern const uint8_t jquery_3_3_1_min_js_end[]      asm("_binary_jquery_3_3_1_min_js_end");
extern const uint8_t index_html_start[]             asm("_binary_index_html_start");
extern const uint8_t index_html_end[]               asm("_binary_index_html_end");
extern const uint8_t app_css_start[]                asm("_binary_app_css_start");
extern const uint8_t app_css_end[]                  asm("_binary_app_css_end");
extern const uint8_t app_js_start[]                 asm("_binary_app_js_start");
extern const uint8_t app_js_end[]                   asm("_binary_app_js_end");
extern const uint8_t favicon_ico_start[]            asm("_binary_favicon_ico_start");
extern const uint8_t favicon_ico_end[]              asm("_binary_favicon_ico_end");

/* 
    jquery get handler is requested when accessing the web page 
    @param req HTTP request for which the uri needs to be handled 
    @return ESP_OK
 */
static esp_err_t http_server_jquery_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG,"Jquery Requested");

    httpd_resp_set_type(req,"application/javascript");
    httpd_resp_send(req,(const char *)jquery_3_3_1_min_js_start,jquery_3_3_1_min_js_end -jquery_3_3_1_min_js_start );

    return ESP_OK;
}

/* 
    Sends the index.html page  
    @param req HTTP request for which the uri needs to be handled 
    @return ESP_OK
 */
static esp_err_t http_server_index_html_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG,"Index.html Requested");

    httpd_resp_set_type(req,"text/html");
    httpd_resp_send(req,(const char *)index_html_start,index_html_end -index_html_start );

    return ESP_OK;
}

/* 
    app.css get handler is requested when accessing the web page 
    @param req HTTP request for which the uri needs to be handled 
    @return ESP_OK
 */
static esp_err_t http_server_app_css_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG,"app.css Requested");

    httpd_resp_set_type(req,"text/css");
    httpd_resp_send(req,(const char *)app_css_start, app_css_end -app_css_start );

    return ESP_OK;
}

/* 
    app.js get handler is requested when accessing the web page 
    @param req HTTP request for which the uri needs to be handled 
    @return ESP_OK
 */
static esp_err_t http_server_app_js_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG,"app.js requested");

    httpd_resp_set_type(req,"application/javascript");
    httpd_resp_send(req,(const char *)app_js_start , app_js_end -app_js_start );

    return ESP_OK;
}
/* 
    sends the .ico (icon) file when accessing the webpage
    @param req HTTP request for which the uri needs to be handled 
    @return ESP_OK
 */
static esp_err_t http_server_favicon_ico_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG,"favicon.ico Requested");

    httpd_resp_set_type(req,"img/x-icon");
    httpd_resp_send(req,(const char *)favicon_ico_start,favicon_ico_end -favicon_ico_start );

    return ESP_OK;
}













// Configure the default httpd server configuration 
// @returns http server handle if sucessful, NULL otheriwse 
static httpd_handle_t http_server_configure(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    
    // Set up the core on which the http server will run
    config.core_id =HTTP_SERVER_TASK_CORE_ID;

    // Setting the priority to 1 less than the wifi application 
    config.task_priority = HTTP_SERVER_TASK_PRIORITY;
    
    // Increasing the stack size to 4096
    config.stack_size= HTTP_SERVER_TASK_STACK_SIZE;

    // Increasing the max uri handlers 
    config.max_uri_handlers = 20;

    // Increase the timeout limit 
    config.recv_wait_timeout = 10;
    config.send_wait_timeout =10;

    ESP_LOGI(TAG,
                "http_server_configure: Starting server on port: '%d' with task priority: '%d'",
                config.server_port,
                config.task_priority);

    // Start the httpd server 
    if(httpd_start(&http_server_handle, &config)== ESP_OK)
    {
        ESP_LOGI(TAG,"http_server_config: Registering URI handlers");

        // Register query handler 
        httpd_uri_t jquery_js = {
            .uri="/jquery-3.3.1.min.js",
            .method = HTTP_GET,
            .handler = http_server_jquery_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(http_server_handle,&jquery_js);

        // Register index.html handler
        httpd_uri_t index_html = {
            .uri="/",
            .method = HTTP_GET,
            .handler = http_server_index_html_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(http_server_handle,&index_html);


        // Register app.css handler
        httpd_uri_t app_css = {
            .uri="/app.css",
            .method = HTTP_GET,
            .handler = http_server_app_css_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(http_server_handle,&app_css);

         // Register app.js handler
        httpd_uri_t app_js = {
            .uri="/app.js",
            .method = HTTP_GET,
            .handler = http_server_app_js_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(http_server_handle,&app_js);



         // Register favicon.ico handler
        httpd_uri_t favicon_ico = {
            .uri="/favicon.ico",
            .method = HTTP_GET,
            .handler = http_server_favicon_ico_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(http_server_handle,&favicon_ico);
        return http_server_handle;
    }
        return NULL;
}


void http_server_start(void)
{
    if(http_server_handle==NULL)
    {
        http_server_handle = http_server_configure();
    }
} 

void http_server_stop(void)
{
    if(http_server_handle)
    {
        httpd_stop(http_server_handle);
        ESP_LOGI(TAG,"http_server_stop: stopping HTTP server");
        http_server_handle=NULL;
    }
}
