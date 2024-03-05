#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <Arduino.h>
#include <stdint.h>

// Define current sensor
#define THIS_SENSOR_ID 4
// Sleep or no
#define SLEEP_MODE true
// Serial print or no
#define SERIAL_PRINT false
// Test individual test and receive via lora
#define TEST_LORA_INDIVIDUAL_SEND_AND_RECEIVE true
#define DEBUG_MSG_RECEIVE_IN_BYTES true
// Debug ack
#define DEBUG_ACK true
// Debug Sensor info
#define DEBUG_SENSOR_INFO true
// Show wakeup reason
#define SHOW_WAKEUP_REASON false

// Constants definitions for OLED Display
static constexpr gpio_num_t OLED_SDA = GPIO_NUM_4;
static constexpr gpio_num_t OLED_SCL = GPIO_NUM_15;
static constexpr gpio_num_t OLED_RST = GPIO_NUM_16;
static constexpr uint8_t SCREEN_WIDTH = 128;    // OLED display width, in pixels
static constexpr uint8_t SCREEN_HEIGHT = 64;    // OLED display height, in pixels

// Message Type Definitions
static constexpr uint8_t syncSensors          = 8;
static constexpr uint8_t sendOutPing          = 18;
static constexpr uint8_t sendCountsRssi       = 28;
static constexpr uint8_t verifyClusterHead    = 38;
static constexpr uint8_t clusterHeadBroadcast = 48;
static constexpr uint8_t processExtCmd        = 58;
static constexpr uint8_t DEMO                 = 68;
static constexpr uint8_t RREP                 = 78;
static constexpr uint8_t RREQ                 = 88;
static constexpr uint8_t sensorReadings       = 98;
static constexpr uint8_t ACK                  = 108;
static constexpr uint8_t NACK                 = 118;

// Thresholds update protocol
static constexpr uint8_t updateTemp = 10;
static constexpr uint8_t updateDutyCycle = 20;
static constexpr uint8_t updateSleepTime = 30;

// Duty Cycle
static constexpr uint32_t dutyCycle = 2000;

// Packet Counter
RTC_DATA_ATTR static uint16_t packetCounter = 0;

// Battery Value
static uint8_t batteryValue = 100;

// Heart Beat Timeout Duration in milliseconds (default: 1/2minute)
static int heartbeatTimeout = 30000;


#endif // GLOBALS_HPP