#ifndef HTML_GPIO_CONTROL_H
#define HTML_GPIO_CONTROL_H

#include "html_gpio_include.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Set specific pin to output, nopullup, no interrupt
 */
#define CONTROL_PIN_DEFAULT_CONFIG(x) {     \
  .pin_bit_mask = GPIO_Pin_##x,             \
  .mode = GPIO_MODE_OUTPUT,                 \
  .pull_up_en = GPIO_PULLUP_DISABLE,        \
  .pull_down_en = GPIO_PULLDOWN_DISABLE,    \
  .intr_type = GPIO_INTR_DISABLE,           \
}
/**
 * Set all pins to output, no pullup, no interrupt
 */
#define CONTROL_DEFAULT_CONFIG() {  \
  .control_list = {                 \
  CONTROL_PIN_DEFAULT_CONFIG(0),    \
  CONTROL_PIN_DEFAULT_CONFIG(1),    \
  CONTROL_PIN_DEFAULT_CONFIG(2),    \
  CONTROL_PIN_DEFAULT_CONFIG(3),    \
  CONTROL_PIN_DEFAULT_CONFIG(4),    \
  CONTROL_PIN_DEFAULT_CONFIG(5),    \
  CONTROL_PIN_DEFAULT_CONFIG(6),    \
  CONTROL_PIN_DEFAULT_CONFIG(7),    \
  CONTROL_PIN_DEFAULT_CONFIG(8),    \
  CONTROL_PIN_DEFAULT_CONFIG(9),    \
  CONTROL_PIN_DEFAULT_CONFIG(10),   \
  CONTROL_PIN_DEFAULT_CONFIG(11),   \
  CONTROL_PIN_DEFAULT_CONFIG(12),   \
  CONTROL_PIN_DEFAULT_CONFIG(13),   \
  CONTROL_PIN_DEFAULT_CONFIG(14),   \
  CONTROL_PIN_DEFAULT_CONFIG(15),   \
  CONTROL_PIN_DEFAULT_CONFIG(16),   \
  }                                 \
}

static const char *TAG = "html_gpio_control";
static FILE *s_gpio_state;
typedef struct {
  gpio_config_t control_list[17];
} control_config_t;

esp_err_t setup_control(const control_config_t *control_config);

#ifdef __cplusplus
}
#endif

#endif // HTML_GPIO_CONTROL_H