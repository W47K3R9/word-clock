#include <time.h>
#include <sys/time.h>
#include "esp_attr.h"
#include "esp_sleep.h"
#include "esp_sntp.h"

//wifi
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define SSID "GumboNet"
#define PWD "L0ohlYpep@Wuz$"

/* Wi-Fi event handling is based on the esp_event library. Events are sent by
 the Wi-Fi driver to the default event loop. Application may handle these
 events in callbacks registered using esp_event_handler_register()

 There are five tasks that intercommunicate with one another
 MainTask -> AppTask -> EventTask -> LwIPTask -> WiFiTask
 refer to
 https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/wifi.html#esp32-wi-fi-station-general-scenario
 for more information. */

int retry_num = 0;
static void wifi_event_handler(void* event_handler_arg,
                               esp_event_base_t event_base, int32_t event_id,
                               void* event_data)
{
  switch (event_id)
  {
    case WIFI_EVENT_STA_START:printf("WiFi connecting...\n");
      break;
    case WIFI_EVENT_STA_CONNECTED:printf("WiFi connected\n");
      break;
    case WIFI_EVENT_STA_DISCONNECTED:printf("WiFi lost connection\n");
      if (retry_num < 5)
      {
        esp_wifi_connect();
        retry_num++;
        printf("Retrying to connect...\n");
      }
      break;
    case IP_EVENT_STA_GOT_IP:printf("Wifi got IP...\n\n");
  }
}

void connect_gumbonet()
{
  // Step 1.1 create a Lightweight IP (LwIP) task that handles IP requirements
  esp_netif_init();
  // Step 1.2 create Event task. Most of the time the WiFi task will
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
  // location. No need for memcpy or strcpy afterwards if arrays are used!
  wifi_config_t my_config = {.ap = {.ssid = SSID, .password = PWD}};
  esp_wifi_set_config(ESP_IF_WIFI_STA, &my_config);
  esp_wifi_set_mode(WIFI_MODE_STA);
  // Connect event handlers
  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler,
                             NULL);
  esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler,
                             NULL);

  // Step 3 start the driver!
  esp_wifi_start();
  // Step 4 connect to wifi
  esp_wifi_connect();
  vTaskDelay(5000 / portTICK_PERIOD_MS);
}

void time_sync_notification_cb(struct timeval *tv)
{
  printf("Notification of a time synchronization event");
}
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void Get_current_date_time(char *date_time){
	char strftime_buf[64];
	time_t now;
	    struct tm timeinfo;
	    time(&now);
	    localtime_r(&now, &timeinfo);

	    	// Set timezone to Indian Standard Time
	    	    setenv("TZ", "UTC-02:00", 1);
	    	    tzset();
	    	    localtime_r(&now, &timeinfo);

	    	    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
            strcpy(date_time,strftime_buf);
}

static void initialize_sntp(void)
{
    printf("Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
    sntp_init();
}

static void obtain_time(void)
{
    initialize_sntp();
    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        printf("Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}

void Set_SystemTime_SNTP() {
	 time_t now;
	    struct tm timeinfo;
	    time(&now);
	    localtime_r(&now, &timeinfo);
	    // Is time set? If not, tm_year will be (1970 - 1900).
	    if (timeinfo.tm_year < (2016 - 1900)) {
	        printf("Time is not set yet. Connecting to WiFi and getting time over NTP.");
	        obtain_time();
	        // update 'now' variable with current time
	        time(&now);
	    }
}

char Current_Date_Time[100];
void app_main()
{
  nvs_flash_init();
  // returns ESP_OK at success but for demo purposes
  // I'll just test the functionality.
  connect_gumbonet();
  Set_SystemTime_SNTP();
  while (1)
  {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    Get_current_date_time(Current_Date_Time);
    printf("current date and time is = %s\n",Current_Date_Time);
  }
}
