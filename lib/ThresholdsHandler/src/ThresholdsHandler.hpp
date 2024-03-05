/*****************************************************************/
/********************** Thresholds Handler ***********************/
/*****************************************************************/
#ifndef THRESHOLDSHANDLER_HPP
#define THRESHOLDSHANDLER_HPP

#include <Arduino.h>
#include <stdint.h>

namespace ThresholdsHandler
{
// ThresholdsHandler Class
class ThresholdsHandler
{
    private:
    typedef struct _Thresholds
    {
        long flameThreshold;
        long gasThreshold;
        long tempThreshold;
    } Thresholds;
    
    Thresholds _thresholds;

    public:
    // Constructor
    ThresholdsHandler() {}
    ThresholdsHandler(long flameThreshold, long gasThreshold, long tempThreshold) : _thresholds{flameThreshold, gasThreshold, tempThreshold} {}

    // Setters
    bool setFlameThreshold(long flameThreshold);
    bool setGasThreshold(long gasThreshold);
    bool setTempThreshold(long tempThreshold);

    // Getters
    long getFlameThreshold();
    long getGasThreshold();
    long getTempThreshold();
    
}; // class ThresholdsHandler

} // namespace ThresholdsHandler

#endif // THRESHOLDSHANDLER_HPP