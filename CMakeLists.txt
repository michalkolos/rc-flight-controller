cmake_minimum_required(VERSION 3.13)
project(rc_flight_controller)

set(CMAKE_CXX_STANDARD 14)

add_executable(rc_flight_controller src/main.cpp src/sbus.cpp src/sbus.h)