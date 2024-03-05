/*****************************************************************/
/****************** OLED Display Wrapper *************************/
/*****************************************************************/
#ifndef OLED_DISPLAY_WRAPPER_HPP
#define OLED_DISPLAY_WRAPPER_HPP

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../../TempPresSensorWrapper/src/TempPresSensorWrapper.hpp"
#include "../../GasSensorWrapper/src/GasSensorWrapper.hpp"
#include "../../FlameSensorWrapper/src/FlameSensorWrapper.hpp"

namespace OledDisplay
{

// OledDisplayWrapper class for handling OLED Display
class OledDisplayWrapper
{
    private:
    // OLED Display
    Adafruit_SSD1306 _display;

    public:
    // Constructor
    OledDisplayWrapper() : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST) {}
    
    // Destructor
    ~OledDisplayWrapper() {}

    // Initialize OLED Display
    bool initOledDisplay(Adafruit_SSD1306& display, TwoWire& Wire1);

    // Display sensor data
    void displaySensorData(Adafruit_SSD1306& display, 
    double tempValue,
    int gasValue,
    bool gasState,
    int flameValue,
    bool flameState,
    int bootCount,
    int packetCount,
    int role,
    int SENSOR_ID);

}; // end of OledDisplayWrapper class

} // end of OledDisplay namespace

#endif // OLED_DISPLAY_WRAPPER_HPP