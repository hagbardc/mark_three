#ifndef INCLUDED_COMPONENT_BASE_H
#define INCLUDED_COMPONENT_BASE_H

#include <ArduinoJson.h>

namespace component {

class ComponentBase {
public:
    ComponentBase();
    virtual ~ComponentBase() {}

    /*
     * If the component has experienced a state change since the last time
     * the state was queried, populate the json object
     *
     * @param jsonState:  ArduinoJson json object that contains only information about the changed states
     * @return 0 if there was no state change since the last query, 1 if there was such a change
    */
    virtual int getStateChange(JsonObject &jsonState) = 0;

    /*
    * Populates the JSON object with the current state of the component, regardless
    * of whether there has been a state change or not
    */
    virtual void getCurrentState(JsonObject &jsonState) = 0;

    /*
    * Our 'tick' function - representing a cycle of the microcontroller
    *
    * @param json:  A JsonBuffer which can be used for emission of a state change to Serial
    */
    virtual void step(JsonObject &json) = 0;

    //  To be used by those components which can have a simple (binary) state passed in externally
    virtual void setPinState(uint8_t state) {};

    virtual int setPinStateAtIndex(int index, uint8_t state) { return 0; }

    //  Returns the number of input pins supported by this component type
    //  Used to iterate over the pins to set the pin states
    virtual int getNumberOfPins() = 0;

    // Returns pin number at the given index.  We allow our components to require
    // multiple input pins which are not consecutive.  This will return pin number
    // (pin on the microcontroller) for the Nth index
    //
    // Returns -1 if an index is requested out of bounds for this component

    virtual int getPinNumberAtIndex(int index) = 0;

    void setComponentName( const char *name);

protected:
    /*
     * Populates the json object with information for a state change
     * Assumes that createObject() had already been called on json
     * Used by each of the components as a helper function to interact with the
     * JsonObject library
     *
     * @param json:  ArduinoJson json which will be populated with the change info
     * @param element:  Name of the changing element
     * @param action:  Name of the event that occurred
     * @param value:  Any value associated with that event
    */
    void populateStateChange(
        JsonObject &json, const char* element,
        const char *action, const char *value);

    bool m_recentStateChange;
    char m_componentName[20];  // Name of the component
};

} // end namespace Component
#endif
