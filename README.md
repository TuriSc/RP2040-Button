# Raspberry Pi Pico Button
## Button debounce library for Raspberry Pi Pico (RP2040 and RP2350)

A C library to define and debounce push buttons on a Raspberry Pi Pico.
It generates interrupts after listening to GPIO_IRQ events.
It allows to define multiple buttons simultaneously.

Fork of [jkroso/pico-button.c](https://github.com/jkroso/pico-button.c) including [jkroso/pico-gpio-interrupt.c](https://github.com/jkroso/pico-gpio-interrupt.c), both by Jake Rosoman. MIT license.

## Usage

This library supports two callback execution modes:

### Immediate Mode (Default)

Use `create_button()` for immediate callback execution. Callbacks run directly in the alarm interrupt context. No polling required.

```c
#include "button.h"

void onchange(button_t *button) {
  // Handle button state change
  // Keep this fast and avoid blocking operations
  printf("Button on pin %d is now %s\n", button->pin, button->state ? "HIGH" : "LOW");
}

int main() {
  stdio_init_all();
  button_system_init();

  button_t *my_button = create_button(15, onchange);

  while (true) {
    tight_loop_contents();
  }
  return 0;
}
```

**When to use:** Simple callbacks that are interrupt-safe (no blocking, no delays, no I2C/SPI/etc).

### Queued Mode

Use `create_button_queued()` for safer callback execution in main loop context. Requires calling `button_poll_events()`.

```c
#include "button.h"

void onchange(button_t *button) {
  // Safe to use blocking operations, printf, I2C, SPI, delays, etc.
  printf("Button on pin %d is now %s\n", button->pin, button->state ? "HIGH" : "LOW");
  sleep_ms(10); // This is safe in queued mode
}

int main() {
  stdio_init_all();
  button_system_init();

  button_t *my_button = create_button_queued(15, onchange);

  while (true) {
    button_poll_events(); // Process queued events
    sleep_ms(10);
  }
  return 0;
}
```

**When to use:** Callbacks that perform blocking operations, I/O, or need to call functions that aren't interrupt-safe.

### Mixing Both Modes

You can use both modes simultaneously in the same application:

```c
button_t *simple_button = create_button(14, simple_callback);       // Immediate
button_t *complex_button = create_button_queued(15, complex_callback); // Queued

while (true) {
  button_poll_events(); // Only needed for queued buttons
  // … rest of main loop
}
```

## Projects using this library
- [Dodepan](https://github.com/TuriSc/Dodepan)
- [Jukephone](https://github.com/TuriSc/Jukephone)

## Version History

- 2026-01-04 - Add per-button callback modes (immediate and queued). Restores backwards compatibility
- 2025-11-22 - Add queued events polling to invoke callbacks outside interrupt context
- 2023-02-14 - Initial release
