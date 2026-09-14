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
            if (can2040_check_transmit(&cbus) > 0) {
                int status = can2040_transmit(&cbus, &msg);

            } else {
                // 送信バッファが詰まっている場合のデバッグ出力
                printf("CAN Tx buffer full or error\n");
            }

            last_time = delayed_by_us(last_time, 20000);
        }
        

        uint32_t push_pos = MessageQueue.push_pos;
        uint32_t pull_pos = MessageQueue.pull_pos;
        if (push_pos == pull_pos)
            // No new messages read.
            continue;

        // Pop message from local receive queue
        struct can2040_msg *qmsg = &MessageQueue.queue[pull_pos % QUEUE_SIZE];
        struct can2040_msg msg = *qmsg;
        MessageQueue.pull_pos++;    

        // CAN受信したデータをロボマスエンコーダに変換する
        hp_parse_CANMessage(msg.data, &sensor);

        printf("vel=%d, pos=%lf\n", sensor.velocity, sensor.position);   
    }

    return 0;
}
```