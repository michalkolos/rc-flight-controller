//
// Created by kolos on 19.04.2020.
//

#include <iostream>
#include <iomanip>

#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>

#include <cinttypes>
#include <utility>
#include <asm/ioctls.h>
#include <asm/termbits.h>
#include <sys/ioctl.h>
#include <thread>
#include <algorithm>



#include "Sbus.h"

// TODO: Replace C code with C++
bool Sbus::serialBegin() {

    serialPort = open(devicePath.c_str(), O_RDWR | O_NOCTTY);

    if (serialPort < 0) {

        std::cerr << "Error" << errno << "from open: " << strerror(errno) << std::endl;
        return false;
    }

    struct termios2 tio;

    errno = 0;
    if(ioctl(serialPort, TCGETS2, &tio)){
        std::cerr << "Error" << errno << "from ioctl TCGETS2: " << strerror(errno) << std::endl;
        return false;
    }

    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    tio.c_cflag |= CSTOPB; // 2 stop bits
    tio.c_cflag |= PARENB; // enable parity bit, even by default

    tio.c_cflag |= CS8;
    tio.c_cflag &= ~CRTSCTS;
    tio.c_cflag |= CREAD | CLOCAL;
    tio.c_lflag &= ~ICANON;
    tio.c_lflag &= ~ECHO;
    tio.c_lflag &= ~ISIG;
    tio.c_iflag &= ~(IXON | IXOFF | IXANY);
    tio.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    tio.c_oflag &= ~OPOST;
    tio.c_oflag &= ~ONLCR;
    tio.c_cc[VTIME] = 10;
    tio.c_cc[VMIN] = 0;

    tio.c_ispeed = tio.c_ospeed = _sbusBaud;

    if(ioctl(serialPort, TCSETS2, &tio)){
        std::cerr << "Error" << errno << "from ioctl TCSETS2: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

void Sbus::sbusPrint() {

    for(unsigned short channel : channels){
        std::cout << std::setw(5) << channel << " ";
    }
    std::cout << std::endl;
}

bool Sbus::sbusParse(const uint8_t *frame) {

    std::lock_guard<std::mutex> guard(channelsMutex);

    channels[0]  = (uint16_t) ((frame[0]     | frame[1] <<8)                     & 0x07FF);
    channels[1]  = (uint16_t) ((frame[1]>>3  | frame[2] <<5)                     & 0x07FF);
    channels[2]  = (uint16_t) ((frame[2]>>6  | frame[3] <<2 | frame[4]<<10)      & 0x07FF);
    channels[3]  = (uint16_t) ((frame[4]>>1  | frame[5] <<7)                     & 0x07FF);
    channels[4]  = (uint16_t) ((frame[5]>>4  | frame[6] <<4)                     & 0x07FF);
    channels[5]  = (uint16_t) ((frame[6]>>7  | frame[7] <<1 | frame[8]<<9)       & 0x07FF);
    channels[6]  = (uint16_t) ((frame[8]>>2  | frame[9] <<6)                     & 0x07FF);
    channels[7]  = (uint16_t) ((frame[9]>>5  | frame[10]<<3)                     & 0x07FF);
    channels[8]  = (uint16_t) ((frame[11]    | frame[12]<<8)                     & 0x07FF);
    channels[9]  = (uint16_t) ((frame[12]>>3 | frame[13]<<5)                     & 0x07FF);
    channels[10] = (uint16_t) ((frame[13]>>6 | frame[14]<<2 | frame[15]<<10)     & 0x07FF);
    channels[11] = (uint16_t) ((frame[15]>>1 | frame[16]<<7)                     & 0x07FF);
    channels[12] = (uint16_t) ((frame[16]>>4 | frame[17]<<4)                     & 0x07FF);
    channels[13] = (uint16_t) ((frame[17]>>7 | frame[18]<<1 | frame[19]<<9)      & 0x07FF);
    channels[14] = (uint16_t) ((frame[19]>>2 | frame[20]<<6)                     & 0x07FF);
    channels[15] = (uint16_t) ((frame[20]>>5 | frame[21]<<3)                     & 0x07FF);

    if(frame[22] & _channel16) {
        channels[16] = 1;
    } else {
        channels[16] = 0;
    }

    if(frame[22] & _channel17) {
        channels[17] = 1;
    } else {
        channels[17] = 0;
    }

    if (frame[22] & _sbusFailSafe) {
        channels[FAILSAFE] = 1;
    } else {
        channels[FAILSAFE] = 0;
    }


    if (frame[22] & _sbusLostFrame) {
        channels[LOST_FRAME] = 1;
        return -1;
    } else {
        channels[LOST_FRAME] = 0;
    }

    return true;
}

bool Sbus::serialRead(uint8_t *frame) {

    char byteBuffer[1];
    char prevByte = _sbusFooter;
    int frameCouter = 0;

    // TODO: Check if testing for incomming data on serial port is necessary.
    // int available = 0;
    // if( ioctl(serial_port, FIONREAD, &available ) < 0 ) {
    //     printf("Error %i from ioctl FIONREAD: %s\n", errno, strerror(errno));
    // }
    // if(available >0){

    while(frameCouter < _payloadSize){
        int len = read(serialPort, byteBuffer, sizeof(byteBuffer));

        if(len < 0){
            printf("Error %i from read: %s\n", errno, strerror(errno));
            return false;
        }

        if(byteBuffer[0] == _sbusHeader && prevByte == _sbusFooter){
            frameCouter = 0;
        }else{
            frame[frameCouter] = (uint8_t)byteBuffer[0];
            frameCouter++;
        }

        prevByte = byteBuffer[0];
    }

    return true;
}

void Sbus::checkSbus() {
    serialBegin();

    uint8_t frame[_payloadSize];

    serialRead(frame);
    sbusParse(frame);
}

Sbus::Sbus(std::string devicePath) : devicePath(std::move(devicePath)) {

    std::thread sbusInThread(&Sbus::checkSbus, this);
}


std::array<uint16_t, Sbus::_numChannels> Sbus::getChannels() {

    std::lock_guard<std::mutex> guard(channelsMutex);
    return channels;
}


