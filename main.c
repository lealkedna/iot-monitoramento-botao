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

    if (!connect_wifi("", "")) {
        printf("Falha ao conectar no Wi-Fi\n");
        return -1;
    }

    xTaskCreate(button_task, "ButtonTask", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {
        tight_loop_contents();
    }
}
