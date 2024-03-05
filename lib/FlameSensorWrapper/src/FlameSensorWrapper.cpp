#include "FlameSensorWrapper.hpp"

namespace FlameSensor
{
    bool FlameSensorWrapper::initFlameSensor()
    {
        bool valid_initFlameSensor = false;
        // Set the pin mode to input
        pinMode(DO_PIN_FLAME, INPUT);
        pinMode(AO_PIN_FLAME, INPUT);
        valid_initFlameSensor = true;
        return valid_initFlameSensor;
    }

    int FlameSensorWrapper::readFlameValue()
    {
        _flameValue = analogRead(AO_PIN_FLAME);
        return _flameValue;
    }

    bool FlameSensorWrapper::readFlameState()
    {
        _flameState = digitalRead(DO_PIN_FLAME);
        return _flameState;
    }

} // end of FlameSensor namespace