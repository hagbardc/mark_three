#ifndef COMPONENT_BUTTON_PAD_H
#define COMPONENT_BUTTON_PAD_H

#include <component_base.h>

#include <ArduinoJson.h>


//config variables
#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)
#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)
#define NUM_COLORS (3)

#define MAX_DEBOUNCE (3)


namespace component {

class ButtonPad : public component::ComponentBase
{

public:
    ButtonPad();
    ~ButtonPad() {};


    virtual int getStateChange(JsonObject &jsonState) { return 0;};

    virtual void getCurrentState(JsonObject &jsonState) {};

    virtual void step(JsonObject &json);

    //virtual void setPinState(uint8_t state);

    virtual int setPinStateAtIndex(int index, uint8_t state) { return 0;};

    virtual int getNumberOfPins() { return 0;};

    virtual int getPinNumberAtIndex(int index) { return 0;};

private:

    //  Setup input/output mode of pins
    void setupPins();

    uint8_t m_LED_outputs[NUM_LED_COLUMNS][NUM_LED_ROWS];
    unsigned long m_next_scan;

    uint8_t m_btnselpins[4];
    uint8_t m_btnreadpins[4];
    uint8_t m_ledselpins[4];
    uint8_t m_colorpins[4][3];

    int8_t m_debounce_count[NUM_BTN_COLUMNS][NUM_BTN_ROWS];

};

}

#endif 