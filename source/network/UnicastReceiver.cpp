#include <UnicastReceiver.hpp>
#include <Common.hpp>
#include <InterModuleProtocol.hpp>


UnicastReceiver::UnicastReceiver(uint16_t port, uint8_t* ipInterface){
    this->port = port;
    this->ipInterface = std::to_string(ipInterface[0]) + std::string(".") + std::to_string(ipInterface[1]) + std::string(".") + std::to_string(ipInterface[2]) + std::string(".") + std::to_string(ipInterface[3]);
}

UnicastReceiver::~UnicastReceiver(){
    Stop();
}

void UnicastReceiver::Start(void (*callbackDecode)(const std::string&, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint&, const uint8_t*, const uint32_t)){
    // Make sure that the receiver is stopped
    Stop();

    // Open socket
    if(udpSocket.Open() < 0){
        std::ostringstream msg;
        msg << "Could not open UDP socket (" << port << "@" << ipInterface << ")!";
        GUILog(msg.str(), 255, 0, 0);
        return;
    }

    // Set reuse port option and silently ignore errors
    (void) udpSocket.ReusePort(true);

    // Bind port
    if(udpSocket.Bind(port, ipInterface.size() ? ipInterface.c_str() : nullptr) < 0){
        std::ostringstream msg;
        msg << "Could not bind port for UDP socket (" << port << "@" << ipInterface << ")!";
        GUILog(msg.str(), 255, 0, 0);
        udpSocket.Close();
        return;
    }

    // Start the receiver thread
    if(callbackDecode){
        threadRX = std::thread(UnicastReceiver::ThreadRX, this, callbackDecode);
    }
}

void UnicastReceiver::Stop(void){
    // Stop the receiver thread
    udpSocket.Close();
    if(threadRX.joinable()){
        threadRX.join();
    }
}

void UnicastReceiver::ThreadRX(UnicastReceiver* obj, void (*callbackDecode)(const std::string&, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint&, const uint8_t*, const uint32_t)){
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
            RD::Network::Endpoint emptyEndpoint(0,0,0,0,0);
            callbackDecode(obj->ipInterface, emptyEndpoint, obj->udpSocket, source, &bytes[0], (uint32_t)rx);
        }
    }
    delete[] bytes;
}

