#include "AutoNetworkingProtocol.hpp"

// Setter Functions Body
bool AutoNetwProc::AutoNetworkingProtocol::setSensorID(int newSensorID)
{
    bool valid_sensorID = false;

    if (newSensorID >= AutoNetwProc::MIN_SENSOR_ID && 
        newSensorID <= AutoNetwProc::MAX_SENSOR_ID)
    {
        this->SENSOR_ID = newSensorID;
        valid_sensorID = true;
    }

    return valid_sensorID;
}

bool AutoNetwProc::AutoNetworkingProtocol::setDestID(int newDestID)
{
    bool valid_destID = false;

    if (newDestID >= AutoNetwProc::MIN_DEST_ID &&
        newDestID <= AutoNetwProc::MAX_DEST_ID)
    {
        this->DEST_ID = newDestID;
        valid_destID = true;
    }

    return valid_destID;
}

bool AutoNetwProc::AutoNetworkingProtocol::setRole(AutoNetwProc::AutoNetworkingProtocol::Roles newRole)
{
    bool valid_newRole = false;

    if (newRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head ||
        newRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Gateway ||
        newRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node)
    {
        this->role = newRole;
        valid_newRole = true;
    }

    return valid_newRole;
}

bool AutoNetwProc::AutoNetworkingProtocol::setAssignment()
{
    bool assignmentSet = false;

    if(this->SENSOR_ID == 1)
    {
      this->DEST_ID = 4;
      this->role = Cluster_Head;
      assignmentSet = true;
    }

    if(this->SENSOR_ID == 2 || this->SENSOR_ID == 3)
    {
      this->DEST_ID = 1;
      this->role = Member_Node;
      assignmentSet = true;
    }

    if(this->SENSOR_ID == 5 || this->SENSOR_ID == 6 || this->SENSOR_ID == 7)
    {
      this->DEST_ID = 4;
      this->role = Member_Node;
      assignmentSet = true;
    }

    if(this->SENSOR_ID == 4)
    {
      this->DEST_ID = 0;
      this->role = Cluster_Head;
      assignmentSet = true;
    }

    return assignmentSet;
}

// Getter Functions Body
uint8_t AutoNetwProc::AutoNetworkingProtocol::getSensorID()
{
    return this->SENSOR_ID;
}

uint8_t AutoNetwProc::AutoNetworkingProtocol::getDestID()
{
    return this->DEST_ID;
}

AutoNetwProc::AutoNetworkingProtocol::Roles AutoNetwProc::AutoNetworkingProtocol::getRole()
{
    return this->role;
}

void AutoNetwProc::AutoNetworkingProtocol::sendHeartbeat(uint8_t dest_id)
{
    // Send the message
    uint8_t heartbeatMessage[sizeof(uint16_t)];
    heartbeatMessage[0] = sendOutPing;
    heartbeatMessage[1] = dest_id;
    LoRa.beginPacket();
    LoRa.write(heartbeatMessage, sizeof(heartbeatMessage));
    LoRa.endPacket();
    #if DEBUG_ACK
    Serial.println("Heartbeat sent: " + String(heartbeatMessage[0]) + " " + String(heartbeatMessage[1]));
    #endif
    return;
}

bool AutoNetwProc::AutoNetworkingProtocol::receiveHeartbeat(uint8_t &mySensorID)
{
    // Flag
    bool receivedHeartbeat = false;

    // Receive the message
    uint8_t heartbeatMessage[sizeof(uint16_t)];
    LoRa.readBytes(heartbeatMessage, sizeof(heartbeatMessage));

    // Check if the message is a heartbeat
    if (heartbeatMessage[0] == sendOutPing && heartbeatMessage[1] == mySensorID)
    {
        receivedHeartbeat = true;
        #if DEBUG_ACK
        Serial.println("Heartbeat successfully received: " + String(heartbeatMessage[0]) + " " + String(heartbeatMessage[1]));
        #endif
    }

    return receivedHeartbeat;
}

std::vector<uint8_t> AutoNetwProc::AutoNetworkingProtocol::getClusterMemberList()
{
    return this->clusterMemberList;
}

