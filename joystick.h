#ifndef JOYSTICK_H
#define JOYSTICK_H

// Funções para inicializar e ler os dados do joystick
void joystick_init();
int read_joystick_x();
int read_joystick_y();
bool read_joystick_button();
void send_joystick_data(int x, int y, bool button_pressed);

#endif // JOYSTICK_H
