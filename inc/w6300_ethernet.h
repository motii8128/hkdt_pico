#ifndef HKDT_PICO_W6300_ETHERNET_H_
#define HKDT_PICO_W6300_ETHERNET_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 使用するソケット番号 (0~7)
#define SOCKET_NUM 0     
// マイコン側IPアドレス
#define MY_IP_ADDR {192, 168, 11, 2}     
// マイコン側ポート番号
#define SRC_PORT   64201  
// 送信先IPアドレス
#define DEST_ADDR {192, 168, 11, 4}  
// 送信先ポート番号 
#define DEST_PORT  64201     
// ゲートウェイ
#define GATE_WAY  {192, 168, 11, 1}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "wizchip/port_common.h"
#include "wizchip/wizchip_spi.h"
#include "wizchip_conf.h"
#include "W6300/w6300.h"
#include "socket.h"

static wiz_NetInfo w6300_info = {
    .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x56}, // MAC address
    .ip = MY_IP_ADDR,                     // IP address
    .sn = {255, 255, 255, 0},                    // Subnet Mask
    .gw = GATE_WAY,                     // Gateway
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

/// @brief 初期化関数
/// @return SOCKET_NUMなら成功。それ以外なら失敗
int initialize_w6300_ethernet();

/// @brief UDP通信でデータを受信する
/// @param buffer 受信データを格納するバッファ
/// @param size バッファのサイズ
/// @return 受信成功なら受信データのサイズ。失敗なら-1を返す
int recv_w6300_udp(uint8_t* buffer, int size);

/// @brief UDP通信でデータを送信する
/// @param buffer 送信データ
/// @param size 送信データのサイズ
/// @return 
int send_w6300_udp(uint8_t* buffer, int size);

#endif