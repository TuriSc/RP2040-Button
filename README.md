# RP2040 Button
### Button debounce library for Raspberry Pi Pico

A C library to define and debounce push buttons on a Raspberry Pi Pico.
It generates interrupts after listening to GPIO_IRQ events.
It allows to define multiple buttons simultaneously.

Fork of [jkroso/pico-button.c](https://github.com/jkroso/pico-button.c) including [jkroso/pico-gpio-interrupt.c](https://github.com/jkroso/pico-gpio-interrupt.c), both by Jake Rosoman. MIT license.

## Usage Example
```c
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
```
