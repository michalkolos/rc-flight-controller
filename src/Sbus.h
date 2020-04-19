//
// Created by kolos on 19.04.2020.
//

#ifndef RC_FLIGHT_CONTROLLER_SBUS_H
#define RC_FLIGHT_CONTROLLER_SBUS_H


#include <string>


/**
* Handles communication with a radio receiver through a serial interface using
* SBUS protocol. Original SBUS signals need to be electrically inverted before
* connecting to Raspberry Pi's serial interface.
* Raspberry Pi serial pinout:
*  -   PIN06 - GND -> GND
*  -   PIN08 - TX  -> Smart Link
*  -   PIN10 - RX  -> SBUS
*
*/
class Sbus {
private:
    uint32_t serialPort;
    const char* devicePath;

    static const uint32_t _sbusBaud = 100000;
    static const uint8_t _numChannels = 20;
    static const uint8_t _payloadSize = 24;

    static const uint8_t _sbusHeader = 0x0F;
    static const uint8_t _sbusFooter = 0x00;
    static const uint8_t _channel16 = 0x01;
    static const uint8_t _channel17 = 0x02;
    static const uint8_t _sbusLostFrame = 0x04;
    static const uint8_t _sbusFailSafe = 0x08;

    static const uint_fast8_t  FAILSAFE = 18;
    static const uint_fast8_t  LOST_FRAME = 19;

    /**
     * an Array of integers that holds decoded data. Must have the
     * length of at least 20:
     *      - 0 - 15:   16 analogue channels,
     *      - 16 - 17:  2 additional digital channels,
     *      - 18: failsafe activated bit,
     *      - 19: lost frame bit.
     */
    uint16_t channels[_numChannels];

    /**
    * @brief Initializes Raspberry Pi's serial interface to accept communication
    * with the radio receiver.
    *
    * @return bool: true - on success, false - on failure
    */
    bool serialBegin();

    /**
     * @brief Prints channel data to the console.
     **/
    void sbusPrint();

    /**
     * @brief Parses raw data into useful values representing separate control
     * channels
     *
     * @param frame - raw data received from the serial interface.
     * @return bool - true in normal operation, false when lost frame was detected.
     */
    bool sbusParse(const uint8_t* frame);

    /**
     * @brief Reads raw data from the serial interface.
     *
     * @param frame - an array of bytes to store incomming data. Must be minimum 24
     * bytes long
     * @return bool: true - on success, false - on failure
     */
    bool serialRead(uint8_t* frame);

public:



};


#endif //RC_FLIGHT_CONTROLLER_SBUS_H
