#include "html_gpio_setup.h"
#ifdef NEOPIXEL
  #include "../include/Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#endif

#ifdef __cplusplus
  extern "C" { void app_main(void); }
#endif

void app_main(void) {
  ESP_LOGI(TAG, "Setup start");
  ESP_ERROR_CHECK(setup_storage());
  ESP_ERROR_CHECK(setup_network());
  s_httpd_server = setup_httpd_server();
}
