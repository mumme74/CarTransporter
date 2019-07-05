cmake_minimum_required(VERSION 3.0)



# user defined variables
set(INCLUDEDIRS "" CACHE PATH "Add this include path")

option(COMPILE_FOR_RPI "Cross compile for Raspberry" OFF)

# architecture specific
if(MSVC AND NOT COMPILE_FOR_RPI)
  add_compile_options(/W4 /WX)
else()
  if (COMPILE_FOR_RPI)
    SET(CMAKE_SYSTEM_NAME Linux)
  endif()

  add_compile_options(-Wall -Wextra -pedantic)
endif()

project(bootloader-host)
add_executable(${PROJECT_NAME} "commands.c"
                               "socketcan.c"
                               "canbridge.c"
                               "main.c"
                               "../node/crc32/crc32.c")

set(CMAKE_BUILD_TYPE Debug)
#add_definitions(-DDEBUG=1)

include_directories(
    ${INCLUDEDIRS}
    #/Volumes/qt5pi/usr/include/arm-rpi3-linux-gnueabihf
    ../../system_headers/
    ../node/crc32)
