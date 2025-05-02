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






// #include "pico/stdlib.h"
// #include "hardware/adc.h"
// #include "pico/cyw43_arch.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include "lwip/pbuf.h"
// #include "lwip/tcp.h"
// #include "lwip/netif.h"
// #include "wifi.h"

// // Configurações de Wi-Fi
// #define WIFI_SSID "MAMBEE"
// #define WIFI_PASSWORD "1fp1mamb33"

// // Definição dos pinos dos LEDs
// #define LED_PIN CYW43_WL_GPIO_LED_PIN
// #define LED_BLUE_PIN 12  // GPIO12 - LED azul
// #define LED_GREEN_PIN 11 // GPIO11 - LED verde
// #define LED_RED_PIN 13   // GPIO13 - LED vermelho
// #define BUTTON_A 5 // butão A 

// int debounce_button() {
//     static uint32_t last_time = 0;
//     const uint32_t debounce_time = 50; // ms
    
//     if (time_us_32() / 1000 - last_time < debounce_time) {
//         return -1; // Ignorar leituras muito próximas
//     }
    
//     last_time = time_us_32() / 1000;
//     return !gpio_get(BUTTON_A); // Retorna 1 se pressionado, 0 se solto
// }

// // Função de callback para processar requisições HTTP
// static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
// {
//     if (!p)
//     {
//         tcp_close(tpcb);
//         tcp_recv(tpcb, NULL);
//         return ERR_OK;
//     }

//     char *request = (char *)malloc(p->len + 1);
//     memcpy(request, p->payload, p->len);
//     request[p->len] = '\0';

//     printf("Request: %s\n", request);

//     // Controle dos LEDs
//     if (strstr(request, "GET /blue_on") != NULL)
//     {
//         gpio_put(LED_BLUE_PIN, 1);
//     }
//     else if (strstr(request, "GET /blue_off") != NULL)
//     {
//         gpio_put(LED_BLUE_PIN, 0);
//     }
//     else if (strstr(request, "GET /green_on") != NULL)
//     {
//         gpio_put(LED_GREEN_PIN, 1);
//     }
//     else if (strstr(request, "GET /green_off") != NULL)
//     {
//         gpio_put(LED_GREEN_PIN, 0);
//     }
//     else if (strstr(request, "GET /red_on") != NULL)
//     {
//         gpio_put(LED_RED_PIN, 1);
//     }
//     else if (strstr(request, "GET /red_off") != NULL)
//     {
//         gpio_put(LED_RED_PIN, 0);
//     }
//     else if (strstr(request, "GET /on") != NULL)
//     {
//         cyw43_arch_gpio_put(LED_PIN, 1);
//     }
//     else if (strstr(request, "GET /off") != NULL)
//     {
//         cyw43_arch_gpio_put(LED_PIN, 0);
//     }

//     // Leitura da temperatura interna
//     adc_select_input(4);
//     uint16_t raw_value = adc_read();
//     const float conversion_factor = 3.3f / (1 << 12);
//     float temperature = 27.0f - ((raw_value * conversion_factor) - 0.706f) / 0.001721f;

//     // leitura dos botões 
//     int button_state = debounce_button();
//     const char* button_status = (button_state == 1) ? "Pressionado" : "Solto";

//     // Cria a resposta HTML
//     char html[1024];

//     snprintf(html, sizeof(html),
//              "HTTP/1.1 200 OK\r\n"
//              "Content-Type: text/html\r\n"
//              "\r\n"
//              "<!DOCTYPE html>\n"
//              "<html>\n"
//              "<head>\n"
//              "<meta http-equiv=\"refresh\" content=\"1\">\n" 
//              "<title>LED Control</title>\n"
//              "<style>\n"
//              "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
//              "h1 { font-size: 64px; margin-bottom: 30px; }\n"
//              "button { font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
//              ".temperature { font-size: 48px; margin-top: 30px; color: #333; }\n"
//              "</style>\n"
//              "</head>\n"
//              "<body>\n"
//              "<h1>LED Control</h1>\n"
//              "<form action=\"./blue_on\"><button>Ligar Azul</button></form>\n"
//              "<form action=\"./blue_off\"><button>Desligar Azul</button></form>\n"
//              "<form action=\"./green_on\"><button>Ligar Verde</button></form>\n"
//              "<form action=\"./green_off\"><button>Desligar Verde</button></form>\n"
//              "<form action=\"./red_on\"><button>Ligar Vermelho</button></form>\n"
//              "<form action=\"./red_off\"><button>Desligar Vermelho</button></form>\n"
//              "<p class=\"temperature\">Temperatura Interna: %.2f &deg;C</p>\n"
//              "<p class=\"temperature\">Botao: %s</p>\n"
//              "</body>\n"
//              "</html>\n",
//              temperature, button_status);

