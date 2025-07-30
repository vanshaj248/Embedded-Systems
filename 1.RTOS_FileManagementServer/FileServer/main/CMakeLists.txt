idf_component_register(SRCS "main.c"
                    PRIV_REQUIRES spi_flash
                    INCLUDE_DIRS ""
                    PRIV_REQUIRES esp_wifi esp_netif esp_event nvs_flash esp_http_server fatfs)

