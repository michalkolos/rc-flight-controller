//
// Created by kolos on 23.04.2020.
//

#ifndef RC_FLIGHT_CONTROLLER_SERVOI2CCOMMANDER_H
#define RC_FLIGHT_CONTROLLER_SERVOI2CCOMMANDER_H


#include <string>

class ServoI2cCommander {
private:
    int i2cInterfaceFile;
    std::string i2cInterfaceFilePath;
    int i2cDeviceAddress;
    int pwmFrequency;


    //  PCA9685 addresses
    //  =================

    const char START_ADDRESSES[16] = {
        0x06, 0x0A, 0x0E, 0x12, 0x16, 0x1A, 0x1E, 0x22,
        0x26, 0x2A, 0x2E, 0x32, 0x36, 0x3A, 0x3E, 0x42
    };
    const char STOP_ADDRESSES[16] = {
        0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24,
        0x28, 0x2C, 0x30, 0x34, 0x38, 0x3C, 0x40, 0x44
    };
    const char MODE1 = 0x00;
    const char PRE_SCALE = 0xFE;


    bool writeTo(const char bytes[], int length) const;

    void setI2cDeviceAddress() const;
    void setPwmFrequency() const;

public:
    ServoI2cCommander(const std::string &i2CInterfaceFilePath, int i2CDeviceAddress, int pwmFrequency);


    void init();
    void setStopTime(int channel, int val);
    void disable();

};


#endif //RC_FLIGHT_CONTROLLER_SERVOI2CCOMMANDER_H
