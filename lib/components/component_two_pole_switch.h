#ifndef INCLUDED_COMPONENT_TWO_POLE_SWITCH_H
#define INCLUDED_COMPONENT_TWO_POLE_SWITCH_H

#include <component_base.h>

#include <ArduinoJson.h>
#include <bounce3.h>


namespace component {


class TwoPoleSwitch : public component::ComponentBase
{

public:
    TwoPoleSwitch(int pinNumber, int pinModeValue = INPUT);
    ~TwoPoleSwitch();

    virtual int getStateChange(JsonObject &jsonState);

    virtual void getCurrentState(JsonObject &jsonState);

    virtual void step(JsonObject &json);

    virtual void setPinState(uint8_t state);

    virtual int setPinStateAtIndex(int index, uint8_t state);

    virtual int getNumberOfPins() { return 1; }

    virtual int getPinNumberAtIndex(int index);




protected:
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
