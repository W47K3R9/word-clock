#include "webserver.h"
#include <time.h>

enum CurrentTime
{
    SUMMER, WINTER
};
static void reset_timezone(const enum CurrentTime ct)
{
    // If this is zero the environment variable will NOT be overwritten!
    const int overwrite_env_variable = 1;
    switch (ct)
    {
    case WINTER:
        setenv("TZ", "UTC-01:00", overwrite_env_variable);
        break;
    case SUMMER:
        setenv("TZ", "UTC-02:00", overwrite_env_variable);
        break;
    }
    tzset();
}

static esp_err_t summertime_post_handler(httpd_req_t* req)
{
    reset_timezone(SUMMER);
    httpd_resp_send(req, "Die Uhr ist jetzt auf Sommerzeit gestellt (+1h)\n", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t wintertime_post_handler(httpd_req_t* req)
{
    reset_timezone(WINTER);
    httpd_resp_send(req, "Die Uhr ist jetzt auf Winterzeit gestellt (+0h)\n", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

httpd_handle_t register_uri_handlers()
{
    const httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        const httpd_uri_t summertime = {
            .uri       = "/sommer",
            .method    = HTTP_POST,
            .handler   = summertime_post_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &summertime);

        const httpd_uri_t wintertime = {
            .uri       = "/winter",
            .method    = HTTP_POST,
            .handler   = wintertime_post_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &wintertime);
    }
    return server;
}
