#include <Arduino.h>

#include <component_base.h>
#include <component_switch.h>

#include <controller_access_base.h>
#include <controller_access_mega2560.h>

using namespace component;
using namespace controller_access;

ComponentBase **component_array;
ControllerAccessBase *controller;

int numberOfComponents = 1;
int numberOfRegisters = 9;

byte *inputArray;

unsigned long last_execution = millis();
unsigned long last_print = last_execution;



void printPinGroupings()
{
    Serial.print("    |-PINA-| ");
    Serial.print("|-PINB-| ");
    Serial.print("|-PINC-| ");
    Serial.print("|-PIND-| ");
    Serial.print("|-PINE-| ");
    Serial.print("|-PINF-| ");
    Serial.print("|-PING-| ");
    Serial.print("|-PINH-| ");
    Serial.println("|-PINJ-| ");
}

//  Takes in a byte array, and prints out the current state thereof
void serialPrintByteArray(byte *inputArray, const int arraySize)
{
    int checksum(0);

    for(int ii=0; ii<arraySize; ++ii) {
        int byteChecksum(0);
        for(int bitIndex=0; bitIndex<8; ++bitIndex) {
            byte value = (inputArray[ii] >> bitIndex) & 1;
            checksum += value;
            byteChecksum += value;

            Serial.print(value);
        }
        //Serial.print(byteChecksum);
        Serial.print(" ");
    }
    Serial.print(": ");
    Serial.print(checksum);
    Serial.println(' ');

}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.print("Starting Setup...");

    inputArray = new byte[numberOfRegisters];
    //  Allocate memory for component array
    component_array = new ComponentBase*[numberOfComponents];

    //  Allocate each of the components
    component_array[0] = new Switch(10);

    //  Generate a handle to the appropriate microcontroller
    controller = new Mega2560();
}

void loop() {
    unsigned long time_now = millis();

    if(time_now >= (last_print + 1500)) {
        printPinGroupings();
        last_print = time_now;
    }
    if(time_now >= (last_execution + 300)) {

        int count = controller->checksumUsingDigitalRead(54);
        Serial.print(count);
        Serial.print(": ");

        controller->readDigitalPins(&inputArray[0], numberOfRegisters);
        serialPrintByteArray(&inputArray[0], numberOfRegisters);
        last_execution = time_now;


}

}
