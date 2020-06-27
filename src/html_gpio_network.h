#ifndef HTML_GPIO_NETWORK_H
#define HTML_GPIO_NETWORK_H

#include "html_gpio_include.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RECEIVE_IP_BIT BIT(0)

static const char *TAG = "html_gpio_network";
static EventGroupHandle_t s_connection_event_group;

esp_err_t setup_network(void);
static void connection_receive_ip(void *arg, esp_event_base_t event_base,
              int32_t event_id, void *event_data);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_NETWORK_H