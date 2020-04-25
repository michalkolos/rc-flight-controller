cmake_minimum_required(VERSION 3.13)
project(rc_flight_controller)

set(CMAKE_CXX_STANDARD 14)

add_executable(rc_flight_controller src/main.cpp src/Sbus.cpp src/Sbus.h src/WorkerThread.cpp src/WorkerThread.h src/Servos.cpp src/Servos.h src/Servo.cpp src/Servo.h src/ServoI2cCommander.cpp src/ServoI2cCommander.h)
target_link_options(rc_flight_controller PRIVATE -pthread)