#include "pwm.h"

PWM init_pwm(uint gpio, uint frequency)
{
    PWM pwm;
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    pwm.slice_num = pwm_gpio_to_slice_num(gpio);
    pwm.channel = pwm_gpio_to_channel(gpio);
    
    // 分周比は125で固定にしとく。だからカウントの総数は1MHzである
    pwm_set_clkdiv(pwm.slice_num, 125.0f);

    int wrap = 1000000 / frequency;

    pwm.max_duty = wrap-1;
    pwm_set_wrap(pwm.slice_num, wrap-1);

    pwm_set_chan_level(pwm.slice_num, pwm.channel, 0);

    pwm_set_enabled(pwm.slice_num, true);

    return pwm;
}

void control_pwm(PWM* pwm, uint duty)
{
    if(duty > pwm->max_duty)
    {
        pwm_set_chan_level(
            pwm->slice_num, 
            pwm->channel,
            pwm->max_duty
        );
    }
    else
    {
        pwm_set_chan_level(
            pwm->slice_num, 
            pwm->channel,
            duty
        );
    }
}