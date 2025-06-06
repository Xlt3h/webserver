cmake_minimum_required(VERSION 3.11)
project(nginxlighter C)

add_executable(mini
   src/epolls.c 
   src/httpparser.c
   src/log.c
   src/masternginx.c
   src/mini.c
   src/serverconf.c
   src/worker.c
)

target_include_directories(mini PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
