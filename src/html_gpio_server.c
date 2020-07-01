#include "html_gpio_server.h"

#ifdef __cplusplus
extern "C" {
#endif

httpd_uri_t index_html_get = {
  .uri      = "/index.html",
  .method   = HTTP_GET,
  .handler  = index_html_get_handler,
  .user_ctx = NULL,
};

httpd_uri_t base_path_get = {
  .uri      = "/",
  .method   = HTTP_GET,
  .handler  = base_path_get_handler,
  .user_ctx = NULL,
};

esp_err_t setup_server(const server_config_t *server_config) {
  ESP_LOGI(TAG, "Setup server");
  s_httpd_server = NULL;
  ESP_ERROR_CHECK(httpd_start(&s_httpd_server, &server_config));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &index_html_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &base_path_get));
  ESP_LOGI(TAG, "Server OK");
  return ESP_OK;
}

static esp_err_t index_html_get_handler(httpd_req_t *req) {
  ESP_ERROR_CHECK(httpd_resp_set_status(req, "307 Temporary Redirect"));
  ESP_ERROR_CHECK(httpd_resp_set_hdr(req, "Location", "/"));
  ESP_ERROR_CHECK(httpd_resp_send(req, NULL, 0));
  return ESP_OK;
}

static esp_err_t base_path_get_handler(httpd_req_t *req) {
  static uint8_t index_html_get_buf[CONFIG_HTTPD_RESP_BUF_SIZE];
  memset(index_html_get_buf, 0x00, sizeof(index_html_get_buf));
  fpos_t file_pos;
  do {
    fgets(index_html_get_buf, CONFIG_HTTPD_RESP_BUF_SIZE, s_index_html);
    ESP_ERROR_CHECK(httpd_resp_send_chunk(req, index_html_get_buf, CONFIG_HTTPD_RESP_BUF_SIZE));
    fgetpos(s_index_html, file_pos);
  } while (file_pos != EOF);
  rewind(s_index_html);
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif