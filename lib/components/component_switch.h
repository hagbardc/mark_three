#ifndef INCLUDED_COMPONENT_SWITCH_H
#define INCLUDED_COMPONENT_SWITCH_H

#include <component_base.h>

#include <ArduinoJson.h>


namespace component {


class Switch : public component::ComponentBase
{

public:
    Switch(int pinNumber);
    ~Switch();

    virtual int getStateChange(JsonObject &jsonState);

    virtual void getCurrentState(JsonObject &jsonState);

    virtual void step(JsonObject &json);

private:
    bool m_isOn;
};



}  // end namespace Component
#endif
