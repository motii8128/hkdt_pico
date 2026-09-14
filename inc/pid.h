#ifndef HKDT_PICO_PID_H_
#define HKDT_PICO_PID_H_

#include "math.h"

/// @brief PID制御の設定を保存する構造体
typedef struct PidConfig
{
    float p_gain;
    float i_gain;
    float d_gain;
    float i_max;
    float output_max;

    float integral;
    float prev_mesuarment;
}PidConfig;

/// @brief カスケード制御の設定を保存する構造体。PidConfigを２つ格納しているだけ。
typedef struct CascadeConfig
{
    PidConfig position_config;
    PidConfig velocity_config;
}CascadeConfig;

/// @brief PID制御の設定を初期化する
/// @param p_gain Pゲイン
/// @param i_gain Iゲイン
/// @param d_gain Dゲイン
/// @param i_max  積分値の最大値
/// @param output_max 出力の絶対値の最大値
/// @return 
PidConfig hp_init_pid_config(float p_gain, float i_gain, float d_gain, float i_max, float output_max);

/// @brief 微分先行型のPIDを計算する
/// @param config コンフィグ
/// @param target 目標値
/// @param now 現在値
/// @param dt 制御周期
/// @return 
float hp_compute_pi_d(PidConfig* config, float target, float now, float dt);

/// @brief PIDを計算する
/// @param config コンフィグ
/// @param target 目標値
/// @param now 現在値
/// @param dt 制御周期
/// @return 
float hp_compute_pid(PidConfig* config, float target, float now, float dt);

/// @brief カスケード制御を行う
/// @param config コンフィグ
/// @param target_position 目標位置
/// @param now_position 現在位置
/// @param now_velocity 現在速度
/// @param dt 制御周期
/// @return 
float hp_compute_cascade(CascadeConfig* config, float target_position, float now_position, float now_velocity, float dt);

#endif