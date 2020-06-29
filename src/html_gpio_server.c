#include "html_gpio_server.h"

#ifdef __cplusplus
extern "C" {
#endif

httpd_uri_t index_html_get = {
  .uri      = "/index.html",
  .method   = HTTP_GET,
  .handler  = index_html_get_handler,
  .user_ctx = NULL
};

httpd_uri_t gpio_post = {
  .uri      = "/index.html",
  .method   = HTTP_POST,
  .handler  = gpio_post_handler,
  .user_ctx = NULL
};

esp_err_t setup_server(const server_config_t *server_config) {
  ESP_LOGI(TAG, "Setup server");
  s_httpd_server = NULL;
  ESP_ERROR_CHECK(httpd_start(&s_httpd_server, &server_config));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &index_html_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &gpio_post));
  ESP_LOGI(TAG, "Server OK");
  return ESP_OK;
}

static esp_err_t index_html_get_handler(httpd_req_t *req) {
  static uint8_t index_html_get_buf[CONFIG_HTTPD_RESP_BUF_SIZE];
  memset(index_html_get_buf, 0x00, sizeof(index_html_get_buf));
  fpos_t file_pos;
  do {
    fgets(index_html_get_buf, CONFIG_HTTPD_RESP_BUF_SIZE, s_index_html);
    httpd_resp_send_chunk(req, index_html_get_buf, CONFIG_HTTPD_RESP_BUF_SIZE);
    fgetpos(s_index_html, file_pos);
  } while (file_pos);
  rewind(s_index_html);
  return ESP_OK;
}

static esp_err_t gpio_post_handler(httpd_req_t *req) {
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif