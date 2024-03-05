/*****************************************************************/
/************************ SleepHandler ***************************/
/*****************************************************************/
#ifndef SLEEPHANDLER_HPP
#define SLEEPHANDLER_HPP

#include <Arduino.h>
#include <stdint.h>
#include <esp_sleep.h>
#include "../../TempPresSensorWrapper/src/TempPresSensorWrapper.hpp"

namespace SleepHandler
{
    // Constants Definitions
    static constexpr int uS_TO_S_FACTOR = 1000000; // Conversion factor for micro seconds to seconds

// SleepHandler Class
class SleepHandler
{
    private:
    int sleepTime;
    int dutyCycle;

    public:
    // Constructor
    SleepHandler() {}
    SleepHandler(int sleepTime, int dutyCycle) : sleepTime(sleepTime), dutyCycle(dutyCycle) {}

    // Setters
    bool setSleepTime(int sleepTime);
    bool setDutyCycle(int dutyCycle);

    // Getters
    int getSleepTime();
    int getDutyCycle();

    // Sleep
    void goToSleep(bool SENSORS_CONNECTED, const long long BITMASK_GAS, const gpio_num_t DO_PIN_FLAME);
    void print_wakeup_reason();

}; // class SleepHandler

} // namespace SleepHandler

#endif // SLEEPHANDLER_HPP