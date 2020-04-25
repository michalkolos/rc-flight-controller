//
// Created by kolos on 22.04.2020.
//

#include "Servo.h"



Servo::Servo(int lowRange, int highRange) : lowRange(lowRange), highRange(highRange) {
    checkIfValidRange();
    position = (int)(highRange - lowRange) / 2;
    checkIfValidPos();
    relativePosition = 500;

    enabled = true;
}


void Servo::checkIfValidPos() {
    if(position < lowRange){ position = lowRange; }
    if(position > highRange){ position = highRange; }
}

void Servo::checkIfValidRange() {
    if(lowRange > highRange){
        int temp = highRange;
        highRange = lowRange;
        lowRange = temp;
    }

    if(lowRange < minLowRange){ lowRange = minLowRange; }
    if(highRange > maxHighRange){ highRange = maxHighRange; }
}

int Servo::mapToAbsolutePosition(int relativePosition) const {
    return (int)(lowRange + (relativePosition - minRelativeRange) *
            (highRange - lowRange) / (maxRelativeRange - minRelativeRange));
}


int Servo::getLowRange() const {
    return lowRange;
}

void Servo::setLowRange(int lowRange) {
    this->lowRange = lowRange;
    checkIfValidRange();
}

int Servo::getHighRange() const {
    return highRange;
}

void Servo::setHighRange(int highRange) {
    this->highRange = highRange;
    checkIfValidRange();
}

int Servo::setRelativePosition(int relativePosition) {
    this->relativePosition = relativePosition;
    position = mapToAbsolutePosition(relativePosition);

    return position;
}

bool Servo::isEnabled() const {
    return enabled;
}

int Servo::getRelativePosition() const {
    return relativePosition;
}







