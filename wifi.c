#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"
#include "wifi.h"

// === CONFIGURAÇÕES DO LED ONBOARD ===
#define LED_PIN CYW43_WL_GPIO_LED_PIN

// === VARIÁVEL GLOBAL PARA O ESTADO DO BOTÃO ===
static bool estado_botao_global = false;

// === Função para conectar ao Wi-Fi ===
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

// === Atualiza o estado do botão (usado pelo main.c) ===
void update_button_state(bool pressed) {
    estado_botao_global = pressed;
}

// === Função auxiliar (opcional) para logar o status do botão ===
void send_button_status(bool pressed) {
    printf("[Wi-Fi] Botão: %s\n", pressed ? "PRESSIONADO" : "SOLTO");
}

// === Função auxiliar (opcional) para logar temperatura ===
void send_temperature_status() {
    adc_select_input(4); // Temperatura interna
    uint16_t raw = adc_read();
    const float conv = 3.3f / (1 << 12);
    float temp_c = 27.0f - ((raw * conv) - 0.706f) / 0.001721f;

    printf("[Wi-Fi] Temperatura interna: %.2f °C\n", temp_c);
}

// === Callback ao receber requisição HTTP ===
static err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    pbuf_free(p); // Liberar o buffer de entrada

    char html[512];
    snprintf(html, sizeof(html),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><head><meta http-equiv='refresh' content='1'></head><body>"
        "<h1>Status do Botão</h1>"
        "<p>Botão: <strong>%s</strong></p>"
        "</body></html>",
        estado_botao_global ? "PRESSIONADO" : "SOLTO"
    );

    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);

    return ERR_OK;
}

// === Callback ao aceitar conexão ===
static err_t http_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_recv);
    return ERR_OK;
}

// === Inicia o servidor HTTP ===
void iniciar_http_server() {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar socket TCP\n");
        return;
    }

    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao fazer bind da porta 80\n");
        return;
    }

    pcb = tcp_listen(pcb);
    tcp_accept(pcb, http_accept);

    printf("[HTTP] Servidor iniciado na porta 80\n");
}