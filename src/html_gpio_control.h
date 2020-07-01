#ifndef HTML_GPIO_CONTROL_H
#define HTML_GPIO_CONTROL_H

#include "html_gpio_include.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Set specific pin to output */
#define CONTROL_PIN_DEFAULT_CONFIG(x) { \
  .pin_bit_mask = GPIO_Pin_##x,         \
  .mode = GPIO_MODE_OUTPUT,             \
}
/* Set all pins to output */
#define CONTROL_DEFAULT_CONFIG() {      \
  .persistent_pin_state = false,        \
  .control_list = {                     \
    CONTROL_PIN_DEFAULT_CONFIG(0),      \
    CONTROL_PIN_DEFAULT_CONFIG(1),      \
    CONTROL_PIN_DEFAULT_CONFIG(2),      \
    CONTROL_PIN_DEFAULT_CONFIG(3),      \
    CONTROL_PIN_DEFAULT_CONFIG(4),      \
    CONTROL_PIN_DEFAULT_CONFIG(5),      \
    CONTROL_PIN_DEFAULT_CONFIG(6),      \
    CONTROL_PIN_DEFAULT_CONFIG(7),      \
    CONTROL_PIN_DEFAULT_CONFIG(8),      \
    CONTROL_PIN_DEFAULT_CONFIG(9),      \
    CONTROL_PIN_DEFAULT_CONFIG(10),     \
    CONTROL_PIN_DEFAULT_CONFIG(11),     \
    CONTROL_PIN_DEFAULT_CONFIG(12),     \
    CONTROL_PIN_DEFAULT_CONFIG(13),     \
    CONTROL_PIN_DEFAULT_CONFIG(14),     \
    CONTROL_PIN_DEFAULT_CONFIG(15),     \
    CONTROL_PIN_DEFAULT_CONFIG(16),     \
  },                                    \
}

static const char *TAG = "html_gpio_control";
/* Persistent map of pin states */
static FILE *s_gpio_state;
/* Control pin state */
typedef struct {
  bool persistent_pin_state;        // Use gpio_state file?
  gpio_config_t control_list[17];   // Collection of GPIO configs, one for each pin
} control_config_t;

/* Initialize GPIO control */
esp_err_t setup_control(const control_config_t *control_config);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_CONTROL_H