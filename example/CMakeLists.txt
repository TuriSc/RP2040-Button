cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(button_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
        example.c)

add_subdirectory(.. button)

target_link_libraries(${PROJECT_NAME} PRIVATE
        pico_stdlib
        button
        )

pico_set_program_name(${PROJECT_NAME} ${PROJECT_NAME})

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)