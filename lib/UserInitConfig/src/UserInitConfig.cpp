#include "UserInitConfig.hpp"

namespace UserInitConfig
{
    // Parse csv
    void UserInitConfiguration::parseCsvLine(const String& line, UserConfig& userConfig) 
    {
        char* token;
        char* context = nullptr;

        // Parse CSV data using strtok_r
        token = strtok_r((char*)line.c_str(), ",", &context);
        if (token != nullptr) userConfig.sensorID = atoi(token);

        token = strtok_r(nullptr, ",", &context);
        if (token != nullptr) userConfig.destID = atoi(token);

        token = strtok_r(nullptr, ",", &context);
        if (token != nullptr) userConfig.role = static_cast<AutoNetwProc::AutoNetworkingProtocol::Roles>(atoi(token));

        token = strtok_r(nullptr, ",", &context);
        if (token != nullptr) userConfig.flameThreshold = atol(token);

        token = strtok_r(nullptr, ",", &context);
        if (token != nullptr) userConfig.gasThreshold = atol(token);

        token = strtok_r(nullptr, ",", &context);
        if (token != nullptr) userConfig.tempThreshold = atol(token);

        token = strtok_r(nullptr, ",", &context);
        if (token != nullptr) userConfig.sleepTime = atoi(token);

        token = strtok_r(nullptr, ",", &context);
        if (token != nullptr) userConfig.dutyCycle = atoi(token);

        return;
    }


    // Read user config 
    void UserInitConfiguration::readUserConfig(const std::string& filename, UserConfig& config)
    {
        // Check if the SD card is available
        if (!SPIFFS.begin(true)) 
        {
            Serial.println("Error: Unable to mount SPIFFS filesystem.");
            return;
        }

        // Filename
        String myFilename = filename.c_str();

        // Read the configuration from the .csv file
        File file = SPIFFS.open(myFilename, "r");
  
        if (!file) 
        {
            Serial.println("Error: File not found");
            
        } else 
        {
            Serial.println("File found");
        } 

        // Check the file size
        if (file.size() == 0) 
        {
            Serial.println("Error: File is empty");
            file.close();
            return;
        }

        // Skip header row (optional, adjust if no header)
        if(file.available())
        {
            String header = file.readStringUntil('\n');
            Serial.println("I am here");
        }
        else 
        {
            Serial.println("Error: Unable to read the header line");
            file.close();
            return;
        }

        // Read and parse the rest of the file content
        while(file.available())
        {
            String line = file.readStringUntil('\n');

            // Skip empty lines
            if (line.length() > 0)
            {
                parseCsvLine(line, config);

                // Print parsed values (for verification)
                Serial.println("Parsed UserConfig values:");
                Serial.println("sensorID: " + String(config.sensorID));
                Serial.println("destID: " + String(config.destID));
                Serial.println("role: " + String(config.role));
                Serial.println("gasThreshold: " + String(config.gasThreshold));
                Serial.println("tempThreshold: " + String(config.tempThreshold));
                Serial.println("flameThreshold: " + String(config.flameThreshold));
                Serial.println("sleepTime: " + String(config.sleepTime));
                Serial.println("dutyCycle: " + String(config.dutyCycle));
            }
        }

        file.close();

        return;
    }
    
    void UserInitConfiguration::printUserConfig(const UserConfig& userConfig)
    {
        Serial.println("");
        Serial.println("User Configuration:");
        Serial.print("Sensor ID: ");
        Serial.println(userConfig.sensorID);
        Serial.print("Destination ID: ");
        Serial.println(userConfig.destID);
        Serial.print("Role: ");
        Serial.println(userConfig.role);
        Serial.print("Flame Threshold: ");
        Serial.println(userConfig.flameThreshold);
        Serial.print("Gas Threshold: ");
        Serial.println(userConfig.gasThreshold);
        Serial.print("Temperature Threshold: ");
        Serial.println(userConfig.tempThreshold);
        Serial.print("Sleep Time: ");
        Serial.println(userConfig.sleepTime);
        Serial.print("Duty Cycle: ");
        Serial.println(userConfig.dutyCycle);
        Serial.println("");
    
        return;
    }

} // namespace UserInitConfig