Welcome to LoRa Wireless Sensor Network!

This is a repository that allows you to configure the sensor (ESP32) which is either equipped with a LoRa module or is connected to an external LoRa module XL1276. It is developed in PlatformIO based on Arduino.

## Sensor

This section contains the dependencies and the steps needed to configure the sensor such that it works as intended.

## Libraries
- Arduino.h
- google/googletest@^1.12.1
- sandeepmistry/LoRa@^0.8.0
- SPI
- Wire
- adafruit/Adafruit BMP085 Library@^1.2.4
- adafruit/Adafruit Unified Sensor@^1.1.14
- adafruit/Adafruit BusIO@^1.15.0
- adafruit/Adafruit GFX Library@^1.11.9
- adafruit/Adafruit SSD1306@^2.5.9
- bakercp/CRC32@^2.0.0

## Sensor ID, Destination ID, and Role configuration
The configuration should be set by using the UserInitConfig Constructors by specifying its respective parameters in this order:
1. Sensor ID (0 .. 15)
2. Destination ID (0 .. 15)
3. Role (Member_Node, Gateway, Member_Node)
4. Flame Threshold (long)
5. Gas Threshold (long)
6. Temperature Threshold (long)
7. Sleep Time (int)
8. Duty Cycle (int)

As currently configured, it is necessary to predefine the entire mesh network first before it can be operated. The self-clustering protocol will only be initiated once a Cluster Head is unplugged.
The code is developed and tested on ESP32 with XL1276 LoRa module.

* Routing only works when the Cluster Head has coverage to the Pi, if a Cluster Head that is too far to reach the Pi will not be able to perform its Routing procedure.


## Edge and Cloud

This section contains the dependencies and the steps needed to execute the scripts on both the edge (Raspberry Pi) and the cloud (local computer).

## Dependencies

### Edge 

- **Python 3.8 or above**
- **pyLoRa**: Library for LoRa communication.
- **RPi.GPIO**: Library for interacting with GPIO pins on Raspberry Pi.
- **paho-mqtt**: MQTT client library for Python.
- **Mosquitto**: MQTT broker for hosting the MQTT server.

### Cloud 

- **Python 3.8 or above**
- **paho-mqtt**: MQTT client library for Python.
- **influxdb_client**: Library for managing and interacting with InfluxDB.
- **schedule**: Library for scheduling tasks.

## Executing edge script

1. Connect the Raspberry Pi to the S127x LoRa module and RGB LED.
2. Ensure Access Point is set up on the Raspberry Pi.
2. Install the required dependencies using `pip3 install pyLoRa RPi.GPIO paho-mqtt`.
3. Ensure Mosquitto MQTT broker is installed and running on the Raspberry Pi.
4. Run `main.py` in the directory 2_Pi on the Raspberry Pi.

## Executing cloud script

1. Install InfluxDB on the local computer.
2. Install the required Python dependencies using `pip3 install paho-mqtt influxdb-client schedule`.
3. Create a new project on the InfluxDB at `localhost:8086` by the name of `IoT_Project`.
4. Follow the steps on the browser for creating Python script query, ensuring the bucket name is `Sensor_Data`.
5. Connect the copmputer to the Raspberry Pi access point.
6. Modify the line ```laptop_client.connect("10.42.0.1", 1883)``` in `main.py` in the directory 3_Cloud, ensuring the first parameter is the IP address of the Pi.
7. Run `main.py` on the local computer.


### Using the .csv files

- The `thresholds.csv` file follows this format:
    
    ```
    update_status (0 or 1) (0 for no updates and vice versa)
    temperature_threshold
    flame_state_threshold
    
    ```

- The `sensor_thresholds.csv` file follows this format:
    ```
    update_status (0 or 1) (0 for no updates and vice versa)
    sensor_id_1,sensor_id_2,sensor_id_3,...,sensor_id_n
    threshold_type_1,threshold_type_2,threshold_type_3,...,threshold_type_n
    threshold_value_1,threshold_value_2,threshold_value_3,...,threshold_value_n
    ```

#### Note

- The number of sensor_id, threshold_type and threshold_value entries in `sensor_thresholds.csv` should be equal.
- The `update_status` in both the csvs should be set to 1 for updating the thresholds. 