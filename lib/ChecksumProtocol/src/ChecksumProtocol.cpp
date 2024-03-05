#include "ChecksumProtocol.hpp"

namespace ChksumProc
{
    uint8_t ChecksumProtocol::calculateChecksum(const uint8_t* data, const uint8_t length)
    {
        if (data == nullptr)
        {
            throw std::invalid_argument("Data is null");
        }
        if (length == 0)
        {
            throw std::invalid_argument("Data is of zero length");
        }

        uint8_t checksum = 0;

        for (uint8_t i = 0; i < length; i++)
        {
            checksum ^= data[i];
        }

        // CRC32 crc;
        // crc.update(data, length);
        // checksum = crc.finalize();

        return checksum;
    }

    bool ChecksumProtocol::verifyChecksum(const uint8_t* data, const uint8_t length, const uint8_t received_checksum)
    {
        if (data == nullptr)
        {
            throw std::invalid_argument("Data is null");
        }
        if (length == 0)
        {
            throw std::invalid_argument("Data is of zero length");
        }

        uint8_t checksum = 0;

        for (uint8_t i = 0; i < length; i++)
        {
            checksum ^= data[i];
        }

        return checksum == received_checksum;


        // uint32_t checksum = calculateCRC32(data, length);
        // uint32_t received_checksum
    }

} // namespace ChksumProc