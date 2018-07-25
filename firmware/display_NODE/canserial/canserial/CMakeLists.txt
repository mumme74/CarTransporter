cmake_minimum_required(VERSION 2.8)

# This one is important
SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

project(canserial)

add_executable(${PROJECT_NAME} "main.c")

set(CMAKE_BUILD_TYPE Debug)

include_directories(
    /Volumes/qt5pi/usr/include/arm-rpi3-linux-gnueabihf
    ../../../system_headers/)