/* ------------------------------------ Clustering ------------------------------------------ */
/* Clustering Algorithm */
int totalRssiSum = 0;  // Variable to store the total RSSI sum
int interval_ = 6000;
int interval_2 = 2000;
byte checksum = 99;
bool DISCARD = true;

//Send out and receive ping to and from other nearby sensors 
void AutoNetwProc::AutoNetworkingProtocol::sendClusteringMsg(ClusteringMsg &message) {
  // Broadcast the number of unique sensors to everyone else
  uint8_t buffer[sizeof(ClusteringMsg)];
  // Serialize message
  memcpy(buffer, &message, sizeof(ClusteringMsg));

  LoRa.beginPacket();
  LoRa.write(sendCountsRssi); // Message type
  LoRa.write(buffer, sizeof(buffer));
  LoRa.endPacket();
}

void AutoNetwProc::AutoNetworkingProtocol::receiveClusteringMsg(ClusteringMsg &message){
  if (LoRa.parsePacket() > 0) {
      byte messageType = LoRa.read(); //read out message type
      // Serial.println("sendCountsRssi" + String(messageType));
      uint8_t buffer[sizeof(ClusteringMsg)];
      LoRa.readBytes(buffer, sizeof(buffer));

    // Verify the checksum
    uint8_t calculatedChecksum = 66; // Exclude the checksum itself
    if (calculatedChecksum == buffer[sizeof(buffer) - 1]) 
    {
        // Checksum is valid, proceed to deserialize and process the message
      memcpy(&message, buffer, sizeof(ClusteringMsg));
      Serial.println("Clustering control message received with valid checksum");
      DISCARD = false;
        // Process the received message as needed
    } 
    else 
    {
        // Checksum is not valid, discard the message
        Serial.println("Invalid control message checksum, discarding the message");
    }
  }  
}

