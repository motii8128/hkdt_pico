#include "w6300_ethernet.h"

int initialize_w6300_ethernet()
{
    // wizchip_conf.hにおいて
    // #define _WIZCHIP_    W6300
    // wizchip_spi.hにおいて
    // #define DEVICE_BOARD_NAME W6300_EVB_PICO2
    // と追記している
    // この２つのフラグによって
    // W6300-EVB-PICO2ではPIOを用いたQSPI通信を初期化するようにマクロで管理されてる
    wizchip_spi_initialize();
    wizchip_cris_initialize();

    wizchip_reset();
    wizchip_initialize();
    wizchip_check();

    // アドレスを指定してネットワークを初期化
    network_initialize(w6300_info);

    // ポートを指定してソケット通信をUDPプロトコルで初期化
    return socket(SOCKET_NUM, Sn_MR_UDP, SRC_PORT, 0);
}

int recv_w6300_udp(uint8_t* buffer, int size)
{
    // 受信データの大きさを確認する
    uint16_t rx_size = 0;
    getsockopt(SOCKET_NUM, SO_RECVBUF, &rx_size);

    // 受信データが０より大きいなら受信開始
    if(rx_size > 0)
    {
        uint8_t remote_ip[4] = {0}; 
        uint16_t remote_port = 0;  
        uint8_t addr_len = 0;  

        // このretに実際に受信データの数が入る
        int32_t ret = recvfrom(SOCKET_NUM, buffer, size - 1, remote_ip, &remote_port, &addr_len);

        if(ret > 0)
        {
            return ret;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

int send_w6300_udp(uint8_t* buffer, int size)
{
    uint8_t remote_ip[4] = DEST_ADDR;
    uint16_t remote_port = DEST_PORT; 

    return sendto(SOCKET_NUM, buffer, size, remote_ip, remote_port, 4);
}