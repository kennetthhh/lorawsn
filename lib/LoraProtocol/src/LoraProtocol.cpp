#include "LoraProtocol.hpp"

namespace LoraProtocol
{
    // Instantiate the ChecksumProtocol
    ChksumProc::ChecksumProtocol checksumProtocol;

    // Public Methods
    // Serialize message before sending
    void LoraProtocol::serializeMsg(LoRaMessage &loraMessage, uint8_t *loraMessageSerialized)
    {
        memcpy(loraMessageSerialized, &loraMessage, sizeof(LoRaMessage));

        return;
    }

    // Deseralize message after receiving
    void LoraProtocol::deserializeMsg(uint8_t *loraMessageSerialized, LoRaMessage &loraMessage)
    {
        memcpy(&loraMessage, loraMessageSerialized, sizeof(LoRaMessage));

        return;
    }

    void LoraProtocol::packNibblesIntoByte(uint8_t nibble1, uint8_t nibble2, uint8_t &resultByte)
    {
        // Constrain the nibbles to 4 bits
        nibble1 &= 0x0F;
        nibble2 &= 0x0F;

        // Pack the nibbles into a byte
        resultByte = (nibble1 << 4) | nibble2;

        return;
    }

    void LoraProtocol::unpackByteIntoNibbles(uint8_t packedByte, uint8_t &nibble1, uint8_t &nibble2)
    {
        // Unpack the byte into nibbles
        nibble1 = (packedByte >> 4) & 0x0F;
        nibble2 = packedByte & 0x0F;
        
        return;
    }

    void LoraProtocol::packTwoBooleansIntoByte(bool bool1, bool bool2, byte &resultByte)
    {
        // Initialize the result byte
        resultByte = 0x00;

        // Pack the booleans into a byte
        resultByte |= (bool1 << 0); // Shift the first boolean to the first bit
        resultByte |= (bool2 << 1); // Shift the second boolean to the second bit

        return;
    }

    // write me a sample code in cpp to handle reliable transmission and reception of lora messages with acknowledgement mechanism that includes timeout. One acknowledgement is not being received for more than 1 minute, the system should trigger reeelection to determine who to send to. Do this in a state machine
    void LoraProtocol::LoraStateMachine()
    {
        // Initialize the state
        LoraStates loraState = LoraStates::IDLE;

        // Initialize the timer
        unsigned long timer = millis();

        // Initialize the election result
        ElectionResult electionResult;

        // Loop
        while (true)
        {
            // Get the current time
            unsigned long currentTime = millis();

            // State Machine
            switch (loraState)
            {
            case LoraStates::IDLE:
                // Check if there is a message to send
                if (true) // TODO: Replace with a condition
                {
                    // Process the transmission
                    // processTransmission();

                    // Set the state to WAITING_FOR_MSG
                    loraState = LoraStates::WAITING_FOR_ACK;

                    // Reset the timer
                    timer = currentTime;
                }
                break;

            case LoraStates::WAITING_FOR_ACK:
                // Check if there is a message to receive
                if (true) // TODO: Replace with a condition
                {
                    // Process the reception
                    // processReception();

                    // Set the state to IDLE
                    loraState = LoraStates::IDLE;
                }
                else if (currentTime - timer > 60000) // 1 minute
                {
                    // Trigger re-election
                    electionResult = triggerReelection();

                    // Set the state to REELECTION
                    loraState = LoraStates::REELECTION;
                }
                break;

            case LoraStates::REELECTION:
                // Check if the re-election is complete
                if (true) // TODO: Replace with a condition
                {
                    // Set the state to IDLE
                    loraState = LoraStates::IDLE;
                }
                break;

            default:
                break;
            }
        }

        return;
    }


    void LoraProtocol::processMessage(LoRaMessage &loraMessage)
    {
        // Process the message
        Serial.println("Processing the message...");
        return;
    }



    long LoraProtocol::processTransmission(LoRaWrapper::LoRaWrapper &loraWrapper, 
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
                                            int receive_ack_duration)

