#include "html_gpio_setup.h"

#ifdef __cplusplus
extern "C" {
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
  s_connection_event_group = xEventGroupCreate();
  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  tcpip_adapter_ip_info_t ip_info;
  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connection_receive_ip, &ip_info));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  wifi_config_t sta_config = {
    .sta = {
      .ssid = CONFIG_WIFI_SSID,
      .password = CONFIG_WIFI_PASSWORD,
    }
  };
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
	ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_connect());
  xEventGroupWaitBits(s_connection_event_group, RECEIVE_IP_BIT, true, true, 
    CONFIG_NETWORK_TIMEOUT * 1000 / portTICK_PERIOD_MS);
  ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
  ESP_LOGD(TAG, "IP receive: " IPSTR, IP2STR(&ip_info.ip));
  #if CONFIG_IP4_ADDRESS
    ip_info.ip.addr = IP4_ADDRESS_U32;
    ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
    ESP_LOGD(TAG, "New IP: " IPSTR, IP2STR(&ip_info.ip));
  #endif
  ESP_LOGI(TAG, "Network OK");
  return ESP_OK;
}

httpd_handle_t setup_httpd_server(void) {
  httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG();
  httpd_handle_t httpd_server = NULL;
  ESP_ERROR_CHECK(httpd_start(&httpd_server, &httpd_config));
  ESP_LOGI(TAG, "HTTPD server OK");
  return httpd_server;
}

static void connection_receive_ip(void *arg, esp_event_base_t event_base,
              int32_t event_id, void *event_data) {
  ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
  tcpip_adapter_ip_info_t *event_ip_info = &arg;
  memcpy(&event_ip_info->ip, &event->ip_info.ip, sizeof(ip4_addr_t));
  xEventGroupSetBits(s_connection_event_group, RECEIVE_IP_BIT);
}

#ifdef __cplusplus
}
#endif