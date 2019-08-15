#include <component_three_way_switch.h>

namespace component {

//  Component for a simple switch
// Implemented using the Bounce2 library for debouncing


ThreeWaySwitch::ThreeWaySwitch(int pinArray[2], int pinModeValue)
:   ComponentBase(),
    m_currentState(0),
    m_oldState(0)

{

    for(int pinIndex=0; pinIndex <=1; ++pinIndex) {
        // Set the local pin values for reference
        m_pinNumber[pinIndex] = pinArray[pinIndex];
        pinMode(m_pinNumber[pinIndex], pinModeValue);

        //  Wire up the debouncer
        m_debouncerArray[pinIndex].attach(m_pinNumber[pinIndex]);
        m_debouncerArray[pinIndex].interval(5); // debounce interval in ms

        //  Set the initial values of the switch
        m_debouncerArray[pinIndex].update();
    }

    bool pinIndexOneState = m_debouncerArray[0].read();;
    bool pinIndexTwoState = m_debouncerArray[1].read();;

    //  Default value for m_currentState is 0
    if(pinIndexOneState) {
        m_currentState = 1;
    } else if(pinIndexTwoState) {
        m_currentState = 2;
    }

    m_oldState = m_currentState;

    //  Set up the component name
    sprintf(m_componentName, "switch-%02d-%02d", m_pinNumber[0], m_pinNumber[1]);


}

ThreeWaySwitch::~ThreeWaySwitch()
{}

int ThreeWaySwitch::getStateChange(JsonObject &jsonState)
{


    if( ! this->m_recentStateChange ) {
        return 0;
    }

    this->m_recentStateChange = false;

    sprintf(this->m_stateValue, "%d", this->m_currentState);

    //  "Element" is non-applicable:  This is only used for more complex
    //  components that have multiple elements (like a sub panel with
    //  multiple buttons)
    this->populateStateChange( jsonState,
        "n/a", "switch", this->m_stateValue);

    return 1;
}

void ThreeWaySwitch::getCurrentState(JsonObject &jsonState)
{
    jsonState["state"] = m_currentState;
}

void ThreeWaySwitch::step(JsonObject &json)
{
    // Update the Bounce instance
    for( int pinIndex=0; pinIndex <= 1; ++pinIndex) {
        m_debouncerArray[pinIndex].update();
    }

    this->updateCurrentStateValue();

    if(m_currentState == m_oldState) {
        this->m_recentStateChange = false;
        return;
    }

    this->m_recentStateChange = true;
    strncpy(this->m_stateValue, "%d", m_currentState);

    m_oldState = this->m_currentState;
}

void ThreeWaySwitch::updateCurrentStateValue()
{
    bool pinIndexOneState = m_debouncerArray[0].read();
    bool pinIndexTwoState = m_debouncerArray[1].read();

    if(! pinIndexOneState) {
        m_currentState = 1;
    } else if(! pinIndexTwoState) {
        m_currentState = 2;
    } else {
        m_currentState = 0;
    }
}

int ThreeWaySwitch::setPinStateAtIndex(int index, uint8_t state)
{
    if(index == 0 || index == 1) {
        m_debouncerArray[index].setPinState(state);
        return 0;
    }

    return 1;
}


int ThreeWaySwitch::getPinNumberAtIndex(int index) {
    if(index == 0 || index == 1) {
        return m_pinNumber[index];
    }

    return -1;
}




}
