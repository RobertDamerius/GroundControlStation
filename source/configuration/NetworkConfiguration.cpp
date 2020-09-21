#include <NetworkConfiguration.hpp>
#include <Common.hpp>
#include <CFGParser.hpp>
#include <FileManager.hpp>


void NetworkConfiguration::Read(void){
    // Clear current configuration
    this->unicast.clear();
    this->multicast.clear();
    this->joystick.clear();

    // Read and parse the configuration file
    CFGParser cfg;
    uint32_t error = cfg.Parse(FileManager::GetName(FILE_CONFIGURATION_NETWORK));
    if(error){
        GUILog(std::string("Syntax error in line ") + std::to_string(error) + std::string(" of network configuration file!"), 255, 0, 0);
        return;
    }

    // Decode the unicast configuration
    auto got = cfg.configuration.find(std::string("Network.Unicast"));
    if(got != cfg.configuration.end()){
        for(auto&& str: got->second){
            unsigned p = 0, a = 0, b = 0, c = 0, d = 0;
            int num;
            int r = sscanf(str.c_str(),"%u@%u.%u.%u.%u%n",&p,&a,&b,&c,&d,&num);
            if((5 != r) || ((int)str.size() != num) || (p > 65535) || (a > 255) || (b > 255) || (c > 255) || (d > 255)){
                GUILog(std::string("Invalid unicast configuration \"") + str + std::string("\"!"), 255, 0, 0);
                this->unicast.clear();
                return;
            }
            this->unicast.push_back(unicast_configuration());
            this->unicast.back().port = (uint16_t)p;
            this->unicast.back().ipInterface[0] = (uint8_t)a;
            this->unicast.back().ipInterface[1] = (uint8_t)b;
            this->unicast.back().ipInterface[2] = (uint8_t)c;
            this->unicast.back().ipInterface[3] = (uint8_t)d;
        }
    }

    // Decode the multicast configuration
    got = cfg.configuration.find(std::string("Network.Multicast"));
    if(got != cfg.configuration.end()){
        for(auto&& str: got->second){
            unsigned w = 0, x = 0, y = 0, z = 0, p = 0, a = 0, b = 0, c = 0, d = 0;
            int num;
            int r = sscanf(str.c_str(),"%u.%u.%u.%u:%u@%u.%u.%u.%u%n",&w,&x,&y,&z,&p,&a,&b,&c,&d,&num);
            if((9 != r) || ((int)str.size() != num) || (w > 255) || (x > 255) || (y > 255) || (z > 255) || (p > 65535) || (a > 255) || (b > 255) || (c > 255) || (d > 255)){
                GUILog(std::string("Invalid multicast configuration \"") + str + std::string("\"!"), 255, 0, 0);
                this->unicast.clear();
                this->multicast.clear();
                return;
            }
            this->multicast.push_back(multicast_configuration());
            this->multicast.back().ipGroup[0] = (uint8_t)w;
            this->multicast.back().ipGroup[1] = (uint8_t)x;
            this->multicast.back().ipGroup[2] = (uint8_t)y;
            this->multicast.back().ipGroup[3] = (uint8_t)z;
            this->multicast.back().port = (uint16_t)p;
            this->multicast.back().ipInterface[0] = (uint8_t)a;
            this->multicast.back().ipInterface[1] = (uint8_t)b;
            this->multicast.back().ipInterface[2] = (uint8_t)c;
            this->multicast.back().ipInterface[3] = (uint8_t)d;
        }
    }

    // Joystick configurations
    for(auto&& conf : cfg.configuration){
        std::string key = conf.first;
        if(!key.substr(0,8).compare("Joystick")){
            int num;
            uint8_t joystickID;
            if(1 == sscanf(key.c_str(),"Joystick%hhu.Destination%n",&joystickID,&num)){
                if((1 != conf.second.size()) || (num != (int)key.size())){
                    GUILog(std::string("Invalid syntax for unicast destination of joystick ") + std::to_string(joystickID) + std::string(". Entry is ignored."), 255, 0, 0);
                    continue;
                }
                unsigned w = 0, x = 0, y = 0, z = 0, p = 0, a = 0, b = 0, c = 0, d = 0;
                int r = sscanf(conf.second[0].c_str(),"%u.%u.%u.%u:%u@%u.%u.%u.%u%n",&w,&x,&y,&z,&p,&a,&b,&c,&d,&num);
                if((9 != r) || ((int)conf.second[0].size() != num) || (w > 255) || (x > 255) || (y > 255) || (z > 255) || (p > 65535) || (a > 255) || (b > 255) || (c > 255) || (d > 255)){
                    GUILog(std::string("Invalid syntax for unicast destination of joystick ") + std::to_string(joystickID) + std::string(". Entry is ignored."), 255, 0, 0);
                    continue;
                }
                joystick_configuration entry;
                entry.id = joystickID;
                entry.ipDestination[0] = w;
                entry.ipDestination[1] = x;
                entry.ipDestination[2] = y;
                entry.ipDestination[3] = z;
                entry.port = p;
                entry.ipInterface[0] = a;
                entry.ipInterface[1] = b;
                entry.ipInterface[2] = c;
                entry.ipInterface[3] = d;
                this->joystick.push_back(entry);
            }
        }
    }
}

