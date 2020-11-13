#include <OriginConfiguration.hpp>
#include <Common.hpp>
#include <CFGParser.hpp>
#include <FileManager.hpp>


OriginConfiguration::OriginConfiguration(){
    this->latitude = 0.0;
    this->longitude = 0.0;
    this->altitude = 0.0;
    this->undefined = true;
}

void OriginConfiguration::Read(void){
    // Clear current configuration
    this->latitude = 0.0;
    this->longitude = 0.0;
    this->altitude = 0.0;
    double dlat = 0.0, dlon = 0.0, alt = 0.0;

    // Read and parse the configuration file
    CFGParser cfg;
    uint32_t error = cfg.Parse(FileManager::GetName(FILE_CONFIGURATION_ORIGIN));
    if(error){
        if(0xFFFFFFFF == error){
            GUILog(std::string("Origin configuration file not found!"), 255, 0, 0);
        }
        else{
            GUILog(std::string("Syntax error in line ") + std::to_string(error) + std::string(" of origin configuration file!"), 255, 0, 0);
        }
        return;
    }

    // Decode latitude
    bool latAvailable = false;
    auto got = cfg.configuration.find(std::string("Origin.Latitude"));
    if(got != cfg.configuration.end()){
        if(1 != got->second.size()){
            GUILog(std::string("Origin.Latitude in the origin configuration file should only contain one value!"), 255, 0, 0);
            return;
        }
        int num;
        int r = sscanf(got->second[0].c_str(),"%lf%n",&dlat,&num);
        if((1 != r) || ((int)got->second[0].size() != num) || (dlat < -180.0) || (dlat > 180.0)){
            GUILog(std::string("Invalid latitude origin value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
            return;
        }
        latAvailable = true;
    }

    // Decode longitude
    bool lonAvailable = false;
    got = cfg.configuration.find(std::string("Origin.Longitude"));
    if(got != cfg.configuration.end()){
        if(1 != got->second.size()){
            GUILog(std::string("Origin.Longitude in the origin configuration file should only contain one value!"), 255, 0, 0);
            return;
        }
        int num;
        int r = sscanf(got->second[0].c_str(),"%lf%n",&dlon,&num);
        if((1 != r) || ((int)got->second[0].size() != num) || (dlon < -90.0) || (dlon > 90.0)){
            GUILog(std::string("Invalid longitude origin value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
            return;
        }
        lonAvailable = true;
    }

    // Decode altitude
    bool altAvailable = false;
    got = cfg.configuration.find(std::string("Origin.Altitude"));
    if(got != cfg.configuration.end()){
        if(1 != got->second.size()){
            GUILog(std::string("Origin.Altitude in the origin configuration file should only contain one value!"), 255, 0, 0);
            return;
        }
        int num;
        int r = sscanf(got->second[0].c_str(),"%lf%n",&alt,&num);
        if((1 != r) || ((int)got->second[0].size() != num)){
            GUILog(std::string("Invalid altitude origin value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
            return;
        }
        altAvailable = true;
    }

    // Set values
    this->latitude = 0.0174532925199433 * dlat;
    this->longitude = 0.0174532925199433 * dlon;
    this->altitude = alt;
    this->undefined = !(latAvailable && lonAvailable && altAvailable);
}

