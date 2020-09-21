/**
 *  @file TCPSocketBase.cpp
 *  @details Version 20200513.
 */
#include <Network.hpp>


RD::Network::TCPSocketBase::TCPSocketBase(){}

RD::Network::TCPSocketBase::~TCPSocketBase(){}

int RD::Network::TCPSocketBase::Open(void){
    if((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    return 0;
}

