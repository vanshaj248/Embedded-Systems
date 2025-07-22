#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_http_server.h"

// Wi-Fi credentials (replace with your own)
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"

// Logging tag
static const char *TAG = "file_server";

// HTTP server handle
httpd_handle_t server = NULL;

// Initialize Wi-Fi
static void wifi_init(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();
    ESP_LOGI(TAG, "Wi-Fi initialized");
}

// HTTP GET handler to simulate file listing (no SD card yet)
static esp_err_t list_files_handler(httpd_req_t *req) {
    char resp[1024] = {0};
    strcat(resp, "<html><body><h1>Files on SD Card (Simulated)</h1><ul>");
    strcat(resp, "<li>test1.txt</li><li>test2.txt</li>");
    strcat(resp, "</ul><p>SD card not connected. This is a simulation.</p></body></html>");
    httpd_resp_send(req, resp, strlen(resp));
    return ESP_OK;
}

// Start HTTP server
static void start_http_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t list_files = {
            .uri = "/files",
            .method = HTTP_GET,
            .handler = list_files_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &list_files);
        ESP_LOGI(TAG, "HTTP server started");
    } else {
        ESP_LOGE(TAG, "Failed to start HTTP server");
    }
}

// FreeRTOS task for Wi-Fi and HTTP server
static void wifi_and_server_task(void *pvParameters) {
    wifi_init();
    vTaskDelay(pdMS_TO_TICKS(5000)); // Wait for Wi-Fi connection
    start_http_server();
    vTaskDelete(NULL);
}

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    // Create Wi-Fi and server task
    xTaskCreate(wifi_and_server_task, "wifi_and_server_task", 4096, NULL, 5, NULL);
}