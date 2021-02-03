/**
 *  @file TCPServerSocket.cpp
 *  @details Version 20210203.
 */
#include <Network.hpp>


RD::Network::TCPServerSocket::TCPServerSocket(){}

RD::Network::TCPServerSocket::~TCPServerSocket(){
    RD::Network::TCPSocketBase::Close();
}

int RD::Network::TCPServerSocket::Listen(int backlog){
    return listen(_socket, backlog);
}

int RD::Network::TCPServerSocket::Accept(TCPClientConnection *client){
    if(!client)
        return -1;
    #ifndef _WIN32
    socklen_t sin_size = sizeof(sockaddr_in);
    #else
    int sin_size = sizeof(sockaddr_in);
    #endif
    sockaddr_in client_addr;
    client->Reset();
    int clientSocket = accept(_socket, (sockaddr*)&client_addr , &sin_size);
    if(clientSocket < 0)
        return -1;
    client->_socket = clientSocket;
    client->SetAddress(client_addr);
    return 0;
}

