#include <Arduino.h>

#include <component_base.h>
#include <component_switch.h>

#include <controller_access_base.h>
#include <controller_access_mega2560.h>
#include <controller_access_util.h>

using namespace component;
using namespace controller_access;

ComponentBase **component_array;
ControllerAccessBase *controller;

int numberOfComponents = 2;
int numberOfRegisters = 9;

byte *inputArray;

unsigned long last_execution = millis();
unsigned long last_print = last_execution;
unsigned long current_time;

int readVal;


void setPinStates()
{
    controller->readDigitalPins(inputArray, numberOfRegisters);

    component_array[0]->setPinState(controller->getBitForPin(inputArray, 2));
    component_array[1]->setPinState(controller->getBitForPin(inputArray, 3));
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
    component_array[0] = new Switch(2);
    component_array[1] = new Switch(3);



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
