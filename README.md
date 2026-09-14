# hkdt_pico

# 使用方法
Pico-SDKプロジェクトにこのリポジトリを以下のコマンドでクローンしてください

```
git clone --recursive https://github.com/motii8128/hkdt_pico.git
```

そしてCMakeLists.txtに以下のように記述してください。
```cmake
add_subdirectory(hkdt_pico)

target_link_libraries(your_project
    # 別の依存関係
    hkdt_pico
)
```

# 使用例
## ロボマス
```c
#include "pico/stdlib.h"
#include "stdio.h"

#include "can.h"
#include "robomaster.h"

int main(void)
{
    stdio_init_all();

    /// CANトランシーバーのTXピンをGPIO7
    /// CANトランシーバーのTXピンをGPIO6に接続した
    /// CAN通信の速度を1000000bps = 1Mbpsにした
    canbus_setup(7, 6, 1000000);

    RoboMasterSensor sensor = hp_sensor_init();

    struct can2040_msg msg = {
        .id = 0x200,
        .dlc = 8,
        .data = {0, 0, 0, 0, 0, 0, 0, 0}
    };

    absolute_time_t last_time = get_absolute_time();

    for(;;)
    {
        absolute_time_t now = get_absolute_time();
        // 前回実行時からの経過時間（マイクロ秒）を計算
        int64_t diff_us = absolute_time_diff_us(last_time, now);

        float diff_s = diff_us * 1e-6f;

        int16_t current = 1000;
        msg.data[0] = (current >> 8) & 0xFF;
        msg.data[1] = current & 0xFF;

        //　20msごとにCANを送信
        if(diff_us >= 20000)
        {
            if(can_transmit(msg))
            {
                // 送信成功した場合
            }
            else
            {
                // 送信に失敗した場合
                printf("Failed to send data.\n");
            }

            last_time = delayed_by_us(last_time, 20000);
        }
        

        struct can2040_msg recv_msg;
        
        if(can_receive(&recv_msg))
        {
            // 受信データがあった場合
            hp_parse_CANMessage(recv_msg.data, &sensor);
            printf("ID=%d, vel=%d, pos=%lf\n", recv_msg.id, sensor.velocity, sensor.position);  
        } 
        else
        {
            // 受信データがなかった場合
        }
    }

    return 0;
}
```

## W6300によるUDP通信
```c
#include "pico/stdlib.h"
#include "stdio.h"

#include "w6300_ethernet.h"

int main(void)
{
    stdio_init_all();

    int socket_num = 0;
    uint8_t my_ip_addr = {192, 168, 11, 2};
    uint8_t gate_way = {192, 168, 11, 1};
    uint16_t my_port = 64201;

    while(initialize_w6300_ethernet(socket_num, my_ip_addr, gate_way, my_port) != socket_num)
    {
        printf("initalize w6300 ethernet ...\n");
        sleep_ms(1000);
    }

    uint8_t dest_ip_addr = {192, 168, 11, 4};
    uint16_t dest_port = 64201;

    for(;;)
    {
        uint8_t recv_buffer[128];
        if(recv_w6300_udp(socket_num, recv_buffer, 128) > 0)
        {
            // 受信成功
        }
        else
        {
            // 受信失敗
        }

        if(send_w6300_udp(socket_num, recv_buffer, 128, dest_ip_addr, dest_port) > 0)
        {
            //　送信成功
        }
        else
        {
            // 送信失敗
        }
    }

    return 0;
}
```