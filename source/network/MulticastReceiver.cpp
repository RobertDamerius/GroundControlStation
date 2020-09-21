#include <MulticastReceiver.hpp>
#include <Common.hpp>
#include <InterModuleProtocol.hpp>


MulticastReceiver::MulticastReceiver(uint8_t* group, uint16_t port, uint8_t* ipInterface){
    this->group = std::to_string(group[0]) + std::string(".") + std::to_string(group[1]) + std::string(".") + std::to_string(group[2]) + std::string(".") + std::to_string(group[3]);
    this->port = port;
    this->groupEndpoint.DecodeFromString(this->group + std::string(":") + std::to_string(this->port));
    this->ipInterface = std::to_string(ipInterface[0]) + std::string(".") + std::to_string(ipInterface[1]) + std::string(".") + std::to_string(ipInterface[2]) + std::string(".") + std::to_string(ipInterface[3]);
}

MulticastReceiver::~MulticastReceiver(){
    Stop();
}

void MulticastReceiver::Start(void (*callbackDecode)(const std::string&, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint&, const uint8_t*, const uint32_t)){
    // Make sure that the receiver is stopped
    Stop();

    // Open socket
    if(udpSocket.Open() < 0){
        std::ostringstream msg;
        msg << "Could not open multicast UDP socket (" << group << ":" << port << "@" << ipInterface << ")!";
        GUILog(msg.str(), 255, 0, 0);
        return;
    }

    // Set reuse port option and silently ignore errors
    (void) udpSocket.ReusePort(true);

    // Bind port (ALWAYS FOR ANY INTERFACE!)
    if(udpSocket.Bind(port) < 0){
        std::ostringstream msg;
        msg << "Could not bind port for multicast UDP socket (" << group << ":" << port << "@" << ipInterface << ")!";
        GUILog(msg.str(), 255, 0, 0);
        udpSocket.Close();
        return;
    }

    // Join multicast group
    if(udpSocket.JoinMulticastGroup(group.c_str(), ipInterface.size() ? ipInterface.c_str() : nullptr) < 0){
        std::ostringstream msg;
        msg << "Could not join multicast group for multicast UDP socket (" << group << ":" << port << "@" << ipInterface << ")!";
        GUILog(msg.str(), 255, 0, 0);
        udpSocket.Close();
        return;
    }

    // Set multicast interface
    if(ipInterface.size()){
        if(udpSocket.SetMulticastInterface(group.c_str(), ipInterface.c_str()) < 0){
            std::ostringstream msg;
            msg << "Could not set multicast interface for multicast UDP socket (" << group << ":" << port << "@" << ipInterface << ")!";
            GUILog(msg.str(), 255, 0, 0);
            udpSocket.Close();
            return;
        }
    }

    // Start the receiver thread
    if(callbackDecode){
        threadRX = std::thread(MulticastReceiver::ThreadRX, this, callbackDecode);
    }
}

void MulticastReceiver::Stop(void){
    // Leave multicast group and close socket
    (void)udpSocket.LeaveMulticastGroup(group.c_str(), ipInterface.size() ? ipInterface.c_str() : nullptr);

    // Stop the receiver thread
    udpSocket.Close();
    if(threadRX.joinable()){
        threadRX.join();
    }
}

void MulticastReceiver::ThreadRX(MulticastReceiver* obj, void (*callbackDecode)(const std::string&, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint&, const uint8_t*, const uint32_t)){
    uint8_t* bytes = new uint8_t[65535];
    for(;;){
        RD::Network::Endpoint source;
        int rx = obj->udpSocket.ReceiveFrom(source, &bytes[0], 65535);
        if(rx < 0){
            break;
        }
        if(!obj->udpSocket.IsOpen() || (obj->udpSocket.GetPort() < 0)){
            break;
        }
        if(rx > 0){
            callbackDecode(obj->ipInterface, obj->groupEndpoint, obj->udpSocket, source, &bytes[0], (uint32_t)rx);
        }
    }
    delete[] bytes;
}

