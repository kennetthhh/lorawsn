/*****************************************************************/
/******************* BMP180 Sensor Wrapper ***********************/
/*****************************************************************/

/*** Pressure & Temperature Sensor BMP180
Wiring connections:
BMP180  Heltec WiFi LoRa 32
VIN     3.3V
GND     GND
SCL     22
SDA     21
***/

#ifndef TEMP_PRES_SENSOR_WRAPPER_HPP
#define TEMP_PRES_SENSOR_WRAPPER_HPP

#include <Arduino.h>
#include <stdint.h>
#include <Adafruit_BMP085.h>
#include "../../Globals/src/Globals.hpp"

namespace TempPresSensor
{
    // Constants Definitions
    static constexpr gpio_num_t I2C_SDA = GPIO_NUM_21;
    static constexpr gpio_num_t I2C_SCL = GPIO_NUM_22;
    static constexpr int SEALEVELPRESSURE_HPA = 1013.25;

// FlameSensorWrapper Class
class TempPresSensorWrapper
{
    private:
    double _tempValue;
    double _pressureValue;

    public:
    // Constructor
    TempPresSensorWrapper() : _tempValue(0), _pressureValue(0) {}

    // Initialisation
    bool initTempPresSensor(Adafruit_BMP085& _bmp, TwoWire& Wire1);

    // Read Temperature and Pressure
    double readTemperature(Adafruit_BMP085& bmp);
    double readPressure(Adafruit_BMP085& bmp);

}; // class TempPresSensorWrapper

} // namespace TempPresSensor

#endif // TEMP_PRES_SENSOR_WRAPPER_HPP