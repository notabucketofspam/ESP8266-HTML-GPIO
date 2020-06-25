#define NEOPIXEL

#ifdef NEOPIXEL
  #include "../include/Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "nvs_flash.h"
#include "esp_spiffs.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"
#include "esp_http_server.h"
#include "esp_event.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Fix for _ESP_FILE__ not defined
#ifdef ESP_ERROR_CHECK
#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x) do {                                         \
        esp_err_t __err_rc = (x);                                       \
        if (__err_rc != ESP_OK) {                                       \
            _esp_error_check_failed(__err_rc, __FILE__, __LINE__,       \
                                    __ASSERT_FUNC, #x);                 \
        }                                                               \
    } while(0);
#endif

#ifdef __cplusplus
  extern "C" {
    void app_main(void);
  }
#endif

esp_err_t eht_setup(void) {
	nvs_flash_init();
	tcpip_adapter_init();

  esp_vfs_spiffs_conf_t spiffsCfg = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = false
	};
	ESP_ERROR_CHECK(esp_vfs_spiffs_register(&spiffsCfg));

  return ESP_OK;
}

void app_main(void) {
  ESP_ERROR_CHECK(eht_setup());
  
}
