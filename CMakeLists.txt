cmake_minimum_required(VERSION 3.15)
project(EasySPI)

set(CMAKE_CXX_STANDARD 14)

add_library(EasySPI EasySPI.cpp EasySPI.h)

# change the directory location to your arduino install location for fetching Arduino.h
set(SOURCE_DIR ~/Documents/arduino-1.8.10/)
include_directories(${SOURCE_DIR}/hardware/arduino/avr/cores/arduino/)
include_directories(${SOURCE_DIR}/hardware/arduino/avr/libraries/SPI/src)
include_directories(${SOURCE_DIR}/hardware/tools/avr/avr/include/)