AutoNetwProc::AutoNetworkingProtocol::Roles AutoNetwProc::AutoNetworkingProtocol::doClustering()
{
    // Print out sensor id, role and dest id
    Serial.println("***-------------------***");
    Serial.println("Sensor ID: " + String(SENSOR_ID));
    Serial.println("Role: " + String(role));
    Serial.println("Destination ID: " + String(DEST_ID));
    Serial.println("***-------------------***");

    //0. Synchronize all the sensors in the cluster member list before starting the clustering process
    unsigned long syncStartTime = millis();
    unsigned long syncLastBroadcastTime = millis();
    unsigned long syncInterval = 0;
    long SYNC_PHASE_DURATION = 300000;  // 300 seconds

    // Ensure DEST_ID (cluster head) is not in the expected participants list
    clusterMemberList.erase(
        std::remove(clusterMemberList.begin(), clusterMemberList.end(), DEST_ID),
        clusterMemberList.end()
    );
    // Remove current sensor ID from the expected participants list
    clusterMemberList.erase(
        std::remove(clusterMemberList.begin(), clusterMemberList.end(), SENSOR_ID),
        clusterMemberList.end()
    );

    Serial.println("--------------------");
    Serial.println("Cluster Member List Size: " + String(clusterMemberList.size()));
    for (const byte &sensorID : clusterMemberList) 
    {
        Serial.println("Sensor ID: " + String(sensorID));
    }
    Serial.println("--------------------");

    std::set<byte> expectedParticipants(clusterMemberList.begin(), clusterMemberList.end());
    std::map<byte, int> syncMessageCount;  // Map to store synchronization message count for each participant

    int synced_cycles = 0;
    bool sensorsSynced = false;

    // If the cluster member list is empty, skip the clustering process
    // it is its own cluster head
    if (expectedParticipants.empty()) 
    {
        role = Cluster_Head;
        return role;
    }

    while (millis() - syncStartTime < SYNC_PHASE_DURATION && !sensorsSynced && !expectedParticipants.empty())
    {
        // Periodically broadcast intention to start clustering
        if (millis() - syncLastBroadcastTime > syncInterval)
        {
            // Broadcast synchronization message
            LoRa.beginPacket();
            LoRa.write(syncSensors);
            LoRa.write(SENSOR_ID);
            LoRa.write(checksum);  // Include checksum in the message
            LoRa.endPacket();

            syncLastBroadcastTime = millis();
            syncInterval = random(2000) + 1000;  // 2-3 seconds
        }

        // Receive synchronization messages
        if (LoRa.parsePacket() > 0)
        {
            byte messageType = LoRa.read();
            byte participantID = LoRa.read();
            byte receivedChecksum = LoRa.read();

            // Simple checksum verification
            if (receivedChecksum == checksum)
            {
                Serial.println("Received synchronization message from Sensor: " + String(participantID));

                // Increment the synchronization message count for the participant
                syncMessageCount[participantID]++;

                // Remove the participant from the expected participants list if it reaches the required count
                if (syncMessageCount[participantID] >= 5)
                {
                    expectedParticipants.erase(participantID);
                    Serial.println("Removed Sensor: " + String(participantID) + " from the expected participants list");
                }
                else
                {
                    Serial.println("Sensor: " + String(participantID) + " needs more synchronization messages");
                }
            }
            synced_cycles++;
        }

        // Check if all expected participants have sent enough synchronization messages
        if (expectedParticipants.empty() && synced_cycles > 100)
        {
            Serial.println("All cluster members have participated. Proceeding with clustering.");
            sensorsSynced = true;
        }
    }

    // 1. Sends Out Ping
    unsigned long startTime = millis();  // Timestamp when the loop started
    unsigned long lastSendTime = millis();
    std::set<byte> uniqueSensorIDs;  // Set to store unique sensor IDs
    std::map<byte, std::vector<int>> rssiValuesMap;  // Map to store sensor IDs and their first 20 RSSI values
    std::map<byte, int> rssiSumMap;  // Map to store the sum of RSSI values for each sensor ID
    std::map<byte, int> uniqueSensorCounts;  // Map to store the number of unique sensors each node has received

    unsigned long syncPhaseStartTime = startTime;
    unsigned long syncPhaseDuration = 10000;  // 10 seconds for synchronization phase

    while (millis() - startTime < 30000) 
    {  // Run the loop for 30 seconds

        // Check if it's still within the synchronization phase
        if (millis() - syncPhaseStartTime < syncPhaseDuration)
        {
            // Periodically broadcast intention to start clustering during the synchronization phase
            if (millis() - syncLastBroadcastTime > syncInterval)
            {
                // Broadcast synchronization message
                LoRa.beginPacket();
                LoRa.write(syncSensors);
                LoRa.write(SENSOR_ID);
                LoRa.write(checksum);  // Include checksum in the message
                LoRa.endPacket();

                syncLastBroadcastTime = millis();
                syncInterval = random(20) + 100;  // 2-3 seconds
            }
        }

        // Send out pings during the entire 30 seconds
        if (millis() - lastSendTime > interval_) 
        {
            // send out pings with a simple checksum
            LoRa.beginPacket();
            LoRa.write(sendOutPing); // Message type
            LoRa.write(SENSOR_ID);
            LoRa.write(122);  // Include checksum in the message
            LoRa.endPacket();

            lastSendTime = millis();            // timestamp the message
            interval_ = random(2000) + 1000;    // 2-3 seconds
        }

        // receive pings
        if (LoRa.parsePacket() == 0) 
        {
        // No incoming messages
        } 
        else 
        {
            //Read out message type
            byte messageType_ = LoRa.read();
            // Serial.println("sendOutPing: " + String(messageType_));
            // Check the integrity of the received ping message
            byte sender = LoRa.read();
            // Serial.println("Sender: " + String(sender));
            byte receivedChecksum = LoRa.read();

            // Simple checksum verification
            if (receivedChecksum == 122) 
            {
                delay(30);
                int rssi = LoRa.packetRssi();
                
                // Convert to Negative
                if (rssi > 0) {rssi = -rssi;}

                Serial.println("RSSI of " + String(sender) + " is " + String(rssi));

                // Save unique sensor IDs
                uniqueSensorIDs.insert(sender);

                // Store the first 20 RSSI values for each unique sensor ID
                if (rssiValuesMap[sender].size() < 20) {
                rssiValuesMap[sender].push_back(rssi);
                }

                // Sum up RSSI values for each unique sensor ID
                rssiSumMap[sender] += rssi;

                // Accumulate the total RSSI sum
                totalRssiSum += rssi;

                // Update the number of unique sensors received by the sender
                uniqueSensorCounts[sender] = uniqueSensorIDs.size();
            } 
            else 
            {
                Serial.println("Invalid checksum, discarding the message");
            }
        }
    }

    // Print the sum of RSSI values for each unique sensor ID
    for (const byte &sensorID : uniqueSensorIDs) 
    {
        Serial.println("Sensor ID: " + String(sensorID) + ", RSSI Sum: " + String(rssiSumMap[sensorID]));
    }
    // Print the total RSSI sum
    Serial.println("Total RSSI Sum: " + String(totalRssiSum));

    // Return the number of unique sensor IDs received
    int numberOfUniqueSensorIDs = uniqueSensorIDs.size();
    Serial.println("Node " + String(SENSOR_ID) + " received " + String(numberOfUniqueSensorIDs) + " unique sensors.");


    //2. Send out Unique Sensor Counts and RssiSumScores
    unsigned long startTime_ = millis();
    unsigned long lastBroadcastTime = millis(); 
    std::map<byte, CommonClusteringMsg> uniqueSensorInfo;  // Map to store unique sensor info
    while (millis() - startTime_ < 40000) 
    {  // Run the loop for 30 seconds

        // Periodically broadcast the number of unique sensors to everyone else
        if (millis() - lastBroadcastTime > interval_2) 
        {  // Broadcast every 5 seconds
            ClusteringMsg clustermsg;
            clustermsg.controlmsg.sensorID = SENSOR_ID;
            clustermsg.controlmsg.numIncomingNodes = numberOfUniqueSensorIDs;
            clustermsg.controlmsg.rssi = totalRssiSum;
            clustermsg.checksum = 66;
            sendClusteringMsg(clustermsg);

            lastBroadcastTime = millis();
            interval_2 = random(3000) + 1000;    // 2-3 seconds
        }

        unsigned long last_time = millis();
        int rssi = 0;
        // Receives the number of unique sensors from everyone
        if (LoRa.parsePacket() == 0) 
        {
        // No incoming messages
        // Serial.println("No message");
        } 
        else 
        {
            ClusteringMsg receivedClusterMsg;
            while (LoRa.available() && ((millis() - last_time) < 25000)) 
            {
                receiveClusteringMsg(receivedClusterMsg);
                // Update the information for the received sensor
                if(DISCARD == true)
                {
                // Serial.println("Not including this control message");
                }
                else 
                {
                    // Update the information for the sensor
                    uniqueSensorInfo[receivedClusterMsg.controlmsg.sensorID].sensorID = receivedClusterMsg.controlmsg.sensorID;
                    rssi = constrain(receivedClusterMsg.controlmsg.rssi,-99999, 0);
                    uniqueSensorInfo[receivedClusterMsg.controlmsg.sensorID].rssi = rssi;
                    uniqueSensorInfo[receivedClusterMsg.controlmsg.sensorID].numIncomingNodes = receivedClusterMsg.controlmsg.numIncomingNodes;
                }
                last_time = millis();
            }

            Serial.println("RSSI of Sensor: " + String(receivedClusterMsg.controlmsg.sensorID)
            + " is " + String(receivedClusterMsg.controlmsg.rssi) + " Incoming nodes: " +
            String(receivedClusterMsg.controlmsg.numIncomingNodes));
        }
    }
    Serial.println("Unique Sensor Information:");

    // Clear the cluster member list
    if (!clusterMemberList.empty()) {
        // Clear the list
        clusterMemberList.clear();
    }
    
    int i = 0;
    for (const auto &entry : uniqueSensorInfo) 
    {
        Serial.println("Sensor ID: " + String(entry.first) +
                    ", RSSI: " + String(entry.second.rssi) +
                    ", Incoming Nodes: " + String(entry.second.numIncomingNodes));

        // update the cluster member list
        clusterMemberList.push_back(entry.first);
    }

    // Print out members of the cluster
    Serial.println("--------------------");
    Serial.println("Cluster Member List:");
    for (const byte &sensorID : clusterMemberList) 
    {
        Serial.println("Sensor ID: " + String(sensorID));
    }
    Serial.println("--------------------");

    

    // Update Cluster Head
    byte clusterHeadID = SENSOR_ID;  // Default cluster head is the current node
    byte maxIncomingNodesSensorID = SENSOR_ID;
    byte maxRssiSensorID = SENSOR_ID;
    int maxIncomingNodes = 0;
    int maxRssi = -9999;  // Initialize with a low value
    int sizeOfList = uniqueSensorInfo.size();
    int cnt_size_list = 0;

    bool hasMostIncoming = false;
    bool hasSameNumberNodes = false; // Flag to track if there are different numbers of incoming nodes

    // Check for number of incoming nodes
    for (const auto &entry : uniqueSensorInfo) 
    {
        if (entry.second.numIncomingNodes > maxIncomingNodes) 
        {
            maxIncomingNodes = entry.second.numIncomingNodes;
            maxIncomingNodesSensorID = entry.first;
            Serial.println("Its max. incoming node(s) is: " + String(maxIncomingNodes) + " max. node(s) sensorID is: " + String(maxIncomingNodesSensorID));
        } 
        else if(entry.second.numIncomingNodes == maxIncomingNodes)
        {
            cnt_size_list++;
        }
    }

    if(cnt_size_list == sizeOfList - 1) 
    {
        hasSameNumberNodes = true;
    }

    // Check for lowest Rssi value
    if(hasSameNumberNodes)
    {
        for (const auto &entry : uniqueSensorInfo) 
        {
            if(entry.second.rssi > maxRssi)
            {
            maxRssi = entry.second.rssi;
            maxRssiSensorID = entry.first;
            Serial.println("Max Rssi is: " + String(maxRssi) + "; The sensor is: " + String(maxRssiSensorID));
            }
        }
    }
    Serial.println("Final max. rssi: " + String(maxRssi));
    Serial.println("That sensor is  " + String(maxRssiSensorID));
    // clusterHeadID = maxRssiSensorID;
    // If all sensors have the same number of incoming nodes, compare their RSSI values
    if (hasSameNumberNodes) 
    {
        clusterHeadID = maxRssiSensorID;
        Serial.println("I have same number of nodes");
    } 
    else 
    {
        clusterHeadID = maxIncomingNodesSensorID;
        Serial.println("I have different number of nodes");
    }

    Serial.println("Cluster Head: " + String(clusterHeadID));


    //3. Send out who everyone thinks is the cluster head to verify
    unsigned long startTime_3 = millis();
    unsigned long lastSent = millis();
    unsigned long interval_4 = 0;

    while(millis() - startTime_3 < 7000)
    {
        if(millis() - lastSent > interval_4)
        {
            // send out pings with a simple checksum
            LoRa.beginPacket();
            LoRa.write(verifyClusterHead); //Message type
            LoRa.write(clusterHeadID);
            LoRa.write(checksum);  // Include checksum in the message
            LoRa.endPacket();

            lastSent = millis();            // timestamp the message
            interval_4 = random(2000) + 1000;    // 2-3 seconds
        }

        // receive pings
        if (LoRa.parsePacket() == 0) 
        {
        // No incoming messages
        } 
        else 
        {
            //Read out message type
            byte messageType2 = LoRa.read();
            // Serial.println("Verify cluster head" + String(messageType2));
            // Check the integrity of the received ping message
            byte clusterHead = LoRa.read();
            // Serial.println("Sender: " + String(sender));
            byte receivedChecksum = LoRa.read();

            // Simple checksum verification
            if (receivedChecksum == checksum) 
            {
                Serial.println("Everyone is saying " + String(clusterHead) + " is Cluster Head!");
                DEST_ID = clusterHead;
                if (clusterHead == SENSOR_ID) {role = Cluster_Head;}
            }
        }
    }

    //4. The cluster head will broadcast the message to invite others to join 
    unsigned long startTime_4 = millis();
    unsigned long lastSent_ = millis();
    unsigned long interval_5 = 0;
    std::set<byte> uniqueClusterHeads;  // Set to store unique cluster head IDs
    while(millis() - startTime_4 < 15000)
    {
        if(millis() - lastSent_ > interval_5 && role == Cluster_Head)
        {
            // send out pings with a simple checksum
            Serial.println("Broadcasting that I (" + String(SENSOR_ID) + ") am the CH!!!");

            LoRa.beginPacket();
            LoRa.write(clusterHeadBroadcast);
            LoRa.write(SENSOR_ID);
            LoRa.write(checksum);  // Include checksum in the message
            LoRa.endPacket();

            lastSent_ = millis();            // timestamp the message
            interval_5 = random(2000) + 1000;    // 2-3 seconds
        }

        // receive pings
        if (LoRa.parsePacket() == 0) 
        {
        // No incoming messages
        } 
        else 
        {
            //Read out the message type 
            byte messageType3 = LoRa.read();
            // Serial.println("clusterHeadBroadcast" + String(messageType3));
            // Check the integrity of the received cluster head broadcast
            byte clusterHead = LoRa.read();
            byte receivedChecksum = LoRa.read();

            // Simple checksum verification
            if (receivedChecksum == checksum) 
            {
                Serial.println("My RSSI of the broadcast of the Cluster Head is: " + String(LoRa.packetRssi()));
                // Serial.println("My cluster head now is " + String(DEST_ID));
                // Store the unique cluster head ID
                uniqueClusterHeads.insert(clusterHead);
            }
        }
    }
    // Print out the unique cluster head IDs at the end of the loop
    Serial.println("Unique Cluster Head IDs:");
    for (const auto &clusterHeadID : uniqueClusterHeads) 
    {
        Serial.println("Cluster Head ID: " + String(clusterHeadID));
    }

    // Determine if the current node is a common gateway or a member node
    if(role == Cluster_Head || uniqueClusterHeads.empty())
    {
        role = Cluster_Head;
        Serial.println("I am a Cluster Head");
    }
    else
    {
        if (uniqueClusterHeads.size() > 1) 
        {
        Serial.println("I am a Common Gateway!");
        role = Gateway;
        } 
        else 
        {
        Serial.println("I am a Member Node!");
        role = Member_Node;
        DEST_ID = clusterHeadID;
        }
    }
    return role; 
}


