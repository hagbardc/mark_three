#include <component_pot_readout.h>

#include <Arduino.h>


namespace component {

int PotReadout::NUMBER_OF_SAMPLES = 10;

PotReadout::PotReadout(int pinNumber, int readoutAddress) :
    m_pinNumber(pinNumber)
    , m_minimumIncrement(11)
    , m_readoutAddress(readoutAddress)
{
    m_displayMatrix.begin(m_readoutAddress);
    m_displayMatrix.print(0, DEC);
    m_displayMatrix.writeDisplay();

    m_sampleArray = new int[PotReadout::NUMBER_OF_SAMPLES];

}

PotReadout::~PotReadout() {}


void PotReadout::step(JsonObject &json)
{
    //  If there are performance problems, we should look into reading this
    //  value as part of the array-reads.
    //

    // TODO: If this is proving slow, we should do one read per step, and keep an 
    //       internal array over which to do our average (or median)
    int val = 0;
    for( int ii=0; ii<NUMBER_OF_SAMPLES; ++ii) {
        val += (analogRead(m_pinNumber)/100)*100;
    }
    int analogIntVal = val / NUMBER_OF_SAMPLES;

    //int analogIntValReduced = (analogIntVal) * 10;
    this->m_currentState = analogIntVal;

/*
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
*/

    if(abs(this->m_currentState - this->m_oldState) < this->m_minimumIncrement) {
        this->m_recentStateChange = false;
        return;
    }

    this->m_recentStateChange = true;

    m_oldState = this->m_currentState;

    if((m_currentTime - m_lastUpdateTime) > 250) {
        m_displayMatrix.print(m_currentState, DEC);
        m_displayMatrix.writeDisplay();
        m_lastUpdateTime = m_currentTime;
    }

}

int PotReadout::getStateChange(JsonObject &jsonState)
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

void PotReadout::getCurrentState(JsonObject &jsonState)
{
    jsonState["state"] = m_currentState;
}

int PotReadout::getPinNumberAtIndex(int index)
{
    if(index != 0) {
        return -1;
    }
    return m_pinNumber;
}




}  // end namespace
