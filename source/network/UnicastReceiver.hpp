#pragma once


#include <RD.hpp>


class UnicastReceiver {
    public:
        /**
         *  @brief Create an unicast receiver.
         *  @param [in] port The UDP port for the unicast receiver.
         *  @param [in] ipInterface The interface address, 0.0.0.0 for default interface.
         */
        UnicastReceiver(uint16_t port, uint8_t* ipInterface);

        /**
         *  @brief Delete the unicast receiver.
         */
        ~UnicastReceiver();

        /**
         *  @brief Start the unicast receiver.
         *  @param [in] callbaclDecode The decode callback function.
         */
        void Start(void (*callbackDecode)(const std::string&, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint&, const uint8_t*, const uint32_t));

        /**
         *  @brief Stop the unicast receiver.
         */
        void Stop(void);

    private:
        uint16_t port;                    ///< UDP port set at construction.
        std::string ipInterface;          ///< Interface address set at construction.
        RD::Network::UDPSocket udpSocket; ///< The multicast UDP socket.
        std::thread threadRX;             ///< The main receiver thread.

        /* Receiver thread function */
        static void ThreadRX(UnicastReceiver* obj, void (*callbackDecode)(const std::string&, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint&, const uint8_t*, const uint32_t));
};

