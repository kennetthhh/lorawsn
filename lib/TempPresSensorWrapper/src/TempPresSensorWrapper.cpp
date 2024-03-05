#include "TempPresSensorWrapper.hpp"

namespace TempPresSensor
{
    bool TempPresSensorWrapper::initTempPresSensor(Adafruit_BMP085& bmp, TwoWire& Wire1)
    {
        bool valid_init = false;

        // Set the pin mode to input
        bool wireStatus = Wire1.begin(I2C_SDA, I2C_SCL);
        if (!wireStatus)
        {
            Serial.println("Wire1 failed to begin");
        }

        bool bmpStatus = bmp.begin(0x77, &Wire1);
        if (!bmpStatus)
        {
            Serial.println("BMP180 sensor failed to begin");
        }

        if (wireStatus && bmpStatus)
        {
            valid_init = true;
        }

        return valid_init;
    }

    double TempPresSensorWrapper::readTemperature(Adafruit_BMP085& bmp)
    {
        _tempValue = bmp.readTemperature();
        return _tempValue;
    }

    double TempPresSensorWrapper::readPressure(Adafruit_BMP085& bmp)
    {
        _pressureValue = bmp.readPressure();
        return _pressureValue;
    }

} // namespace TempPresSensor