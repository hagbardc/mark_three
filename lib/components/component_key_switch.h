#ifndef INCLUDED_COMPONENT_KEY_SWITCH_H
#define INCLUDED_COMPONENT_KEY_SWITCH_H

#include <component_base.h>
#include <component_two_pole_switch.h>

#include <ArduinoJson.h>

namespace component {


//  The only significant difference (code wise) between the two pole and three pole
//  switches is that we want the three pole switch to operate in INPUT_PULLUP mode
class KeySwitch : public component::TwoPoleSwitch
{

public:
    KeySwitch(int pinNumber, int pinModeValue = INPUT_PULLUP);
    ~KeySwitch() {};

};



}  // end namespace Component
#endif
