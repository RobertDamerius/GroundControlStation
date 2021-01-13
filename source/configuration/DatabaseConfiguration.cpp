#include <DatabaseConfiguration.hpp>
#include <Common.hpp>
#include <CFGParser.hpp>
#include <FileManager.hpp>


void DatabaseConfiguration::Read(void){
    // Clear current configuration
    this->entry.clear();

    // Read and parse the configuration file
    CFGParser cfg;
    uint32_t error = cfg.Parse(FileManager::GetName(FILE_CONFIGURATION_DATABASE));
    if(error){
        if(0xFFFFFFFF == error){
            GUILog(std::string("Database configuration file not found!"), 255, 0, 0);
        }
        else{
            GUILog(std::string("Syntax error in line ") + std::to_string(error) + std::string(" of database configuration file!"), 255, 0, 0);
        }
        return;
    }
    auto got = cfg.configuration.find(std::string("Database.Initial"));
    if(got == cfg.configuration.end())
        return;

    // Decode the database configuration
    for(auto&& str: got->second){
        // First scan for IP:PORT@INTERFACE
        unsigned w = 0, x = 0, y = 0, z = 0, p = 0, a = 0, b = 0, c = 0, d = 0;
        int num;
        int r = sscanf(str.c_str(),"%u.%u.%u.%u:%u@%u.%u.%u.%u%n",&w,&x,&y,&z,&p,&a,&b,&c,&d,&num);
        if((9 != r) || (w > 255) || (x > 255) || (y > 255) || (z > 255) || (p > 65535) || (a > 255) || (b > 255) || (c > 255) || (d > 255)){
            GUILog(std::string("Invalid network settings for database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            this->entry.clear();
            return;
        }

        // Get remaining characters of line (line now starts with ',' after INTERFACE)
        std::string line = str.substr(num);

        // Scan dimension and offset
        float dx, dy, dz, ox, oy, oz;
        r = sscanf(line.c_str(),",%f,%f,%f,%f,%f,%f,%n",&dx,&dy,&dz,&ox,&oy,&oz,&num);
        if(6 != r){
            GUILog(std::string("Invalid dimension/offset settings for database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            this->entry.clear();
            return;
        }

        // Get remaining characters of line (line now starts with '"' of first string)
        line = line.substr(num);

        // Get name and description
        if(!line.size() || ('"' != line[0])){
            GUILog(std::string("Invalid vehicle name/description for database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            this->entry.clear();
            return;
        }
        auto se = line.find_first_of('"',1);
        if(se == std::string::npos){
            GUILog(std::string("Invalid vehicle name/description for database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            this->entry.clear();
            return;
        }
        std::string name = line.substr(1,se - 1);
        line = line.substr(se + 1);
        if((line.size() < 3) || (',' != line[0]) || ('"' != line[1]) || ('"' != line.back())){
            GUILog(std::string("Invalid vehicle name/description for database configuration \"") + str + std::string("\"!"), 255, 0, 0);
            this->entry.clear();
            return;
        }
        std::string description = line.substr(2, line.size() - 3);

        // Decoding succeeded, add new entry
        this->entry.push_back(database_entry_configuration());
        this->entry.back().ipAddress[0] = (uint8_t)w;
        this->entry.back().ipAddress[1] = (uint8_t)x;
        this->entry.back().ipAddress[2] = (uint8_t)y;
        this->entry.back().ipAddress[3] = (uint8_t)z;
        this->entry.back().port = (uint16_t)p;
        this->entry.back().ipInterface[0] = (uint8_t)a;
        this->entry.back().ipInterface[1] = (uint8_t)b;
        this->entry.back().ipInterface[2] = (uint8_t)c;
        this->entry.back().ipInterface[3] = (uint8_t)d;
        this->entry.back().dimension[0] = dx;
        this->entry.back().dimension[1] = dy;
        this->entry.back().dimension[2] = dz;
        this->entry.back().offset[0] = ox;
        this->entry.back().offset[1] = oy;
        this->entry.back().offset[2] = oz;
        this->entry.back().name = name;
        this->entry.back().description = description;
    }
}

