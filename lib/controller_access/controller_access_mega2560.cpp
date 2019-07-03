#include <controller_access_mega2560.h>


#define BITS_IN_BYTE 8

using namespace controller_access;

namespace controller_access {

int Mega2560::s_numberOfDigitalPins = 54;
int Mega2560::s_numberOfAnalogPins = 16;

Mega2560::Mega2560()
:   ControllerAccessBase()
{
    this->m_maxPinNumber = Mega2560::s_numberOfDigitalPins;

    Serial.print(this->m_maxPinNumber);
    Serial.print(" is the max pin number");
    for(int ii=0; ii<Mega2560::s_numberOfDigitalPins; ++ii) {
        pinMode(ii, INPUT_PULLUP);
    }
}

Mega2560::~Mega2560()
{}

int Mega2560::readDigitalPins(byte *inputArray, const int intputArraySize)
{
    /*
    if( (intputArraySize * BITS_IN_BYTE) > Mega2560::s_numberOfDigitalPins) {
        Serial.println("Error in input to readDigitalPins");
        return 1;
    }
    */

    inputArray[0] = PINA;
    inputArray[1] = PINB;
    inputArray[2] = PINC;
    inputArray[3] = PIND;
    inputArray[4] = PINE;
    inputArray[5] = PINF;
    inputArray[6] = PING;
    inputArray[7] = PINH;
    inputArray[8] = PINJ;
    //inputArray[9] = PINK;

    return 0;
}


}
