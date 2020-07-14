#include "html_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

httpd_uri_t pins_get = {
  .uri      = "/pins",
  .method   = HTTP_GET,
  .handler  = pins_get_handler,
  .user_ctx = NULL,
};

httpd_uri_t save_get = {
  .uri      = "/save",
  .method   = HTTP_GET,
  .handler  = save_get_handler,
  .user_ctx = NULL,
};

httpd_uri_t load_get = {
  .uri      = "/load",
  .method   = HTTP_GET,
  .handler  = load_get_handler,
  .user_ctx = NULL,
};

httpd_uri_t gpio_get = {
  .uri      = "/gpio",
  .method   = HTTP_GET,
  .handler  = gpio_get_handler,
  .user_ctx = NULL,
};

esp_err_t setup_core(const core_config_t *core_config) {
  ESP_LOGI(TAG, "Setup core");
  ESP_ERROR_CHECK(esp_event_loop_create_default());;
  ESP_ERROR_CHECK(setup_storage(&core_config->storage_config));
  ESP_ERROR_CHECK(storage_access(s_gpio_state, "/spiffs/gpio_state", STORAGE_READ_WRITE));
  ESP_ERROR_CHECK(setup_control(&core_config->control_config));
  ESP_ERROR_CHECK(setup_network(&core_config->network_config));
  ESP_ERROR_CHECK(storage_access(s_index_html, "/spiffs/index.html", STORAGE_READ));
  ESP_ERROR_CHECK(setup_server(&core_config->server_config));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &pins_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &save_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &load_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &gpio_get));
  ESP_LOGI(TAG, "Core OK");
  return ESP_OK;
}

static esp_err_t pins_get_handler(httpd_req_t *req) {
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, s_pin_bit_mask, sizeof(s_pin_bit_mask)));
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, "\n", strlen("\n")));
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, s_pin_mode, sizeof(s_pin_mode)));
  ESP_ERROR_CHECK(httpd_resp_send(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t save_get_handler(httpd_req_t *req) {
  uint8_t state_index;
  for (state_index = 0; state_index < GPIO_PIN_COUNT; ++state_index)
    fputc(s_gpio_state_mem[state_index], s_gpio_state);
  fflush(s_gpio_state);
  rewind(s_gpio_state);
  ESP_ERROR_CHECK(httpd_resp_send(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t load_get_handler(httpd_req_t *req) {
  ESP_ERROR_CHECK(load_persistent_gpio_state());
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, s_gpio_state_mem, sizeof(s_gpio_state_mem)));
  ESP_ERROR_CHECK(httpd_resp_send(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t gpio_get_handler(httpd_req_t *req) {
  static uint8_t query_str_buf[128];
  memset(query_str_buf, 0x00, sizeof(query_str_buf));
  ESP_ERROR_CHECK(httpd_req_get_url_query_str(req, query_str_buf, httpd_req_get_url_query_len(req)));
  static uint8_t query_gpio_pin_num_buf[4];
  memset(query_gpio_pin_num_buf, 0x00, sizeof(query_gpio_pin_num_buf));
  ESP_ERROR_CHECK(httpd_query_key_value(query_str_buf, "gpio_pin_num", 
    query_gpio_pin_num_buf, sizeof(query_gpio_pin_num_buf)));
  int gpio_pin_num = atoi(query_gpio_pin_num_buf);
  static uint8_t query_gpio_set_level_buf[8];
  memset(query_gpio_set_level_buf, 0x00, sizeof(query_gpio_set_level_buf));
  ESP_ERROR_CHECK(httpd_query_key_value(query_str_buf, "gpio_set_level", 
    query_gpio_set_level_buf, sizeof(query_gpio_set_level_buf)));
  bool gpio_level = (strcmp(query_gpio_set_level_buf, "true") == 0);
  static uint8_t gpio_level_buf[2];
  memset(gpio_level_buf, 0x00, sizeof(gpio_level_buf));
  switch (atoi(s_pin_mode[GPIO_RTL(gpio_pin_num)])) {
    case 1:
      gpio_level_buf[0] = 0x30 + gpio_get_level(gpio_pin_num);
      ESP_ERROR_CHECK(httpd_resp_send_chunk(req, gpio_level_buf, strlen(gpio_level_buf)));
      break;
    case 2:
      ESP_ERROR_CHECK(gpio_set_level(gpio_pin_num, gpio_level));
      s_gpio_state_mem[GPIO_RTL(gpio_pin_num)] = gpio_level;
      break;
    default: break;
  }
  ESP_ERROR_CHECK(httpd_resp_send(req, NULL, 0));
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif
