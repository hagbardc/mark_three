#include <component_potentiometer.h>

#include <Arduino.h>

namespace component {

Potentiometer::Potentiometer(int pinNumber) :
    m_pinNumber(pinNumber)
    , m_minimumIncrement(45)
{

}

Potentiometer::~Potentiometer() {}


void Potentiometer::step(JsonObject &json)
{
    //  If there are performance problems, we should look into reading this
    //  value as part of the array-reads.
    //

    int analogIntVal = analogRead(m_pinNumber);
    this->m_currentState = analogIntVal * 6;
    if(abs(this->m_currentState - this->m_oldState) < this->m_minimumIncrement) {
        this->m_recentStateChange = false;
        return;
    }
    this->m_recentStateChange = true;

    m_oldState = this->m_currentState;

    Serial.println(analogIntVal);
    return;

    //  Range for analogRead appears to be [0, 4423]
    //  We want to the range to be 0 - 9999 (for the digial display)
    if(analogIntVal < 20) {
        this->m_currentState = 0;
    } else if(analogIntVal > 1000) {
        this->m_currentState = 9999;
    } else {

        // Normalize analog value to [0,1] and scale state
        double doubleVal = (double)analogIntVal / (double)1024;
        this->m_currentState = (int)(9999 * doubleVal);
    }

    if(this->m_currentState > 9999) { this->m_currentState = 9999; }


    if(abs(this->m_currentState - this->m_oldState) < this->m_minimumIncrement) {
        this->m_recentStateChange = false;
        return;
    }

    this->m_recentStateChange = true;

    m_oldState = this->m_currentState;
}

int Potentiometer::getStateChange(JsonObject &jsonState)
{
    if( ! this->m_recentStateChange ) {
        return 0;
    }

    this->m_recentStateChange = false;


    //  "Element" is non-applicable:  This is only used for more complex
    //  components that have multiple elements (like a sub panel with
    //  multiple buttons)
    itoa(this->m_currentState, this->m_stateValue, 10);

    this->populateStateChange( jsonState,
        "n/a", "pot", this->m_stateValue);

    return 1;

}

void Potentiometer::getCurrentState(JsonObject &jsonState)
{
    jsonState["state"] = m_currentState;
}

int Potentiometer::getPinNumberAtIndex(int index)
{
    if(index != 0) {
        return -1;
    }
    return m_pinNumber;
}




}  // end namespace
