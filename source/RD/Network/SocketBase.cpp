/**
 *  @file SocketBase.cpp
 *  @details Version 20210203.
 */
#include <Network.hpp>


RD::Network::SocketBase::SocketBase():_socket(-1){}

RD::Network::SocketBase::~SocketBase(){
    Close();
}

bool RD::Network::SocketBase::IsOpen(void){
    return (_socket >= 0);
}

void RD::Network::SocketBase::Close(void){
    if(_socket >= 0){
        #ifdef _WIN32
        (void) shutdown(_socket, SD_BOTH);
        (void) closesocket(_socket);
        #else
        (void) shutdown(_socket, SHUT_RDWR);
        (void) close(_socket);
        #endif
        _socket = -1;
    }
}

int RD::Network::SocketBase::SetOption(int level, int optname, const void *optval, int optlen){
    #ifdef _WIN32
    return setsockopt(this->_socket, level, optname, (const char*)optval, optlen);
    #else
    return setsockopt(this->_socket, level, optname, optval, (socklen_t)optlen);
    #endif
}

int RD::Network::SocketBase::GetOption(int level, int optname, void *optval, int *optlen){
    #ifdef _WIN32
    return getsockopt(this->_socket, level, optname, (char*)optval, optlen);
    #else
    return getsockopt(this->_socket, level, optname, optval, (socklen_t*)optlen);
    #endif
}

int RD::Network::SocketBase::ReuseAddr(bool reuse){
    unsigned yes = (unsigned)reuse;
    return SetOption(SOL_SOCKET, SO_REUSEADDR, (const void*)&yes, sizeof(yes));
}

int RD::Network::SocketBase::ReusePort(bool reuse){
    unsigned yes = (unsigned)reuse;
    #ifdef _WIN32
    return SetOption(SOL_SOCKET, SO_REUSEADDR, (const void*)&yes, sizeof(yes));
    #else
    return SetOption(SOL_SOCKET, SO_REUSEPORT, (const void*)&yes, sizeof(yes));
    #endif
}

