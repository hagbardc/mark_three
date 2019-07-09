#ifndef INCLUDED_CONTROLLER_ACCESS_BASE_H
#define INCLUDED_CONTROLLER_ACCESS_BASE_H

#include <Arduino.h>

namespace controller_access {


// This class is intended to provide microcontroller independent
// access to functionality (primarily and perhaps exclusively bulk
// pin reading access)
class ControllerAccessBase
{
public:
    ControllerAccessBase() { }
    virtual ~ControllerAccessBase() {}

    //  Returns 1 if there was some error reading the input array, 0 else
    virtual int readDigitalPins(byte *inputArray, const int intputArraySize) = 0;

    int checksumUsingDigitalRead(const int numDigitalPins);

    int getMaxPinNumber() { return m_maxPinNumber; }

    ///  Prints out a formatted pin group to serial
    ///  For use in association with controller_access::serialPrintByteArray
    virtual void printPinGroupings() = 0;

    /// Returns the value of the bit in the input array (as generated from a
    /// call to readDigialPins) corresponding to the pin input for this
    /// particular board
    virtual bool getBitForPin( byte *inputArray, const int pinNumber) = 0;


protected:
    int m_maxPinNumber;
};




}


#endif
