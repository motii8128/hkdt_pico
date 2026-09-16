#ifndef HKDT_PICO_W6300_ETHERNET_H_
#define HKDT_PICO_W6300_ETHERNET_H_

#include "wizchip/port_common.h"
#include "wizchip/wizchip_spi.h"
#include "wizchip_conf.h"
#include "W6300/w6300.h"
#include "socket.h"

/// @brief w6300によるUDP通信を初期化する
/// @param socket_num ソケット番号
/// @param my_ip_addr 自分のIPアドレス(要素数４の整数配列)
/// @param my_gateway 自分のゲートウェイ(要素数４の整数配列)
/// @param my_port 自分のポート
/// @return 成功したらsocket_numを返す
int initialize_w6300_ethernet(int socket_num, uint8_t my_ip_addr[4], uint8_t my_gateway[4], uint16_t my_port);

/// @brief UDP通信でデータを受信する
/// @param socket_num ソケット番号
/// @param buffer 受信データを格納するバッファ
/// @param size バッファのサイズ
/// @return 受信成功なら受信データのサイズ。失敗なら-1を返す
int recv_w6300_udp(int socket_num, uint8_t* buffer, int size);

/// @brief UDP通信でデータを送信する
/// @param socket_num ソケット番号
/// @param buffer 送信データ
/// @param size 送信するデータのサイズ
/// @param dest_ip 送信先のIPアドレス
/// @param dest_port 送信先のポート
/// @return 
int send_w6300_udp(int socket_num, uint8_t* buffer, int size, uint8_t dest_ip[4], uint16_t dest_port);

/// @brief UDP通信を終了しハードウェアリソースを開放する
/// @param socket_num 
void close_w6300_ethernet(int socket_num);

#endif