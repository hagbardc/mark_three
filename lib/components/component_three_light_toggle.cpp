#include <component_three_light_toggle.h>

#include <Arduino.h>

namespace component {

unsigned long ThreeLightToggle::s_blinkDelay = 125;  // in ms

ThreeLightToggle::ThreeLightToggle(int switch_pin,
                                    int pinA, int pinB, int pinC,
                                    unsigned long millisecDelay) :  m_switchPin(switch_pin),
                                                                    m_switchPinState(0),
                                                                    m_errorModeLightTimestamp(0),
                                                                    m_switchOnTimestamp(0),
                                                                    m_millisecDelay(millisecDelay)
{
    m_pinArray[0] = pinA;
    m_pinArray[1] = pinB;
    m_pinArray[2] = pinC;

    pinMode(m_pinArray[0], OUTPUT);
    pinMode(m_pinArray[1], OUTPUT);
    pinMode(m_pinArray[2], OUTPUT);
    pinMode(m_switchPin, INPUT);

    m_pinArrayState[0] = LOW;
    m_pinArrayState[1] = LOW;
    m_pinArrayState[2] = LOW;

    //  We can't start the system in an activated state
    if( digitalRead(m_switchPin) == HIGH ) {
        //m_componentState = ToggleMode::INVALID_MODE;
        m_switchPinState = 1;
    } else {
        m_componentState = ToggleMode::WAITING_MODE;
        m_switchPinState = 0;
    }
    m_previousSwitchPinState = m_switchPinState;

    m_switch = new TwoPoleSwitch(m_switchPin);

    memcpy(this->m_componentStateAsString, "\0", sizeof(this->m_componentStateAsString));
}

ThreeLightToggle::~ThreeLightToggle() 
{ 
    delete m_switch;
}
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
        "n/a", "statechange", this->m_componentStateAsString);

    return 1;
}

void ThreeLightToggle::setPinState(uint8_t state)
{
   m_switchPinState = state;
   m_switch->setPinState(state);
}

int ThreeLightToggle::setPinStateAtIndex(int index, uint8_t state)
{
    if(0 == index) {
        m_switchPinState = state;
        m_switch->setPinState(state);

        return 0;
    }

    return 1;
}

int ThreeLightToggle::getPinNumberAtIndex(int index) {
    if(0 == index) {
        return m_switchPin;
    }

    return -1;
}

void ThreeLightToggle::setCurrentTime(unsigned long currentTime)
{
    m_currentTime = currentTime;
    m_switch->setCurrentTime(currentTime);
}

void ThreeLightToggle::setLEDState(int ledIndex, int state)
{
    //  In the case of a "TURN ON PIN"
    if(state) {
        if(! m_pinArrayState[ledIndex]) {
            digitalWrite(m_pinArray[ledIndex], HIGH);
            m_pinArrayState[ledIndex] = 1;
        }
    }
    else {  // In the case of a "TURN OFF PIN"
        if(m_pinArrayState[ledIndex]) {
            digitalWrite(m_pinArray[ledIndex], LOW);
            m_pinArrayState[ledIndex] = 0;
        }
    }
}
    


void ThreeLightToggle::invalid_step()
{
    // If the toggle is OFF, turn off the LEDs, reset the system to waiting, and return
    // If this stays here, it means that an external source can't keep this in INVALID mode
    // when the switch is off....

    // If the toggle is ON, continue with a blink
    // We reuse the 'toggleOnTimestamp' here
    if(0 == m_errorModeLightTimestamp) {
        m_errorModeLightTimestamp = this->m_currentTime;
        return;
    }

    unsigned long volatile timeDelaySinceStateChange = this->m_currentTime - m_errorModeLightTimestamp;
    // If the time delay has exceeded our delay, switch the state of the LEDS
    // Reset the 'toggleOnTimestamp' to reset delay timer
    if( timeDelaySinceStateChange > s_blinkDelay) {
        int stateToSet = m_pinArrayState[0] ? LOW : HIGH;
        setLEDState(0, stateToSet);
        setLEDState(1, stateToSet);
        setLEDState(2, stateToSet);
        m_errorModeLightTimestamp = this->m_currentTime;
    }
}

