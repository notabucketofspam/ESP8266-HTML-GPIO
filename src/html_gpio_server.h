#ifndef HTML_GPIO_SERVER_H
#define HTML_GPIO_SERVER_H

#include "html_gpio_include.h"
#include "esp_http_server.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *TAG = "html_gpio_server";
static httpd_handle_t s_httpd_server;
extern httpd_uri_t index_html_get;
extern httpd_uri_t gpio_post;
static FILE *s_index_html;

esp_err_t setup_server(void);
static esp_err_t index_html_get_handler(httpd_req_t *req);
static esp_err_t gpio_post_handler(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif //HTML_GPIO_SERVER_H