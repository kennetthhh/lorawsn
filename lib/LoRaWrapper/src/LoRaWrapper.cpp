#include "LoRaWrapper.hpp"

namespace LoRaWrapper
{
    // Sleep LoRa module
    bool LoRaWrapper::sleepLoRa()
    {
        LoRa.sleep();
        return true;
    }

    // Stop LoRa module
    bool LoRaWrapper::stopLoRa()
    {
        LoRa.end();
        return true;
    }

    // Send LoRa message
    void LoRaWrapper::sendLoRaMessage(uint8_t* messageToSend, uint8_t messageSize)
    {
        LoRa.beginPacket();
        LoRa.write(messageToSend, messageSize);
        LoRa.endPacket();

        return;
    }

    // Receive LoRa message
    void LoRaWrapper::receiveLoRaMessage(uint8_t* receivedMessage, uint8_t messageSize)
    {
        LoRa.readBytes(receivedMessage, messageSize);

        return;
    }

    // Setter Functions Body
    bool LoRaWrapper::setLORA_RST(int newLORA_RST)
    {
        bool valid_LORA_RST = false;

        if (newLORA_RST >= MIN_GPIO_PIN && newLORA_RST <= MAX_GPIO_PIN)
        {
            this->LORA_RST = (gpio_num_t) newLORA_RST;
            valid_LORA_RST = true;
        }

        return valid_LORA_RST;
    }

    bool LoRaWrapper::setLORA_SS_CS(int newLORA_SS_CS)
    {
        bool valid_LORA_SS_CS = false;

        if (newLORA_SS_CS >= MIN_GPIO_PIN && newLORA_SS_CS <= MAX_GPIO_PIN)
        {
            this->LORA_SS_CS = (gpio_num_t) newLORA_SS_CS;
            valid_LORA_SS_CS = true;
        }

        return valid_LORA_SS_CS;
    }

    bool LoRaWrapper::setLORA_SCK(int newLORA_SCK)
    {
        bool valid_LORA_SCK = false;

        if (newLORA_SCK >= MIN_GPIO_PIN && newLORA_SCK <= MAX_GPIO_PIN)
        {
            this->LORA_SCK = (gpio_num_t) newLORA_SCK;
            valid_LORA_SCK = true;
        }

        return valid_LORA_SCK;
    }

    bool LoRaWrapper::setLORA_MOSI(int newLORA_MOSI)
    {
        bool valid_LORA_MOSI = false;

        if (newLORA_MOSI >= MIN_GPIO_PIN && newLORA_MOSI <= MAX_GPIO_PIN)
        {
            this->LORA_MOSI = (gpio_num_t) newLORA_MOSI;
            valid_LORA_MOSI = true;
        }

        return valid_LORA_MOSI;
    }

    bool LoRaWrapper::setLORA_MISO(int newLORA_MISO)
    {
        bool valid_LORA_MISO = false;

        if (newLORA_MISO >= MIN_GPIO_PIN && newLORA_MISO <= MAX_GPIO_PIN)
        {
            this->LORA_MISO = (gpio_num_t) newLORA_MISO;
            valid_LORA_MISO = true;
        }

        return valid_LORA_MISO;
    }

    bool LoRaWrapper::setLORA_DIO0(int newLORA_DIO0)
    {
        bool valid_LORA_DIO0 = false;

        if (newLORA_DIO0 >= MIN_GPIO_PIN && newLORA_DIO0 <= MAX_GPIO_PIN)
        {
            this->LORA_DIO0 = (gpio_num_t) newLORA_DIO0;
            valid_LORA_DIO0 = true;
        }

        return valid_LORA_DIO0;
    }

    bool LoRaWrapper::setLORA_BAND(int newLORA_BAND)
    {
        bool valid_LORA_BAND = false;

        // LoRa band supported by the library
        if (newLORA_BAND == 433E6 ||
            newLORA_BAND == 868E6 ||
            newLORA_BAND == 915E6)
        {
            this->LORA_BAND = newLORA_BAND;
            valid_LORA_BAND = true;
        }

        return valid_LORA_BAND;
    }

    bool LoRaWrapper::setLORA_FREQ(int newLORA_FREQ)
    {
        bool valid_LORA_FREQ = false;

        // LoRa band supported by the library
        if (newLORA_FREQ == 433E6 ||
            newLORA_FREQ == 868E6 ||
            newLORA_FREQ == 915E6)
        {
            this->LORA_FREQ = newLORA_FREQ;
            LoRa.setFrequency(newLORA_FREQ);
            valid_LORA_FREQ = true;
        }

        return valid_LORA_FREQ;
    }

    bool LoRaWrapper::setLORA_TX_POWER(int newTX_POWER_VALUE)
    {
        bool valid_LORA_TX_POWER = false;

        // LoRa TX Power supported by the library
        if (newTX_POWER_VALUE >= 2 && newTX_POWER_VALUE <= 20)
        {
            this->LORA_TX_POWER = newTX_POWER_VALUE;
            LoRa.setTxPower(newTX_POWER_VALUE);
            valid_LORA_TX_POWER = true;
        }

        return valid_LORA_TX_POWER;
    }

