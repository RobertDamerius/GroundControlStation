#include <FileManager.hpp>


std::string FileManager::rootDirectory;


void FileManager::Initialize(const char* argv0){
    // Get root directory
    std::string str(argv0);
    std::size_t pos = str.find_last_of("/\\");
    rootDirectory.clear();
    if(std::string::npos != pos){
        rootDirectory = str.substr(0, pos);
        rootDirectory.push_back('\\');
        for(int i = 0; i < (int)rootDirectory.size(); i++){
            if('\\' == rootDirectory[i]){
                rootDirectory[i] = '/';
            }
            if((i > 0) && ('/' == rootDirectory[i])){
                if('/' == rootDirectory[i-1]){
                    rootDirectory.erase(rootDirectory.begin() + i);
                    i--;
                }
            }
        }
    }
}

std::string FileManager::GetName(file_t file){
    switch(file){
        case FILE_SHADER_POLYGON:               return rootDirectory + std::string(DIRECTORY_SHADER "Polygon" FILE_EXTENSION_SHADER);
        case FILE_SHADER_VEHICLE:               return rootDirectory + std::string(DIRECTORY_SHADER "Vehicle" FILE_EXTENSION_SHADER);
        case FILE_SHADER_TRANSPARENT_VEHICLE:   return rootDirectory + std::string(DIRECTORY_SHADER "TransparentVehicle" FILE_EXTENSION_SHADER);
        case FILE_SHADER_GROUNDPLANE:           return rootDirectory + std::string(DIRECTORY_SHADER "GroundPlane" FILE_EXTENSION_SHADER);
        case FILE_SHADER_ENVIRONMENT:           return rootDirectory + std::string(DIRECTORY_SHADER "Environment" FILE_EXTENSION_SHADER);
        case FILE_SHADER_POST_PROCESSING:       return rootDirectory + std::string(DIRECTORY_SHADER "PostProcessing" FILE_EXTENSION_SHADER);
        case FILE_SHADER_SCENE_TEXT:            return rootDirectory + std::string(DIRECTORY_SHADER "SceneText" FILE_EXTENSION_SHADER);
        case FILE_SHADER_LINE:                  return rootDirectory + std::string(DIRECTORY_SHADER "Line" FILE_EXTENSION_SHADER);
        case FILE_SHADER_LINE_STRIP:            return rootDirectory + std::string(DIRECTORY_SHADER "LineStrip" FILE_EXTENSION_SHADER);
        case FILE_SHADER_CIRCULAR_LINE_BUFFER:  return rootDirectory + std::string(DIRECTORY_SHADER "CircularLineBuffer" FILE_EXTENSION_SHADER);
        case FILE_CONFIGURATION_GCS:            return rootDirectory + std::string("GCS.json");
    }
    return std::string("");
}

std::string FileManager::GetRootDirectory(void){
    return FileManager::rootDirectory;
}

void FileManager::MakeStylesDirectory(void){
    std::filesystem::path dir = GetRootDirectory();
    dir /= DIRECTORY_STYLES;
    std::filesystem::create_directory(dir);
}


