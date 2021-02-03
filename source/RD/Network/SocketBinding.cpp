/**
 *  @file SocketBinding.cpp
 *  @details Version 20210203.
 */
#include <Network.hpp>


RD::Network::SocketBinding::SocketBinding():_port(-1){}

RD::Network::SocketBinding::~SocketBinding(){}

void RD::Network::SocketBinding::Close(void){
    _port = -1;
    RD::Network::SocketBase::Close();
}

int RD::Network::SocketBinding::GetPort(void){
    return _port;
}

int RD::Network::SocketBinding::Bind(uint16_t port, const char *ip){
    if(_socket < 0)
        return -1;
    if(_port >= 0)
        return -1;
    struct sockaddr_in addr_this;
    addr_this.sin_addr.s_addr = ip ? inet_addr(ip) : htonl(INADDR_ANY);
    addr_this.sin_family = AF_INET;
    addr_this.sin_port = htons((int)port);
    int s = (int)_socket;
    if(!bind(s, (struct sockaddr *)&addr_this, sizeof(struct sockaddr_in))){
        _port = (int)port;
        return 0;
    }
    return -1;
}

int RD::Network::SocketBinding::Bind(uint16_t portBegin, uint16_t portEnd, const char *ip){
    if(_socket < 0)
        return -1;
    if(_port >= 0)
        return -1;
    struct sockaddr_in addr_this;
    addr_this.sin_addr.s_addr = ip ? inet_addr(ip) : htonl(INADDR_ANY);
    addr_this.sin_family = AF_INET;
    int port = (int)((portBegin < portEnd) ? portBegin : portEnd);
    int end = (int)((portEnd < portBegin) ? portBegin : portEnd);
    for(; port <= end; port++){
        addr_this.sin_port = htons(port);
        int s = (int)_socket;
        if(!bind(s, (struct sockaddr *)&addr_this, sizeof(struct sockaddr_in))){
            return (_port = port);
        }
    }
    return -1;
}

