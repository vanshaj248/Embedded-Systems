#include <stdio.h>
#include <string.h>
#include <dirent.h>

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

#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

// ---------------- Wi-Fi Credentials ----------------
#define WIFI_SSID "Vanshaj's iPhone"
#define WIFI_PASS "20052408"

// ---------------- SD Card SPI Pins ----------------
#define PIN_NUM_MISO 10  // MISO pin on ESP32-S3
#define PIN_NUM_MOSI 13  // MOSI pin on ESP32-S3
#define PIN_NUM_CLK  12  // SCK pin on ESP32-S3
#define PIN_NUM_CS   11  // CS pin on ESP32-S3

// ---------------- Misc ----------------
#define MIN(a,b) ((a) < (b) ? (a) : (b))
static const char *TAG = "FileServer";

// ---------------- Globals ----------------
httpd_handle_t server = NULL;
static const char *MOUNT_POINT = "/sdcard";

static esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
};

// ---------------- HTTP Handlers ----------------

// List Files
static esp_err_t list_files_handler(httpd_req_t *req) {
    httpd_resp_sendstr_chunk(req, "<html><body><h1>Files on SD Card</h1><ul>");

    DIR *dir = opendir(MOUNT_POINT);
    if (!dir) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to open directory");
        return ESP_FAIL;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char line[300];
        snprintf(line, sizeof(line), "<li>%s</li>", entry->d_name);
        httpd_resp_sendstr_chunk(req, line);
    }
    closedir(dir);

    httpd_resp_sendstr_chunk(req, "</ul></body></html>");
    httpd_resp_sendstr_chunk(req, NULL); // End response
    return ESP_OK;
}

// Read File
static esp_err_t read_file_handler(httpd_req_t *req) {
    char filepath[256];
    char query[128];
    char filename[64];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) != ESP_OK ||
        httpd_query_key_value(query, "file", filename, sizeof(filename)) != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing file parameter");
        return ESP_FAIL;
    }

    snprintf(filepath, sizeof(filepath), "%s/%s", MOUNT_POINT, filename);
    FILE *f = fopen(filepath, "r");
    if (!f) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
        return ESP_FAIL;
    }

    char buffer[512];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        httpd_resp_send_chunk(req, buffer, bytes_read);
    }
    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0); // End response
    return ESP_OK;
}

// Write File
static esp_err_t write_file_handler(httpd_req_t *req) {
    char filepath[256];
    char query[128];
    char filename[64];
    char content[1024];

    if (httpd_req_get_url_query_str(req, query, sizeof(query)) != ESP_OK ||
        httpd_query_key_value(query, "file", filename, sizeof(filename)) != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing file parameter");
        return ESP_FAIL;
    }

    snprintf(filepath, sizeof(filepath), "%s/%s", MOUNT_POINT, filename);
    FILE *f = fopen(filepath, "w");
    if (!f) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to open file");
        return ESP_FAIL;
    }

    int received;
    size_t total = 0;
    while (total < req->content_len) {
        received = httpd_req_recv(req, content, MIN(sizeof(content), req->content_len - total));
        if (received <= 0) {
            fclose(f);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive data");
            return ESP_FAIL;
        }
        fwrite(content, 1, received, f);
        total += received;
    }
    fclose(f);

    httpd_resp_sendstr(req, "File written successfully");
    return ESP_OK;
}

// ---------------- HTTP Server Init ----------------
static void start_http_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.stack_size = 8192;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t list_files = {
            .uri = "/files",
            .method = HTTP_GET,
            .handler = list_files_handler
        };
        httpd_register_uri_handler(server, &list_files);

        httpd_uri_t read_file = {
            .uri = "/read",
            .method = HTTP_GET,
            .handler = read_file_handler
        };
        httpd_register_uri_handler(server, &read_file);

        httpd_uri_t write_file = {
            .uri = "/write",
            .method = HTTP_POST,
            .handler = write_file_handler
        };
        httpd_register_uri_handler(server, &write_file);

        ESP_LOGI(TAG, "HTTP server started");
    } else {
        ESP_LOGE(TAG, "Failed to start HTTP server");
    }
}

// ---------------- Wi-Fi Event Handler ----------------
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Retrying connection...");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Wi-Fi connected, starting HTTP server...");
        start_http_server();
    }
}

// ---------------- Wi-Fi Init ----------------
static void wifi_init(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &event_handler,
                                        NULL,
                                        &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &event_handler,
                                        NULL,
                                        &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
}

// ---------------- SD Card Init ----------------
static void sd_card_init(void) {
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI2_HOST;  // Match your bus
    
    // Configure SPI bus
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA));
    
    // Configure SD card device
    sdspi_device_config_t dev_cfg = SDSPI_DEVICE_CONFIG_DEFAULT();
    dev_cfg.host_id = host.slot;
    dev_cfg.gpio_cs = PIN_NUM_CS;
    
    // Card info
    sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &dev_cfg, &mount_config, &card);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "SD card mounted successfully");
    } else {
        ESP_LOGE(TAG, "Failed to mount SD card: %s", esp_err_to_name(ret));
    }
}
// ---------------- Main ----------------
void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    sd_card_init();
    wifi_init();
}