    {
        packetCounter++;

        // Pack the sensors data into the lora message structure
        loraMessage.msg_type = sensorReadings;
        packNibblesIntoByte(destinationAddress, sourceAddress, loraMessage.header.addresses);
        loraMessage.header.packetCounter = packetCounter;
        loraMessage.payload.temperature = temperature * TWO_DECIMAL_FACTOR;
        loraMessage.payload.flameValue = flameValue;
        packTwoBooleansIntoByte(flameState, gasState, loraMessage.payload.states);
        loraMessage.payload.gasValue = gasValue;
        loraMessage.payload.batteryValue = batteryValue;
        loraMessage.checksum = checksumProtocol.calculateChecksum((uint8_t *)&loraMessage, sizeof(loraMessage) - 1);

        // Serialize the lora message
        uint8_t loraMessageSerialized[sizeof(loraMessage)];
        serializeMsg(loraMessage, loraMessageSerialized);

        // Blink LED
        if(!OLED_DISPLAY || myRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head) 
        {
        digitalWrite(LED_ONBOARD,HIGH);
        }

        // Send 
        loraWrapper.sendLoRaMessage(loraMessageSerialized, sizeof(loraMessageSerialized));

        // Blink LED
        if(!OLED_DISPLAY && myRole != AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head) 
        {
        digitalWrite(LED_ONBOARD,LOW);
        }

        return packetCounter;
    }

    void LoraProtocol::forwardMessage(LoRaWrapper::LoRaWrapper &loraWrapper, LoRaMessage &loraMessage, uint8_t &sender_id, uint8_t &dest_id)
    {
        // Pack the nibbles into a byte
        packNibblesIntoByte(dest_id, sender_id, loraMessage.header.addresses);
        Serial.println("Forwarding the message...");
        Serial.println("Sender ID: " + String(sender_id));
        Serial.println("Destination ID: " + String(dest_id));

        // Increment the packet counter
        loraMessage.header.packetCounter++;

        // Initialize ChecksumProtocol
        loraMessage.checksum = checksumProtocol.calculateChecksum((uint8_t *)&loraMessage, sizeof(loraMessage) - 1);

        // Serialize the lora message
        uint8_t loraMessageSerialized[sizeof(loraMessage)];
        serializeMsg(loraMessage, loraMessageSerialized);

        // Send the message
        loraWrapper.sendLoRaMessage(loraMessageSerialized, sizeof(loraMessageSerialized));

        return;
    }

    // Counter for valid and invalid checksums
    long cnt_valid = 0;
    long cnt_invalid = 0;

    void LoraProtocol::processReception(LoRaWrapper::LoRaWrapper &loraWrapper, 
    LoRaMessage &loraMessage, uint8_t &mySensorID, uint8_t &myDestID, 
    AutoNetwProc::AutoNetworkingProtocol::Roles &myRole,
    int send_ack_duration)
    {
        // Receive the message
        uint8_t loraMessageSerialized[sizeof(LoRaMessage)];
        loraWrapper.receiveLoRaMessage(loraMessageSerialized, sizeof(loraMessageSerialized));

        // LoRa.readBytes(loraMessageSerialized, sizeof(loraMessageSerialized));

        // Verify the checksum before processing the message 
        // Print the size of LoRaMessage
        // Serial.println("Size of LoRaMessage: " + String(sizeof(LoRaMessage)));
        // Print Checksum received
        int checksumReceived = loraMessageSerialized[sizeof(LoRaMessage) - 1];
        // Serial.println("Checksum received: " + String(checksumReceived));
        int checksumCalculated = checksumProtocol.calculateChecksum(loraMessageSerialized, sizeof(loraMessageSerialized) - 1);
        // Serial.println("Checksum calculated: " + String(checksumCalculated));
        if (checksumCalculated == checksumReceived)
        {
            cnt_valid++;

            // Deserialize the message
            deserializeMsg(loraMessageSerialized, loraMessage);

            // Get the roles and destination address by unpacking the nibbles
            uint8_t senderID, destinationID;
            unpackByteIntoNibbles(loraMessage.header.addresses, destinationID, senderID);

            // Check if the message is for me
            if (destinationID == mySensorID)
            // || destinationID == BROADCAST_ADDRESS)
            {
                // Print the message for debug
                #if DEBUG_MSG_RECEIVE_IN_BYTES
                Serial.println("\nReceived a message...");
                Serial.println("Sender ID: " + String(senderID));
                Serial.println("Destination ID: " + String(destinationID));
                Serial.println("Packet Counter: " + String(loraMessage.header.packetCounter));
                Serial.println("Temperature: " + String((double)loraMessage.payload.temperature / TWO_DECIMAL_FACTOR));
                Serial.println("Flame Value: " + String(loraMessage.payload.flameValue));
                // Serial.println("Flame State: " + String((loraMessage.payload.states >> 0) & 0x01));
                Serial.println("Flame State: " + String((loraMessage.payload.states & (1 << 0)) >> 0));
                Serial.println("Gas State: " + String((loraMessage.payload.states >> 1) & 0x01));
                // Serial.println("Gas State: " + String((loraMessage.payload.states & (1 << 1)) >> 1));
                Serial.println("Gas Value: " + String(loraMessage.payload.gasValue));
                Serial.println("Battery Value: " + String(loraMessage.payload.batteryValue));
                Serial.println("Checksum: " + String(loraMessage.checksum));
                Serial.println();
                #endif

                // Process the message
                processMessage(loraMessage);

            }
            if (destinationID == mySensorID && (myRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Gateway|| myRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head))
            {
                // Forward the message
                forwardMessage(loraWrapper, loraMessage, senderID, myDestID);
                
            }
        }
        else
        {
            cnt_invalid++;
            // Serial.println("Checksum failed...");
        }

        // Print out the number of valid and invalid checksums
        // Serial.println("Valid Checksums: " + String(cnt_valid));
        // Serial.println("Invalid Checksums: " + String(cnt_invalid));

        return;
    }

