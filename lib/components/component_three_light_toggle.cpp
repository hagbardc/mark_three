#include <component_three_light_toggle.h>

#include <Arduino.h>

namespace component {

ThreeLightToggle::ThreeLightToggle(int switch_pin,
                                    int pinA, int pinB, int pinC,
                                    unsigned long millisecDelay) : m_switchPin(switch_pin)
{
    m_pinArray[0] = pinA;
    m_pinArray[1] = pinB;
    m_pinArray[2] = pinC;

    pinMode(m_pinArray[0], OUTPUT);
    pinMode(m_pinArray[1], OUTPUT);
    pinMode(m_pinArray[2], OUTPUT);

    pinMode(m_switchPin, INPUT);

    //  We can't start the system in an activated state
    if( digitalRead(m_switchPin) == HIGH ) {
        m_componentState = ToggleMode::INVALID_MODE;
    } else {
        m_componentState = ToggleMode::WAITING_MODE;
    }

    memcpy(this->m_componentStateAsString, "\0", sizeof(this->m_componentStateAsString));
    

}


ThreeLightToggle::~ThreeLightToggle(){}


void ThreeLightToggle::toggleModeAsString(ToggleMode toggleMode, char* toggleModeAsString)
{
    if(! toggleModeAsString ){
        return;
    }

    switch(toggleMode) {
        case RESET_FROM_ERROR_MODE:
            strncpy(toggleModeAsString, "RESET", sizeof("RESET"));
            break;
        case INVALID_MODE:
            strncpy(toggleModeAsString, "INVALID", sizeof("INVALID"));
            break;
        case PROCESSING_MODE:
            strncpy(toggleModeAsString, "PROCESSING", sizeof("PROCESSING"));
            break;
        case ACTIVE_MODE:
            strncpy(toggleModeAsString, "ACTIVE", sizeof("ACTIVE"));
            break;
        case WAITING_MODE:
        default:
            strncpy(toggleModeAsString, "WAITING", sizeof("WAITING"));
            break;
        
    }
}


void ThreeLightToggle::setStringState()
{
    int ledLit(0);
    if(m_pinArrayState[2]) {
        ledLit = 3;
    } else if(m_pinArrayState[1]) {
        ledLit = 2;
    } else if(m_pinArrayState[0]) {
        ledLit = 1;
    }

    char toggleMode[12];
    ThreeLightToggle::toggleModeAsString(m_componentState, &toggleMode[0]);
    snprintf(   this->m_componentStateAsString, sizeof(this->m_componentStateAsString), 
                "%s:%d",
                toggleMode, ledLit);
}


void ThreeLightToggle::getCurrentState(JsonObject &jsonState)
{
    this->setStringState();
    jsonState["state"] = this->m_componentStateAsString;
}


int ThreeLightToggle::getStateChange(JsonObject &jsonState)
{
    if( ! this->m_recentStateChange ) {
        return 0;
    }

    this->m_recentStateChange = false;


    //  "Element" is non-applicable:  This is only used for more complex
    //  components that have multiple elements (like a sub panel with
    //  multiple buttons)

    this->setStringState();

    this->populateStateChange( jsonState,
        "n/a", "3lighttoggle", this->m_componentStateAsString);

    return 1;
}




}