void ThreeLightToggle::resetToWaitingMode()
{
    setLEDState(0, 0);
    setLEDState(1, 0);
    setLEDState(2, 0);
    m_errorModeLightTimestamp = 0;
    m_switchOnTimestamp = 0;
    m_componentState = ToggleMode::WAITING_MODE;
    this->m_recentStateChange = true;
}


//  This state is for when the LEDs are completely lit.  Should be fairly quiescent
void ThreeLightToggle::active_mode_step()
{
    if(!m_switchPinState) {
        resetToWaitingMode();
    }
    return;
}

void ThreeLightToggle::processing_mode_step()
{
    if(0 == m_switchOnTimestamp) {
        m_switchOnTimestamp = m_currentTime;
        return;
    }

    // Calculate time between now and when the toggle switch was turned on.
    // Turn on LEDs depending on how far from that time we are
    unsigned long volatile timeDelaySinceOn = m_currentTime - m_switchOnTimestamp;

    if( timeDelaySinceOn > m_millisecDelay && 
        timeDelaySinceOn < (2*m_millisecDelay) &&
        LOW == m_pinArrayState[0]) {
        setLEDState(0, 1);
        m_recentStateChange = true;
    }
    if( timeDelaySinceOn > (2*m_millisecDelay) &&
        LOW == m_pinArrayState[1]) {
        setLEDState(1, 1);
        m_recentStateChange = true;
    }
    if( timeDelaySinceOn > (3*m_millisecDelay) &&
        LOW == m_pinArrayState[2]) {
        setLEDState(2, 1);
        m_componentState = ToggleMode::ACTIVE_MODE;  // Only move to active when the last LED has turned on
        m_recentStateChange = true;
    }

}

void ThreeLightToggle::waiting_mode_step()
{
    // This is really checking if we're transitioning to a waiting mode state
    // (If the switch is off, and there is a value in the timestamp, that means 
    // reset to waiting mode was never called)
    if(!m_switchPinState &&
        m_switchOnTimestamp) {
            resetToWaitingMode();
    }
}



void ThreeLightToggle::step(JsonObject &json)
{
    //  The current state of the pin is set externally by the component manager, so by the time 
    //  we're here, m_switchPinState is up to date

    // TODO: Json Object needs to be populated
    m_switch->step(json);

    //  Establish what state we're in, and call the corresponding step function
    
    if( m_componentState == ToggleMode::INVALID_MODE) {
        //  Nothing to do here.  This state is set externally, and so we 
        //  don't have to check our switch status
    }
    else if( m_switchPinState && ! m_previousSwitchPinState) {  // transitioning from OFF to ON
        m_componentState = ToggleMode::PROCESSING_MODE;
    } else if( !m_switchPinState && m_previousSwitchPinState) {  // transitioning from ON to OFF
        m_componentState = ToggleMode::WAITING_MODE;
    } else if(m_switchPinState && m_previousSwitchPinState) { //  Switch ON, no change
        
        // If all the LEDs are on, we're in active mode, else in processing mode
        if( m_pinArrayState[2]) {
            m_componentState = ToggleMode::ACTIVE_MODE;
        } else { m_componentState = ToggleMode::PROCESSING_MODE; }

    } else if( ! m_switchPinState && ! m_previousSwitchPinState) { //  Switch OFF, no change
        m_componentState = ToggleMode::WAITING_MODE;
    }

    switch(m_componentState)
    {
        case ToggleMode::INVALID_MODE:
        {
            invalid_step();
            break;
        }
        case ToggleMode::ACTIVE_MODE:
        {
            active_mode_step();
            break;
        }
        case ToggleMode::PROCESSING_MODE:
        {
            processing_mode_step();
            break;
        }
        case ToggleMode::RESET_FROM_ERROR_MODE:
        case ToggleMode::WAITING_MODE:
        default:
        {
            waiting_mode_step();
            break;
        }
    }
    
    if( m_previousSwitchPinState != m_switchPinState) {
        m_previousSwitchPinState = m_switchPinState;
    }
}




}