    void LoraProtocol::sendACK(LoRaWrapper::LoRaWrapper &loraWrapper, uint8_t &myID)
    {
        // Send the message
        uint8_t ackMessage[sizeof(uint8_t)];
        ackMessage[0] = ACK;
        ackMessage[1] = myID;
        loraWrapper.sendLoRaMessage(ackMessage, sizeof(uint8_t));

        #if DEBUG_ACK
        Serial.println("Ack sent: " + String(ackMessage[0]) + " " + String(ackMessage[1]));
        #endif

        return;
    }

    bool LoraProtocol::checkACK(LoRaWrapper::LoRaWrapper &loraWrapper, uint8_t &myDestID)
    {
        // Receive the message
        uint8_t ackMessage[sizeof(uint8_t)];
        loraWrapper.receiveLoRaMessage(ackMessage, sizeof(uint8_t));

        #if DEBUG_ACK
        Serial.println("Ack received: " + String(ackMessage[0]) + " " + String(ackMessage[1]));
        #endif

        // Check if the message is an ACK
        if (ackMessage[0] == ACK && ackMessage[1] == myDestID)
        {
            return true;
        }
        else
        {
            return false;
        }
    }



    void LoraProtocol::processExtCommands(uint8_t &SENSOR_ID, ThresholdsHandler::ThresholdsHandler &th,
                                           SleepHandler::SleepHandler &sh)

    {
        if (LoRa.parsePacket() == 0) {
            // No incoming messages
        } 
        else 
        {
            // Check the integrity of the received cluster head broadcast
            byte msgType = LoRa.read();
            byte sensorID = LoRa.read();
            if(msgType == processExtCmd && sensorID == SENSOR_ID)
            {
                byte typeOfThresholdsUpdate = LoRa.read();
                byte newThresholdsValue = LoRa.read();
                byte secondThresholdsValue = LoRa.read(); 
                byte chksum = LoRa.read();
                Serial.println(String(msgType) + ", " + String(sensorID) + ", " + String(typeOfThresholdsUpdate) + ", " + String(newThresholdsValue) + ", " + String(secondThresholdsValue) + ", " + String(chksum));
                int combined_value = (newThresholdsValue << 8) + secondThresholdsValue;

                uint8_t packet_data[] = {sensorID, typeOfThresholdsUpdate, newThresholdsValue, secondThresholdsValue};

                // uint8_t calculated_checksum = calculateChecksum(packet_data, sizeof(packet_data));
                // Serial.println("Calc. chksm" + String(calculated_checksum));

                // Simple checksum verification
                if (chksum == 77) 
                {
                    switch (typeOfThresholdsUpdate)
                    {
                        case updateTemp:
                        Serial.println("Old temp. threshold is " + String(th.getTempThreshold()));
                        th.setTempThreshold(combined_value);
                        Serial.println("New temp. threshold is " + String(th.getTempThreshold()));
                        break;

                        case updateDutyCycle:
                        Serial.println("Old duty cycle is " + String(sh.getDutyCycle()));
                        sh.setDutyCycle(combined_value);
                        Serial.println("New duty cycle  is " + String(sh.getDutyCycle()));
                        break;

                        case updateSleepTime:
                        Serial.println("Old time to sleep is " + String(sh.getSleepTime()));
                        sh.setSleepTime(combined_value);
                        Serial.println("New time to sleep is "+ String(sh.getSleepTime()));
                        break;

                        default:
                        break;
                    }    
                }
            }
        }
    }

} // namespace LoraProtocol
