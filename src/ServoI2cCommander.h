//
// Created by kolos on 23.04.2020.
//

#ifndef RC_FLIGHT_CONTROLLER_SERVOI2CCOMMANDER_H
#define RC_FLIGHT_CONTROLLER_SERVOI2CCOMMANDER_H


#include <string>

/**
 * Used to communicate with servo I2C hardware.
 */
class ServoI2cCommander {
private:
    /// File descriptor of the I2C device file.
    int i2cInterfaceFile;
    /// System path to the I2C device file.
    std::string i2cInterfaceFilePath;
    /// Address of the servo multiplexer device on the I2C bus.
    int i2cDeviceAddress;
    /// Register value that controls PWM frequency.
    int pwmFrequency;


    //  PCA9685 addresses
    //  =================

    /// Register addresses that set time for the PWM signal to go high for individual channels.
    const char START_ADDRESSES[16] = {
        0x06, 0x0A, 0x0E, 0x12, 0x16, 0x1A, 0x1E, 0x22,
        0x26, 0x2A, 0x2E, 0x32, 0x36, 0x3A, 0x3E, 0x42
    };

    /// Register addresses that set time for the PWM signal to go low for individual channels.
    const char STOP_ADDRESSES[16] = {
        0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24,
        0x28, 0x2C, 0x30, 0x34, 0x38, 0x3C, 0x40, 0x44
    };
    /// Address of the interface's MODE1 register.
    const char MODE1 = 0x00;
    /// Address of the interface's PRE_SCALE register.
    const char PRE_SCALE = 0xFE;

    /**
     * @brief Writes bytes to the device.
     * @param bytes Array of bytes sent to the device.
     * @param length Number of bytes sent to the device.
     * @return Returns true when there where no errors.
     */
    bool writeTo(const char bytes[], int length) const;

    void setI2cDeviceAddress() const;
    void setPwmFrequency() const;

public:
    ServoI2cCommander(const std::string &i2CInterfaceFilePath, int i2CDeviceAddress, int pwmFrequency);

    /**
     * @brief Run hardware initialization sequence.
     */
    void init();

    /**
     * @brief Changes the time when the PWM signal goes low for an individual channel.
     * @param channel Servo hardware channel.
     * @param val Value to be written to the register.
     */
    void setStopTime(int channel, int val);

     /**
     * @brief Turn of the interface.
     */
    void disable();
};


#endif //RC_FLIGHT_CONTROLLER_SERVOI2CCOMMANDER_H
