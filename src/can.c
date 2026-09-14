#include "can.h"

void canbus_setup(uint32_t tx_pin, uint32_t rx_pin, uint32_t bit_rate)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = clock_get_hz(clk_sys), bitrate = bit_rate;
    uint32_t gpio_rx = rx_pin, gpio_tx = tx_pin;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}