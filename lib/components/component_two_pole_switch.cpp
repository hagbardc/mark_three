#include <component_two_pole_switch.h>

namespace component {

//  Component for a simple switch
// Implemented using the Bounce2 library for debouncing


TwoPoleSwitch::TwoPoleSwitch(int pinNumber, int pinModeValue)
:   ComponentBase(),
    m_pinNumber(pinNumber),
    m_isOn(false),
    m_wasOn(false)

{
    // Set up the input pin
    pinMode(m_pinNumber, pinModeValue);

    // After setting up the button, setup the Bounce instance :
    m_debouncer.attach(m_pinNumber);
    m_debouncer.interval(5); // debounce interval in ms

    //  Set the initial values of the switch
    m_debouncer.update();
    m_isOn = m_debouncer.read();
    m_wasOn = m_debouncer.read();

    //  Set up the component name
    sprintf(m_componentName, "switch-%02d", m_pinNumber);


}

TwoPoleSwitch::~TwoPoleSwitch()
{}

int TwoPoleSwitch::getStateChange(JsonObject &jsonState)
{


    if( ! this->m_recentStateChange ) {
        return 0;
    }

    this->m_recentStateChange = false;

    if(this->m_isOn) {
        strncpy(this->m_stateValue, "1", 1);
    } else {
        strncpy(this->m_stateValue, "0", 1);
    }

    //  "Element" is non-applicable:  This is only used for more complex
    //  components that have multiple elements (like a sub panel with
    //  multiple buttons)
    this->populateStateChange( jsonState,
        "n/a", "switch", this->m_stateValue);

    return 1;
}

void TwoPoleSwitch::getCurrentState(JsonObject &jsonState)
{
    jsonState["state"] = m_isOn;
}

void TwoPoleSwitch::step(JsonObject &json)
{
    // Update the Bounce instance :
    m_debouncer.update();

    this->m_isOn = m_debouncer.read();

    if(m_isOn == m_wasOn) {
        this->m_recentStateChange = false;
        return;
    }

    this->m_recentStateChange = true;

    if( ! this->m_wasOn && this->m_isOn) {
        strncpy(this->m_stateValue, "1", 2);
    } else if( this->m_wasOn && !this->m_isOn ) {
        strncpy(this->m_stateValue, "0", 2);
    }

    this->m_wasOn = this->m_isOn;
}

void TwoPoleSwitch::setPinState(uint8_t state)
{
    m_debouncer.setPinState(state);
}




}