//     /*       snprintf(html, sizeof(html),
//              "HTTP/1.1 200 OK\r\n"
//              "Content-Type: text/html\r\n"
//              "\r\n"
//              "<!DOCTYPE html>\n"
//              "<html>\n"
//              "<head>\n"
//              "<title>LED Control</title>\n"
//              "<style>\n"
//              "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
//              "h1 { font-size: 64px; margin-bottom: 30px; }\n"
//              "button { font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
//              ".temperature { font-size: 48px; margin-top: 30px; color: #333; }\n"
//              "</style>\n"
//              "</head>\n"
//              "<body>\n"
//              "<h1>LED Control</h1>\n"
//              "<form action=\"./blue_on\"><button style=\"background-color: #2196F3; color: white;\">Ligar Azul</button></form>\n"
//              "<form action=\"./blue_off\"><button style=\"background-color: #BBDEFB; color: black;\">Desligar Azul</button></form>\n"
//              "<form action=\"./green_on\"><button style=\"background-color: #4CAF50; color: white;\">Ligar Verde</button></form>\n"
//              "<form action=\"./green_off\"><button style=\"background-color: #C8E6C9; color: black;\">Desligar Verde</button></form>\n"
//              "<form action=\"./red_on\"><button style=\"background-color: #F44336; color: white;\">Ligar Vermelho</button></form>\n"
//              "<form action=\"./red_off\"><button style=\"background-color: #FFCDD2; color: black;\">Desligar Vermelho</button></form>\n"
//              "<p class=\"temperature\">Temperatura Interna: %.2f &deg;C</p>\n"
//              "</body>\n"
//              "</html>\n",
//              temperature);
//     */

//     tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
//     tcp_output(tpcb);

//     free(request);
//     pbuf_free(p);

//     return ERR_OK;
// }

// // Função de callback ao aceitar conexões TCP
// static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
// {
//     tcp_recv(newpcb, tcp_server_recv);
//     return ERR_OK;
// }

// // Função principal
// // int main()
// // {
// //     stdio_init_all();
// //     sleep_ms(3000);  // Espera 3 segundos para garantir conexão serial
// //     printf("Iniciando...\n");

// //     // inicializaçao do botão A 
// //     gpio_init(BUTTON_A);
// //     gpio_set_dir(BUTTON_A, GPIO_IN);
// //     gpio_pull_up(BUTTON_A);  // botão com pull-up interno

// //     // Configuração dos LEDs como saída
// //     gpio_init(LED_BLUE_PIN);
// //     gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
// //     gpio_put(LED_BLUE_PIN, false);

// //     gpio_init(LED_GREEN_PIN);
// //     gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
// //     gpio_put(LED_GREEN_PIN, false);

// //     gpio_init(LED_RED_PIN);
// //     gpio_set_dir(LED_RED_PIN, GPIO_OUT);
// //     gpio_put(LED_RED_PIN, false);

// //     // cyw43_arch_deinit(); // Desativa o Wi-Fi

// //     while (cyw43_arch_init())
// //     {
// //         printf("Falha ao inicializar Wi-Fi\n");
// //         sleep_ms(100);
// //         return -1;
// //     }

// //     cyw43_arch_gpio_put(LED_PIN, 0);
// //     cyw43_arch_enable_sta_mode();

// //     printf("Conectando ao Wi-Fi...\n");
// //     while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 20000))
// //     {
// //         printf("Falha ao conectar ao Wi-Fi\n");
// //         sleep_ms(100);
// //         return -1;
// //     }

// //     printf("Conectado ao Wi-Fi\n");

// //     if (netif_default)
// //     {
// //         printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
// //     }

// //     // Configura o servidor TCP
// //     struct tcp_pcb *server = tcp_new();
// //     if (!server)
// //     {
// //         printf("Falha ao criar servidor TCP\n");
// //         return -1;
// //     }

// //     if (tcp_bind(server, IP_ADDR_ANY, 80) != ERR_OK)
// //     {
// //         printf("Falha ao associar servidor TCP à porta 80\n");
// //         return -1;
// //     }

// //     server = tcp_listen(server);
// //     tcp_accept(server, tcp_server_accept);

// //     printf("Servidor ouvindo na porta 80\n");

// //     // Inicializa o ADC
// //     adc_init();
// //     adc_set_temp_sensor_enabled(true);

// //     while (true)
// //     {
// //         cyw43_arch_poll();

// //         static absolute_time_t last_print = 0;
// //         if (absolute_time_diff_us(last_print, get_absolute_time()) > 1000000) {
// //             last_print = get_absolute_time();
// //             printf("Botão A: %s\n", !gpio_get(BUTTON_A) ? "Pressionado" : "Solto");
// //         }

// //     }

// //     cyw43_arch_deinit();
// //     return 0;
// // }


// bool connect_wifi(const char* ssid, const char* password) {
//     if (cyw43_arch_init()) {
//         printf("Erro ao inicializar o Wi-Fi\n");
//         return false;
//     }

//     cyw43_arch_enable_sta_mode();

//     if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 20000)) {
//         printf("Falha ao conectar ao Wi-Fi\n");
//         return false;
//     }

//     if (netif_default) {
//         printf("Conectado com IP: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
//     }

//     return true;
// }

// void send_button_status(bool pressed) {
//     // Simulação: no futuro você pode mandar para um servidor via HTTP ou socket
//     printf("[Wi-Fi] Enviando status do botão: %s\n", pressed ? "PRESSIONADO" : "SOLTO");
// }