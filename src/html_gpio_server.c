#include "html_gpio_server.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t setup_server(void) {
  ESP_LOGI(TAG, "Setup server");
  s_httpd_server = NULL;
  httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG();
  ESP_ERROR_CHECK(httpd_start(&s_httpd_server, &httpd_config));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &uri_get));
  ESP_ERROR_CHECK(httpd_register_uri_handler(s_httpd_server, &uri_post));
  ESP_LOGI(TAG, "Server OK");
  return ESP_OK;
}

httpd_uri_t uri_get = {
  .uri      = "/uri",
  .method   = HTTP_GET,
  .handler  = get_handler,
  .user_ctx = NULL
};

httpd_uri_t uri_post = {
  .uri      = "/uri",
  .method   = HTTP_POST,
  .handler  = post_handler,
  .user_ctx = NULL
};

esp_err_t get_handler(httpd_req_t *req) {
  const char resp[] = "URI GET Response";
  httpd_resp_send(req, resp, strlen(resp));
  return ESP_OK;
}

esp_err_t post_handler(httpd_req_t *req) {
  char content[100];
  size_t recv_size = MIN(req->content_len, sizeof(content));
  int ret = httpd_req_recv(req, content, recv_size);
  if (ret <= 0) {
    if (ret == HTTPD_SOCK_ERR_TIMEOUT)
      httpd_resp_send_408(req);
    return ESP_FAIL;
  }
  const char resp[] = "URI POST Response";
  httpd_resp_send(req, resp, strlen(resp));
  return ESP_OK;
}

#ifdef __cplusplus
}
#endif