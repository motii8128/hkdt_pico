#ifndef HKDT_PICO_ROBOMASTER_H_
#define HKDT_PICO_ROBOMASTER_H_

#include "pico.h"
#include "pico/stdio.h"

/// @brief ロボマスモーター１つのセンサーデータを管理する構造体
typedef struct RoboMasterSensor
{
    short velocity;
    float position;
    short torque;
    short temperature;
    short angle;
}RoboMasterSensor;

/// @brief RoboMasterSensorを初期化する
/// @return 
RoboMasterSensor hp_sensor_init();


/// @brief CAN通信で受信したデータを構造体に格納する
/// @param data CAN通信で受信したデータ(バイト列)
/// @param sensor 格納する構造体のポインタ
void hp_parse_CANMessage(unsigned char* data, RoboMasterSensor* sensor);

#endif