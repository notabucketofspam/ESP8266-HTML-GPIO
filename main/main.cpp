#include "html_gpio_core.h"
#ifdef NEOPIXEL
  #include "../include/Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#endif

#ifdef __cplusplus
  extern "C" { void app_main(void); }
#endif

void app_main(void) {
  ESP_ERROR_CHECK(setup_core());
}
