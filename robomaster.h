#ifndef HKDT_PICO_ROBOMASTER_H_
#define HKDT_PICO_ROBOMASTER_H_

#include "pico.h"
#include "pico/stdio.h"

typedef struct RoboMasterSensor
{
    short velocity;
    float position;
    short torque;
    short temperature;
    short angle;
}RoboMasterSensor;

RoboMasterSensor hp_sensor_init()
{
    RoboMasterSensor sensor;

    sensor.velocity = 0;
    sensor.position = 0.0;
    sensor.torque = 0;
    sensor.temperature = 0;
    sensor.angle = -1;

    return sensor;
}

void hp_parse_CANMessage(unsigned char* data, RoboMasterSensor* sensor)
{
    short angle_data = data[0] << 8 | data[1];
	sensor->velocity = data[2] << 8 | data[3];
	sensor->torque = data[4] << 8 | data[5];
	sensor->temperature = data[6];

    if(sensor->angle == -1)
    {
        sensor->angle = angle_data;
        sensor->position = 0.0;

        return;
    }

    short angle_diff = angle_data - sensor->angle;
    sensor->angle = angle_data;
    if(angle_diff > 4096)
    {
        angle_diff -= 8192;
    }
    else if(angle_diff < -4096)
    {
        angle_diff += 8192;
    }


    sensor->position += (float)angle_diff / 8192.0f;

    return;
}

#endif