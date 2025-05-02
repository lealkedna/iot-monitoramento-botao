// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "pico/cyw43_arch.h"
// #include "FreeRTOS.h"
// #include "task.h"

// #include "wifi.h"
// #include "button.h"

// void button_task(void *pvParameters) {
//     button_init();
    
//     while (1) {
//         bool pressed = button_is_pressed();
//         printf("[Button Task] Botão está %s\n", pressed ? "PRESSIONADO" : "SOLTO");

//         send_button_status(pressed);

//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// int main() {
//     stdio_init_all();
//     sleep_ms(3000);

//     if (cyw43_arch_init()) {
//         printf("Erro ao inicializar o Wi-Fi\n");
//         return -1;
//     }

//     if (!connect_wifi("SEU_SSID", "SUA_SENHA")) {
//         printf("Falha ao conectar no Wi-Fi\n");
//         return -1;
//     }

//     xTaskCreate(button_task, "ButtonTask", 256, NULL, 1, NULL);

//     vTaskStartScheduler();

//     while (1) { tight_loop_contents(); }
// }

// #include <stdio.h>
// #include "pico/cyw43_arch.h"
// #include "wifi.h"

// // Substitua com os dados reais de IP se quiser
// bool connect_wifi(const char* ssid, const char* password) {
//     printf("[Wi-Fi] Conectando a %s...\n", ssid);
//     if (cyw43_arch_init_with_country(CYW43_COUNTRY_BRAZIL)) {
//         printf("[Wi-Fi] Erro ao inicializar o driver Wi-Fi\n");
//         return false;
//     }

//     if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
//         printf("[Wi-Fi] Falha ao conectar ao Wi-Fi\n");
//         return false;
//     }

//     printf("[Wi-Fi] Conectado com sucesso!\n");
//     return true;
// }

// void send_button_status(bool pressed) {
//     printf("[Wi-Fi] Enviando status do botão: %s\n", pressed ? "PRESSIONADO" : "SOLTO");
//     // Aqui você pode enviar via socket ou HTTP futuramente
// }


#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include "task.h"

#include "wifi.h"
#include "button.h"

void button_task(void *pvParameters) {
    button_init();
    
    while (1) {
        bool pressed = button_is_pressed();
        printf("[Button Task] Botão está %s\n", pressed ? "PRESSIONADO" : "SOLTO");

        send_button_status(pressed);
        update_button_state(pressed);  

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


int main() {
    stdio_init_all();
    sleep_ms(3000);
    printf(">> Programa iniciado com sucesso!\n");

    if (!connect_wifi("Liara2.4GHz", "Liara@ifpi")) {
        printf("Falha ao conectar no Wi-Fi\n");
        return -1;
    }

    xTaskCreate(button_task, "ButtonTask", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {
        tight_loop_contents();
    }
}
