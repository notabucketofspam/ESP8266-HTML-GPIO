#include "html_gpio_core.h"

#ifdef __cplusplus
extern "C" {
#endif

httpd_uri_t save_get = {
  .uri      = "/save",
  .method   = HTTP_GET,
  .handler  = save_get_handler,
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
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &save_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &gpio_get));
  ESP_LOGI(TAG, "Core OK");
  return ESP_OK;
}

static esp_err_t save_get_handler(httpd_req_t *req) {
  uint8_t state_index;
  for (state_index = 0; state_index < 17; ++state_index)
    fputc(s_gpio_state_mem[state_index], s_gpio_state);
  rewind(s_gpio_state);
  ESP_ERROR_CHECK(httpd_resp_send(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t gpio_get_handler(httpd_req_t *req) {
  static uint8_t query_str_buf[64];
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
  ESP_ERROR_CHECK(gpio_set_level(gpio_pin_num, gpio_level));
  s_gpio_state_mem[gpio_pin_num] = gpio_level;
  ESP_ERROR_CHECK(httpd_resp_send(req, NULL, 0));
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif