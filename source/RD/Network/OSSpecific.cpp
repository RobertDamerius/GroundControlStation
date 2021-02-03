/**
 *  @file OSSpecific.cpp
 *  @details Version 20210203.
 */
#include <Network.hpp>


#ifdef _WIN32
static WSADATA __rd_net_wsadata;
#endif


bool RD::Network::InitializeNetwork(void){
    // Make sure that the network module is terminated
    RD::Network::TerminateNetwork();

    // Windows specific
    #ifdef _WIN32
    if(WSAStartup(MAKEWORD(2, 2), &__rd_net_wsadata)){
        RD::Network::TerminateNetwork();
        return false;
    }
    if(LOBYTE(__rd_net_wsadata.wVersion) != 2 || HIBYTE(__rd_net_wsadata.wVersion) != 2){
        RD::Network::TerminateNetwork();
        return false;
    }
    #endif

    // Success
    return true;
}

void RD::Network::TerminateNetwork(void){
    #ifdef _WIN32
    WSACleanup();
    #endif
}

