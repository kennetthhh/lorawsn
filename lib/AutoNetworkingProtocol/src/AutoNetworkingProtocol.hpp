#ifndef AutoNetworkingProtocol_HPP
#define AutoNetworkingProtocol_HPP

#include <Arduino.h>
#include <set>
#include <map>
#include <vector>
#include <limits>
#include <LoRa.h>
#include "../../Globals/src/Globals.hpp"

namespace AutoNetwProc
{
    // Constants Definitions
    static constexpr uint8_t MIN_SENSOR_ID = std::numeric_limits<uint8_t>::min();
    static constexpr uint8_t MAX_SENSOR_ID = std::numeric_limits<uint8_t>::max();
    
    static constexpr uint8_t MIN_DEST_ID = std::numeric_limits<uint8_t>::min();
    static constexpr uint8_t MAX_DEST_ID = std::numeric_limits<uint8_t>::max();

    // AutoNetworkingProtocol Class
class AutoNetworkingProtocol
{
    public:
    std::vector<uint8_t> clusterMemberList = {1, 4, 7};

    // Enum for the roles of the nodes
    enum Roles
    {
        Cluster_Head,
        Gateway,
        Member_Node
    };
    
    // Default values for the roles and IDs
    // These values will be changed by the user
    // and according to the reelection protocol
    Roles role = Member_Node;
    uint8_t SENSOR_ID = 1;
    uint8_t DEST_ID = 0;

    private:

    /* Message Structure for Clustering */
    // Packed to avoid padding 
    #pragma pack(1)
    struct CommonClusteringMsg 
    {
        uint8_t sensorID;
        uint8_t numIncomingNodes;
        int rssi;
    };

    struct ClusteringMsg 
    {
        CommonClusteringMsg controlmsg;
        uint8_t checksum;
    };
    #pragma pack()


    public:
    /* Constructor */

    // Default Constructor
    AutoNetworkingProtocol() 
        : SENSOR_ID(1), DEST_ID(0), role(Member_Node) {}

    // Constructor with parameters to set initial IDs 
    // and role
    AutoNetworkingProtocol(uint8_t sensorID, 
                           uint8_t destID,
                           Roles   role) 
        : SENSOR_ID(sensorID), DEST_ID(destID), 
          role(role) {}

    
    // Setter
    bool setSensorID(int newSensorID);
    bool setDestID(int newDestID);
    bool setRole(AutoNetwProc::AutoNetworkingProtocol::Roles newRole);
    bool setAssignment();

    // Getter
    uint8_t getSensorID();
    uint8_t getDestID();
    Roles   getRole();
    std::vector<uint8_t> getClusterMemberList();

    // Auto Networking Protocol
    bool processReelection();

    // Clustering
    Roles doClustering();
    bool sendPing();
    bool sendCountsAndRssi();
    bool verifyNewClusterHead();
    bool newClusterHeadBroadcast();

    // Routing
    uint8_t doRouting();
    // void replyRREQ();

    //Misc
    void sendClusteringMsg(ClusteringMsg &msg);
    void receiveClusteringMsg(ClusteringMsg &msg);
    void sendHeartbeat(uint8_t dest_id);
    bool receiveHeartbeat(uint8_t &mySensorID);


    

}; // class AutoNetworkingProtocol

} // namespace AutoNetwProc

#endif