#include <component_button_pad.h>

#include <Arduino.h>


// Global variables 
/*
static uint8_t LED_outputs[NUM_LED_COLUMNS][NUM_LED_ROWS];
static int32_t next_scan;

static const uint8_t btnselpins[4]   = {3, 2, 4, 5};
static const uint8_t btnreadpins[4] = {34,36,38,40};
static const uint8_t ledselpins[4]   = {35,37,39,41};

// RGB pins for each of 4 rows 
static const uint8_t colorpins[4][3] = {
  {23,30,26}, 
  {22,31,27},
  {24,32,28},
  {25,33,29}};


static int8_t debounce_count[NUM_BTN_COLUMNS][NUM_BTN_ROWS];
*/

namespace component {

ButtonPad::ButtonPad()
:   ComponentBase()
{
    m_btnselpins[0] = 3;  m_btnselpins[1] = 2; m_btnselpins[2] = 4; m_btnselpins[3] = 5;
    m_btnreadpins[0] = 34;  m_btnreadpins[1] = 36; m_btnreadpins[2] = 38; m_btnreadpins[3] = 40;
    m_ledselpins[0] = 35;  m_ledselpins[1] = 37; m_ledselpins[2] = 39; m_ledselpins[3] = 41;

    m_colorpins[0][0] = 23; m_colorpins[0][1] = 30; m_colorpins[0][2] = 26;
    m_colorpins[1][0] = 22; m_colorpins[1][1] = 31; m_colorpins[1][2] = 27;
    m_colorpins[2][0] = 24; m_colorpins[2][1] = 32; m_colorpins[2][2] = 28;
    m_colorpins[3][0] = 25; m_colorpins[3][1] = 33; m_colorpins[3][2] = 29;

    setupPins();
    for(uint8_t i = 0; i < NUM_LED_ROWS; i++)
    {
        for(uint8_t j = 0; j < NUM_LED_COLUMNS; j++)
        {
            m_LED_outputs[i][j] = 0;
        }
    }



    m_next_scan = millis() + 1;
}

void ButtonPad::setupPins()
{
    uint8_t i;

    // initialize
    // select lines
    for(i = 0; i < NUM_LED_COLUMNS; i++)
    {
        pinMode(m_ledselpins[i], OUTPUT);

        // with nothing selected by default
        digitalWrite(m_ledselpins[i], HIGH);
    }

    for(i = 0; i < NUM_BTN_COLUMNS; i++)
    {
        pinMode(m_btnselpins[i], OUTPUT);

        // with nothing selected by default
        digitalWrite(m_btnselpins[i], HIGH);
    }

    // key return lines
    for(i = 0; i < 4; i++)
    {
        pinMode(m_btnreadpins[i], INPUT_PULLUP);
    }

    // LED drive lines
    for(i = 0; i < NUM_LED_ROWS; i++)
    {
        for(uint8_t j = 0; j < NUM_COLORS; j++)
        {
            pinMode(m_colorpins[i][j], OUTPUT);
            digitalWrite(m_colorpins[i][j], LOW);
        }
    }

    for(uint8_t i = 0; i < NUM_BTN_COLUMNS; i++)
    {
        for(uint8_t j = 0; j < NUM_BTN_ROWS; j++)
        {
            m_debounce_count[i][j] = 0;
        }
    }
}

void ButtonPad::step(JsonObject &json)
{
/*
    for( int ii=0; ii<4; ii++) {
        Serial.print(digitalRead(m_btnreadpins[ii]));
        Serial.print(" ");
    }
    Serial.println("");
    return;
  */  




    //  If the current time is less than our next expected scan time
    //  don't do anything
    if(m_currentTime < m_next_scan) {
        return;
    }

    m_next_scan = m_currentTime + 1;
    
    static uint8_t current = 0;
    uint8_t val;
    uint8_t i, j;

    //run
    digitalWrite(m_btnselpins[current], LOW);
    digitalWrite(m_ledselpins[current], LOW);

    for(i = 0; i < NUM_LED_ROWS; i++)
    {
        uint8_t val = (m_LED_outputs[current][i] & 0x03);

        if(val)
        {
            digitalWrite(m_colorpins[i][val-1], HIGH);
        }
    }


    delay(1);

    for( j = 0; j < NUM_BTN_ROWS; j++)
    {
        val = digitalRead(m_btnreadpins[j]);

        if(val == LOW)
        {
            // active low: val is low when btn is pressed
            if( m_debounce_count[current][j] < MAX_DEBOUNCE)
            {
                m_debounce_count[current][j]++;
                if( m_debounce_count[current][j] == MAX_DEBOUNCE )
                {
                    Serial.print("Key Down ");
                    Serial.println((current * NUM_BTN_ROWS) + j);

                    m_LED_outputs[current][j]++;
                }
            }
        }
        else
        {
            // otherwise, button is released
            if( m_debounce_count[current][j] > 0)
            {
            m_debounce_count[current][j]--;
            if( m_debounce_count[current][j] == 0 )
            {
                Serial.print("Key Up ");
                Serial.println((current * NUM_BTN_ROWS) + j);
            }
            }
        }
    }// for j = 0 to 3;

    delay(1);

    digitalWrite(m_btnselpins[current], HIGH);
    digitalWrite(m_ledselpins[current], HIGH);

    for(i = 0; i < NUM_LED_ROWS; i++)
    {
        for(j = 0; j < NUM_COLORS; j++)
        {
            digitalWrite(m_colorpins[i][j], LOW);
        }
    }

    current++;
    if (current >= NUM_BTN_COLUMNS)
    {
        current = 0;
    }

}


}