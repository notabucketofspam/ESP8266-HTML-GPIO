#include "../src/html_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

void app_main(void) {
  ESP_ERROR_CHECK(setup_core());
}

#ifdef __cplusplus
}
#endif
