#ifndef HKDT_PICO_PWM_H_
#define HKDT_PICO_PWM_H_

#include "pico/stdlib.h"
#include "hardware/pwm.h"

/// @brief PWM出力を行うための構造体
typedef struct PWM
{
    uint slice_num;
    uint channel;
    uint max_duty;
}PWM;

/// @brief 指定されたピンをPWMとして扱う
/// @param gpio PWM出力したいピン
/// @param frequency PWM周波数を指定する
/// @return PWM構造体
PWM init_pwm(uint gpio, uint frequency);

/// @brief PWMのデューティ比を設定する
/// @param pwm PWM構造体のポインタを渡す
/// @param duty デューティ比
void control_pwm(PWM* pwm, uint duty);

#endif