#include "w6300_ethernet.h"

int initialize_w6300_ethernet(int socket_num, uint8_t my_ip_addr[4], uint8_t my_gateway[4], uint16_t my_port)
{
    wiz_NetInfo w6300_info = {
        .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x56}, // MAC address
        .ip = my_ip_addr,                     // IP address
        .sn = {255, 255, 255, 0},                    // Subnet Mask
        .gw = my_gateway,                     // Gateway
        .lla = {
            0xfe, 0x80, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x02, 0x08, 0xdc, 0xff,
            0xfe, 0x57, 0x57, 0x25
        },             // Link Local Address
        .gua = {
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        },             // Global Unicast Address
        .sn6 = {
            0xff, 0xff, 0xff, 0xff,
            0xff, 0xff, 0xff, 0xff,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        },             // IPv6 Prefix
        .gw6 = {
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        },             // Gateway IPv6 Address
        .dns = {8, 8, 8, 8},                         // DNS server
        .dns6 = {
            0x20, 0x01, 0x48, 0x60,
            0x48, 0x60, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x88, 0x88
        },             // DNS6 server
        .ipmode = NETINFO_STATIC_ALL,
    };

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
    return socket(socket_num, Sn_MR_UDP, my_port, 0);
}

int recv_w6300_udp(int socket_num, uint8_t* buffer, int size)
{
    // 受信データの大きさを確認する
    uint16_t rx_size = 0;
    getsockopt(socket_num, SO_RECVBUF, &rx_size);

    // 受信データが０より大きいなら受信開始
    if(rx_size > 0)
    {
        uint8_t remote_ip[4] = {0}; 
        uint16_t remote_port = 0;  
        uint8_t addr_len = 0;  

        // このretに実際に受信データの数が入る
        int32_t ret = recvfrom(socket_num, buffer, size - 1, remote_ip, &remote_port, &addr_len);

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

int send_w6300_udp(int socket_num, uint8_t* buffer, int size, uint8_t dest_ip[4], uint16_t dest_port)
{

    return sendto(socket_num, buffer, size, dest_ip, dest_port, 4);
}