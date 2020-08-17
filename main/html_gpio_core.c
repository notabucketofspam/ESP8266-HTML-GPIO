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

/* Import from general_server.c bugfix */

httpd_uri_t index_html_get_core = {
  .uri      = "/index.html",
  .method   = HTTP_GET,
  .handler  = index_html_get_handler_core,
  .user_ctx = NULL,
};

httpd_uri_t base_path_get_core = {
  .uri      = "/",
  .method   = HTTP_GET,
  .handler  = base_path_get_handler_core,
  .user_ctx = NULL,
};

esp_err_t setup_core(core_config_t core_config) {
  ESP_LOGI(CORE_TAG, "Setup core");
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  // ESP_ERROR_CHECK(setup_storage(core_config.storage_config));
  ESP_ERROR_CHECK(esp_vfs_spiffs_register(&core_config.storage_config));
  size_t total_bytes, used_bytes;
  ESP_ERROR_CHECK(esp_spiffs_info(NULL, &total_bytes, &used_bytes));
  ESP_LOGD(STORAGE_TAG,"SPIFFS: total bytes: %d used bytes: %d", total_bytes, used_bytes);
  //ESP_ERROR_CHECK(storage_access(f_gpio_state, "/spiffs/gpio_state", STORAGE_READ_WRITE));
  f_gpio_state = fopen("/spiffs/gpio_state", "r+");
  // core_config.control_config.keep_peripheral = CONTROL_UART_0 | CONTROL_UART_1;
  ESP_ERROR_CHECK(setup_control(core_config.control_config));
  if (core_config.control_config.auto_load_persistent_pin_state)
    ESP_ERROR_CHECK(load_persistent_gpio_state(f_gpio_state));
  ESP_ERROR_CHECK(setup_network(core_config.network_config));
  //ESP_ERROR_CHECK(storage_access(f_index_html, "/spiffs/index.html", STORAGE_READ));
  vTaskDelay(500 / portTICK_PERIOD_MS);
  f_index_html_core = fopen("/spiffs/index.html", "r");
  if (f_index_html_core != NULL)
    ESP_LOGI(CORE_TAG, "index 0x%x", (unsigned int) f_index_html_core);
  else
    ESP_LOGE(CORE_TAG, "index load failure");
  // while(1)
    // ESP_LOGE(CORE_TAG, "%c", fgetc(f_index_html_core));
  // ESP_ERROR_CHECK(setup_server(core_config.server_config));
  // Start server import
  httpd_config_t httpd_config_core = HTTPD_DEFAULT_CONFIG();
  s_httpd_server_core = NULL;
  ESP_ERROR_CHECK(httpd_start(&s_httpd_server_core, &httpd_config_core));
  //ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server_core, &index_html_get_core));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server_core, &base_path_get_core));
  ESP_LOGD(CORE_TAG, "handle 0x%x", (unsigned int) s_httpd_server_core);
  // End server import
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server_core, &pins_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server_core, &save_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server_core, &load_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server_core, &gpio_get));
  ESP_LOGI(CORE_TAG, "Core OK");
  // ESP_LOGI(CORE_TAG, "start index.html");
  // char buf[256] = { 0 };
  // while (fgets(buf, sizeof(buf), f_index_html_core) != NULL)
  //   ESP_LOGD(CORE_TAG, "%s", buf);
  // rewind(f_index_html_core);
  // ESP_LOGI(CORE_TAG, "conclude index.html");
  return ESP_OK;
}

static esp_err_t pins_get_handler(httpd_req_t *req) {
  char buf[GPIO_PIN_COUNT + 1] = { 0 };
  memcpy(buf, s_pin_bit_mask, sizeof(s_pin_bit_mask));
  ESP_LOGW(CORE_TAG, "mask: %s", buf);
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, buf, -1));
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, " ", -1));
  memcpy(buf, s_pin_mode, sizeof(s_pin_mode));
  ESP_LOGW(CORE_TAG, "mode: %s", buf);
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, buf, -1));
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t save_get_handler(httpd_req_t *req) {
  uint8_t state_index;
  for (state_index = 0; state_index < GPIO_PIN_COUNT; ++state_index)
    fputc(s_gpio_state_mem[state_index], f_gpio_state);
  fflush(f_gpio_state);
  rewind(f_gpio_state);
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t load_get_handler(httpd_req_t *req) {
  ESP_ERROR_CHECK(load_persistent_gpio_state(f_gpio_state));
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, s_gpio_state_mem, sizeof(s_gpio_state_mem)));
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t gpio_get_handler(httpd_req_t *req) {
  char query_str_buf[128] = { 0 };
  ESP_ERROR_CHECK(httpd_req_get_url_query_str(req, query_str_buf, sizeof(query_str_buf)));
  char query_gpio_pin_num_buf[4] = { 0 };
  ESP_ERROR_CHECK(httpd_query_key_value(query_str_buf, "gpio_pin_num", 
    query_gpio_pin_num_buf, sizeof(query_gpio_pin_num_buf)));
  int gpio_pin_num = atoi(query_gpio_pin_num_buf);
  char query_gpio_set_level_buf[8] = { 0 };
  ESP_ERROR_CHECK(httpd_query_key_value(query_str_buf, "gpio_set_level", 
    query_gpio_set_level_buf, sizeof(query_gpio_set_level_buf)));
  bool gpio_level = (strcmp(query_gpio_set_level_buf, "true") != 0);
  char gpio_level_buf[2] = { 0 };
  switch (s_pin_mode[gpio_pin_num]) {
    case '1':
      gpio_level_buf[0] = 0x30 + gpio_get_level(gpio_pin_num);
      ESP_ERROR_CHECK(httpd_resp_send_chunk(req, gpio_level_buf, strlen(gpio_level_buf)));
      break;
    case '2':
      ESP_ERROR_CHECK(gpio_set_level(gpio_pin_num, gpio_level));
      s_gpio_state_mem[gpio_pin_num] = gpio_level;
      break;
    default: break;
  }
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, NULL, 0));
  return ESP_OK;
}

/* Import bugfix from general_server.c */

static esp_err_t index_html_get_handler_core(httpd_req_t *req) {
  ESP_ERROR_CHECK(httpd_resp_set_status(req, "307 Temporary Redirect"));
  ESP_ERROR_CHECK(httpd_resp_set_hdr(req, "Location", "/"));
  ESP_ERROR_CHECK(httpd_resp_send(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t base_path_get_handler_core(httpd_req_t *req) {
  char index_html_get_buf[CONFIG_HTTPD_RESP_BUF_SIZE + 1] = { 0 };
  fpos_t file_pos;
  do {
    // if (fgets(index_html_get_buf, sizeof(index_html_get_buf), f_index_html_core) == NULL)
    //   return ESP_FAIL;
    // else
    //   ESP_LOGD(CORE_TAG, "%s", index_html_get_buf);
    fgets(index_html_get_buf, sizeof(index_html_get_buf), f_index_html_core);
    if (strlen(index_html_get_buf) == 0)
      break;
    ESP_LOGD(CORE_TAG, "%s", index_html_get_buf);
    ESP_ERROR_CHECK(httpd_resp_send_chunk(req, index_html_get_buf, -1));
    // vTaskDelay(500 / portTICK_PERIOD_MS);
    memset(index_html_get_buf, 0x00, sizeof(index_html_get_buf));
    fgetpos(f_index_html_core, &file_pos);
  } while (file_pos != EOF);
  rewind(f_index_html_core);
  vTaskDelay(500 / portTICK_PERIOD_MS);
  ESP_ERROR_CHECK(httpd_resp_send_chunk(req, NULL, 0));
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif
