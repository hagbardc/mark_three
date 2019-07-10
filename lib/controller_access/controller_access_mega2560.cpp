#include <controller_access_mega2560.h>

#if defined(__AVR_ATmega2560__)

using namespace controller_access;

namespace controller_access {

int Mega2560::s_numberOfDigitalPins = 54;
int Mega2560::s_numberOfAnalogPins = 16;

Mega2560::Mega2560()
:   ControllerAccessBase()
{
    this->m_maxPinNumber = Mega2560::s_numberOfDigitalPins;
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


void Mega2560::printPinGroupings()
{
    Serial.print("    |-PINA-| ");
    Serial.print("|-PINB-| ");
    Serial.print("|-PINC-| ");
    Serial.print("|-PIND-| ");
    Serial.print("|-PINE-| ");
    Serial.print("|-PINF-| ");
    Serial.print("|-PING-| ");
    Serial.print("|-PINH-| ");
    Serial.println("|-PINJ-| ");
}

// TODO::  Complete the implementation of this
bool Mega2560::getBitForPin( byte *inputArray, const int pinNumber)
{
    byte value;
    switch(pinNumber) {
        case 2:
            value = (inputArray[4]) >> 4 & 0x01;
            break;
        case 3:
            value = (inputArray[4]) >> 5 & 0x01;
            break;
        case 4:
            value = (inputArray[6]) >> 5 & 0x01;
            break;
        case 5:
            value = (inputArray[4]) >> 3 & 0x01;
            break;
        case 6:
            value = (inputArray[7]) >> 3 & 0x01;
            break;
        case 7:
            value = (inputArray[7]) >> 4 & 0x01;
            break;
        case 8:
            value = (inputArray[7]) >> 5 & 0x01;
            break;
        case 9:
            value = (inputArray[7]) >> 6 & 0x01;
            break;
        case 10:
            value = (inputArray[1]) >> 4 & 0x01;
            break;
        case 11:
            value = (inputArray[1]) >> 5 & 0x01;
            break;
        case 12:
            value = (inputArray[1]) >> 6 & 0x01;
            break;
        case 13:
            value = (inputArray[1]) >> 7 & 0x01;
            break;
        case 22:
            value = (inputArray[0]) >> 0 & 0x01;
            break;
        case 23:
            value = (inputArray[0]) >> 1 & 0x01;
            break;
        case 24:
            value = (inputArray[0]) >> 2 & 0x01;
            break;
        case 25:
            value = (inputArray[0]) >> 3 & 0x01;
            break;
        case 26:
            value = (inputArray[0]) >> 4 & 0x01;
            break;
        case 27:
            value = (inputArray[0]) >> 5 & 0x01;
            break;
        case 28:
            value = (inputArray[0]) >> 6 & 0x01;
            break;
        case 29:
            value = (inputArray[0]) >> 7 & 0x01;
            break;
        case 30:
            value = (inputArray[2]) >> 7 & 0x01;
            break;
        case 31:
            value = (inputArray[2]) >> 6 & 0x01;
            break;
        case 32:
            value = (inputArray[2]) >> 5 & 0x01;
            break;
        case 33:
            value = (inputArray[2]) >> 4 & 0x01;
            break;
        case 34:
            value = (inputArray[2]) >> 3 & 0x01;
            break;
        case 35:
            value = (inputArray[2]) >> 2 & 0x01;
            break;
        case 36:
            value = (inputArray[2]) >> 1 & 0x01;
            break;
        case 37:
            value = (inputArray[2]) >> 0 & 0x01;
            break;
        case 38:
            value = (inputArray[3]) >> 7 & 0x01;
            break;
        case 39:
            value = (inputArray[6]) >> 2 & 0x01;
            break;
        case 40:
            value = (inputArray[6]) >> 1 & 0x01;
            break;
        case 41:
            value = (inputArray[6]) >> 0 & 0x01;
            break;
        case 50:
            value = (inputArray[1]) >> 3 & 0x01;
            break;
        case 51:
            value = (inputArray[1]) >> 2 & 0x01;
            break;
        case 52:
            value = (inputArray[1]) >> 1 & 0x01;
            break;
        case 53:
            value = (inputArray[1]) >> 0 & 0x01;
            break;
        default:
            value = 0;
    }
    return value;
}



}

#endif
