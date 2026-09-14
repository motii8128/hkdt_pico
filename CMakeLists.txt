add_library(hkdt_pico STATIC
    ${CMAKE_CURRENT_LIST_DIR}
    # 自作
    src/can.c
    src/pid.c
    src/pwm.c
    src/ws2812.c
    src/robomaster.c
    src/w6300_ethernet.c
    
    # LANライブラリ
    ioLibrary_Driver/Ethernet/wizchip_conf.c
    ioLibrary_Driver/Ethernet/socket.c
    ioLibrary_Driver/Ethernet/W6300/w6300.c
    src/wizchip/wizchip_gpio_irq.c
    src/wizchip/wizchip_qspi_pio.c
    src/wizchip/wizchip_spi.c

    # CAN通信ライブラリ
    ${CMAKE_CURRENT_LIST_DIR}/can2040/src/can2040.c
)

pico_generate_pio_header(
    hkdt_pico
    ${CMAKE_CURRENT_LIST_DIR}/inc/wizchip/wizchip_qspi_pio.pio
)

target_include_directories(hkdt_pico
    PUBLIC
    # 自作
    ${CMAKE_CURRENT_LIST_DIR}/
    ${CMAKE_CURRENT_LIST_DIR}/inc/

    # LANライブラリ
    ${CMAKE_CURRENT_LIST_DIR}/ioLibrary_Driver
    ${CMAKE_CURRENT_LIST_DIR}/ioLibrary_Driver/Ethernet
    ${CMAKE_CURRENT_LIST_DIR}/wizchip/

    # CAN通信ライブラリ
    ${CMAKE_CURRENT_LIST_DIR}/can2040/src/
)

target_link_libraries(hkdt_pico
    PUBLIC
        pico_stdlib
        hardware_spi
        hardware_dma
        hardware_pio
        hardware_pwm
)