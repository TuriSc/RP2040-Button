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
static closure_t handlers[28] = {NULL};

/**
 * @var alarm_ids
 * @brief An array of alarm IDs for button debouncing
 */
static alarm_id_t alarm_ids[28];

/**
 * @var gpio_irq_callback_registered
 * @brief Flag to track if GPIO IRQ callback has been registered
 */
static bool gpio_irq_callback_registered = false;

/**
 * @var event_queue
 * @brief Event queue for button events
 */
static button_event_t event_queue[MAX_BUTTON_EVENTS];

/**
 * @var queue_head
 * @brief Head pointer for event queue
 */
static volatile uint8_t queue_head = 0;

/**
 * @var queue_tail
 * @brief Tail pointer for event queue
 */
static volatile uint8_t queue_tail = 0;

/**
 * @brief Queue a button event
 * @param b The button structure
 * @param state The new button state
 */
static void queue_button_event(button_t *b, bool state) {
  if (!b) return;
  
  uint8_t next_head = (queue_head + 1) % MAX_BUTTON_EVENTS;
  if (next_head != queue_tail) {
    event_queue[queue_head].button = b;
    event_queue[queue_head].state = state;
    queue_head = next_head;
  }
}

/**
 * @brief Handles a button alarm
 * @param a The alarm ID (not used)
 * @param p The button structure
 * @return 0
 */
long long int handle_button_alarm(long int a, void *p) {
  button_t *b = (button_t *)(p);
  if (!b) return 0;
  
  bool state = gpio_get(b->pin);
  if (state != b->state) {
    b->state = state;
    queue_button_event(b, state);
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
 * @brief Shared GPIO IRQ handler for all buttons
 * @param gpio The GPIO pin number
 * @param events The interrupt events
 */
static void shared_gpio_irq_handler(uint gpio, uint32_t events) {
  if (gpio < 28 && handlers[gpio].fn != NULL) {
    handlers[gpio].fn(handlers[gpio].argument);
  }
}

/**
 * @brief Handles a GPIO interrupt (legacy, kept for compatibility)
 * @param gpio The GPIO pin number
 * @param events The interrupt events
 */
void handle_interrupt(uint gpio, uint32_t events) {
  shared_gpio_irq_handler(gpio, events);
}

/**
 * @brief Listens for a GPIO interrupt
 * @param pin The GPIO pin number
 * @param condition The interrupt condition
 * @param fn The callback function
 * @param arg The argument to be passed to the callback function
 */
void listen(uint pin, int condition, handler fn, void *arg) {
  if (pin >= 28 || !fn) return;
  
  handlers[pin].argument = arg;
  handlers[pin].fn = fn;
  
  if (!gpio_irq_callback_registered) {
    gpio_set_irq_enabled_with_callback(pin, condition, true, shared_gpio_irq_handler);
    gpio_irq_callback_registered = true;
  } else {
    gpio_set_irq_enabled(pin, condition, true);
  }
}

/**
 * @brief Initialize the button system (call before creating buttons)
 */
void button_system_init(void) {
  queue_head = 0;
  queue_tail = 0;
  gpio_irq_callback_registered = false;
  for (int i = 0; i < 28; i++) {
    handlers[i].fn = NULL;
    handlers[i].argument = NULL;
    alarm_ids[i] = 0;
  }
}

/**
 * @brief Poll for button events and process callbacks (call from main loop)
 * @return Number of events processed
 */
int button_poll_events(void) {
  int count = 0;
  while (queue_tail != queue_head) {
    button_event_t *event = &event_queue[queue_tail];
    if (event->button && event->button->onchange) {
      event->button->onchange(event->button);
    }
    queue_tail = (queue_tail + 1) % MAX_BUTTON_EVENTS;
    count++;
  }
  return count;
}

/**
 * @brief Destroy a button and free its resources
 * @param button Pointer to the button to destroy
 */
void button_destroy(button_t *button) {
  if (!button) return;
  
  if (alarm_ids[button->pin]) {
    cancel_alarm(alarm_ids[button->pin]);
    alarm_ids[button->pin] = 0;
  }
  
  gpio_set_irq_enabled(button->pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
  handlers[button->pin].fn = NULL;
  handlers[button->pin].argument = NULL;
  
  free(button);
}

/**
 * @brief Creates a new button structure
 * @param pin The GPIO pin number
 * @param onchange The onchange callback function
 * @return The new button structure, or NULL on failure
 */
button_t * create_button(int pin, void (*onchange)(button_t *)) {
  if (pin >= 28 || !onchange) return NULL;
  
  gpio_init(pin);
  gpio_pull_up(pin);
  button_t *b = (button_t *)(malloc(sizeof(button_t)));
  if (!b) return NULL;
  
  listen(pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, handle_button_interrupt, b);
  b->pin = pin;
  b->onchange = onchange;
  b->state = gpio_get(pin);
  return b;
}
