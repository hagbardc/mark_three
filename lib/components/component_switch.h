#ifndef INCLUDED_COMPONENT_SWITCH_H
#define INCLUDED_COMPONENT_SWITCH_H

#include <component_base.h>

#include <ArduinoJson.h>
#include <bounce3.h>


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
    Bounce m_debouncer;
    char m_stateValue[2];  //  "0" or "1" for switch off/on
    int m_pinNumber;

    //  m_isOn and m_wasOn are used for identifying state changes in the
    //  switch (since .rose() and .fell() don't seem to be working for the debouncer)
    bool m_isOn;
    bool m_wasOn;
};



}  // end namespace Component
#endif
