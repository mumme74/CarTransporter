cmake_minimum_required(VERSION 2.8)

SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

project(bootloader-host)
add_executable(${PROJECT_NAME} "commands.c"
                               "main.c"
                               "../node/crc32/crc32.c")

set(CMAKE_BUILD_TYPE Debug)
add_definitions(-DDEBUG=1)

include_directories(
    /Volumes/qt5pi/usr/include/arm-rpi3-linux-gnueabihf
    ../../system_headers/
    ../node/crc32)
