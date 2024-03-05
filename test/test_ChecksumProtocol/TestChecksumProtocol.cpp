#include <gtest/gtest.h>
#include <gmock/gmock.h> // Include the necessary header file
#include "../../lib/ChecksumProtocol/src/ChecksumProtocol.hpp"

#if defined(ARDUINO)
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  ::testing::InitGoogleTest();
}

void loop()
{
  if(RUN_ALL_TESTS());
}

TEST(ChecksumProtocolTest, CalculateChecksumNullData) {
  EXPECT_THROW(ChksumProc::ChecksumProtocol checksumProtocol; checksumProtocol.calculateChecksum(nullptr, 10), std::invalid_argument);
}

TEST(ChecksumProtocolTest, CalculateChecksumZeroLength) {
  uint8_t data[] = {};
  EXPECT_THROW(ChksumProc::ChecksumProtocol checksumProtocol; checksumProtocol.calculateChecksum(reinterpret_cast<uint8_t*>(&data), 0), std::invalid_argument);
}

TEST(ChecksumProtocolTest, CalculateChecksumSingleByteData) {
  uint8_t data[] = {0x42};
  ChksumProc::ChecksumProtocol checksumProtocol;
  uint8_t actual_checksum = checksumProtocol.calculateChecksum(data, sizeof(data));
  EXPECT_EQ(0x42, actual_checksum);
}

TEST(ChecksumProtocolTest, CalculateChecksumMultipleByteData) {
  uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
  ChksumProc::ChecksumProtocol checksumProtocol;
  uint8_t actual_checksum = checksumProtocol.calculateChecksum(data, sizeof(data));
  EXPECT_EQ(0x04, actual_checksum);
}

TEST(ChecksumProtocolTest, VerifyChecksumNullData) {
  EXPECT_THROW(ChksumProc::ChecksumProtocol checksumProtocol; checksumProtocol.verifyChecksum(nullptr, 10, 0), std::invalid_argument);
}

TEST(ChecksumProtocolTest, VerifyChecksumZeroLength) {
  uint8_t data[] = {};
  EXPECT_THROW(ChksumProc::ChecksumProtocol checksumProtocol; checksumProtocol.verifyChecksum(reinterpret_cast<uint8_t*>(&data), 0, 0), std::invalid_argument);
}

TEST(ChecksumProtocolTest, VerifyChecksumSingleByteValid) {
  uint8_t data[] = {0x42};
  uint8_t received_checksum = 0x42; // Calculated and received checksum match.

  ChksumProc::ChecksumProtocol checksumProtocol;
  bool actual_validity = checksumProtocol.verifyChecksum(data, sizeof(data), received_checksum);

  EXPECT_TRUE(actual_validity);
}

TEST(ChecksumProtocolTest, VerifyChecksumSingleByteInvalid) {
  uint8_t data[] = {0x42};
  uint8_t received_checksum = 0x43; // Calculated and received checksum differ.

  ChksumProc::ChecksumProtocol checksumProtocol;
  bool actual_validity = checksumProtocol.verifyChecksum(data, sizeof(data), received_checksum);

  EXPECT_FALSE(actual_validity);
}

TEST(ChecksumProtocolTest, VerifyChecksumMultipleByteValid) {
  uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
  uint8_t calculated_checksum = 0x04; // Calculate internal checksum.

  ChksumProc::ChecksumProtocol checksumProtocol;
  bool actual_validity = checksumProtocol.verifyChecksum(data, sizeof(data), calculated_checksum);

  EXPECT_TRUE(actual_validity);
}

TEST(ChecksumProtocolTest, VerifyChecksumMultipleByteInvalid) {
  uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
  uint8_t invalid_checksum = 0x07; // Use an invalid checksum.

  ChksumProc::ChecksumProtocol checksumProtocol;
  bool actual_validity = checksumProtocol.verifyChecksum(data, sizeof(data), invalid_checksum);

  EXPECT_FALSE(actual_validity);
}

#endif
