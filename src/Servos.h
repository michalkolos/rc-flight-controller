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
    /// Number of servo channels available in the I2C interface hardware.
    static const int CHANNEL_NO = 16;
    /// Pointer to I2C interface hardware controller class.
    ServoI2cCommander* servoCommander;
    /// Table of all servo channels that holds individual servo objects.
    std::array<Servo*, CHANNEL_NO> servoArray{};

public:
    /**
     * @param i2cDevicePath System level file path to I2C interface.
     * @param i2cDeviceAddress Address in the I2C interface to the servo
     * multiplexing hardware.
     * @param naturalFrequency Operational frequency for all servos in the interface.
     * Represented in hertz.
     */
    Servos(std::string i2cDevicePath, int i2cDeviceAddress, int naturalFrequency);
    virtual ~Servos();

    /**
     * @brief Initialize the hardware interface
     */
    bool initialize();

    /**
     * @brief Add new servo to the interface.
     * @param channel Physical channel that the servo is connected to.
     * @param lowRange Minimum value that can can be sent to servo hardware.
     * @param highRange Maximum value that can be sent to servo hardware.
     */
    void addServo(int channel, int lowRange, int highRange);

    /**
     * @brief Rotate the servo to a new position.
     * @param channel Channel number of the servo to rotate.
     * @param relativePos New position described in the relative scale.
     */
    void moveServo(int channel, int relativePos);

    /**
     * @brief Turns of the hardware interface. Stops WPM control signals.
     */
    void disableInterface();

    bool test();

};

#endif //RC_FLIGHT_CONTROLLER_SERVOS_H
