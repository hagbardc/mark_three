#ifndef INCLUDED_COMPONENT_THREE_LIGHT_TOGGLE_H
#define INCLUDED_COMPONENT_THREE_LIGHT_TOGGLE_H

#include <component_base.h>
#include <component_two_pole_switch.h>

#include <ArduinoJson.h>


namespace component {

class ThreeLightToggle : public component::ComponentBase
{

public:

    //  Enumeration describing the state of this component
    typedef enum ToggleMode
    {
        RESET_FROM_ERROR_MODE,  // Switch was turned off, but was previously in an error state
        WAITING_MODE,  // Switch is not in ON state, external call has not set to INVALID
        INVALID_MODE,  // External call has set this instance to invalid - needs to be reset externally
        PROCESSING_MODE, // For when switch has been properly turned on, but LEDs are not completely lit
        ACTIVE_MODE    // Switch has been activated properly (ON state)
    } ToggleMode_t;


    /*
    *  @brief Initialize the component.  
    * 
    * This component is a composition of four separate items:  3 leds, and one switch
    * 
    * @param switch_pin : The pin number of the incoming signal (from the switch)
    * @param pinA : Output pin for one of the LEDs
    * @param pinB : Output pin for one of the LEDs    
    * @param pinC : Output pin for one of the LEDs
    * @param millisecDelay : Time between the LEDs turning on during an activation sequence 
    */
    ThreeLightToggle(int switch_pin,
                     int pinA, int pinB, int pinC,
                     unsigned long millisecDelay = 500);
    ~ThreeLightToggle();


    /*  State publication for this will be as follows:
    *  <ToggleModeAsString>:<Last Lit LED>
    * 
    * So if we are in WAITING_MODE, state will be:  "WAITING:0"
    * If we are two LEDs into the PROCESSING_MODE, state will be: "ACTIVATING:2"
    * If we are in ERROR_MODE, state will be either "ERROR:0" or "ERROR:3" depending on state of flashing
    */

    virtual int getStateChange(JsonObject &jsonState);

    virtual void getCurrentState(JsonObject &jsonState);

    virtual void step(JsonObject &json);

    // We don't implement this, since it's not coherent for the Potentiometer class
    //virtual int setPinStateAtIndex(int index, uint8_t state);

    virtual void setPinState(uint8_t state);
    virtual int setPinStateAtIndex(int index, uint8_t state);

    virtual int getNumberOfPins() { return 1; }

    virtual int getPinNumberAtIndex(int index);

    // Need to override this so that we can update the value of the 
    // local switch as well
    virtual void setCurrentTime(unsigned long currentTime);


private:

    // Converts the passed in toggleMode value to a string
    static void toggleModeAsString(ToggleMode toggleMode, char* toggleModeAsString);

    static unsigned long s_blinkDelay; // Length of time for a LED blink

    //  Looks at the current state of the system, and states the local state string appropriately
    void setStringState();

    //  Progresses the action forward one step, depending on the state
    void invalid_step();
    void waiting_mode_step();
    void reset_from_error_mode_step();
    void active_mode_step();
    void processing_mode_step();

    //  Turns on/off the led at ledIndex, and updates local state member variable
    void setLEDState(int ledIndex, int state);

    //  Called when in active mode and the switch has been turned off.  Resets the local variables
    //  and sets the LED states accordingly
    void resetToWaitingMode();


    TwoPoleSwitch *m_switch;
    int m_switchPin;
    uint8_t m_switchPinState;

    // This is the state of the switch the last time .step() executed
    uint8_t m_previousSwitchPinState;

    int m_pinArray[3];
    int m_pinArrayState[3];  //  The current state of the pins

    // Current state of the component
    ToggleMode m_componentState;
    char m_componentStateAsString[30];

    // The last time the LEDs were flipped on (in error mode).  Set to 0 if the we are not in error mode is off
    unsigned long m_errorModeLightTimestamp;

    //  The time at which the switch was turned on.  Set to 0 when the switch is turned off
    unsigned long m_switchOnTimestamp;

    unsigned long m_millisecDelay; // Number of ms between light delays


};

}

#endif