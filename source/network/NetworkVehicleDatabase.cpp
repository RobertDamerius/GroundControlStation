#include <NetworkVehicleDatabase.hpp>
#include <Common.hpp>
#include <Vehicle.hpp>
#include <AppWindow.hpp>


NetworkVehicleDatabase::NetworkVehicleDatabase(){}

NetworkVehicleDatabase::~NetworkVehicleDatabase(){
    Stop();
}

void NetworkVehicleDatabase::Start(void){
    Stop();
}

void NetworkVehicleDatabase::Stop(void){
    // Clear database
    mtxRelations.lock();
    relations.clear();
    mtxRelations.unlock();
    mtxRequests.lock();
    requests.clear();
    mtxRequests.unlock();
}

bool NetworkVehicleDatabase::GetVehicleID(VehicleID& out, RD::Network::Endpoint& group, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint& in){
    bool result = false;
    mtxRelations.lock_shared();
    auto got = relations.find(in);
    if((result = (got != relations.end()))){
        out = got->second;
    }
    mtxRelations.unlock_shared();
    if(!result){
        // Check if we should send a request message
        bool sendRequest = false;
        mtxRequests.lock();

        // If this endpoint is not in the list, add it
        auto a = requests.find(in);
        if(a == requests.end()){
            requests.insert(std::pair<RD::Network::Endpoint, std::chrono::time_point<std::chrono::steady_clock>>(in, std::chrono::steady_clock::now()));
            sendRequest = true;
        }

        // Else, check time
        else{
            auto timeNow = std::chrono::steady_clock::now();
            if(std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - a->second).count() > 1000){
                a->second = timeNow;
                sendRequest = true;
            }
        }
        mtxRequests.unlock();

        // Send request if required (both unicast and multicast, that is the request is also send to the whole group)
        if(sendRequest){
            IMP::InfoRequestMessage msgRequest;
            msgRequest.timestamp = IMP::GetTimestampUTC();
            uint8_t buffer[8];
            int tx = msgRequest.Encode(&buffer[0], 8);
            if(tx > 0){
                (void) udpSocket.SendTo(in, &buffer[0], tx);
                if(group.GetID()){
                    (void) udpSocket.SendTo(group, &buffer[0], tx);
                }
            }
        }
    }
    return result;
}

void NetworkVehicleDatabase::AddNewEntry(RD::Network::Endpoint& source, IMP::InfoResponseMessage& msg){
    // Vehicle names that are equal to the name of the GCS are not allowed
    // This also prevents the GCS from adding itself to the database
    if(!msg.vehicleName.compare(strAppName)){
        return;
    }
    bool addedNewEntry = false;
    VehicleID id(msg.vehicleName);

    // Check if that endpoint is already in the database, if not, add
    mtxRelations.lock();
    auto got = relations.find(source);
    if((addedNewEntry = (got == relations.end()))){
        relations.insert(std::pair<RD::Network::Endpoint, VehicleID>(source, id));
    }
    mtxRelations.unlock();

    // Add vehicle to vehicle manager and remove from request database
    if(addedNewEntry){
        Vehicle v;
        v.name.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
        v.name.shadowColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);
        v.name.textToBeGenerated = msg.vehicleName;
        v.description = msg.description;
        v.CreateDefaultCuboid(msg.dimension[0], msg.dimension[1], msg.dimension[2], msg.offset[0], msg.offset[1], msg.offset[2]);
        appWindow.canvas.scene.vehicleManager.AddVehicle(id, v);
        GUILog(std::string("Linked ") + source.ToString() + std::string(" to vehicle \"") + msg.vehicleName + std::string("\"."));
        mtxRequests.lock();
        requests.erase(source);
        mtxRequests.unlock();
    }
}

