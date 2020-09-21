#pragma once


#include <RD.hpp>
#include <UnicastReceiver.hpp>
#include <MulticastReceiver.hpp>
#include <NetworkVehicleDatabase.hpp>


class NetworkManager {
    public:
        /**
         *  @brief Start the network manager.
         */
        static void Start(void);

        /**
         *  @brief Stop the network manager.
         */
        static void Stop(void);

    private:
        static std::vector<UnicastReceiver*> unicastReceivers;
        static std::vector<MulticastReceiver*> multicastReceivers;
        static std::unordered_map<std::string, NetworkVehicleDatabase*> vehicleDatabase;
        static std::mutex mtxVehicleDatabase;

        /* Decode function */
        static void Decode(const std::string& strInterfaceOfReceiver, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint& source, const uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Get the vehicle ID for an endpoint on an interface.
         *  @param [out] out The output that will be set to the corresponding vehicle ID if available.
         *  @param [in] group The group from which the message was received or 0.0.0.0:0 if unicast.
         *  @param [in] udpSocket The UDP socket at which the message was received.
         *  @param [in] in The network endpoint from which to obtain the vehicle ID.
         *  @param [in] strInterface Interface address string.
         *  @returns True if success, false otherwise. If this endpoint is not in the list, false will be returned.
         *  @note If this endpoint is not in the database then an info request message will be send to that endpoint automatically.
         */
        static bool GetVehicleID(VehicleID& out, RD::Network::Endpoint&, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint& in, const std::string& strInterface);
};

