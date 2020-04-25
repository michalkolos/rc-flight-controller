//
// Created by kolos on 22.04.2020.
//

#include <unistd.h> // required for I2C device access
#include <fcntl.h>  // required for I2C device configuration
#include <sys/ioctl.h> // required for I2C device usage
#include <linux/i2c-dev.h> // required for constant definitions
#include <stdio.h>  // required for printf statements
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cmath>
#include <utility>


#include "Servos.h"
//
bool Servos::test() {

    char *filename = (char*)"/dev/i2c-1"; // Define the filename

    int i2cFile = open(filename, O_RDWR); // open file for R/W
    if (i2cFile < 0) {
        std::cerr << "Error" << errno << "from open: " << strerror(errno) << std::endl;
        return false;
    }

    //  Unlike Python (and Arduino), where the slave address is set on a
    //  per-transaction basis, we'll be setting up an "until further notice"
    //  address. To do this, we use the ioctl() function:
    int addr = 0x40;    // PCA9685 address
    ioctl(i2cFile, I2C_SLAVE, addr);   // Set the I2C address for upcoming
                                        //  transactions

    char buffer[5];   // Create a buffer for transferring data to the I2C device
    int length;

    //  Configure the PCA9685 Chip for Proper Operation
    //  ------------------------------------------------------------------------

    //  Setup for 50hz servos
    //  ------------------------------------------------------------------------
    //  Change the default pwm frequency to 50hz. This frequency is shared by
    //  all of the channels.

    buffer[0] = 0x00;
    buffer[1] = 0x90;
    length = 2;

    write(i2cFile, buffer, length);

    buffer[0] = 0xFE;
    buffer[1] = 0x79;
    length = 2;

    write(i2cFile, buffer, length);



    //  The default setup of the PCA9685 chip is not quite right for our
    //  purposes. We need to write to a couple of registers on the chip to make
    //  things right.

    //  First we must enable the chip, turning on the PWM output. This is
    //  accomplished by writing the value 0x20 to register 0.

    //  buffer[0] is always the register address, and subsequent bytes are
    //  written out in order.

    buffer[0] = 0;                      // target register
    buffer[1] = 0x20;                   // desired value
    length = 2;                     // number of bytes, including address

    write(i2cFile, buffer, length);    // initiate write





    //  Enable multi-byte writing.
    //  ------------------------------------------------------------------------
    //  Next, we must enable multi-byte writing, as we'll be writing two bytes
    //  at a time later when we set the PWM values. This time we don't need to
    //  set the length variable as it's already correctly configured.

    buffer[0] = 0xfe;
    buffer[1] = 0x1e;
    length = 2;

    write(i2cFile, buffer, length);







    //  Write Values to the PWM Registers
    //  ========================================================================
    //  That's all the setup that needs to be done. From here on out, we can
    //  write data to the PWM chip and expect to have it respond. Here's an
    //  example.

    buffer[0] = 0x06;   // "start time" reg for channel 0
                        //  This is the time when the chip will generate a high output.
    buffer[1] = 0;      // We want the pulse to start at time t=0
    buffer[2] = 0;
    length = 3;         // 3 bytes total written
    write(i2cFile, buffer, length); // initiate the write

    buffer[0] = 0x08;               // "stop time" reg for channel 0
    buffer[1] = 1250 & 0xff;        // The "low" byte comes first...
    buffer[2] = (1250>>8) & 0xff;   // followed by the high byte.
    write(i2cFile, buffer, length);// Initiate the write.

    //  The first write is to the "start time" register for channel 0. By
    //  default, the PWM frequency of the chip is 200Hz, or one pulse every 5ms.
    //  The start time register determines when the pulse goes high in the 5ms
    //  cycle. All channels are synchronized to that cycle. Generally, this
    //  should be written to 0.

    //  The second write is to the "stop time" register, and it controls when
    //  the pulse should go low. The range for this value is from 0 to 4095, and
    //  each count represents one slice of that 5ms period (5ms/4095), or about
    //  1.2us. Thus, the value of 1250 written above represents about 1.5ms of
    //  high time per 5ms period.

    //  Servo motors get their control signal from that pulse width. Generally
    //  speaking, a pulse width of 1.5ms yields a "neutral" position, halfway
    //  between the extremes of the motor's range. 1.0ms yields approximately 90
    //  degrees off center, and 2.0ms yields -90 degrees off center. In
    //  practice, those values may be slightly more or less than 90 degrees, and
    //  the motor may be capable of slightly more or less than 90 degrees of
    //  motion in either direction.

    //  To address other channels, simply increase the address of the two
    //  registers above by 4. Thus, start time for channel 1 is 0x0A, for
    //  channel 2 is 0x0E, channel 3 is 0x12, etc. and stop time address for
    //  channel 1 is 0x0C, for channel 2 is 0x10, channel 3 is 0x14, etc. See
    //  the table below.

    //    Channel #     Start Address	Stop Address
    //    Ch 0	        0x06	        0x08
    //    Ch 1	        0x0A	        0x0C
    //    Ch 2	        0x0E	        0x10
    //    Ch 3	        0x12	        0x14
    //    Ch 4	        0x16	        0x18
    //    Ch 5	        0x1A	        0x1C
    //    Ch 6	        0x1E	        0x20
    //    Ch 7	        0x22	        0x24
    //    Ch 8	        0x26	        0x28
    //    Ch 9	        0x2A	        0x2C
    //    Ch 10	        0x2E	        0x30
    //    Ch 11	        0x32	        0x34
    //    Ch 12	        0x36	        0x38
    //    Ch 13	        0x3A	        0x3C
    //    Ch 14	        0x3E	        0x40
    //    Ch 15	        0x42	        0x44

    //  If you write a 0 to the start address, every degree of offset from 90
    //  degrees requires 4.6 counts written to the stop address. In other words,
    //  multiply the number of degrees offset from neutral you wish to achieve
    //  by 4.6, then either add or subtract that result from 1250, depending on
    //  the direction of motion you wish. For example, a 45 degree offset from
    //  center would be 207 (45x4.6) counts either more or less than 1250,
    //  depending upon the direction you desire the motion to be in.



    //  ========================================================================

//    usleep(2000000); // sleep for 2s.
//
//    // Write stop time again. Don't need to write start time again because we
//    //  start time can always be left as zero. 836 corresponds to 1ms time,
//    //  which is 90 degrees offset from neutral.
//
//    buffer[0] = 0x08;   // "stop time" reg for channel 0
//    buffer[1] = 836 & 0xff; // The "low" byte comes first...
//    buffer[2] = (836>>8) & 0xff; // followed by the high byte.
//    write(i2cFile, buffer, length); // Initiate the write.
//
//    usleep(2000000);
//
//    // Write stop time again. 1664 is 2ms, or 90 degrees offset from neutral in
//    //  the opposite direction.
//    buffer[0] = 0x08;   // "stop time" reg for channel 0
//    buffer[1] = 1664 & 0xff; // The "low" byte comes first...
//    buffer[2] = (1664>>8) & 0xff; // followed by the high byte.
//    write(i2cFile, buffer, length); // Initiate the write.

    for(int i = 200; i < 400; i++){
        std::cout << i << std::endl;

        buffer[0] = 0x08;   // "stop time" reg for channel 0
        buffer[1] = i & 0xff; // The "low" byte comes first...
        buffer[2] = (i >> 8) & 0xff; // followed by the high byte.
        write(i2cFile, buffer, length); // Initiate the write.

        usleep(10000);
    }

//    for(int i = 600; i > 100; i--){
//        std::cout << i << std::endl;
//
//        buffer[0] = 0x08;   // "stop time" reg for channel 0
//        buffer[1] = i & 0xff; // The "low" byte comes first...
//        buffer[2] = (i >>8) & 0xff; // followed by the high byte.
//        write(i2cFile, buffer, length); // Initiate the write.
//
//        usleep(1000);
//    }

    return true;
}



//int Servos::calculateFrequency(int naturalFrequency) {
//    return std::round((25000000 / (4096 * naturalFrequency)) - 1);
//}


Servos::Servos(std::string i2cDevicePath, int i2cDeviceAddress, int naturalFrequency){
    servoCommander = new ServoI2cCommander(i2cDevicePath, i2cDeviceAddress, naturalFrequency);
}

Servos::~Servos() {
    delete(servoCommander);
    for(Servo* servo : servoArray){
        delete servo;
    }
}



bool Servos::initialize() {

    servoCommander->init();
    return true;
}

void Servos::addServo(int channel, int lowRange, int highRange) {
    servoArray[channel] = new Servo(lowRange, highRange);
}

void Servos::moveServo(int channel, int relativePos) {
    int absolutePos = servoArray[channel]->setRelativePosition(relativePos);
    servoCommander->setStopTime(channel, absolutePos);
}

void Servos::disableInterface() {
    servoCommander->disable();
}





