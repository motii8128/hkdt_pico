#ifndef HKDT_PICO_WS2812_H_
#define HKDT_PICO_WS2812_H_

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

typedef struct WS2812
{
    PIO pio;
    uint sm;
    uint offset;
}WS2812;


/// @brief WS2812を使用するためにPIOを初期化する
/// @param ws2812 WS2812構造体のポインタ
/// @param pio_num PIOの番号を指定する（0か１）
/// @param ws2812_pin WS2812のDinピンに接続されたピン番号
void init_ws2812_pio(WS2812* ws2812, int pio_num, uint ws2812_pin);

/// @brief WS2812を制御する。１回目の実行では１つ目のLED、２回目の実行は２つ目のLEDというように制御する
/// @param ws2812 WS2812構造体のポインタ
/// @param r 赤の強さ(0~255)
/// @param g 緑の強さ(0~255)
/// @param b 青の強さ(0~255)
void control_ws2812(WS2812* ws2812, uint8_t r, uint8_t g, uint8_t b);


#endif