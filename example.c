#include "pico/stdlib.h"
#include <stdio.h>
#include <button.h>
#define BUTTON_PIN 21

void onchange(button_t *button_p) {
  button_t *button = (button_t*)button_p;
  printf("Button on pin %d changed its state to %d\n", button->pin, button->state);
}

int main() {
  stdio_init_all();
  button_t *my_button = create_button(BUTTON_PIN, onchange);
  printf("Button created. Its state is %d\n", my_button->state);
  while (true) tight_loop_contents();
  return 0;
}
