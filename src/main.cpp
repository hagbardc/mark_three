#include <Arduino.h>

#include <component_base.h>
#include <component_two_pole_switch.h>

#include <controller_access_base.h>
#include <controller_access_mega2560.h>
#include <controller_access_util.h>

using namespace component;
using namespace controller_access;

ComponentBase **component_array;
ControllerAccessBase *controller;

int numberOfComponents = 4;
int numberOfRegisters = 9;

byte *inputArray;

unsigned long last_execution = millis();
unsigned long last_print = last_execution;
unsigned long current_time;

int readVal;


void setPinStates()
{
    controller->readDigitalPins(inputArray, numberOfRegisters);

    component_array[0]->setPinState(controller->getBitForPin(inputArray, 8));
    component_array[1]->setPinState(controller->getBitForPin(inputArray, 9));

    component_array[2]->setPinState(controller->getBitForPin(inputArray, 22));
    component_array[3]->setPinState(controller->getBitForPin(inputArray, 23));
    //component_array[4]->setPinState(controller->getBitForPin(inputArray, 24));
    //component_array[5]->setPinState(controller->getBitForPin(inputArray, 25));

}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Starting Setup...");

    inputArray = new byte[numberOfRegisters];

    //  Allocate memory for component array
    component_array = new ComponentBase*[numberOfComponents];


    //  Generate a handle to the appropriate microcontroller
    controller = new Mega2560();

    //  Setup the various components
<<<<<<< HEAD
    component_array[0] = new Switch(7);
    component_array[1] = new Switch(8);
=======
    component_array[0] = new TwoPoleSwitch(8);
    component_array[1] = new TwoPoleSwitch(9);
    component_array[2] = new TwoPoleSwitch(22);
    component_array[3] = new TwoPoleSwitch(23);
    //component_array[4] = new TwoPoleSwitch(24);
    //component_array[5] = new TwoPoleSwitch(25);
>>>>>>> 66a2eb25056f32ff82738980444aed3d90efe775



}

void loop() {

    setPinStates();

    for(int ii = 0; ii < numberOfComponents; ++ii ) {

        // TODO:  evaluate if this is too time consuming for the loop
        DynamicJsonDocument jsonDoc(256);
        JsonObject jsonObject = jsonDoc.to<JsonObject>();

        component_array[ii]->step(jsonObject);

        if(component_array[ii]->getStateChange(jsonObject)) {
            serializeJson(jsonObject, Serial);
            jsonObject.clear();
            Serial.println("");
        }

        component_array[ii]->getCurrentState(jsonObject);
    }



}
