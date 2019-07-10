#ifndef INCLUDED_COMPONENT_THREE_POLE_ROCKER_H
#define INCLUDED_COMPONENT_THREE_POLE_ROCKER_H

#include <component_base.h>
#include <component_two_pole_switch.h>

#include <ArduinoJson.h>

namespace component {


//  The only significant difference (code wise) between the two pole and three pole
//  switches is that we want the three pole switch to operate in INPUT_PULLUP mode
class ThreePoleRocker : public component::TwoPoleSwitch
{

public:
    ThreePoleRocker(int pinNumber, int pinModeValue = INPUT_PULLUP);
    ~ThreePoleRocker() {};

};



}  // end namespace Component
#endif
