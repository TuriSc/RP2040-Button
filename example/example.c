/**
 * @file example/example.c
 * @brief Example usage of the button debounce library
 *
 * This example demonstrates both callback modes:
 * - PLAY_BUTTON uses immediate mode (no polling required)
 * - PAUSE_BUTTON uses queued mode (requires button_poll_events())
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "button.h"

/**
 * @def PLAY_BUTTON
 * @brief The GPIO pin number of the play button (immediate mode)
 */
#define PLAY_BUTTON 16

/**
 * @def PAUSE_BUTTON
 * @brief The GPIO pin number of the pause button (queued mode)
 */
#define PAUSE_BUTTON 17

/**
 * @brief Simple callback for immediate mode button
 * @param button The button structure
 * @note This runs in interrupt context - keep it fast and simple!
 */
void play_callback(button_t *button) {
  // Simple, fast operation safe for interrupt context
  if(!button->state) { // Button pressed (using pullup)
    printf("Play\n");
  }
}

/**
 * @brief Complex callback for queued mode button
 * @param button The button structure
 * @note This runs in main loop context - safe to use delays, I/O, etc.
 */
void pause_callback(button_t *button) {
  // Can safely use blocking operations in queued mode
  if(!button->state) { // Button pressed (using pullup)
    printf("Pause\n");
    sleep_ms(10); // This is safe in queued mode
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

  // Create button with immediate callback (no polling needed)
  button_t *play_button = create_button(PLAY_BUTTON, play_callback);

  // Create button with queued callback (requires polling)
  button_t *pause_button = create_button_queued(PAUSE_BUTTON, pause_callback);

  while (true) {
    // Process queued button events (only needed for create_button_queued)
    button_poll_events();

    // If no polling is needed, just spin a busy‑wait loop:
    // tight_loop_contents();
  }
  return 0;
}
