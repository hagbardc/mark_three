#include <controller_access_uno.h>



using namespace controller_access;

namespace controller_access {

int Uno::s_numberOfDigitalPins = 14;
int Uno::s_numberOfAnalogPins = 6;

Uno::Uno()
:   ControllerAccessBase()
{
    this->m_maxPinNumber = Uno::s_numberOfDigitalPins;
}

Uno::~Uno()
{}

int Uno::readDigitalPins(byte *inputArray, const int intputArraySize)
{

    inputArray[0] = PINB;
    inputArray[1] = PINC;
    inputArray[2] = PIND;

    return 0;
}


void Uno::printPinGroupings()
{
    Serial.print("    |-PINB-| ");
    Serial.print("|-PINC-| ");
    Serial.println("|-PIND-| ");
}

bool Uno::getBitForPin( byte *inputArray, const int pinNumber)
{
    byte value;
    switch(pinNumber) {
        case 2:
            value = inputArray[2] >> 2;
            break;
        case 3:
            value = inputArray[2] >> 3;
            break;
        case 4:
            value = inputArray[2] >> 4;
            break;
        case 5:
            value = inputArray[2] >> 5;
            break;
        case 6:
            value = inputArray[2] >> 6;
            break;
        case 7:
            value = inputArray[2] >> 7;
            break;
        case 8:
            value = inputArray[0] >> 0;
            break;
        case 9:
            value = inputArray[0] >> 1;
            break;
        case 10:
            value = inputArray[0] >> 2;
            break;
        case 11:
            value = inputArray[0] >> 3;
            break;
        case 12:
            value = inputArray[0] >> 4;
            break;
        case 13:
            value = inputArray[1] >> 5;
            break;
        default:
            value = 0;
    }
    return value;
}



}
