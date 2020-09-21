#pragma once


#include <RD.hpp>


class MulticastReceiver {
    public:
        /**
         *  @brief Create a multicast receiver.
         *  @param [in] group The multicast group address.
         *  @param [in] port The UDP port for the multicast receiver.
         *  @param [in] ipInterface The interface address, 0.0.0.0 for default interface.
         */
        MulticastReceiver(uint8_t* group, uint16_t port, uint8_t* ipInterface);

        /**
         *  @brief Delete the multicast receiver.
         */
        ~MulticastReceiver();

        /**
         *  @brief Start the multicast receiver.
         *  @param [in] callbaclDecode The decode callback function.
         */
        void Start(void (*callbackDecode)(const std::string&, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint&, const uint8_t*, const uint32_t));

        /**
         *  @brief Stop the multicast receiver.
         */
        void Stop(void);

    private:
        std::string group;                    ///< Multicast grounp address set at construction.
        uint16_t port;                        ///< UDP port set at construction.
        RD::Network::Endpoint groupEndpoint;  ///< The endpoint corresponding to @ref group and @ref port set at construction.
        std::string ipInterface;              ///< Interface address set at construction.
        RD::Network::UDPSocket udpSocket;     ///< The multicast UDP socket.
        std::thread threadRX;                 ///< The main receiver thread.

        /* Receiver thread function */
        static void ThreadRX(MulticastReceiver* obj, void (*callbackDecode)(const std::string&, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint&, const uint8_t*, const uint32_t));
};

