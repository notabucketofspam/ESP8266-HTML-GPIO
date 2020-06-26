#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "esp_event.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"
#include "esp_http_server.h"

#include "../build/include/sdkconfig.h"
#include "configuration.hpp"
#ifdef NEOPIXEL
  #include "../include/Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#endif

// Fix for __ESP_FILE__ not defined
#ifndef __ESP_FILE__
#undef __ESP_FILE__
#define __ESP_FILE__ __FILE__
#endif

static const char *TAG = "HTML GPIO";

esp_err_t setup_storage(void);
esp_err_t setup_network(void);
httpd_handle_t setup_httpd_server(void);
#ifdef __cplusplus
  extern "C" { void app_main(void); }
#endif

esp_err_t setup_storage(void) {
	ESP_ERROR_CHECK(nvs_flash_init());
  esp_vfs_spiffs_conf_t spiffs_config = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 16,
    .format_if_mount_failed = false,
	};
	ESP_ERROR_CHECK(esp_vfs_spiffs_register(&spiffs_config));
  size_t total_bytes, used_bytes;
  ESP_ERROR_CHECK(esp_spiffs_info(NULL, &total_bytes, &used_bytes));
  ESP_LOGD(TAG,"SPIFFS: total bytes: %llu used bytes: %llu", total_bytes, used_bytes);
  ESP_LOGI(TAG, "Storage OK");
  return ESP_OK;
}

esp_err_t setup_network(void) {
  tcpip_adapter_init();
  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  wifi_config_t sta_config = {
    .sta = {
      .ssid = CONFIG_WIFI_SSID,
      .password = CONFIG_WIFI_PASSWORD,
    }
  };
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
	ESP_ERROR_CHECK(esp_wifi_start());
  tcpip_adapter_ip_info_t ip_info;
  ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA,&ip_info));
  ip_info.ip.addr = IP4_ADDRESS_U32;
  ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
  ESP_LOGD(TAG, "IP get: %s", IPSTR, IP2STR(&ip_info.ip));
  ESP_LOGI(TAG, "Network OK");
  return ESP_OK;
}

httpd_handle_t setup_httpd_server(void) {
  httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG();
  httpd_handle_t httpd_server = NULL;
  ESP_ERROR_CHECK(httpd_start(&httpd_server, &httpd_config));
  return httpd_server;
}

void app_main(void) {
  ESP_LOGI(TAG, "Setup start");
  ESP_ERROR_CHECK(setup_storage());
  ESP_ERROR_CHECK(setup_network());
  httpd_handle_t httpd_server = setup_httpd_server();
}
