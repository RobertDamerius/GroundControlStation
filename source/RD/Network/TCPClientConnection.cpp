/**
 *  @file TCPClientConnection.cpp
 *  @details Version 20210203.
 */
#include <Network.hpp>


RD::Network::TCPClientConnection::TCPClientConnection(){}

RD::Network::TCPClientConnection::~TCPClientConnection(){}

int RD::Network::TCPClientConnection::Send(uint8_t *bytes, int size){
    if(!bytes)
        return -1;
    return send(_socket, (char*) bytes, size, 0);
}

int RD::Network::TCPClientConnection::Receive(uint8_t *bytes, int size){
    if(!bytes)
        return -1;
    return recv(_socket, (char*) bytes, size, 0);
}

int RD::Network::TCPClientConnection::Receive(uint8_t *bytes, int size, uint32_t timeout_s){
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

