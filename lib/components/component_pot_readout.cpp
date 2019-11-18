#include <component_pot_readout.h>

#include <Arduino.h>


namespace component {

int PotReadout::NUMBER_OF_SAMPLES = 10;

PotReadout::PotReadout(int pinNumber, int readoutAddress) :
    m_pinNumber(pinNumber)
    , m_minimumIncrement(11)
    , m_readoutAddress(readoutAddress)
    , m_sampleArrayIndex(0)
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

    m_sampleArray[m_sampleArrayIndex] = analogRead(m_pinNumber) / 10;
    m_sampleArrayIndex++;

    //  If we haven't filled up the sample array, just return
    if(m_sampleArrayIndex != NUMBER_OF_SAMPLES) {
        this->m_recentStateChange = false;
        return;
    }


    m_sampleArrayIndex = 0;
    int newValue = 0;
    for( int ii = 0; ii<NUMBER_OF_SAMPLES; ++ii) {
        newValue += m_sampleArray[ii];
    }

    this->m_currentState = newValue;
    if(abs(this->m_currentState - this->m_oldState) < this->m_minimumIncrement) {
        this->m_recentStateChange = false;
        return;
    }

    this->m_recentStateChange = true;
    this->m_oldState = this->m_currentState;

    if((m_currentTime - m_lastUpdateTime) > 200) {
        int toDisplay = m_currentState * 8;
        m_displayMatrix.print(toDisplay, DEC);
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
