#ifndef HTML_GPIO_INCLUDE_H
#define HTML_GPIO_INCLUDE_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "driver/gpio.h"
#include "esp_http_server.h"
#include "esp_spiffs.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"

#ifdef SDKCONFIG_H_NOT_FOUND
#include "../build/include/sdkconfig.h"
#endif

#ifndef __ESP_FILE__
#define __ESP_FILE__ __FILE__
#endif

#endif // HTML_GPIO_INCLUDE_H
