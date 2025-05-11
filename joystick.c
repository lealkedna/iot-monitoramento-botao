#include "joystick.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h>

#define JOYSTICK_X_PIN 26  // Pino do eixo X (ADC0)
#define JOYSTICK_Y_PIN 27  // Pino do eixo Y (ADC1)
#define JOYSTICK_BUTTON_PIN 22  // Pino do botão

// Função para inicializar o joystick
void joystick_init() {
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
}

// Função para ler o valor do eixo X
int read_joystick_x() {
    adc_select_input(0);  // Seleciona o canal ADC0 (Eixo X)
    return adc_read();    // Lê o valor do eixo X
}

// Função para ler o valor do eixo Y
int read_joystick_y() {
    adc_select_input(1);  // Seleciona o canal ADC1 (Eixo Y)
    return adc_read();    // Lê o valor do eixo Y
}

// Função para ler o estado do botão do joystick
bool read_joystick_button() {
    return gpio_get(JOYSTICK_BUTTON_PIN) == 0;  // Retorna se o botão está pressionado (0 = pressionado)
}

// Função para enviar os dados do joystick (simulação por enquanto)
void send_joystick_data(int x, int y, bool button_pressed) {
    printf("[Joystick] X: %d, Y: %d, Button: %s\n", x, y, button_pressed ? "PRESSED" : "RELEASED");
    // Simulação dos dados enviados, aqui você pode integrar com o servidor
}
