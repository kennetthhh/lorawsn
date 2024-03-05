#include <gtest/gtest.h>
#include <gmock/gmock.h> // Include the necessary header file
#include "../../../lib/AutoNetworkingProtocol/src/AutoNetworkingProtocol.hpp"

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

TEST(AutoNetworkingProtocolTest, getDestIDReturnsInitialValue) 
{
  AutoNetwProc::AutoNetworkingProtocol anp; // Default constructor
  EXPECT_EQ(anp.getDestID(), 0); // Expect initial DEST_ID to be 0
}

TEST(AutoNetworkingProtocolTest, getDestIDReturnsSetValue) 
{
  AutoNetwProc::AutoNetworkingProtocol anp(1, 42, AutoNetwProc::AutoNetworkingProtocol::Member_Node);
  EXPECT_EQ(anp.getDestID(), 42); // Expect DEST_ID to be set value
}

TEST(AutoNetworkingProtocolTest, getSensorIDReturnsInitialValue)
{
  AutoNetwProc::AutoNetworkingProtocol anp;
  EXPECT_EQ(anp.getSensorID(), 1); // Expect initial SENSOR_ID to be 1
}

TEST(AutoNetworkingProtocolTest, getSensorIDReturnsSetValue) 
{
  AutoNetwProc::AutoNetworkingProtocol anp(10, 0, AutoNetwProc::AutoNetworkingProtocol::Member_Node);
  EXPECT_EQ(anp.getSensorID(), 10); // Expect SENSOR_ID to be set value
}

TEST(AutoNetworkingProtocolTest, getRoleReturnsInitialValue) 
{
  AutoNetwProc::AutoNetworkingProtocol anp;
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Member_Node); // Expect initial role to be Member_Node
}

TEST(AutoNetworkingProtocolTest, getRoleReturnsSetValue) 
{
  AutoNetwProc::AutoNetworkingProtocol anp(1, 0, AutoNetwProc::AutoNetworkingProtocol::Cluster_Head);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Cluster_Head); // Expect role to be set value
}

TEST(AutoNetworkingProtocolTest, setSensorIDSetsValidID) {
  AutoNetwProc::AutoNetworkingProtocol anp;

  // Valid sensor ID within range
  uint8_t validSensorID = 25;  // Assuming MIN_SENSOR_ID <= 25 <= MAX_SENSOR_ID
  EXPECT_TRUE(anp.setSensorID(validSensorID));
  EXPECT_EQ(anp.getSensorID(), validSensorID);
}

TEST(AutoNetworkingProtocolTest, setSensorIDRejectsInvalidIDBelowMin) {
  AutoNetwProc::AutoNetworkingProtocol anp;

  // Invalid sensor ID below minimum
  auto invalidSensorID = AutoNetwProc::MIN_SENSOR_ID - 1;
  EXPECT_FALSE(anp.setSensorID(invalidSensorID));
  EXPECT_NE(anp.getSensorID(), invalidSensorID);  // Ensure it didn't change
}

TEST(AutoNetworkingProtocolTest, setSensorIDRejectsInvalidIDAboveMax) {
  AutoNetwProc::AutoNetworkingProtocol anp;

  // Invalid sensor ID above maximum
  auto invalidSensorID = AutoNetwProc::MAX_SENSOR_ID + 1;
  EXPECT_FALSE(anp.setSensorID(invalidSensorID));
  EXPECT_NE(anp.getSensorID(), invalidSensorID);  // Ensure it didn't change
}

TEST(AutoNetworkingProtocolTest, setDestIDSetsValidID) {
  AutoNetwProc::AutoNetworkingProtocol anp;

  // Valid destination ID within range
  uint8_t validDestID = 42; // Assuming uint8_t type based on MIN/MAX
  EXPECT_TRUE(anp.setDestID(validDestID));
  EXPECT_EQ(anp.getDestID(), validDestID);
}

TEST(AutoNetworkingProtocolTest, setDestIDRejectsInvalidIDBelowMin) {
  AutoNetwProc::AutoNetworkingProtocol anp;

  // Invalid destination ID below minimum
  int invalidDestID = AutoNetwProc::MIN_DEST_ID - 1;
  EXPECT_FALSE(anp.setDestID(invalidDestID));
  EXPECT_NE(anp.getDestID(), invalidDestID); // Ensure it didn't change
}

