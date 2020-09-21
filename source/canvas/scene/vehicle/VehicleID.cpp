#include <VehicleID.hpp>


VehicleID::VehicleID(std::string name):name(name){}

VehicleID::~VehicleID(){}

VehicleID& VehicleID::operator=(const VehicleID& rhs){
    name = rhs.name;
    return *this;
}

