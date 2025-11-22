# Raspberry Pi Pico Button
## Button debounce library for Raspberry Pi Pico (RP2040 and RP2350)

A C library to define and debounce push buttons on a Raspberry Pi Pico.
It generates interrupts after listening to GPIO_IRQ events.
It allows to define multiple buttons simultaneously.

Fork of [jkroso/pico-button.c](https://github.com/jkroso/pico-button.c) including [jkroso/pico-gpio-interrupt.c](https://github.com/jkroso/pico-gpio-interrupt.c), both by Jake Rosoman. MIT license.

## Usage

See included example.

## Important Notes

- **Event Queue Pattern**: User callbacks are executed in the main loop context, not interrupt context. This prevents crashes from blocking operations or unsafe functions in callbacks.
- **Polling Required**: You **must** call `button_poll_events()` regularly in your main loop to process queued events and invoke callbacks.

## Breaking Changes

**Version Update**: As of this version, you **must** call `button_poll_events()` in your main loop for button callbacks to execute. Previous versions may have appeared to work without polling, but callbacks would not actually be invoked.

If you're upgrading from an older version:
- Add `button_poll_events()` calls in your main loop
- Replace `tight_loop_contents()` with `sleep_ms(10)` when polling events
- See the example above for the correct usage pattern

## Projects using this library
- [Dodepan](https://github.com/TuriSc/Dodepan)
- [Jukephone](https://github.com/TuriSc/Jukephone)

## Version History

- 2025-11-22 - Add quequed events polling to invoke callbacks outside interrupt context
- 2023-02-14 - Initial release
