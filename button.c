/**
 * @file button.c
 * @brief Button debounce library for Raspberry Pi Pico
 *
 * This library provides a simple way to debounce push buttons on a Raspberry Pi Pico.
 * It generates interrupts after listening to GPIO_IRQ events and allows defining multiple buttons simultaneously.
 * Fork of https://github.com/jkroso/pico-button.c
 * including https://github.com/jkroso/pico-gpio-interrupt.c,
 * both by Jake Rosoman. MIT license.
 *
 * @author Turi Scandurra
 * @date 2023-02-13
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "button.h"

/**
 * @var handlers
 * @brief An array of closure structures for GPIO interrupt handlers
 */
closure_t handlers[28] = {NULL};

/**
 * @var alarm_ids
 * @brief An array of alarm IDs for button debouncing
 */
alarm_id_t alarm_ids[28];

/**
 * @brief Handles a button alarm
 * @param a The alarm ID (not used)
 * @param p The button structure
 * @return 0
 */
long long int handle_button_alarm(long int a, void *p) {
  button_t *b = (button_t *)(p);
  bool state = gpio_get(b->pin);
  if (state != b->state) {
    b->state = state;
    b->onchange(b);
  }
  return 0;
}

/**
 * @brief Handles a button interrupt
 * @param p The button structure
 */
void handle_button_interrupt(void *p) {
  button_t *b = (button_t *)(p);
  if (alarm_ids[b->pin]) cancel_alarm(alarm_ids[b->pin]);
  alarm_ids[b->pin] = add_alarm_in_us(DEBOUNCE_US, handle_button_alarm, b, true);
}

/**
 * @brief Handles a GPIO interrupt
 * @param gpio The GPIO pin number
 * @param events The interrupt events
 */
void handle_interrupt(uint gpio, uint32_t events) {
  closure_t handler = handlers[gpio];
  handler.fn(handler.argument);
}

/**
 * @brief Listens for a GPIO interrupt
 * @param pin The GPIO pin number
 * @param condition The interrupt condition
 * @param fn The callback function
 * @param arg The argument to be passed to the callback function
 */
void listen(uint pin, int condition, handler fn, void *arg) {
  gpio_set_irq_enabled_with_callback(pin, condition, true, handle_interrupt);
  closure_t *handler = malloc(sizeof(closure_t));
  handler->argument = arg;
  handler->fn = fn;
  handlers[pin] = *handler;
}

/**
 * @brief Creates a new button structure
 * @param pin The GPIO pin number
 * @param onchange The onchange callback function
 * @return The new button structure
 */
button_t * create_button(int pin, void (*onchange)(button_t *)) {
  gpio_init(pin);
  gpio_pull_up(pin);
  button_t *b = (button_t *)(malloc(sizeof(button_t)));
  listen(pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, handle_button_interrupt, b);
  b->pin = pin;
  b->onchange = onchange;
  b->state = gpio_get(pin);
  return b;
}
