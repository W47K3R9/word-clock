#include "webserver.h"

esp_err_t summertime_post_handler(httpd_req_t* req)
{
    // Handle the request
    // ...

    // Return ESP_OK if the request was handled successfully
    return ESP_OK;

    // Return an error code to close the connection
    // return ESP_FAIL;
}

erp_err_t wintertime_post_handler(httpd_req_t* req)
{
  return ESP_OK;
}

httpd_handle_t register_uri_handlers(httpd_handle_t server)
{

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_uri_t summertime = {
        .uri       = "/sommer",
        .method    = HTTP_POST,
        .handler   = summertime_post_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &summertime);

    httpd_uri_t wintertime = {
        .uri       = "/winter",
        .method    = HTTP_POST,
        .handler   = wintertime_post_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &wintertime);
    return server;
}
