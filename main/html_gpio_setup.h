#ifndef HTML_GPIO_SETUP_H
#define HTML_GPIO_SETUP_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"
#include "esp_http_server.h"

#include "../build/include/sdkconfig.h"
#include "html_gpio_config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Fix for __ESP_FILE__ not defined
#ifndef __ESP_FILE__
  #define __ESP_FILE__ __FILE__
#endif
#define RECEIVE_IP_BIT BIT(0)

static const char *TAG = "HTML GPIO";
static EventGroupHandle_t s_connection_event_group;
static httpd_handle_t s_httpd_server;

esp_err_t setup_storage(void);
esp_err_t setup_network(void);
static httpd_handle_t setup_httpd_server(void);
static void connection_receive_ip(void *arg, esp_event_base_t event_base,
              int32_t event_id, void *event_data);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_SETUP_H