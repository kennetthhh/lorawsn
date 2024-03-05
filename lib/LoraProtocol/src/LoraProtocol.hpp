/*****************************************************************/
/********************** LoraProtocol *****************************/
/*****************************************************************/
#ifndef LORAPROTOCOL_HPP
#define LORAPROTOCOL_HPP

#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include "../../LoRaWrapper/src/LoRaWrapper.hpp"
#include "../../AutoNetworkingProtocol/src/AutoNetworkingProtocol.hpp"
#include "../../ChecksumProtocol/src/ChecksumProtocol.hpp"  
#include "../../Globals/src/Globals.hpp"
#include "../../ThresholdsHandler/src/ThresholdsHandler.hpp"
#include "../../SleepHandler/src/SleepHandler.hpp"

namespace LoraProtocol
{
    // Constants Definitions
    static constexpr uint8_t TWO_DECIMAL_FACTOR = 100;

// LoraProtocol Class
class LoraProtocol
{
    public:

    // Heartbeat timeout duration in milliseconds (default: 1/2minute)
    int heartbeatTimeout = 30000;

    // Lora Message Structure
    #pragma pack(1)
    struct Msg_Header 
    {
    uint8_t  addresses;
    uint16_t packetCounter;
    };

    struct Payload
    {
    uint16_t temperature;
    uint16_t flameValue;
    uint8_t  states;
    uint16_t gasValue;
    uint8_t batteryValue;
    };

    struct LoRaMessage
    {
    uint8_t msg_type;
    Msg_Header header;
    Payload payload;
    uint8_t checksum;
    };
    #pragma pack()

    // Election Result Structure
    struct ElectionResult
    {
        AutoNetwProc::AutoNetworkingProtocol::Roles role;
        uint8_t newDestID;
    };

    // States for the Lora State Machine
    enum class LoraStates
    {
        IDLE,
        WAITING_FOR_ACK,
        REELECTION
    };

    // Constructor
    LoraProtocol() = default;

    // Public Methods
    void LoraStateMachine();
    void processMessage(LoRaMessage &msg);
    void processReception(LoRaWrapper::LoRaWrapper &loraWrapper, LoRaMessage &msg, 
                            uint8_t &sender_id, uint8_t &dest_id, 
                            AutoNetwProc::AutoNetworkingProtocol::Roles &role,
                            int send_ack_timeout = 1000);
    void forwardMessage(LoRaWrapper::LoRaWrapper &loraWrapper, LoRaMessage &msg, 
                        uint8_t &sender_id, uint8_t &dest_id);
    long processTransmission(LoRaWrapper::LoRaWrapper &loraWrapper, 
                            LoRaMessage &loraMessage,
                            uint8_t &sourceAddress,
                            uint8_t &destinationAddress,
                            uint8_t batteryValue,
                            double &temperature,
                            int &flameValue,
                            bool &flameState,
                            int &gasValue,
                            bool &gasState,
                            AutoNetwProc::AutoNetworkingProtocol::Roles &myRole,
                            bool OLED_DISPLAY,
                            gpio_num_t LED_ONBOARD,
                            int receive_ack_timeout = 1000);
    void sendACK(LoRaWrapper::LoRaWrapper &loraWrapper, uint8_t &sender_id);
    bool checkACK(LoRaWrapper::LoRaWrapper &loraWrapper, uint8_t &dest_id);
    ElectionResult triggerReelection();
    
    // Misc Methods
    void packNibblesIntoByte(uint8_t nibble1, uint8_t nibble2, uint8_t &resultByte);
    void unpackByteIntoNibbles(uint8_t packedByte, uint8_t &nibble1, uint8_t &nibble2);
    void packTwoBooleansIntoByte(bool bool1, bool bool2, uint8_t &resultByte);
    void unpackByteIntoTwoBooleans(uint8_t packedByte, bool &bool1, bool &bool2);
    void serializeMsg(LoRaMessage &msg, uint8_t *buffer);
    void deserializeMsg(uint8_t *buffer, LoRaMessage &msg);
    void processExtCommands(uint8_t &SENSOR_ID, ThresholdsHandler::ThresholdsHandler &th,
                                        SleepHandler::SleepHandler &sh);

}; // class LoraProtocol

} // namespace LoraProtocol

#endif // LORAPROTOCOL_HPP