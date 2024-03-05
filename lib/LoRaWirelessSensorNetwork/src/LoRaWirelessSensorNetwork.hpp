#ifndef LoRaWirelessSensorNetwork_HPP
#define LoRaWirelessSensorNetwork_HPP

#include <Arduino.h>
#include <Wire.h>
#include <LoRa.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../../Globals/src/Globals.hpp"
#include "../../AutoNetworkingProtocol/src/AutoNetworkingProtocol.hpp"
#include "../../UserInitConfig/src/UserInitConfig.hpp"
#include "../../ThresholdsHandler/src/ThresholdsHandler.hpp"
#include "../../SleepHandler/src/SleepHandler.hpp"
#include "../../TempPresSensorWrapper/src/TempPresSensorWrapper.hpp"
#include "../../FlameSensorWrapper/src/FlameSensorWrapper.hpp"
#include "../../GasSensorWrapper/src/GasSensorWrapper.hpp"
#include "../../LoRaWrapper/src/LoRaWrapper.hpp"
#include "../../OledDisplayWrapper/src/OledDisplayWrapper.hpp"
#include "../../LoraProtocol/src/LoraProtocol.hpp"

class LoRaWirelessSensorNetwork
{
    public:
    LoRaWirelessSensorNetwork() 
    {
        // Initialize the Serial Baudrate
        Serial.begin(115200);
    }
    void initNode();
    void processCyclic();
};


#endif
