#include "ws2812.h"

void init_ws2812_pio(WS2812* ws2812, int pio_num, uint ws2812_pin)
{
    if(pio_num == 0)
    {
        ws2812->pio = pio0;
    }
    else if(pio_num == 1)
    {
        ws2812->pio = pio1;
    }
    else
    {
        ws2812->pio = pio0;   
    }

    ws2812->sm = 0;
    ws2812->offset = pio_add_program(ws2812->pio, &ws2812_program);

    ws2812_program_init(
        ws2812->pio, 
        ws2812->sm, 
        ws2812->offset, 
        ws2812_pin, 
        800000, 
        false
    );
}

void control_ws2812(WS2812* ws2812, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t rgb_code = 
        ((uint32_t) (r) << 8) |
        ((uint32_t) (g) << 16) |
        ((uint32_t) (0) << 24) |
        (uint32_t) (b);

    pio_sm_put_blocking(
        ws2812->pio,
        ws2812->sm,
        rgb_code << 8u
    );
}