/* ------------------------------------ Routing ------------------------------------------ */
uint8_t AutoNetwProc::AutoNetworkingProtocol::doRouting()
{
    unsigned long startT = millis();
    unsigned long lastSendT = millis();
    unsigned long interval_6 = 3000;
    int checksum_routing = 77;
    bool msg_received = false;
    // Sends Out Ping
    while (millis() - startT < 10000 && msg_received == false) 
    {  // Run the loop for 30 seconds
        if (millis() - lastSendT > interval_6) 
        {
            // send out pings with a simple checksum
            LoRa.beginPacket();
            LoRa.write(RREQ);
            Serial.println("RREQ: " + String(RREQ));
            LoRa.write(checksum_routing);  // Include checksum in the message
            LoRa.endPacket();

            lastSendT = millis();            // timestamp the message
            interval_6 = random(200) + 1000;    // 2-3 seconds
        }

        // receive pings
        if (LoRa.parsePacket() == 0) 
        {
            // No incoming messages
        } 
        else 
        {
            //Message type
            byte messageType4 = LoRa.read();
            Serial.println("RREP: " + String(messageType4));
            // Serial.println("Pi ID: " + String(Pi_ID));
            byte receivedChecksum = LoRa.read();
            // Serial.println("chksm: " + String(receivedChecksum));

            // Simple checksum verification
            if (receivedChecksum == checksum_routing && messageType4 == RREP) 
            {
                DEST_ID = 0;
                Serial.println("Message from PI received!");
                break;
            }
        }  
    }

    if(role == Cluster_Head)
    {
        Serial.println("My Pi ID is " + String(DEST_ID));
    }
    else
    {
        Serial.println("My Destination ID is " + String(DEST_ID));
    }

    return DEST_ID;
}

// Auto Networking Protocol
bool AutoNetwProc::AutoNetworkingProtocol::processReelection()
{
    Roles newRole = doClustering();
    uint8_t newDestID = doRouting();
    bool valid_processReelection = false;

    if ((newRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Cluster_Head ||
        newRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Member_Node  ||
        newRole == AutoNetwProc::AutoNetworkingProtocol::Roles::Gateway)
        &&
        (newDestID >= AutoNetwProc::MIN_DEST_ID && newDestID <= AutoNetwProc::MAX_DEST_ID))
    {
        valid_processReelection = true;
    }

    return valid_processReelection;
}

