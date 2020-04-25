//
// Created by kolos on 22.04.2020.
//

#ifndef RC_FLIGHT_CONTROLLER_SERVO_H
#define RC_FLIGHT_CONTROLLER_SERVO_H

class Servo {
private:
    static const int minLowRange = 0;       //  0% duty cycle of servo PWM signal.
    static const int maxHighRange = 4095;   //  100% duty cycle of servo PWM signal.
    static const int minRelativeRange = 0;
    static const int maxRelativeRange = 1000;

    bool enabled;

    int lowRange;
    int highRange;
    int position;
    int relativePosition;

    void checkIfValidPos();
    void checkIfValidRange();
    int mapToAbsolutePosition(int relativePosition) const;

public:
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
