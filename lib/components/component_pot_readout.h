#ifndef INCLUDED_COMPONENT_POT_READOUT_H
#define INCLUDED_COMPONENT_POT_READOUT_H

#include <component_base.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include <ArduinoJson.h>


namespace component {

class PotReadout : public component::ComponentBase
{

public:

    static int NUMBER_OF_SAMPLES;

    // e.g. PotReadout pr(A0, 0x73)
    PotReadout(int analogPin, int readoutAddress);
    ~PotReadout();


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

    Adafruit_7segment m_displayMatrix;

    //  The least sized step we'll allow for the values
    int m_minimumIncrement;

    unsigned long m_lastUpdateTime;

    int m_readoutAddress;

    int *m_sampleArray;
    int m_sampleArrayIndex;  // How many samples have been read

};



}  // end namespace Component
#endif
