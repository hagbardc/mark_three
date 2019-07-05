#include <controller_access_util.h>


namespace controller_access {

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
        Serial.print(" ");
    }
    Serial.print(": ");
    Serial.print(checksum);
    Serial.println(' ');

}

}
