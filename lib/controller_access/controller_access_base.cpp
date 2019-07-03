
#include <controller_access_base.h>


namespace controller_access
{


int ControllerAccessBase::checksumUsingDigitalRead(const int numDigitalPins)
{
    int count = 0;

    // We start at 2 here because digital pins 0 and 1 are special flowers
    for( int ii=2; ii<numDigitalPins; ++ii) {
        uint8_t value = digitalRead(ii);
        count += value;
    }

    return count;
}


}
