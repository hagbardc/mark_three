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


    // LED Rockers
    componentManager->addComponent(new ThreePoleRocker(22), 22);
    componentManager->addComponent(new ThreePoleRocker(23), 23);
    componentManager->addComponent(new ThreePoleRocker(24), 24);
    componentManager->addComponent(new ThreePoleRocker(25), 25);
    componentManager->addComponent(new ThreePoleRocker(26), 26);
    componentManager->addComponent(new ThreePoleRocker(27), 27);
    componentManager->addComponent(new ThreePoleRocker(28), 28);
    componentManager->addComponent(new ThreePoleRocker(29), 29);

    // The two extra ones on the left side, near the joystick
    componentManager->addComponent(new ThreePoleRocker(8), 8);
    componentManager->addComponent(new ThreePoleRocker(9), 9);

    //  Black rectangular toggles
    componentManager->addComponent(new TwoPoleSwitch(30, INPUT_PULLUP), 30);
    componentManager->addComponent(new TwoPoleSwitch(31, INPUT_PULLUP), 31);

    // 5-pole LED Momentary push buttons
    componentManager->addComponent(new TwoPoleSwitch(32, INPUT_PULLUP), 32);
    componentManager->addComponent(new TwoPoleSwitch(33, INPUT_PULLUP), 33);
    componentManager->addComponent(new TwoPoleSwitch(34, INPUT_PULLUP), 34);
    componentManager->addComponent(new TwoPoleSwitch(35, INPUT_PULLUP), 35);
    componentManager->addComponent(new TwoPoleSwitch(36, INPUT_PULLUP), 36);
    componentManager->addComponent(new TwoPoleSwitch(37, INPUT_PULLUP), 37);
    componentManager->addComponent(new TwoPoleSwitch(38, INPUT_PULLUP), 38);
    componentManager->addComponent(new TwoPoleSwitch(39, INPUT_PULLUP), 39);
    componentManager->addComponent(new TwoPoleSwitch(40, INPUT_PULLUP), 40);
    componentManager->addComponent(new TwoPoleSwitch(41, INPUT_PULLUP), 41);



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
