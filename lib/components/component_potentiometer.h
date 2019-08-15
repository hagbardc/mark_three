#ifndef INCLUDED_COMPONENT_POTENTIOMETER_H
#define INCLUDED_COMPONENT_POTENTIOMETER_H

#include <component_base.h>

#include <ArduinoJson.h>


namespace component {


class Potentiometer : public component::ComponentBase
{

public:
    Potentiometer(int analogPin);
    ~Potentiometer();


    virtual int getStateChange(JsonObject &jsonState);

    virtual void getCurrentState(JsonObject &jsonState);

    virtual void step(JsonObject &json);

    // We don't implement this, since it's not coherent for the Potentiometer class
    //virtual int setPinStateAtIndex(int index, uint8_t state);

    virtual int getNumberOfPins() { return 1; }

    virtual int getPinNumberAtIndex(int index);


protected:

    int m_pinNumber;

    int m_currentState;
    int m_oldState;

    char m_stateValue[6];  // 0 - 1023

private:

    //  The least sized step we'll allow for the values
    int m_minimumIncrement;
};



}  // end namespace Component
#endif
