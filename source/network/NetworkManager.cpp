#include <NetworkManager.hpp>
#include <NetworkConfiguration.hpp>
#include <InterModuleProtocol.hpp>
#include <Common.hpp>
#include <RD.hpp>
#include <AppWindow.hpp>
#include <RigidBodyState.hpp>
#include <Configuration.hpp>


std::vector<UnicastReceiver*> NetworkManager::unicastReceivers;
std::vector<MulticastReceiver*> NetworkManager::multicastReceivers;
std::unordered_map<std::string, NetworkVehicleDatabase*> NetworkManager::vehicleDatabase;
std::mutex NetworkManager::mtxVehicleDatabase;


void NetworkManager::Start(void){
    // Make sure that the network manager is stopped
    NetworkManager::Stop();

    // Initialize network
    if(!RD::Network::InitializeNetwork()){
        GUILog(std::string("Could not start network manager: network initialization failed!"), 255, 0, 0);
        return;
    }

    // Start all unicast receivers
    for(auto&& conf : Configuration::network.unicast){
        UnicastReceiver* receiver = new UnicastReceiver(conf.port, &conf.ipInterface[0]);
        unicastReceivers.push_back(receiver);
        receiver->Start(&(NetworkManager::Decode));
    }

    // Start all multicast receivers
    for(auto&& conf : Configuration::network.multicast){
        MulticastReceiver* receiver = new MulticastReceiver(&conf.ipGroup[0], conf.port, &conf.ipInterface[0]);
        multicastReceivers.push_back(receiver);
        receiver->Start(&(NetworkManager::Decode));
    }

    // Get all network interfaces
    std::unordered_set<std::string> interfaces;
    for(auto&& conf : Configuration::network.unicast){
        interfaces.insert(std::to_string(conf.ipInterface[0]) + std::string(".") + std::to_string(conf.ipInterface[1]) + std::string(".") + std::to_string(conf.ipInterface[2]) + std::string(".") + std::to_string(conf.ipInterface[3]));
    }
    for(auto&& conf : Configuration::network.multicast){
        interfaces.insert(std::to_string(conf.ipInterface[0]) + std::string(".") + std::to_string(conf.ipInterface[1]) + std::string(".") + std::to_string(conf.ipInterface[2]) + std::string(".") + std::to_string(conf.ipInterface[3]));
    }

    // Start vehicle database for all interfaces
    for(auto&& strInterface : interfaces){
        std::pair<std::string, NetworkVehicleDatabase*> database(strInterface, new NetworkVehicleDatabase());
        database.second->Start();
        vehicleDatabase.insert(std::move(database));
    }
}

void NetworkManager::Stop(void){
    // Stop all unicast receivers
    for(size_t n = 0; n < unicastReceivers.size(); n++){
        unicastReceivers[n]->Stop();
        delete unicastReceivers[n];
    }
    unicastReceivers.clear();

    // Stop all multicast receivers
    for(size_t n = 0; n < multicastReceivers.size(); n++){
        multicastReceivers[n]->Stop();
        delete multicastReceivers[n];
    }
    multicastReceivers.clear();

    // Stop vehicle database for all interfaces
    for(auto&& database : vehicleDatabase){
        database.second->Stop();
        delete database.second;
    }
    vehicleDatabase.clear();

    // Terminate network
    RD::Network::TerminateNetwork();
}

