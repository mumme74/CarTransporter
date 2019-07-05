cmake_minimum_required(VERSION 3.0)


#common driver sources
set(DRIVER_SOURCES "slcan.c")

# user defined variables
set(INCLUDEDIRS "" CACHE PATH "Add this include path")

option(COMPILE_FOR_RPI "Cross compile for Raspberry" OFF)
option(SET_DEBUG_FLAG "Set preprocessor DEBUG=1" OFF)

# architecture specific
if(MSVC AND NOT COMPILE_FOR_RPI)
  add_compile_options(/W4 /WX)
else()
  if (COMPILE_FOR_RPI)
    set(RPI_SYSROOT CACHE PATH "Sysroot for raspberrypi" "")
    set(RPI_STAGING_PREFIX CACHE PATH "Install to" "")
    set(RPI_TOOLCHAIN_PATH CACHE PATH "Path to crosscompiler toolchain")
    set(CMAKE_SYSTEM_NAME Linux)
    #set(INCLUDEDIRS "/Volumes/qt5pi/usr/include/arm-rpi3-linux-gnueabihf" ${INCLUDEDIRS})

    set(CMAKE_SYSTEM_PROCESSOR arm)

    if (RPI_SYSROOT)
        set(CMAKE_SYSROOT ${RPI_SYSROOT})
    endif()
    if (RPI_STAGING_PREFIX)
        set(CMAKE_STAGING_PREFIX ${RPI_STAGING_PREFIX})
    endif()

    set(tools ${RPI_TOOLCHAIN_PATH})
    set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
    set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
  endif()
  if(CMAKE_SYSTEM_NAME STREQUAL Linux)
      option(BUILD_SOCKETCAN_DRIVER "Build the socketcan driver (only on linux)" ON)
      add_definitions(-DBUILD_SOCKETCAN)
      set(DRIVER_SOURCES "socketcan.c" ${DRIVER_SOURCES})
  endif()

  add_compile_options(-Wall -Wextra -pedantic)
endif()


# app sources
set(SOURCES ${DRIVER_SOURCES}
            "commands.c"
            "canbridge.c"
            "main.c"
            "../node/crc32/crc32.c")



# setup for compile
project(bootloader-host)
add_executable(${PROJECT_NAME} ${SOURCES})

set(CMAKE_BUILD_TYPE Debug)
if(SET_DEBUG_FLAG)
    add_definitions(-DDEBUG=1)
endif(SET_DEBUG_FLAG)

include_directories(
    ${INCLUDEDIRS}
    ../../system_headers/
    ../node/crc32)
