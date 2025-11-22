/**
 * @file button.h
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

#ifndef PICO_BUTTON_H
#define PICO_BUTTON_H

#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def DEBOUNCE_US
 * @brief Debounce time in microseconds
 */
#define DEBOUNCE_US 200

/**
 * @def MAX_BUTTON_EVENTS
 * @brief Maximum number of button events that can be queued
 */
#define MAX_BUTTON_EVENTS 16

/**
 * @struct button_t
 * @brief Represents a button with its pin, state, and onchange callback
 */
typedef struct button_t {
  /**
   * @var pin
   * @brief The GPIO pin number of the button
   */
  uint8_t pin;
  /**
   * @var state
   * @brief The current state of the button (true for high, false for low)
   */
  bool state;
  /**
   * @var onchange
   * @brief The callback function to be called when the button state changes
   */
  void (*onchange)(struct button_t *button);
} button_t;

/**
 * @typedef handler
 * @brief A function pointer type for onchange callbacks
 */
typedef void (*handler)(void *argument);

/**
 * @struct closure_t
 * @brief Represents a closure with a function pointer and an argument
 */
typedef struct {
  /**
   * @var argument
   * @brief The argument to be passed to the function
   */
  void * argument;
  /**
   * @var fn
   * @brief The function pointer
   */
  handler fn;
} closure_t;

/**
 * @struct button_event_t
 * @brief Represents a button event in the queue
 */
typedef struct {
  /**
   * @var button
   * @brief Pointer to the button that generated the event
   */
  button_t *button;
  /**
   * @var state
   * @brief The new state of the button
   */
  bool state;
} button_event_t;

/**
 * @brief Handles a button alarm
 * @param a The alarm ID (not used)
 * @param p The button structure
 * @return 0
 */
long long int handle_button_alarm(long int a, void *p);

/**
 * @brief Handles a button interrupt
 * @param p The button structure
 */
void handle_button_interrupt(void *p);

/**
 * @brief Handles a GPIO interrupt
 * @param gpio The GPIO pin number
 * @param events The interrupt events
 */
void handle_interrupt(uint gpio, uint32_t events);

/**
 * @brief Listens for a GPIO interrupt
 * @param pin The GPIO pin number
 * @param condition The interrupt condition
 * @param fn The callback function
 * @param arg The argument to be passed to the callback function
 */
void listen(uint pin, int condition, handler fn, void *arg);

/**
 * @brief Initialize the button system (call before creating buttons)
 */
void button_system_init(void);

/**
 * @brief Creates a new button structure
 * @param pin The GPIO pin number
 * @param onchange The onchange callback function
 * @return The new button structure, or NULL on failure
 */
button_t * create_button(int pin, void (*onchange)(button_t *));

/**
 * @brief Poll for button events and process callbacks (call from main loop)
 * @return Number of events processed
 */
int button_poll_events(void);

/**
 * @brief Destroy a button and free its resources
 * @param button Pointer to the button to destroy
 */
void button_destroy(button_t *button);

#ifdef __cplusplus
}
#endif

#endif
