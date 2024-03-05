#include <gtest/gtest.h>
#include <gmock/gmock.h> // Include the necessary header file
#include "../../../lib/LoRaWrapper/src/LoRaWrapper.hpp"

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

TEST(LoRaWrapperTest, setLORA_RSTValidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_TRUE(lw.setLORA_RST(0)); // Expect valid LORA_RST value
}

TEST(LoRaWrapperTest, setLORA_RSTInvalidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_FALSE(lw.setLORA_RST(40)); // Expect invalid LORA_RST value
}

TEST(LoRaWrapperTest, setLORA_SS_CSValidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_TRUE(lw.setLORA_SS_CS(0)); // Expect valid LORA_SS_CS value
}

TEST(LoRaWrapperTest, setLORA_SS_CSInvalidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_FALSE(lw.setLORA_SS_CS(40)); // Expect invalid LORA_SS_CS value
}

TEST(LoRaWrapperTest, setLORA_SCKValidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_TRUE(lw.setLORA_SCK(0)); // Expect valid LORA_SCK value
}

TEST(LoRaWrapperTest, setLORA_SCKInvalidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_FALSE(lw.setLORA_SCK(40)); // Expect invalid LORA_SCK value
}

TEST(LoRaWrapperTest, setLORA_MOSIValidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_TRUE(lw.setLORA_MOSI(0)); // Expect valid LORA_MOSI value
}

TEST(LoRaWrapperTest, setLORA_MOSIInvalidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_FALSE(lw.setLORA_MOSI(40)); // Expect invalid LORA_MOSI value
}

TEST(LoRaWrapperTest, setLORA_MISOValidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_TRUE(lw.setLORA_MISO(0)); // Expect valid LORA_MISO value
}

TEST(LoRaWrapperTest, setLORA_MISOInvalidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_FALSE(lw.setLORA_MISO(40)); // Expect invalid LORA_MISO value
}

TEST(LoRaWrapperTest, setLORA_BANDValidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_TRUE(lw.setLORA_BAND(868E6)); // Expect valid LORA_BAND value
}

TEST(LoRaWrapperTest, setLORA_BANDInvalidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_FALSE(lw.setLORA_BAND(40)); // Expect invalid LORA_BAND value
}

TEST(LoRaWrapperTest, setLORA_SPREADING_FACTORValidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_TRUE(lw.setLORA_SPREADING_FACTOR(7)); // Expect valid LORA_SPREADING_FACTOR value
}

TEST(LoRaWrapperTest, setLORA_SPREADING_FACTORInvalidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_FALSE(lw.setLORA_SPREADING_FACTOR(40)); // Expect invalid LORA_SPREADING_FACTOR value
}

TEST(LoRaWrapperTest, setLORA_SIGNAL_BANDWIDTHValidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_TRUE(lw.setLORA_SIGNAL_BANDWIDTH(7.8E3)); // Expect valid LORA_SIGNAL_BANDWIDTH value
}

TEST(LoRaWrapperTest, setLORA_SIGNAL_BANDWIDTHInvalidValue) 
{
  LoRaWrapper::LoRaWrapper lw;
  EXPECT_FALSE(lw.setLORA_SIGNAL_BANDWIDTH(10.5E3)); // Expect invalid LORA_SIGNAL_BANDWIDTH value
}

TEST(LoRaWrapperTest, getLORA_RST) 
{
  LoRaWrapper::LoRaWrapper lw;
  lw.setLORA_RST(17);
  EXPECT_EQ(lw.getLORA_RST(), 17); // Expect LORA_RST value to be 0
}

TEST(LoRaWrapperTest, getLORA_SS_CS) 
{
  LoRaWrapper::LoRaWrapper lw;
  lw.setLORA_SS_CS(17);
  EXPECT_EQ(lw.getLORA_SS_CS(), 17); // Expect LORA_SS_CS value to be 0
}

TEST(LoRaWrapperTest, getLORA_SCK) 
{
  LoRaWrapper::LoRaWrapper lw;
  lw.setLORA_SCK(17);
  EXPECT_EQ(lw.getLORA_SCK(), 17); // Expect LORA_SCK value to be 0
}

TEST(LoRaWrapperTest, getLORA_MOSI) 
{
  LoRaWrapper::LoRaWrapper lw;
  lw.setLORA_MOSI(17);
  EXPECT_EQ(lw.getLORA_MOSI(), 17); // Expect LORA_MOSI value to be 0
}

TEST(LoRaWrapperTest, getLORA_MISO) 
{
  LoRaWrapper::LoRaWrapper lw;
  lw.setLORA_MISO(17);
  EXPECT_EQ(lw.getLORA_MISO(), 17); // Expect LORA_MISO value to be 0
}

TEST(LoRaWrapperTest, getLORA_BAND) 
{
  LoRaWrapper::LoRaWrapper lw;
  lw.setLORA_BAND(868E6);
  EXPECT_EQ(lw.getLORA_BAND(), 868E6); // Expect LORA_BAND value to be 868E6
}

TEST(LoRaWrapperTest, getLORA_SPREADING_FACTOR) 
{
  LoRaWrapper::LoRaWrapper lw;
  lw.setLORA_SPREADING_FACTOR(7);
  EXPECT_EQ(lw.getLORA_SPREADING_FACTOR(), 7); // Expect LORA_SPREADING_FACTOR value to be 7
}

TEST(LoRaWrapperTest, getLORA_SIGNAL_BANDWIDTH) 
{
  LoRaWrapper::LoRaWrapper lw;
  lw.setLORA_SIGNAL_BANDWIDTH(10.4E3);
  EXPECT_EQ(lw.getLORA_SIGNAL_BANDWIDTH(), 10.4E3); // Expect LORA_SIGNAL_BANDWIDTH value to be 125E3
}

#endif