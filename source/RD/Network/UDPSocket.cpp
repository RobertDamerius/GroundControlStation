/**
 *  @file UDPSocket.cpp
 *  @details Version 20210203.
 */
#include <Network.hpp>


RD::Network::UDPSocket::UDPSocket(){}

RD::Network::UDPSocket::~UDPSocket(){
    Close();
}

int RD::Network::UDPSocket::Open(void){
    if((_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;
    #ifdef _WIN32
    BOOL bNewBehavior = FALSE;
    DWORD dwBytesReturned = 0;
    WSAIoctl(_socket, _WSAIOW(IOC_VENDOR, 12), &bNewBehavior, sizeof bNewBehavior, NULL, 0, &dwBytesReturned, NULL, NULL);
    #endif
    return 0;
}

int RD::Network::UDPSocket::SendTo(RD::Network::Endpoint& endpoint, uint8_t *bytes, int size){
    if(!bytes)
        return -1;
    return sendto(_socket, (const char*) bytes, size, 0, (const struct sockaddr*) &endpoint.addr, sizeof(endpoint.addr));
}

int RD::Network::UDPSocket::Broadcast(uint16_t destinationPort, uint8_t *bytes, int size){
    int oldValue;
    int newValue = 1;
    struct sockaddr_in broadcast_addr;
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons((int)destinationPort);
    broadcast_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    #ifndef _WIN32
    socklen_t optLen = sizeof(int);
    #else
    int optLen = sizeof(int);
    #endif
    if(getsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&oldValue, &optLen))
        return -1;
    if(setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (const char*)&newValue, optLen))
        return -1;
    int tx = sendto(_socket, (const char*) bytes, size, 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));
    if(setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (const char*)&oldValue, optLen))
        return -1;
    return tx;
}

int RD::Network::UDPSocket::ReceiveFrom(RD::Network::Endpoint& endpoint, uint8_t *bytes, int size){
    if(!bytes)
        return -1;
    endpoint.Reset();
    #ifndef _WIN32
    socklen_t address_size = sizeof(endpoint.addr);
    #else
    int address_size = sizeof(endpoint.addr);
    #endif
    int result = recvfrom(_socket, (char*) bytes, size, 0, (struct sockaddr*) &endpoint.addr, &address_size);
    endpoint.UpdateID();
    return result;
}

int RD::Network::UDPSocket::ReceiveFrom(RD::Network::Endpoint& endpoint, uint8_t *bytes, int size, uint32_t timeout_s){
    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(_socket, &fds);
    tv.tv_sec = (long)timeout_s;
    tv.tv_usec = 0;
    if(select(_socket, &fds, 0, 0, &tv) <= 0)
        return -1;
    return ReceiveFrom(endpoint, bytes, size);
}

int RD::Network::UDPSocket::JoinMulticastGroup(const char* strGroup, const char* strInterface){
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(strGroup);
    mreq.imr_interface.s_addr = strInterface ? inet_addr(strInterface) : htonl(INADDR_ANY);
    return SetOption(IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void*) &mreq, sizeof(mreq));
}

int RD::Network::UDPSocket::LeaveMulticastGroup(const char* strGroup, const char* strInterface){
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(strGroup);
    mreq.imr_interface.s_addr = strInterface ? inet_addr(strInterface) : htonl(INADDR_ANY);
    return SetOption(IPPROTO_IP, IP_DROP_MEMBERSHIP, (const void*) &mreq, sizeof(mreq));
}

int RD::Network::UDPSocket::SetMulticastInterface(const char* ip, const char* strInterface){
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(ip);
    mreq.imr_interface.s_addr = strInterface ? inet_addr(strInterface) : htonl(INADDR_ANY);
    return SetOption(IPPROTO_IP, IP_MULTICAST_IF, (const void*) &mreq, sizeof(mreq));
}

int RD::Network::UDPSocket::SetMulticastTTL(uint8_t ttl){
    return SetOption(IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
}

