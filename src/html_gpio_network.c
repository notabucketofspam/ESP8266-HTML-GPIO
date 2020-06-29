#include "html_gpio_network.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t setup_network(const network_config_t *network_config) {
  ESP_LOGI(TAG, "Setup network");
  s_connection_event_group = xEventGroupCreate();
	ESP_ERROR_CHECK(nvs_flash_init());
  tcpip_adapter_init();
  tcpip_adapter_ip_info_t ip_info;
  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connection_receive_ip, &ip_info));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &network_config));
	ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_connect());
  xEventGroupWaitBits(s_connection_event_group, RECEIVE_IP_BIT, true, true, 
    CONFIG_NETWORK_TIMEOUT * 1000 / portTICK_PERIOD_MS);
  ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
  ESP_LOGD(TAG, "IP receive: " IPSTR, IP2STR(&ip_info.ip));
  #if CONFIG_LOG_DEFAULT_LEVEL > 3
    static uint8_t s_system_mac[6];
    esp_read_mac(&s_system_mac, ESP_MAC_WIFI_STA);
    ESP_LOGD(TAG, "System MAC: " MACSTR, MAC2STR(s_system_mac));
  #endif
  #if CONFIG_IP4_ADDRESS
    ip_info.ip.addr = PP_HTONL(LWIP_MAKEU32(CONFIG_IP4_ADDRESS_1,\
      CONFIG_IP4_ADDRESS_2,CONFIG_IP4_ADDRESS_3,CONFIG_IP4_ADDRESS_4));
    ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
    ESP_LOGD(TAG, "New IP: " IPSTR, IP2STR(&ip_info.ip));
  #endif
  ESP_LOGI(TAG, "Network OK");
  return ESP_OK;
}

static void connection_receive_ip(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
  tcpip_adapter_ip_info_t *event_ip_info = &arg;
  memcpy(&event_ip_info->ip, &event->ip_info.ip, sizeof(ip4_addr_t));
  xEventGroupSetBits(s_connection_event_group, RECEIVE_IP_BIT);
}

#ifdef __cplusplus
}
#endif