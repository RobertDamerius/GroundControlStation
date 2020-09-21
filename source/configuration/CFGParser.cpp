/**
 *  @file CFGParser.cpp
 *  @details Version 20200920.
 */
#include <CFGParser.hpp>


uint32_t CFGParser::Parse(std::string filename){
    configuration.clear();
    std::ifstream file(filename);
    file.unsetf(std::ios::skipws);
    if(!file.is_open())
        return 0xFFFFFFFF;
    std::string line;
    uint32_t lineNumber = 0;
    std::string key;
    while(std::getline(file, line)){
        lineNumber++;

        // Replace all '\t' by ' '
        std::replace(line.begin(), line.end(), '\t', ' ');

        // Remove unallowed characters (only printable ASCII is allowed)
        line.erase(remove_if(line.begin(), line.end(), [](char c){return ((c < 0x20) || (c > 0x7E));}), line.end());

        // Remove all ' ' at beginning and at end
        std::size_t pos = line.find_first_not_of(" ");
        if(pos == std::string::npos){
            continue;
        }
        line = line.substr(pos);
        pos = line.find_last_not_of(" ");
        if(pos != std::string::npos){
            line = line.substr(0,pos+1);
        }

        // Empty line
        if(!line.size()){
            continue;
        }

        // Comment line
        if('#' == line[0]){
            continue;
        }

        // New key value
        if('[' == line[0]){
            pos = line.find_first_of("]");
            if(pos == std::string::npos){
                file.close();
                configuration.clear();
                return lineNumber;
            }
            if(std::string::npos != line.substr(pos+1).find_first_not_of(" ")){
                file.close();
                configuration.clear();
                return lineNumber;
            }
            key = line.substr(1,pos-1);
            configuration.insert(std::pair<std::string, std::vector<std::string>>(key, std::vector<std::string>()));
            continue;
        }

        // Add values for current key
        auto nomous = configuration.find(key);
        if(nomous == configuration.end()){
            return lineNumber;
        }
        nomous->second.push_back(line);
    }
    file.close();
    return 0;
}

bool CFGParser::FindSingleValue(bool& out, const char* key){
    std::string strKey(key);
    auto got = configuration.find(strKey);
    if(got == configuration.end())
        return false;
    if(1 != got->second.size())
        return false;
    std::string s = got->second[0];
    for(auto&& c : s){
        c = ((c >= 'a') && (c <= 'z')) ? (c - 0x20) : c;
    }
    if(!s.compare("FALSE")){
        out = false;
        return true;
    }
    else if(!s.compare("TRUE")){
        out = true;
        return true;
    }
    return false;
}

bool CFGParser::FindSingleValue(int32_t& out, const char* key){
    std::string strKey(key);
    auto got = configuration.find(strKey);
    if(got == configuration.end())
        return false;
    if(1 != got->second.size())
        return false;
    int num;
    int32_t i;
    int r = sscanf(got->second[0].c_str(),"%d%n",&i,&num);
    if((1 != r) || ((int)got->second[0].size() != num))
        return false;
    out = i;
    return true;
}

bool CFGParser::FindSingleValue(uint32_t& out, const char* key){
    std::string strKey(key);
    auto got = configuration.find(strKey);
    if(got == configuration.end())
        return false;
    if(1 != got->second.size())
        return false;
    if(std::string::npos != got->second[0].find_first_of("-"))
        return false;
    int num;
    uint32_t u;
    int r = sscanf(got->second[0].c_str(),"%u%n",&u,&num);
    if((1 != r) || ((int)got->second[0].size() != num))
        return false;
    out = u;
    return true;
}

bool CFGParser::FindSingleValue(double& out, const char* key){
    std::string strKey(key);
    auto got = configuration.find(strKey);
    if(got == configuration.end())
        return false;
    if(1 != got->second.size())
        return false;
    int num;
    double d;
    int r = sscanf(got->second[0].c_str(),"%lf%n",&d,&num);
    if((1 != r) || ((int)got->second[0].size() != num))
        return false;
    out = d;
    return true;
}

bool CFGParser::FindSingleValue(std::array<int32_t,3>& out, const char* key){
    std::string strKey(key);
    auto got = configuration.find(strKey);
    if(got == configuration.end())
        return false;
    if(1 != got->second.size())
        return false;
    int num;
    int32_t x, y, z;
    int r = sscanf(got->second[0].c_str(),"%d,%d,%d%n",&x,&y,&z,&num);
    if((3 != r) || ((int)got->second[0].size() != num))
        return false;
    out[0] = x;
    out[1] = y;
    out[2] = z;
    return true;
}

bool CFGParser::FindSingleValue(std::array<uint32_t,3>& out, const char* key){
    std::string strKey(key);
    auto got = configuration.find(strKey);
    if(got == configuration.end())
        return false;
    if(1 != got->second.size())
        return false;
    if(std::string::npos != got->second[0].find_first_of("-"))
        return false;
    int num;
    uint32_t x, y, z;
    int r = sscanf(got->second[0].c_str(),"%u,%u,%u%n",&x,&y,&z,&num);
    if((3 != r) || ((int)got->second[0].size() != num))
        return false;
    out[0] = x;
    out[1] = y;
    out[2] = z;
    return true;
}

bool CFGParser::FindSingleValue(std::array<double,3>& out, const char* key){
    std::string strKey(key);
    auto got = configuration.find(strKey);
    if(got == configuration.end())
        return false;
    if(1 != got->second.size())
        return false;
    int num;
    double x, y, z;
    int r = sscanf(got->second[0].c_str(),"%lf,%lf,%lf%n",&x,&y,&z,&num);
    if((3 != r) || ((int)got->second[0].size() != num))
        return false;
    out[0] = x;
    out[1] = y;
    out[2] = z;
    return true;
}

