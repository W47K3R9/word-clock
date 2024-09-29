/**
 * Author: Lucas Scheidt
 * Date: 29.09.24
 * 
 * Description:Wi-Fi event handling is based on the esp_event library. Events are sent by
 * the Wi-Fi driver to the default event loop. Application may handle these
 * events in callbacks registered using esp_event_handler_register()
 *
 * There are five tasks that intercommunicate with one another
 * MainTask -> AppTask -> EventTask -> LwIPTask -> Wi-FiTask
 * refer to
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/wifi.html#esp32-wi-fi-station-general-scenario
 * for more information.
 */

#include "esp_wifi.h"
#include "esp_event.h"
#include "wifi_connection.h"
#include "constants.h"

static void wifi_event_handler(void* /* event_handler_arg */, esp_event_base_t /* event_base */, int32_t event_id,
                               void* /* event_data */)
{
    unsigned char retry_num = 0;
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("[INFO] Wi-Fi connecting...\n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("[SUCCESS] Wi-Fi connected!\n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("[WARNING] Wi-Fi lost connection!\n");
        if (retry_num < 5)
        {
            esp_wifi_connect();
            retry_num++;
            printf("[INFO] retrying to connect...\n");
        }
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("[INFO] Wi-Fi got IP...\n");
        break;
    default:
        printf("[ERROR] no valid event_id recognized!\n");
        break;
    }
}

void connect_to_station()
{
    // Step 1.1 create a Lightweight IP (LwIP) task that handles IP requirements
    esp_netif_init();
    // Step 1.2 create Event task. Most of the time the Wi-Fi task will
    // communicate with the event task and the latter forwards the information
    // to the AppTask.
    esp_event_loop_create_default();
    // Step 1.3 bind station with tcp/ip task.
    esp_netif_create_default_wifi_sta();
    // Step 1.4 initialize wifi driver.
    wifi_init_config_t wifi_init = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init);

    // Step 2 once the driver is initialized, it can now be configured.
    // Something new learned: C99 introduced designated initializers
    // for struct, that directly copy the initialized value to the correct
    // location. No need for memcpy or strcpy afterward if arrays are used!
    wifi_config_t my_config = {.ap = {.ssid = SSID, .password = PWD}};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &my_config);
    esp_wifi_set_mode(WIFI_MODE_STA);
    // Connect event handlers
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    // Step 3 start the driver!
    esp_wifi_start();
    // Step 4 connect to Wi-Fi
    esp_wifi_connect();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
}