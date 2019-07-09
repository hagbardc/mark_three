#include <Arduino.h>

#include <component_manager.h>
#include <component_base.h>
#include <component_two_pole_switch.h>

#include <controller_access_base.h>
#include <controller_access_mega2560.h>
#include <controller_access_util.h>

using namespace component;
using namespace controller_access;

ComponentManager *componentManager;
ControllerAccessBase *controller;

int numberOfRegisters = 9;

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


    //Serial.println(pinState);
    /*
    component_array[1]->setPinState(controller->getBitForPin(inputArray, 23));

    component_array[2]->setPinState(controller->getBitForPin(inputArray, 24));
    component_array[3]->setPinState(controller->getBitForPin(inputArray, 25));

    component_array[4]->setPinState(controller->getBitForPin(inputArray, 26));
    component_array[5]->setPinState(controller->getBitForPin(inputArray, 27));

    component_array[6]->setPinState(controller->getBitForPin(inputArray, 28));
    component_array[7]->setPinState(controller->getBitForPin(inputArray, 29));

    component_array[8]->setPinState(controller->getBitForPin(inputArray, 7));
    component_array[9]->setPinState(controller->getBitForPin(inputArray, 8));
    */
}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Starting Setup...");


    inputArray = new byte[numberOfRegisters];

    //  Allocate memory for component array
    componentManager = new ComponentManager();


    //  Generate a handle to the appropriate microcontroller
    controller = new Mega2560();

    //  Setup the various components
    componentManager->addComponent(new TwoPoleSwitch(27, INPUT), 27);
    componentManager->addComponent(new TwoPoleSwitch(28, INPUT), 28);
    /*component_array[0] = new TwoPoleSwitch(27);
    component_array[1] = new TwoPoleSwitch(23);
    component_array[2] = new TwoPoleSwitch(24);
    component_array[3] = new TwoPoleSwitch(25);
    component_array[4] = new TwoPoleSwitch(26);
    component_array[5] = new TwoPoleSwitch(27);
    component_array[6] = new TwoPoleSwitch(28);
    component_array[7] = new TwoPoleSwitch(29 );
    component_array[8] = new TwoPoleSwitch(7 );
    component_array[9] = new TwoPoleSwitch(8 );*/



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
