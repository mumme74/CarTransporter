cmake_minimum_required(VERSION 3.0)
project(bootloader-host)


#common driver sources
set(DRIVER_SOURCES "slcan.c" "buffer.c")

# user defined variables
set(INCLUDEDIRS "" CACHE PATH "Add this include path")

option(COMPILE_FOR_RPI "Cross compile for Raspberry" OFF)
option(SET_DEBUG_FLAG "Set preprocessor DEBUG=1" OFF)

# architecture specific
if(WIN32 AND NOT COMPILE_FOR_RPI)
    add_compile_options(/W3) # /WX)
    set(WIN_FIXES "win_common.c")
else()
  set(UNIX_FIXES "unix_common.c")
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
            ${WIN_FIXES}
            ${UNIX_FIXES}
            "commands.c"
            "canbridge.c"
            "main.c"
            "../node/crc32/crc32.c")

# QTCREATOR fixes, find files not set by current configuration
# such as conditional inclution windows, linux files
FILE(GLOB_RECURSE HeadFiles "*.h")
add_custom_target(headers SOURCES ${HeadFiles})
FILE(GLOB_RECURSE CFiles "*.c")
FILE(GLOB_RECURSE CppFiles "*.cpp")
add_custom_target(srcs SOURCES ${CFiles} ${CppFiles})
#add_custom_target(other SOURCES qtcreator_debugger_helper.py)


# setup for compile
project(bootloader-host)
add_executable(${PROJECT_NAME} ${SOURCES})


if (WIN32 AND NOT COMPILE_FOR_RPI)
    target_link_libraries(${PROJECT_NAME} winmm)
endif()


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(SET_DEBUG_FLAG)
    add_definitions(-DDEBUG=1)
endif(SET_DEBUG_FLAG)

include_directories(
    ${INCLUDEDIRS}
    ../../system_headers/
    ../node/crc32)
