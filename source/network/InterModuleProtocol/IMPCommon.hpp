/**
 *  @file IMPCommon.hpp
 *  @brief Version 20210203.
 *  @details Macros that can be defined:
 *  IMP_DONT_INCLUDE_NETWORK_HEADERS ... If defined a custom htonl function is used to swap byte order, otherwise the htonl() is used (network headers are required).
 */
#pragma once


/* Include external C/C++ headers */
#include <vector>
#include <string>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <cmath>


/* Include OS-specific network headers */
#ifndef IMP_DONT_INCLUDE_NETWORK_HEADERS
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#endif /* IMP_DONT_INCLUDE_NETWORK_HEADERS */


/* Byte order swapping */
inline uint32_t IMPSwapByteOrder(uint32_t u){
    #ifdef IMP_DONT_INCLUDE_NETWORK_HEADERS
    union {
        uint32_t u;
        uint8_t b[4];
    } un;
    un.u = 1;
    if(un.b[0]){
        un.u = u;
        uint8_t h = un.b[0]; un.b[0] = un.b[3]; un.b[3] = h;
        h = un.b[1]; un.b[1] = un.b[2]; un.b[2] = h;
        return un.u;
    }
    return u;
    #else
    return htonl(u);
    #endif
}

