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

static struct tm create_current_time_struct_and_set_time(HourAndMinute* hour_minute)
{
    time_t now;
    struct tm time_spec;
    time(&now);
    localtime_r(&now, &time_spec);
    hour_minute->hour = (uint8_t)time_spec.tm_hour;
    hour_minute->minute = (uint8_t)time_spec.tm_min;
    return time_spec;
}

void get_current_time_with_description(char* date_time, const size_t buffer_size, HourAndMinute* hour_minute)
{
    const struct tm time_spec = create_current_time_struct_and_set_time(hour_minute);
    strftime(date_time, buffer_size, "%c", &time_spec);
}

void get_current_time(HourAndMinute* hour_minute)
{
    create_current_time_struct_and_set_time(hour_minute);
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
    const uint8_t max_retries = 12;
    uint8_t retry_count = 0;
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