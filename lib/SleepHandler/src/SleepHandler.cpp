#include "SleepHandler.hpp"

namespace SleepHandler
{
    bool SleepHandler::setSleepTime(int sleepTime)
    {
        bool valid_sleepTime = false;

        if (sleepTime > 0)
        {
            this->sleepTime = sleepTime;
            valid_sleepTime = true;
        }

        return valid_sleepTime;
    }

    bool SleepHandler::setDutyCycle(int dutyCycle)
    {
        bool valid_dutyCycle = false;

        if (dutyCycle > 0)
        {
            this->dutyCycle = dutyCycle;
            valid_dutyCycle = true;
        }

        return valid_dutyCycle;
    }

    int SleepHandler::getSleepTime()
    {
        return this->sleepTime;
    }

    int SleepHandler::getDutyCycle()
    {
        return this->dutyCycle;
    }

    void SleepHandler::goToSleep(bool SENSORS_CONNECTED, const long long BITMASK_GAS, const gpio_num_t DO_PIN_FLAME)
    {
        // Configure the wake up source, set ESP32 to wake up every x seconds
        esp_sleep_enable_timer_wakeup(sleepTime * uS_TO_S_FACTOR);

        if(SENSORS_CONNECTED) {
            esp_sleep_enable_ext0_wakeup(DO_PIN_FLAME,0);                       // set ESP32 to wake up if pin is 0 = LOW, 1 = HIGH
            esp_sleep_enable_ext1_wakeup(BITMASK_GAS,ESP_EXT1_WAKEUP_ALL_LOW);  // set ESP32 to wake up if pin is LOW
        }

        // Go to sleep
        Serial.println("Going to sleep now");
        Serial.println();
        Serial.flush();
        delay(2);
        esp_deep_sleep_start();
        Serial.println("This will never be printed");
    }

    void SleepHandler::print_wakeup_reason() 
    {
        esp_sleep_wakeup_cause_t wakeup_reason;

        wakeup_reason = esp_sleep_get_wakeup_cause();

        switch(wakeup_reason)
        {
            case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
            case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
            case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
            case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
            case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
            default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
        }
    }

} // namespace SleepHandler