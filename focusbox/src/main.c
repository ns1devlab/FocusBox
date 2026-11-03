#include "gd32vf103.h"
#include <stdint.h>

/* Accelerated values for bench testing. Use 25/5 minutes for normal use. */
#define FOCUS_MS 10000ULL
#define BREAK_MS  5000ULL
#define BEEP_MS   1000ULL

#define GREEN_LED GPIO_PIN_0  /* PB0 */
#define BLUE_LED  GPIO_PIN_1  /* PB1 */
#define BUZZER    GPIO_PIN_10 /* PB10, transistor base through 1 kOhm */
#define BUTTON    GPIO_PIN_5  /* PA5, button to GND */

/* Time conversion used by the working course SDK setup. */
static uint64_t time_ms(void)
{
    return get_timer_value() / (SystemCoreClock / 4000U);
}

int main(void)
{
    int focus = 1;
    int running = 0;
    int beeping = 0;

    uint64_t remaining = FOCUS_MS;
    uint64_t beep_started = 0;
    uint64_t previous;
    uint64_t changed_at;

    int reading;
    int last_reading;
    int stable_button;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_bit_reset(GPIOB, GREEN_LED | BLUE_LED | BUZZER);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ,
              GREEN_LED | BLUE_LED | BUZZER);
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, BUTTON);

    previous = time_ms();
    changed_at = previous;
    last_reading = gpio_input_bit_get(GPIOA, BUTTON);
    stable_button = last_reading;

    while (1) {
        uint64_t now = time_ms();
        uint64_t elapsed = now - previous;
        previous = now;

        if (running) {
            if (elapsed >= remaining) {
                focus = !focus;
                remaining = focus ? FOCUS_MS : BREAK_MS;
                beep_started = now;
                beeping = 1;
            } else {
                remaining -= elapsed;
            }
        }

        /* Debounce the button: accept a changed level after 40 ms. */
        reading = gpio_input_bit_get(GPIOA, BUTTON);
        if (reading != last_reading) {
            last_reading = reading;
            changed_at = now;
        }
        if (now - changed_at >= 40 && reading != stable_button) {
            stable_button = reading;
            if (stable_button == RESET) {
                running = !running;
            }
        }

        if (!running || now - beep_started >= BEEP_MS) {
            beeping = 0;
        }
        if (beeping) {
            gpio_bit_set(GPIOB, BUZZER);
        } else {
            gpio_bit_reset(GPIOB, BUZZER);
        }

        if (running && focus) {
            gpio_bit_reset(GPIOB, BLUE_LED);
            gpio_bit_set(GPIOB, GREEN_LED);
        } else if (running) {
            gpio_bit_reset(GPIOB, GREEN_LED);
            gpio_bit_set(GPIOB, BLUE_LED);
        } else {
            gpio_bit_reset(GPIOB, GREEN_LED | BLUE_LED);
        }
    }
}
