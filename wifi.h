#ifndef WIFI_H
#define WIFI_H

#include <stdbool.h>

bool connect_wifi(const char *ssid, const char *password);
void send_button_status(bool pressed);
void send_temperature_status(); 
void update_button_state(bool pressed);
void iniciar_http_server();

#endif

