cmake_minimum_required(VERSION 3.11)
project(nginxlighter C)

add_executable(mini
    src/mini.c
    src/http.c
    src/log.c
    src/serverconf.c
)

target_include_directories(mini PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
