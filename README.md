# RP2040 Button
## Button debounce library for Raspberry Pi Pico

A C library to define and debounce push buttons on a Raspberry Pi Pico.
It generates interrupts after listening to GPIO_IRQ events.
It allows to define multiple buttons simultaneously.

Fork of [jkroso/pico-button.c](https://github.com/jkroso/pico-button.c) including [jkroso/pico-gpio-interrupt.c](https://github.com/jkroso/pico-gpio-interrupt.c), both by Jake Rosoman. MIT license.


### Projects using this library
- [Dodepan](https://github.com/TuriSc/Dodepan)
- [Jukephone](https://github.com/TuriSc/Jukephone)