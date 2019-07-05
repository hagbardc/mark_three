#ifndef INCLUDED_COMPONENT_ACCESS_MEGA2560_H
#define INCLUDED_COMPONENT_ACCESS_MEGA2560_H

#include <controller_access_base.h>

#include <Arduino.h>

namespace controller_access {

class Mega2560 : public ControllerAccessBase
{
public:

    Mega2560();
    ~Mega2560();

    //  Returns 1 if there was some error reading the input array, 0 else
    virtual int readDigitalPins(byte *inputArray, const int intputArraySize);

    virtual void printPinGroupings();

    virtual bool getBitForPin( byte *inputArray, const int pinNumber);



private:
    static int s_numberOfDigitalPins;
    static int s_numberOfAnalogPins;


};

}



#endif
