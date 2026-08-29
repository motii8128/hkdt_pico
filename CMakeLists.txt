add_library(hkdt_pico STATIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(hkdt_pico
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/
)

target_link_libraries(hkdt_pico
    PUBLIC
        pico_stdlib
)