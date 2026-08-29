#ifndef HKDT_PICO_PID_H_
#define HKDT_PICO_PID_H_

#include "math.h"

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

typedef struct CascadeConfig
{
    PidConfig position_config;
    PidConfig velocity_config;
}CascadeConfig;

PidConfig hp_init_pid_config(float p_gain, float i_gain, float d_gain, float i_max, float output_max)
{
    PidConfig conf;
    conf.p_gain = p_gain;
    conf.i_gain = i_gain;
    conf.d_gain = d_gain;
    conf.i_max = i_max;
    conf.output_max = output_max;

    conf.integral = 0.0;
    conf.prev_mesuarment = 0.0;

    return conf;
}

/// @brief 微分先行型のPIDを計算する
/// @param config コンフィグ
/// @param target 目標値
/// @param now 現在値
/// @param dt 制御周期
/// @return 
float hp_compute_pi_d(PidConfig* config, float target, float now, float dt)
{
    float error = target - now;

    config->integral += error * dt;
    if(fabs(config->integral) > config->i_max)
    {
        if(config->integral > 0.0)
        {
            config->integral = config->i_max;
        }
        else
        {
            config->integral = -1.0 * config->i_max;
        }
    }

    float derivative = (now - config->prev_mesuarment) / dt;


    float output = config->p_gain * error + config->i_gain * config->integral + config->d_gain * derivative;

    if(fabs(output) > config->output_max)
    {
        if(output > 0.0)
        {
            output = config->output_max;
        }else
        {
            output = -1.0*config->output_max;
        }
    }

    config->prev_mesuarment = now;

    return output;
}

/// @brief PIDを計算する
/// @param config コンフィグ
/// @param target 目標値
/// @param now 現在値
/// @param dt 制御周期
/// @return 
float hp_compute_pid(PidConfig* config, float target, float now, float dt)
{
    float error = target - now;

    config->integral += error * dt;
    if(fabs(config->integral) > config->i_max)
    {
        if(config->integral > 0.0)
        {
            config->integral = config->i_max;
        }
        else
        {
            config->integral = -1.0 * config->i_max;
        }
    }

    float derivative = (error - config->prev_mesuarment) / dt;


    float output = config->p_gain * error + config->i_gain * config->integral + config->d_gain * derivative;

    if(fabs(output) > config->output_max)
    {
        if(output > 0.0)
        {
            output = config->output_max;
        }else
        {
            output = -1.0*config->output_max;
        }
    }

    config->prev_mesuarment = error;

    return output;
}

float hp_compute_cascade(CascadeConfig* config, float target_position, float now_position, float now_velocity, float dt)
{
    float target_vel = hp_compute_pid(&config->position_config, target_position, now_position, dt);

    return hp_compute_pi_d(&config->velocity_config, target_vel, now_velocity, dt);
}

#endif