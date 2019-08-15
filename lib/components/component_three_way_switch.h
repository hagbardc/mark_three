#ifndef INCLUDED_COMPONENT_THREE_WAY_SWITCH_H
#define INCLUDED_COMPONENT_THREE_WAY_SWITCH_H

#include <component_base.h>

#include <ArduinoJson.h>
#include <bounce3.h>


namespace component {


class ThreeWaySwitch : public component::ComponentBase
{

public:
    ThreeWaySwitch(int pinArray[2], int pinModeValue = INPUT);
    ~ThreeWaySwitch();


    virtual int getStateChange(JsonObject &jsonState);

    virtual void getCurrentState(JsonObject &jsonState);

    virtual void step(JsonObject &json);

    //virtual void setPinState(uint8_t state);

    virtual int setPinStateAtIndex(int index, uint8_t state);

    virtual int getNumberOfPins() { return 2; }

    virtual int getPinNumberAtIndex(int index);


protected:

    //  This component is implemented as just an array of two debouncers
    Bounce m_debouncerArray[2];
    char m_stateValue[2];  //  "0", "1", "2" for switch off/onA/onB
    int m_pinNumber[2];

    //  m_currentState and m_oldState are used for identifying state changes in the
    //  switch (since .rose() and .fell() don't seem to be working for the debouncer)
    int m_currentState;  // 0, 1, 2
    int m_oldState;  // 0, 1, 2

private:

    //  Calls .read() on the debouncers, updates m_oldState and m_currentState
    void updateCurrentStateValue();

};



}  // end namespace Component
#endif
