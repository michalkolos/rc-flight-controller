//
// Created by kolos on 22.04.2020.
//

#ifndef RC_FLIGHT_CONTROLLER_SERVO_H
#define RC_FLIGHT_CONTROLLER_SERVO_H

/**
 * Holds settings for individual servos.
 */
class Servo {
private:
    /// Minimum value that can can be sent to servo hardware.
    static const int minLowRange = 0;       //  0% duty cycle of servo PWM signal.
    /// Maximum value that can be sent to servo hardware.
    static const int maxHighRange = 4095;   //  100% duty cycle of servo PWM signal.
    /// Minimum value that can be used by high level api to describe servo position.
    static const int minRelativeRange = 0;
    /// Minimum value that can be used by high level api to describe servo position.
    static const int maxRelativeRange = 1000;

    /// Flag to enable servo (currently unused).
    bool enabled;

    /// Lowest value that this servo can accept.
    int lowRange;
    /// Highest value that this servo can accept.
    int highRange;
    /// Current servo position.
    int position;
    /// Current position on a 0 - 1000 scale.
    int relativePosition;

    /**
     * @brief Tests if servo position fits into the valid range.
     */
    void checkIfValidPos();

    /**
     * @brief Tests if the range is valid on the hardware level.
     */
    void checkIfValidRange();

    /**
     * @brief Converts servo position from a high level value to hardware level
     * value.
     * @param relativePosition High level API position.
     * @return Position in the hardware level range.
     */
    int mapToAbsolutePosition(int relativePosition) const;

public:
    /**
     * @param lowRange Low level minimum value accepted by the servo.
     * @param highRange Low level maximum value accepted by the servo.
     */
    Servo(int lowRange, int highRange);

    int getLowRange() const;
    void setLowRange(int lowRange);
    int getHighRange() const;
    void setHighRange(int highRange);
    int getRelativePosition() const;
    int setRelativePosition(int relativePosition);
    bool isEnabled() const;
};


#endif //RC_FLIGHT_CONTROLLER_SERVO_H
