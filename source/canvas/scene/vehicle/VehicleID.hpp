#pragma once


#include <RD.hpp>


class VehicleID {
    public:
        std::string name;   ///< A unique name that indicates the vehicle.

        /**
         *  @brief Create a vehicle ID.
         *  @param [in] name The unique vehicle name, defaults to an empty string.
         */
        VehicleID(std::string name = std::string(""));

        /**
         *  @brief Copy constructor.
         */
        VehicleID(const VehicleID& id) = default;

        /**
         *  @brief Delete the vehicle ID.
         */
        ~VehicleID();

        /**
         *  @brief Assignment operator.
         */
        VehicleID& operator=(const VehicleID& rhs);

        /* Operators */
        inline bool operator==(const VehicleID& rhs)const{ return !this->name.compare(rhs.name); }
};


namespace std {
    template<> struct hash<VehicleID> {
        size_t operator()(const VehicleID& obj)const{
            return hash<std::string>()(obj.name);
        }
    };
}

