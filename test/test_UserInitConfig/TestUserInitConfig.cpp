#include <gtest/gtest.h>
#include <gmock/gmock.h> // Include the necessary header file

#if defined(ARDUINO)
#include <Arduino.h>
#include "../../lib/UserInitConfig/src/UserInitConfig.hpp"

void setup()
{
  Serial.begin(115200);
  ::testing::InitGoogleTest();
}

void loop()
{
  if(RUN_ALL_TESTS());
}

TEST(UserInitConfig, TestUserInitConfig)
{
  // Instantiate UserInitConfiguration
  UserInitConfig::UserInitConfiguration userInitConfig(1, 2, AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head, 2000, 1000, 30, 20, 10);

  // Test getUserConfig
  UserInitConfig::UserInitConfiguration::UserConfig userConfig = userInitConfig.getUserConfig();
  EXPECT_EQ(userConfig.sensorID, 1);
  EXPECT_EQ(userConfig.destID, 2);
  EXPECT_EQ(userConfig.role, AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head);
  EXPECT_EQ(userConfig.flameThreshold, 2000);
  EXPECT_EQ(userConfig.gasThreshold, 1000);
  EXPECT_EQ(userConfig.tempThreshold, 30);
  EXPECT_EQ(userConfig.sleepTime, 20);
  EXPECT_EQ(userConfig.dutyCycle, 10);
}

#endif
