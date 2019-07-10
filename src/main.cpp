#include <Arduino.h>

#include <component_manager.h>
#include <component_base.h>
#include <component_two_pole_switch.h>
#include <component_three_pole_rocker.h>

#include <controller_access_base.h>
#include <controller_access_mega2560.h>
#include <controller_access_util.h>

using namespace component;
using namespace controller_access;

ComponentManager *componentManager;
ControllerAccessBase *controller;

int numberOfRegisters(0);

byte *inputArray;

unsigned long last_execution = millis();
unsigned long last_print = last_execution;
unsigned long current_time;

int readVal;


// We expect pinStart and pinEnd to be the same for most (single signal)
// components



void setPinStates()
{
    controller->readDigitalPins(inputArray, numberOfRegisters);

    int numberOfComponents = componentManager->getNumberOfComponents();
    for( int componentIndex = 0;
        componentIndex<numberOfComponents;
        ++componentIndex ) {

        component_data *componentData = componentManager->getComponentDataAtIndex(componentIndex);
        int pinNumber = componentData->pinStart;
        int pinState = controller->getBitForPin(inputArray, pinNumber);
        componentData->component->setPinState(pinState);
    }

}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Starting Setup...");



    //  Allocate memory for component array
    componentManager = new ComponentManager();


    //  Generate a handle to the appropriate microcontroller
    controller = new Mega2560();

    inputArray = new byte[controller->getNumberOfRegisters()];

    //  Setup the various components
    componentManager->addComponent(new TwoPoleSwitch(10, INPUT_PULLUP), 10);
    componentManager->addComponent(new TwoPoleSwitch(11, INPUT_PULLUP), 11);
    componentManager->addComponent(new ThreePoleRocker(22), 22);
    componentManager->addComponent(new ThreePoleRocker(23), 23);

}

void loop() {

    setPinStates();

     for(int ii = 0; ii < componentManager->getNumberOfComponents(); ++ii ) {

        // TODO:  evaluate if this is too time consuming for the loop
        DynamicJsonDocument jsonDoc(256);
        JsonObject jsonObject = jsonDoc.to<JsonObject>();

        componentManager->getComponentAtIndex(ii)->step(jsonObject);

        if(componentManager->getComponentAtIndex(ii)->getStateChange(jsonObject)) {
            serializeJson(jsonObject, Serial);
            jsonObject.clear();
            Serial.println("");
        }

        componentManager->getComponentAtIndex(ii)->getCurrentState(jsonObject);
    }


}
