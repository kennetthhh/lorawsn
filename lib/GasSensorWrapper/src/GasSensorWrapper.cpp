#include "GasSensorWrapper.hpp"

namespace GasSensor
{
    bool GasSensorWrapper::initGasSensor()
    {
        bool valid_initGasSensor = false;
        // Set the pin mode to input
        pinMode(DO_PIN_GAS, INPUT);
        pinMode(AO_PIN_GAS, INPUT);
        valid_initGasSensor = true;
        return valid_initGasSensor;
    }

    int GasSensorWrapper::readGasValue()
    {
        _gasValue = analogRead(AO_PIN_GAS);
        return _gasValue;
    }

    bool GasSensorWrapper::readGasState()
    {
        _gasState = digitalRead(DO_PIN_GAS);
        return _gasState;
    }

    long long GasSensorWrapper::getBitmaskGas() const
    {
        return BITMASK_GAS;
    }

} // end of GasSensor namespace
