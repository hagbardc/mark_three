#include <component_switch.h>

#include <Bounce2.h>


namespace component {

//  Component for a simple switch
// Implemented using the Bounce2 library for debouncing


Switch::Switch(int pinNumber)
:   ComponentBase(),
    m_pinNumber(pinNumber),
    m_isOn(false),
    m_wasOn(false)

{
    // Set up the input pin
    pinMode(m_pinNumber, INPUT_PULLUP);

    // After setting up the button, setup the Bounce instance :
    m_debouncer.attach(m_pinNumber);
    m_debouncer.interval(5); // debounce interval in ms

    //  Set the initial values of the switch
    m_debouncer.update();
    m_isOn = m_debouncer.read();
    m_wasOn = m_debouncer.read();

    sprintf(m_componentName, "switch-%02d", m_pinNumber);


}

Switch::~Switch()
{}

int Switch::getStateChange(JsonObject &jsonState)
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

    this->populateStateChange( jsonState,
        this->m_componentName, "switch", this->m_stateValue);

    Serial.print("State Change: ");
    Serial.print(this->m_componentName);
    Serial.print(": ");
    Serial.println(this->m_stateValue);

    return 1;
}

void Switch::getCurrentState(JsonObject &jsonState)
{
    jsonState["state"] = m_isOn;
}

void Switch::step(JsonObject &json)
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
        Serial.println("Rose");
        strncpy(this->m_stateValue, "1", 2);
    } else if( this->m_wasOn && !this->m_isOn ) {
        Serial.println("Fell");
        strncpy(this->m_stateValue, "0", 2);
    }

    this->m_wasOn = this->m_isOn;
}





}
