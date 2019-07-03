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

protected:
    int m_maxPinNumber;
};




}


#endif
