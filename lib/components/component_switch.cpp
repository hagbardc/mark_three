#include <component_switch.h>


namespace component {

Switch::Switch(int pinNumber) : m_isOn(false)
{

}

Switch::~Switch()
{}

int Switch::getStateChange(JsonObject &jsonState)
{
    return 1;
}

void Switch::getCurrentState(JsonObject &jsonState)
{
}

void Switch::step(JsonObject &json)
{

}





}
