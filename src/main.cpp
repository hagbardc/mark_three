#include <Arduino.h>

#include <component_manager.h>
#include <component_base.h>
#include <component_two_pole_switch.h>
#include <component_three_pole_rocker.h>
#include <component_three_way_switch.h>

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

    // Iterate over the number of components, and set the pin state read from
    // the microcontroller digital pins
    int numberOfComponents = componentManager->getNumberOfComponents();
    for( int componentIndex = 0;
        componentIndex<numberOfComponents;
        ++componentIndex ) {

        component_data *componentData = componentManager->getComponentDataAtIndex(componentIndex);

        int numberOfPins = componentData->component->getNumberOfPins();
        for(int pinIndex = 0; pinIndex < numberOfPins; ++pinIndex) {
            int pinNumber = componentData->component->getPinNumberAtIndex(pinIndex);
            int pinState = controller->getStateForPin(inputArray, pinNumber);
            componentData->component->setPinStateAtIndex(pinIndex, pinState);
        }



    }

}

void setupButtonPadArduino()
{
    ///  This is for the 6 3-way switches on the right side (by the 2ndary arduino)
    int switchArray[2] = {42, 49};
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));
    switchArray[0] = 43;  switchArray[1] = 45;
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));
    switchArray[0] = 46;  switchArray[1] = 44;
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));
    switchArray[0] = 47;  switchArray[1] = 48;
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));
    switchArray[0] = 50;  switchArray[1] = 52;
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));
    switchArray[0] = 51;  switchArray[1] = 53;
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));

}

void setupCenterArduino()
{
    // LED Rockers
    componentManager->addComponent(new ThreePoleRocker(22));
    componentManager->addComponent(new ThreePoleRocker(23));
    componentManager->addComponent(new ThreePoleRocker(24));
    componentManager->addComponent(new ThreePoleRocker(25));
    componentManager->addComponent(new ThreePoleRocker(26));
    componentManager->addComponent(new ThreePoleRocker(27));
    componentManager->addComponent(new ThreePoleRocker(28));
    componentManager->addComponent(new ThreePoleRocker(29));

    // The two extra ones on the left side, near the joystick
    componentManager->addComponent(new ThreePoleRocker(8));
    componentManager->addComponent(new ThreePoleRocker(9));

    //  Black rectangular toggles
    componentManager->addComponent(new TwoPoleSwitch(30, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(31, INPUT_PULLUP));

    // 5-pole LED Momentary push buttons
    componentManager->addComponent(new TwoPoleSwitch(32, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(33, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(34, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(35, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(36, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(37, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(38, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(39, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(40, INPUT_PULLUP));
    componentManager->addComponent(new TwoPoleSwitch(41, INPUT_PULLUP));

    // Blue three-way switches
    int switchArray[2] = {42, 43};
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));
    switchArray[0] = 44;  switchArray[1] = 45;
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));
    switchArray[0] = 46;  switchArray[1] = 47;
    componentManager->addComponent(new ThreeWaySwitch(switchArray, INPUT_PULLUP));

    // Key Switch
    componentManager->addComponent(new TwoPoleSwitch(48, INPUT_PULLUP));

}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Starting Setup...");



    //  Allocate memory for component array
    componentManager = new ComponentManager(60);


    //  Generate a handle to the appropriate microcontroller
    controller = new Mega2560();

    inputArray = new byte[controller->getNumberOfRegisters()];

    setupCenterArduino();
    //setupButtonPadArduino();
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
