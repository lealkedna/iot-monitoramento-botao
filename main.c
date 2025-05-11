#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "wifi.h"
#include "button.h"
#include "joystick.h"  // Incluir o arquivo do joystick

// Tarefa do botão
void button_task(void *pvParameters) {
    button_init();

    while (1) {
        bool pressed = button_is_pressed();
        printf("[Button Task] Botão está %s\n", pressed ? "PRESSIONADO" : "SOLTO");

        send_button_status(pressed);
        update_button_state(pressed);  

        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera de 1 segundo
    }
}

// Tarefa do joystick
void joystick_task(void *pvParameters) {
    while (1) {
        int x = read_joystick_x(); // Função para ler o valor do eixo X
        int y = read_joystick_y(); // Função para ler o valor do eixo Y
        bool button_pressed = read_joystick_button(); // Função para ler o botão

        printf("[Joystick] X: %d, Y: %d, Button: %s\n", x, y, button_pressed ? "PRESSED" : "RELEASED");

        send_joystick_data(x, y, button_pressed);  // Envia os dados do joystick para o servidor
        vTaskDelay(pdMS_TO_TICKS(100));  // Espera de 100ms
    }
}

int main() {
    stdio_init_all();
    sleep_ms(3000);  // Atraso para inicializar
    printf(">> Programa iniciado com sucesso!\n");

    // Conectar ao Wi-Fi
    if (!connect_wifi("", "")) {
        printf("Falha ao conectar no Wi-Fi\n");
        return -1;
    }

    // Criar tarefas do FreeRTOS
    xTaskCreate(button_task, "Button Task", 256, NULL, 1, NULL);
    xTaskCreate(joystick_task, "Joystick Task", 256, NULL, 1, NULL);

    // Iniciar o FreeRTOS Scheduler
    vTaskStartScheduler();

    while (1) {
        tight_loop_contents();  // Loop principal
    }
}
