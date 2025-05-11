#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "wifi.h"

#define LED_PIN CYW43_WL_GPIO_LED_PIN
#define SERVER_IP "192.168.1.100"  // IP do servidor
#define SERVER_PORT 3000           // Porta do servidor

static bool estado_botao_global = false;

// Função para conectar ao Wi-Fi
bool connect_wifi(const char* ssid, const char* password) {
    printf("[Wi-Fi] Inicializando...\n");

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_BRAZIL)) {
        printf("[Wi-Fi] Erro ao inicializar o driver Wi-Fi\n");
        return false;
    }

    cyw43_arch_enable_sta_mode();
    printf("[Wi-Fi] Conectando a %s...\n", ssid);

    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("[Wi-Fi] Falha ao conectar ao Wi-Fi\n");
        return false;
    }

    if (netif_default) {
        printf("[Wi-Fi] Conectado com IP: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    cyw43_arch_gpio_put(LED_PIN, 1); // LED onboard ligado ao conectar
    return true;
}

// Função para enviar os dados do joystick ao servidor
void send_joystick_data(int x, int y, bool button_pressed) {
    char body[128];
    int ts = to_ms_since_boot(get_absolute_time()) / 1000;
    int body_len = snprintf(body, sizeof(body),
        "{\"x\":%d,\"y\":%d,\"button\":\"%s\",\"timestamp\":%d}",
        x, y, button_pressed ? "PRESSED" : "RELEASED", ts);

    char req[256];
    int req_len = snprintf(req, sizeof(req),
        "POST /joystick HTTP/1.1\r\n"
        "Host: " SERVER_IP ":%d\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n\r\n"
        "%s",
        SERVER_PORT, body_len, body);

    int sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("[Wi-Fi] Erro ao criar socket.\n");
        return;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port   = htons(SERVER_PORT),
    };
    inet_aton(SERVER_IP, &addr.sin_addr);

    if (lwip_connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
        lwip_send(sock, req, req_len, 0);  // Enviando a requisição
    } else {
        printf("[Wi-Fi] Erro ao conectar no servidor.\n");
    }

    lwip_close(sock);  // Fechando o socket
}
