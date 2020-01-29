cmake_minimum_required(VERSION 3.15)
project(EasySPI)

set(CMAKE_CXX_STANDARD 14)

add_library(EasySPI library.cpp library.h)

# change the directory location to your arduino install location for fetching Arduino.h
include_directories(~/Documents/arduino-1.8.10/hardware/arduino/avr/cores/arduino/)