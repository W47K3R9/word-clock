/**
 * Author: Lucas Scheidt
 * Date: 29.09.24
 * 
 * Description: 
 *
 */

#include "time_server_connection.h"
#include "esp_sntp.h"
#include <time.h>

static void time_sync_notification_cb(struct timeval* /* tv */) { printf("[INFO] synchronizing to time server!\n"); }

void get_current_time(char* date_time, size_t buffer_size, int* t_min_and_hour)
{
    time_t now;
    struct tm time_spec;
    time(&now);
    localtime_r(&now, &time_spec);
    t_min_and_hour[0] = time_spec.tm_hour;
    t_min_and_hour[1] = time_spec.tm_min;
    strftime(date_time, buffer_size, "%c", &time_spec);
}

static void initialize_sntp(void)
{
    const char* ntp_server = "pool.ntp.org";
    printf("[INFO] initializing SNTP server (%s) ...\n", ntp_server);
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, ntp_server);
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    esp_sntp_init();
}

void set_system_time()
{
    initialize_sntp();
    const int max_retries = 10;
    int retry_count = 0;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry_count < max_retries)
    {
        printf("[INFO] waiting for system time to be set, (%d/%d) retries...\n", retry_count, max_retries);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    if (retry_count == max_retries)
    {
        printf("[ERROR] couldn't connect after %d retries, exiting application!\n", max_retries);
        exit(-2);
    }
    setenv("TZ", "UTC-02:00", 1);
    tzset();
}
