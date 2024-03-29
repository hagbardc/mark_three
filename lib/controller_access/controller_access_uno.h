#ifndef INCLUDED_COMPONENT_ACCESS_UNO_H
#define INCLUDED_COMPONENT_ACCESS_UNO_H

#include <controller_access_base.h>

#include <Arduino.h>

namespace controller_access {

class Uno : public ControllerAccessBase
{
public:

    Uno();
    ~Uno();

    //  Returns 1 if there was some error reading the input array, 0 else
    virtual int readDigitalPins(byte *inputArray, const int intputArraySize);

    virtual void printPinGroupings();

    virtual bool getStateForPin( byte *inputArray, const int pinNumber);

    virtual int getNumberOfRegisters() { return 3; }


private:
    static int s_numberOfDigitalPins;
    static int s_numberOfAnalogPins;


};

}



#endif
