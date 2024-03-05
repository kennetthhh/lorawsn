#include "ThresholdsHandler.hpp"

namespace ThresholdsHandler
{
    bool ThresholdsHandler::setFlameThreshold(long flameThreshold)
    {
        bool valid_flameThreshold = false;

        if (flameThreshold >= 0 && flameThreshold <= 4095)
        {
            _thresholds.flameThreshold = flameThreshold;
            valid_flameThreshold = true;
        }

        return valid_flameThreshold;
    }

    bool ThresholdsHandler::setGasThreshold(long gasThreshold)
    {
        bool valid_gasThreshold = false;

        if (gasThreshold >= 0 && gasThreshold <= 4095)
        {
            _thresholds.gasThreshold = gasThreshold;
            valid_gasThreshold = true;
        }

        return valid_gasThreshold;
    }

    bool ThresholdsHandler::setTempThreshold(long tempThreshold)
    {
        bool valid_tempThreshold = false;

        if (tempThreshold >= 0 && tempThreshold <= 200)
        {
            _thresholds.tempThreshold = tempThreshold;
            valid_tempThreshold = true;
        }

        return valid_tempThreshold;
    }

    long ThresholdsHandler::getFlameThreshold()
    {
        return _thresholds.flameThreshold;
    }

    long ThresholdsHandler::getGasThreshold()
    {
        return _thresholds.gasThreshold;
    }

    long ThresholdsHandler::getTempThreshold()
    {
        return _thresholds.tempThreshold;
    }

} // namespace ThresholdsHandler