    bool LoRaWrapper::setLORA_SPREADING_FACTOR(int newLORA_SPREADING_FACTOR)
    {
        bool valid_LORA_SPREADING_FACTOR = false;

        // LoRa spreading factor supported by the library
        if (newLORA_SPREADING_FACTOR >= 6 && newLORA_SPREADING_FACTOR <= 12)
        {
            this->LORA_SPREADING_FACTOR = newLORA_SPREADING_FACTOR;
            LoRa.setSpreadingFactor(newLORA_SPREADING_FACTOR);
            valid_LORA_SPREADING_FACTOR = true;
        }

        return valid_LORA_SPREADING_FACTOR;
    }

    bool LoRaWrapper::setLORA_SIGNAL_BANDWIDTH(long newLORA_SIGNAL_BANDWIDTH)
    {
        bool valid_LORA_SIGNAL_BANDWIDTH = false;

        // LoRa signal bandwidth supported by the library
        if (newLORA_SIGNAL_BANDWIDTH == 7.8E3 ||
            newLORA_SIGNAL_BANDWIDTH == 10.4E3 ||
            newLORA_SIGNAL_BANDWIDTH == 15.6E3 ||
            newLORA_SIGNAL_BANDWIDTH == 20.8E3 ||
            newLORA_SIGNAL_BANDWIDTH == 31.25E3 ||
            newLORA_SIGNAL_BANDWIDTH == 41.7E3 ||
            newLORA_SIGNAL_BANDWIDTH == 62.5E3 ||
            newLORA_SIGNAL_BANDWIDTH == 125E3 ||
            newLORA_SIGNAL_BANDWIDTH == 250E3 ||
            newLORA_SIGNAL_BANDWIDTH == 500E3)
        {
            this->LORA_SIGNAL_BANDWIDTH = newLORA_SIGNAL_BANDWIDTH;
            LoRa.setSignalBandwidth(newLORA_SIGNAL_BANDWIDTH);
            valid_LORA_SIGNAL_BANDWIDTH = true;
        }

        return valid_LORA_SIGNAL_BANDWIDTH;
    }

    bool LoRaWrapper::setLORA_CODING_RATE(int newLORA_CODING_RATE)
    {
        bool valid_LORA_CODING_RATE = false;

        // LoRa coding rate supported by the library
        if (newLORA_CODING_RATE >= 5 && newLORA_CODING_RATE <= 8)
        {
            this->LORA_CODING_RATE = newLORA_CODING_RATE;
            LoRa.setCodingRate4(newLORA_CODING_RATE);
            valid_LORA_CODING_RATE = true;
        }

        return valid_LORA_CODING_RATE;
    }

    void LoRaWrapper::setLORA_CRC_ON(bool newLORA_CRC_ON)
    {
        if (newLORA_CRC_ON)
        {
            LoRa.enableCrc();
        }
        if (!newLORA_CRC_ON)
        {
            LoRa.disableCrc();
        }

        return;
    }

    bool LoRaWrapper::setLORA_LNA_GAIN(int newLORA_LNA_GAIN)
    {
        bool valid_LORA_LNA_GAIN = false;

        // LoRa LNA gain supported by the library
        if (newLORA_LNA_GAIN >=0 && newLORA_LNA_GAIN <=6)
        {
            this->LORA_LNA_GAIN = newLORA_LNA_GAIN;
            LoRa.setGain(newLORA_LNA_GAIN);
            valid_LORA_LNA_GAIN = true;
        }
    
        return valid_LORA_LNA_GAIN;
    }


    // Getter Functions Body
    gpio_num_t LoRaWrapper::getLORA_RST()
    {
        return this->LORA_RST;
    }

    gpio_num_t LoRaWrapper::getLORA_SS_CS()
    {
        return this->LORA_SS_CS;
    }

    gpio_num_t LoRaWrapper::getLORA_SCK()
    {
        return this->LORA_SCK;
    }

    gpio_num_t LoRaWrapper::getLORA_MOSI()
    {
        return this->LORA_MOSI;
    }

    gpio_num_t LoRaWrapper::getLORA_MISO()
    {
        return this->LORA_MISO;
    }

    gpio_num_t LoRaWrapper::getLORA_DIO0()
    {
        return this->LORA_DIO0;
    }

    int LoRaWrapper::getLORA_BAND()
    {
        return this->LORA_BAND;
    }

    int LoRaWrapper::getLORA_FREQ()
    {
        return this->LORA_FREQ;
    }

    uint8_t LoRaWrapper::getLORA_TX_POWER()
    {
        return this->LORA_TX_POWER;
    }

    uint8_t LoRaWrapper::getLORA_SPREADING_FACTOR()
    {
        return this->LORA_SPREADING_FACTOR;
    }

    long LoRaWrapper::getLORA_SIGNAL_BANDWIDTH()
    {
        return this->LORA_SIGNAL_BANDWIDTH;
    }

    uint8_t LoRaWrapper::getLORA_CODING_RATE()
    {
        return this->LORA_CODING_RATE;
    }

    bool LoRaWrapper::getLORA_CRC_ON()
    {
        return this->LORA_CRC;
    }

    uint8_t LoRaWrapper::getLORA_LNA_GAIN()
    {
        return this->LORA_LNA_GAIN;
    }

} // namespace LoRaWrapper