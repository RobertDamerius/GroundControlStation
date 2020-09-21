#pragma once


#include <RD.hpp>
#include <VehicleID.hpp>
#include <InterModuleProtocol.hpp>


namespace std {
    template<> struct hash<RD::Network::Endpoint> {
        size_t operator()(const RD::Network::Endpoint& obj)const{
            return hash<uint64_t>()(obj.GetID());
        }
    };
}


class NetworkVehicleDatabase {
    public:
        /**
         *  @brief Create a network vehicle database.
         */
        NetworkVehicleDatabase();

        /**
         *  @brief Delete the network vehicle database.
         */
        ~NetworkVehicleDatabase();

        /**
         *  @brief Start the database.
         */
        void Start(void);

        /**
         *  @brief Stop the database.
         */
        void Stop(void);

        /**
         *  @brief Get the vehicle ID for an endpoint.
         *  @param [out] out The output that will be set to the corresponding vehicle ID if available.
         *  @param [in] group The group from which the message was received or 0.0.0.0:0 if unicast.
         *  @param [in] udpSocket The UDP socket at which the message was received.
         *  @param [in] in The network endpoint from which to obtain the vehicle ID.
         *  @returns True if success, false otherwise. If this endpoint is not in the list, false will be returned.
         *  @note If this endpoint is not in the database then an info request message will be send to that endpoint automatically.
         */
        bool GetVehicleID(VehicleID& out, RD::Network::Endpoint& group, RD::Network::UDPSocket& udpSocket, RD::Network::Endpoint& in);

        /**
         *  @brief Decode an info response message and add the vehicle to the database.
         *  @param [in] source The source address.
         *  @param [in] msg The decoded info response message.
         *  @details If this endpoint is already known, nothing will happen.
         */
        void AddNewEntry(RD::Network::Endpoint& source, IMP::InfoResponseMessage& msg);

    private:
        /* Relation database */
        std::unordered_map<RD::Network::Endpoint, VehicleID> relations;
        std::shared_mutex mtxRelations;

        /* Request database */
        std::unordered_map<RD::Network::Endpoint, std::chrono::time_point<std::chrono::steady_clock>> requests;
        std::mutex mtxRequests;
};