TEST(AutoNetworkingProtocolTest, setDestIDRejectsInvalidIDAboveMax) {
  AutoNetwProc::AutoNetworkingProtocol anp;

  // Invalid destination ID above maximum
  int invalidDestID = AutoNetwProc::MAX_DEST_ID + 1;
  EXPECT_FALSE(anp.setDestID(invalidDestID));
  EXPECT_NE(anp.getDestID(), invalidDestID); // Ensure it didn't change
}

TEST(AutoNetworkingProtocolTest, setRoleHandlesValidRoles) 
{
  // Create an instance
  AutoNetwProc::AutoNetworkingProtocol anp;

  // Valid Roles: Cluster_Head, Gateway, Member_Node
  std::vector<AutoNetwProc::AutoNetworkingProtocol::Roles> validRoles = {
      AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head,
      AutoNetwProc::AutoNetworkingProtocol::Roles::Gateway,
      AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node};

  // Test each valid role
  for (auto role : validRoles) {
    EXPECT_TRUE(anp.setRole(role));
    EXPECT_EQ(anp.getRole(), role); // Assert role is set correctly
  }
}

TEST(AutoNetworkingProtocolTest, setRoleHandlesInvalidRole) 
{
  // Create an instance
  AutoNetwProc::AutoNetworkingProtocol anp;

  // Invalid role (doesn't exist in enum)
  AutoNetwProc::AutoNetworkingProtocol::Roles invalidRole =
      static_cast<AutoNetwProc::AutoNetworkingProtocol::Roles>(100);

  EXPECT_FALSE(anp.setRole(invalidRole));
  EXPECT_NE(anp.getRole(), invalidRole); // Assert role remains unchanged
}

TEST(AutoNetworkingProtocolTest, setAssignmentSetsValuesForSensorID1) {
  AutoNetwProc::AutoNetworkingProtocol anp;
  anp.setSensorID(1);  // Set SENSOR_ID to 1

  EXPECT_TRUE(anp.setAssignment());
  EXPECT_EQ(anp.getDestID(), 4);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head);
}

TEST(AutoNetworkingProtocolTest, setAssignmentSetsValuesForSensorID2) {
  AutoNetwProc::AutoNetworkingProtocol anp;
  anp.setSensorID(2);  // Set SENSOR_ID to 2

  EXPECT_TRUE(anp.setAssignment());
  EXPECT_EQ(anp.getDestID(), 1);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node);
}

TEST(AutoNetworkingProtocolTest, setAssignmentSetsValuesForSensorID3) {
  AutoNetwProc::AutoNetworkingProtocol anp;
  anp.setSensorID(3);  // Set SENSOR_ID to 3

  EXPECT_TRUE(anp.setAssignment());
  EXPECT_EQ(anp.getDestID(), 1);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node);
}

TEST(AutoNetworkingProtocolTest, setAssignmentSetsValuesForSensorID4) {
  AutoNetwProc::AutoNetworkingProtocol anp;
  anp.setSensorID(4);  // Set SENSOR_ID to 4

  EXPECT_TRUE(anp.setAssignment());
  EXPECT_EQ(anp.getDestID(), 0);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head);
}

TEST(AutoNetworkingProtocolTest, setAssignmentSetsValuesForSensorID5) {
  AutoNetwProc::AutoNetworkingProtocol anp;
  anp.setSensorID(5);  // Set SENSOR_ID to 5

  EXPECT_TRUE(anp.setAssignment());
  EXPECT_EQ(anp.getDestID(), 4);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node);
}

TEST(AutoNetworkingProtocolTest, setAssignmentSetsValuesForSensorID6) {
  AutoNetwProc::AutoNetworkingProtocol anp;
  anp.setSensorID(6);  // Set SENSOR_ID to 6

  EXPECT_TRUE(anp.setAssignment());
  EXPECT_EQ(anp.getDestID(), 4);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node);
}

TEST(AutoNetworkingProtocolTest, setAssignmentSetsValuesForSensorID7) {
  AutoNetwProc::AutoNetworkingProtocol anp;
  anp.setSensorID(7);  // Set SENSOR_ID to 7

  EXPECT_TRUE(anp.setAssignment());
  EXPECT_EQ(anp.getDestID(), 4);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node);
}

TEST(AutoNetworkingProtocolTest, setAssignmentReturnsFalseForInvalidSensorID) {
  AutoNetwProc::AutoNetworkingProtocol anp;
  anp.setSensorID(8);  // Invalid sensor ID

  EXPECT_FALSE(anp.setAssignment());
  // Check that DEST_ID and role remain unchanged (or handled appropriately)
  EXPECT_EQ(anp.getDestID(), 0);
  EXPECT_EQ(anp.getRole(), AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node);
}


#endif
