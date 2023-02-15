#include "pico/stdlib.h"
#include <stdio.h>
#include <button.h>
#define PLAY_BUTTON 16  // Button on GPIO 16
#define PAUSE_BUTTON 17  // Button on GPIO 17

void onchange(button_t *button_p) {
  button_t *button = (button_t*)button_p;
  printf("Button on pin %d changed its state to %d\n", button->pin, button->state);

  if(button->state) return; // Ignore button release. Invert the logic if using
                            // a pullup (internal or external).

  switch(button->pin){
    case PLAY_BUTTON:
        printf("Play\n");
    break;
    case PAUSE_BUTTON:
        printf("Pause\n");
    break;
  }
}

int main() {
  stdio_init_all();
  button_t *play_button = create_button(PLAY_BUTTON, onchange);
  button_t *pause_button = create_button(PAUSE_BUTTON, onchange);

  while (true) tight_loop_contents();
  return 0;
}
