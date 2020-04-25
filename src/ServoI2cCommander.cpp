//
// Created by kolos on 23.04.2020.
//

#include "ServoI2cCommander.h"
#include <linux/i2c-dev.h> // required for constant definitions
#include <sys/ioctl.h>
#include <unistd.h> // required for I2C device access
#include <iostream>
#include <cerrno>
#include <string>
#include <cstring>
#include <cmath>
#include <utility>
#include <fcntl.h>  // required for I2C device configuration
#include <cstdio>  // required for printf statements

ServoI2cCommander::ServoI2cCommander(const std::string &i2CInterfaceFilePath, int i2CDeviceAddress, int pwmFrequency)
        : i2cInterfaceFilePath(i2CInterfaceFilePath), i2cDeviceAddress(i2CDeviceAddress), pwmFrequency(pwmFrequency) {

    i2cInterfaceFile = 0;
}



bool ServoI2cCommander::writeTo(const char bytes[], int length) const {

    int n = write(i2cInterfaceFile, bytes, length);
    if(n == 0){
        std::cerr << "Error, nothing was written to the device!" << std::endl;
        return false;
    }else if(n < 0){
        std::cerr << "Error " << errno << " from write: " << strerror(errno) << ": " << std::hex << bytes << std::endl;
        return false;
    }

    return true;
}

void ServoI2cCommander::setI2cDeviceAddress() const {
    ioctl(i2cInterfaceFile, I2C_SLAVE, i2cDeviceAddress);
    std::cout << "Servo interface address set to: " << std::hex << i2cDeviceAddress << std::endl;
}

void ServoI2cCommander::setPwmFrequency() const {
    char calculatedPwm = std::round((25000000 / (4096 * pwmFrequency)) - 1);
    char payload[5];

    payload[0] = MODE1;
    payload[1] = 0x90;
    writeTo(payload, 2);

    payload[0] = PRE_SCALE;
    payload[1] = calculatedPwm;
    writeTo(payload, 2);
}

void ServoI2cCommander::init() {

    i2cInterfaceFile = open(i2cInterfaceFilePath.c_str(), O_RDWR); // open file for R/W
    if (i2cInterfaceFile < 0) {
        std::cerr << "Error " << errno << " from open: " << strerror(errno) << ": " << i2cInterfaceFilePath << std::endl;
    }

    setI2cDeviceAddress();
    setPwmFrequency();
    char payload[5];

    //  EnableChip
    payload[0] = MODE1;
    payload[1] = 0x20;
    writeTo(payload, 2);

    //  Enable multi-byte writing
    payload[0] = PRE_SCALE;
    payload[1] = 0x1E;
    writeTo(payload, 2);

    //  Set start time for all channels

    payload[1] = 0;
    payload[2] = 0;

    for(char i : START_ADDRESSES){
        payload[0] = i;
        writeTo(payload, 3);
    }
}
void ServoI2cCommander::setStopTime(int channel, int val) {
    char payload[3];
    payload[0] = STOP_ADDRESSES[channel];
    payload[1] = val & 0xff;
    payload[2] = (val >> 8) & 0xff;

    writeTo(payload, 3);
}

void ServoI2cCommander::disable() {
    char payload[5];

    payload[0] = MODE1;
    payload[1] = 0x90;
    writeTo(payload, 2);
}






