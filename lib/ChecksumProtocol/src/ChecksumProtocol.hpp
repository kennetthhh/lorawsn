/*****************************************************************/
/********************** ChecksumProtocol *************************/
/*****************************************************************/
#ifndef ChecksumProtocol_HPP
#define ChecksumProtocol_HPP

#include <Arduino.h>
#include <stdint.h>
#include <CRC32.h>

namespace ChksumProc
{

// ChecksumProtocol Class
class ChecksumProtocol
{
    private:
    uint8_t _checksum;

    public:
    // Constructor
    ChecksumProtocol() : _checksum(0) {}
    uint8_t calculateChecksum(const uint8_t* data, const uint8_t length);
    bool verifyChecksum(const uint8_t* data, const uint8_t length, const uint8_t received_checksum);

}; // class ChecksumProtocol

} // namespace ChksumProc

#endif // ChecksumProtocol_HPP