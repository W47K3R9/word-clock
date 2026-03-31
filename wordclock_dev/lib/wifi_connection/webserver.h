/**
 * Author: Lucas Scheidt
 * Date: 31.03.26
 * 
 * Description: Sets up the URI handler to set winter and summertime.
 */

#ifndef WEBSERVER_H
#define WEBSERVER_H
#include <esp_http_server.h>

httpd_handle_t register_uri_handlers();

#endif // WEBSERVER_H
