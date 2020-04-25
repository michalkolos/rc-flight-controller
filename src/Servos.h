//
// Created by kolos on 22.04.2020.
//

#ifndef RC_FLIGHT_CONTROLLER_SERVOS_H
#define RC_FLIGHT_CONTROLLER_SERVOS_H


#include <array>
#include "Servo.h"
#include "ServoI2cCommander.h"

class Servos {
private:

    static const int CHANNEL_NO = 16;
    ServoI2cCommander* servoCommander;
    std::array<Servo*, CHANNEL_NO> servoArray{};

public:
    Servos(std::string i2cDevicePath, int i2cDeviceAddress, int naturalFrequency);
    virtual ~Servos();

    bool initialize();

    void addServo(int channel, int lowRange, int highRange);
    void moveServo(int channel, int relativePos);

    void disableInterface();

    bool test();

};

#endif //RC_FLIGHT_CONTROLLER_SERVOS_H
