/**
 * @file example/example.c
 * @brief Example usage of the button debounce library
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "button.h"

/**
 * @def PLAY_BUTTON
 * @brief The GPIO pin number of the play button
 */
#define PLAY_BUTTON 16

/**
 * @def PAUSE_BUTTON
 * @brief The GPIO pin number of the pause button
 */
#define PAUSE_BUTTON 17

/**
 * @brief Callback function for button state changes
 * @param button_p The button structure
 */
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

/**
 * @brief Main function
 * @return 0
 */
int main() {
  stdio_init_all();
  
  // Initialize button system
  button_system_init();
  
  button_t *play_button = create_button(PLAY_BUTTON, onchange);
  button_t *pause_button = create_button(PAUSE_BUTTON, onchange);

  while (true) {
    // Process button events so that callbacks run in main loop context, not interrupt context
    button_poll_events();
    // Throttle loop to reduce CPU usage, adjust as needed
    sleep_ms(10);
  }
  return 0;
}
