#ifndef HKDT_PICO_CAN_H_
#define HKDT_PICO_CAN_H_

#include "can2040/src/can2040.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

// 以下は使用者が変更する設定
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// メッセージキューの大きさ
#define QUEUE_SIZE 128 // Must be power of 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////// グローバル変数です /////////////////
/// @brief CAN通信で受信したデータを保存していくQueueのグローバル変数
static struct {
    uint32_t pull_pos;
    volatile uint32_t push_pos;
    struct can2040_msg queue[QUEUE_SIZE];
} MessageQueue;

// CAN通信を行うグローバル変数
static struct can2040 cbus;
///////////////// グローバル変数です /////////////////

// CANを受信したときのコールバック関数
static void
can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    if (notify == CAN2040_NOTIFY_RX) {
        // Example message filter
        uint32_t id = msg->id;
        
        uint32_t push_pos = MessageQueue.push_pos;
        uint32_t pull_pos = MessageQueue.pull_pos;
        if (push_pos + 1 == pull_pos)
            // No space in queue
            return;
        MessageQueue.queue[push_pos % QUEUE_SIZE] = *msg;
        MessageQueue.push_pos = push_pos + 1;
    }
}

/// @brief CAN通信における割り込み処理側にCANバス構造体を渡す。特に変えない
static void
PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

/// @brief CAN通信周りを初期化する。PIO0を使用する 
/// @param tx_pin CANトランシーバーのTXピンに接続するピン番号
/// @param rx_pin CANトランシーバーのRXピンに接続するピン番号
/// @param bit_rate CAN通信の速度[bps]を決める。例えば1Mbpsで通信したいなら、ここに1000000
void canbus_setup(uint32_t tx_pin, uint32_t rx_pin, uint32_t bit_rate);

/// @brief CAN通信によりデータを送信する
/// @param msg 送信するメッセージ
/// @return 可否
bool can_transmit(struct can2040_msg msg);

/// @brief CAN通信により受信したメッセージがあればそれを引き出す
/// @param msg 受信したメッセージを格納する構造体のポインタ
/// @return メッセージがあればtrue、なければfalseを返す
bool can_receive(struct can2040_msg* msg);

#endif