void NetworkManager::Decode(const std::string& strInterfaceOfReceiver, RD::Network::Endpoint& group, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint& source, const uint8_t* bytes, const uint32_t length){
    IMP::InfoRequestMessage msgRequest;
    IMP::InfoResponseMessage msgInfoResponse;
    IMP::GuidanceMessage msgGuidance;
    IMP::LogMessage msgLog;
    IMP::NavigationMessage msgNavigation;
    IMP::ConvexPolygonMessage msgConvexPolygon;
    IMP::GuidanceTrajectoryMessage msgGuidanceTrajectory;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Info Request
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if(msgRequest.Decode(bytes, length) > 0){
        msgInfoResponse.timestamp = GetTimestampUTC();
        msgInfoResponse.dimension[0] = 0.0f;
        msgInfoResponse.dimension[1] = 0.0f;
        msgInfoResponse.dimension[2] = 0.0f;
        msgInfoResponse.offset[0] = 0.0f;
        msgInfoResponse.offset[1] = 0.0f;
        msgInfoResponse.offset[2] = 0.0f;
        msgInfoResponse.vehicleName = std::string(strAppName);
        msgInfoResponse.description = std::string(strVersion);
        uint8_t buffer[128];
        int tx = msgInfoResponse.Encode(&buffer[0], 128);
        if(tx > 0){
            udpSocket.SendTo(source, &buffer[0], tx);
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Info Response
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if(msgInfoResponse.Decode(bytes, length) > 0){
        mtxVehicleDatabase.lock();
        auto got = vehicleDatabase.find(strInterfaceOfReceiver);
        if(got != vehicleDatabase.end()){
            got->second->AddNewEntry(source, msgInfoResponse);
        }
        mtxVehicleDatabase.unlock();
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Navigation
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if(msgNavigation.Decode(bytes, length) > 0){
        VehicleID id;
        if(NetworkManager::GetVehicleID(id, group, udpSocket, source, strInterfaceOfReceiver)){
            RigidBodyState state(msgNavigation);
            appWindow.canvas.scene.vehicleManager.UpdateVehicleNavigation(id, state);
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Guidance
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if(msgGuidance.Decode(bytes, length) > 0){
        VehicleID id;
        if(NetworkManager::GetVehicleID(id, group, udpSocket, source, strInterfaceOfReceiver)){
            RigidBodyState state(msgGuidance);
            appWindow.canvas.scene.vehicleManager.UpdateVehicleGuidance(id, state);
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Guidance Trajectory
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if(msgGuidanceTrajectory.Decode(bytes, length) > 0){
        VehicleID id;
        if(NetworkManager::GetVehicleID(id, group, udpSocket, source, strInterfaceOfReceiver)){
            std::vector<RigidBodyState> list;
            for(auto&& state : msgGuidanceTrajectory.points){
                list.push_back(RigidBodyState(state));
            }
            if(msgGuidanceTrajectory.deltaTime > 0.0){
                appWindow.canvas.scene.vehicleManager.SetTrajectory(id, list, msgGuidanceTrajectory.deltaTime);
            }
            else{
                appWindow.canvas.scene.vehicleManager.SetWaypoints(id, list);
            }
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Polygon
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if(msgConvexPolygon.Decode(bytes, length) > 0){
        VehicleID id;
        if(NetworkManager::GetVehicleID(id, group, udpSocket, source, strInterfaceOfReceiver)){
            std::vector<std::vector<std::array<double, 2>>> polygons;
            std::vector<std::array<double, 3>> velocities;
            glm::dvec3 xyz;
            appWindow.canvas.scene.origin.LLA2NED(xyz, glm::dvec3(msgConvexPolygon.latitude, msgConvexPolygon.longitude, msgConvexPolygon.altitude));
            for(auto&& msgPolygon : msgConvexPolygon.polygons){
                polygons.push_back(std::move(msgPolygon.vertices));
                for(auto&& v : polygons.back()){
                    v[0] += xyz.x;
                    v[1] += xyz.y;
                }
                velocities.push_back(std::array<double, 3>({msgPolygon.velocityNorth, msgPolygon.velocityEast, msgPolygon.yawRate}));
            }
            appWindow.canvas.scene.vehicleManager.SetPolygons(id, msgConvexPolygon.segmentIndex, msgConvexPolygon.maxSegmentIndex, polygons, velocities);
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Log
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if(msgLog.Decode(bytes, length) > 0){
        VehicleID id;
        if(NetworkManager::GetVehicleID(id, group, udpSocket, source, strInterfaceOfReceiver)){
            appWindow.canvas.gui.AddLogEntry(LogEntry(msgLog.timestamp, id, msgLog.text, msgLog.color[0], msgLog.color[1], msgLog.color[2]));
            appWindow.textToSpeech.AddToQueue(msgLog.text);
        }
    }
}

bool NetworkManager::GetVehicleID(VehicleID& out, RD::Network::Endpoint& group, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint& in, const std::string& strInterface){
    const std::lock_guard<std::mutex> lockGuard(mtxVehicleDatabase);
    auto got = vehicleDatabase.find(strInterface);
    if(got == vehicleDatabase.end()){
        return false;
    }
    return got->second->GetVehicleID(out, group, udpSocket, in);
}

