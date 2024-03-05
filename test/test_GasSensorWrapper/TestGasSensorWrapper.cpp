#include <gtest/gtest.h>
#include <gmock/gmock.h> // Include the necessary header file
#include "../../lib/GasSensorWrapper/src/GasSensorWrapper.hpp"

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

TEST(GasSensorWrapperTest, GetBitmaskGasConst) {
  const GasSensor::GasSensorWrapper wrapper;
  // Using const_cast to enforce const correctness while calling
  long long actual_bitmask = const_cast<GasSensor::GasSensorWrapper&>(wrapper).getBitmaskGas();

  EXPECT_EQ(GasSensor::BITMASK_GAS, actual_bitmask);
}

#endif
