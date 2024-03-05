#include <gtest/gtest.h>
#include <gmock/gmock.h> // Include the necessary header file

#if defined(ARDUINO)
#include <Arduino.h>
#include "../../lib/ThresholdsHandler/src/ThresholdsHandler.hpp"

void setup()
{
  Serial.begin(115200);
  ::testing::InitGoogleTest();
}

void loop()
{
  if(RUN_ALL_TESTS());
}

TEST(ThresholdsHandler, setGasThreshold)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  EXPECT_EQ(thresholdsHandler.setGasThreshold(0), false);
  EXPECT_EQ(thresholdsHandler.setGasThreshold(4096), false);
  EXPECT_EQ(thresholdsHandler.setGasThreshold(2048), true);
}

TEST(ThresholdsHandler, setTempThreshold)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  EXPECT_EQ(thresholdsHandler.setTempThreshold(0), false);
  EXPECT_EQ(thresholdsHandler.setTempThreshold(-1), false);
  EXPECT_EQ(thresholdsHandler.setTempThreshold(1), true);
}

TEST(ThresholdsHandler, setPressureThreshold)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  EXPECT_EQ(thresholdsHandler.setPressureThreshold(0), false);
  EXPECT_EQ(thresholdsHandler.setPressureThreshold(-1), false);
  EXPECT_EQ(thresholdsHandler.setPressureThreshold(1), true);
}

TEST(ThresholdsHandler, setSleepTime)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  EXPECT_EQ(thresholdsHandler.setSleepTime(0), false);
  EXPECT_EQ(thresholdsHandler.setSleepTime(-1), false);
  EXPECT_EQ(thresholdsHandler.setSleepTime(1), true);
}

TEST(ThresholdsHandler, setDutyCycle)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  EXPECT_EQ(thresholdsHandler.setDutyCycle(0), false);
  EXPECT_EQ(thresholdsHandler.setDutyCycle(-1), false);
  EXPECT_EQ(thresholdsHandler.setDutyCycle(1), true);
}

TEST(ThresholdsHandler, getGasThreshold)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  thresholdsHandler.setGasThreshold(2048);
  EXPECT_EQ(thresholdsHandler.getGasThreshold(), 2048);
}

TEST(ThresholdsHandler, getTempThreshold)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  thresholdsHandler.setTempThreshold(1);
  EXPECT_EQ(thresholdsHandler.getTempThreshold(), 1);
}

TEST(ThresholdsHandler, getPressureThreshold)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  thresholdsHandler.setPressureThreshold(1);
  EXPECT_EQ(thresholdsHandler.getPressureThreshold(), 1);
}

TEST(ThresholdsHandler, getSleepTime)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  thresholdsHandler.setSleepTime(1);
  EXPECT_EQ(thresholdsHandler.getSleepTime(), 1);
}

TEST(ThresholdsHandler, getDutyCycle)
{
  ThresholdsHandler::ThresholdsHandler thresholdsHandler;
  thresholdsHandler.setDutyCycle(1);
  EXPECT_EQ(thresholdsHandler.getDutyCycle(), 1);
}

#endif
