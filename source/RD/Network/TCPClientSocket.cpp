/**
 *  @file TCPClientSocket.cpp
 *  @details Version 20200702.
 */
#include <Network.hpp>


RD::Network::TCPClientSocket::TCPClientSocket(){}

RD::Network::TCPClientSocket::~TCPClientSocket(){}

int RD::Network::TCPClientSocket::Connect(RD::Network::Endpoint& endpoint){
    return connect(_socket, (struct sockaddr*)(&endpoint.addr), sizeof(endpoint.addr));
}

int RD::Network::TCPClientSocket::Send(uint8_t *bytes, int size){
    if(!bytes)
        return -1;
    return send(_socket, (char*) bytes, size, 0);
}

int RD::Network::TCPClientSocket::Receive(uint8_t *bytes, int size){
    if(!bytes)
        return -1;
    return recv(_socket, (char*) bytes, size, 0);
}

int RD::Network::TCPClientSocket::Receive(uint8_t *bytes, int size, uint32_t timeout_s){
    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(_socket, &fds);
    tv.tv_sec = (long)timeout_s;
    tv.tv_usec = 0;
    if(select(_socket, &fds, 0, 0, &tv) <= 0)
        return -1;
    return Receive(bytes, size);
}

