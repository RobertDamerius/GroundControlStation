#include <StyleConfiguration.hpp>
#include <Common.hpp>
#include <FileManager.hpp>


StyleConfiguration::StyleConfiguration(){
    Reset();
}

void StyleConfiguration::Read(void){
    // Clear current configuration
    Reset();

    // Read and parse the configuration file
    CFGParser cfg;
    uint32_t error = cfg.Parse(FileManager::GetName(FILE_CONFIGURATION_STYLE));
    if(error){
        GUILog(std::string("Syntax error in line ") + std::to_string(error) + std::string(" of style configuration file!"), 255, 0, 0);
        return;
    }
    FindCameraMode(cfg, cameraMode, "Camera.Mode");
    FindPositiveDouble(cfg, displayGamma, "Display.Gamma");
    FindColor(cfg, displayGroundColor, "Display.GroundColor");
    FindBool(cfg, altitudeToZero, "Vehicle.AltitudeToZero");
    FindBool(cfg, positionHistoryEnable, "PositionHistory.Enable");
    FindPositiveDouble(cfg, positionHistoryMaximumSOG, "PositionHistory.MaximumSOG");
    FindPositiveDouble(cfg, positionHistoryLineWidth, "PositionHistory.LineWidth");
    FindUint32(cfg, positionHistoryUpdatePeriodMs, "PositionHistory.UpdatePeriodMs");
    FindUint32(cfg, positionHistoryBufferSize, "PositionHistory.BufferSize");
    FindBool(cfg, waypointsEnable, "Waypoints.Enable");
    FindColor(cfg, waypointsColor, "Waypoints.Color");
    FindColor(cfg, waypointsVehicleColor, "Waypoints.VehicleColor");
    FindPositiveDouble(cfg, waypointsVehicleAlpha, "Waypoints.VehicleAlpha");
    FindPositiveDouble(cfg, waypointsLineWidth, "Waypoints.LineWidth");
    FindPositiveDouble(cfg, waypointsSymbolSize, "Waypoints.SymbolSize");
    FindBool(cfg, trajectoryEnable, "Trajectory.Enable");
    FindColor(cfg, trajectoryColor, "Trajectory.Color");
    FindColor(cfg, trajectoryVehicleColor, "Trajectory.VehicleColor");
    FindPositiveDouble(cfg, trajectoryVehicleAlpha, "Trajectory.VehicleAlpha");
    FindPositiveDouble(cfg, trajectoryVehicleStride, "Trajectory.VehicleStride");
    FindPositiveDouble(cfg, trajectoryLineWidth, "Trajectory.LineWidth");
    FindBool(cfg, compassEnable, "Compass.Enable");
    FindPositiveDouble(cfg, compassRadius, "Compass.Radius");
    FindColor(cfg, compassColor, "Compass.Color");
    FindBool(cfg, polygonsEnable, "Polygons.Enable");
    FindDouble(cfg, polygonsUpperLimit, "Polygons.UpperLimit");
    FindDouble(cfg, polygonsLowerLimit, "Polygons.LowerLimit");
    FindColor(cfg, polygonsColor, "Polygons.Color");
    FindPositiveDouble(cfg, sceneReflection, "Scene.Reflection");
    FindPositiveDouble(cfg, sceneSpecularity, "Scene.Specularity");
    FindPositiveDouble(cfg, sceneAmbient, "Scene.Ambient");
    FindPositiveDouble(cfg, sceneShininess, "Scene.Shininess");
    if(sceneReflection > 1.0){
        GUILog(std::string("Scene.Specularity in the style configuration file has an invalid value \"") + std::to_string(sceneReflection) + std::string("\" and is set to 1.0!"), 255, 0, 0);
        sceneReflection = 1.0;
    }
    if(waypointsVehicleAlpha > 1.0){
        GUILog(std::string("Waypoints.VehicleAlpha in the style configuration file has an invalid value \"") + std::to_string(waypointsVehicleAlpha) + std::string("\" and is set to 1.0!"), 255, 0, 0);
        waypointsVehicleAlpha = 1.0;
    }
    if(trajectoryVehicleAlpha > 1.0){
        GUILog(std::string("Trajectory.VehicleAlpha in the style configuration file has an invalid value \"") + std::to_string(trajectoryVehicleAlpha) + std::string("\" and is set to 1.0!"), 255, 0, 0);
        trajectoryVehicleAlpha = 1.0;
    }
}

void StyleConfiguration::Reset(void){
    cameraMode = false;
    displayGamma = GAMMA_DEFAULT;
    displayGroundColor = glm::dvec3(0.52549019f,0.61960784f,0.71372549f);
    altitudeToZero = false;
    positionHistoryEnable = false;
    positionHistoryMaximumSOG = 2.0;
    positionHistoryLineWidth = 2.0;
    positionHistoryUpdatePeriodMs = 20.0;
    positionHistoryBufferSize = 100;
    waypointsEnable = true;
    waypointsColor = glm::dvec3(0.0, 1.0, 0.0);
    waypointsVehicleColor = glm::dvec3(1.0, 1.0, 1.0);
    waypointsVehicleAlpha = 0.5;
    waypointsLineWidth = 2.0;
    waypointsSymbolSize = 1.0;
    trajectoryEnable = true;
    trajectoryColor = glm::dvec3(1.0, 0.0, 1.0);
    trajectoryVehicleColor = glm::dvec3(1.0, 1.0, 1.0);;
    trajectoryVehicleAlpha = 0.5;
    trajectoryVehicleStride = 1.0;
    trajectoryLineWidth = 2.0;
    compassEnable = true;
    compassRadius = 3.0;
    compassColor = glm::dvec3(1.0, 1.0, 1.0);
    polygonsEnable = true;
    polygonsUpperLimit = 1.0;
    polygonsLowerLimit = -1.0;
    polygonsColor = glm::dvec3(1.0, 1.0, 1.0);
    sceneReflection = 0.1;
    sceneSpecularity = 1.0;
    sceneAmbient = 0.1;
    sceneShininess = 32.0;
}

bool StyleConfiguration::FindCameraMode(CFGParser& cfg, bool& value, const char* key){
    std::string strKey(key);
    auto got = cfg.configuration.find(strKey);
    if(got == cfg.configuration.end())
        return false;
    if(1 != got->second.size()){
        GUILog(strKey + std::string(" in the style configuration file should only contain one value!"), 255, 0, 0);
        return false;
    }
    if(!got->second[0].compare("2D")){
        value = false;
        return true;
    }
    else if(!got->second[0].compare("3D")){
        value = true;
        return true;
    }
    GUILog(strKey + std::string(" in the style configuration file has an invalid value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
    return false;
}

bool StyleConfiguration::FindBool(CFGParser& cfg, bool& value, const char* key){
    std::string strKey(key);
    auto got = cfg.configuration.find(strKey);
    if(got == cfg.configuration.end())
        return false;
    if(1 != got->second.size()){
        GUILog(strKey + std::string(" in the style configuration file should only contain one value!"), 255, 0, 0);
        return false;
    }
    std::string s = got->second[0];
    for(auto&& c : s){
        c = ((c >= 'a') && (c <= 'z')) ? (c - 0x20) : c;
    }
    if(!s.compare("FALSE")){
        value = false;
        return true;
    }
    else if(!s.compare("TRUE")){
        value = true;
        return true;
    }
    GUILog(strKey + std::string(" in the style configuration file has an invalid value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
    return false;
}

bool StyleConfiguration::FindPositiveDouble(CFGParser& cfg, double& value, const char* key){
    std::string strKey(key);
    auto got = cfg.configuration.find(strKey);
    if(got == cfg.configuration.end())
        return false;
    if(1 != got->second.size()){
        GUILog(strKey + std::string(" in the style configuration file should only contain one value!"), 255, 0, 0);
        return false;
    }
    int num;
    double d;
    int r = sscanf(got->second[0].c_str(),"%lf%n",&d,&num);
    if((1 != r) || ((int)got->second[0].size() != num) || (d <= 0.0)){
        GUILog(strKey + std::string(" in the style configuration file has an invalid value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
        return false;
    }
    value = d;
    return true;
}

bool StyleConfiguration::FindDouble(CFGParser& cfg, double& value, const char* key){
    std::string strKey(key);
    auto got = cfg.configuration.find(strKey);
    if(got == cfg.configuration.end())
        return false;
    if(1 != got->second.size()){
        GUILog(strKey + std::string(" in the style configuration file should only contain one value!"), 255, 0, 0);
        return false;
    }
    int num;
    double d;
    int r = sscanf(got->second[0].c_str(),"%lf%n",&d,&num);
    if((1 != r) || ((int)got->second[0].size() != num)){
        GUILog(strKey + std::string(" in the style configuration file has an invalid value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
        return false;
    }
    value = d;
    return true;
}

bool StyleConfiguration::FindUint32(CFGParser& cfg, uint32_t& value, const char* key){
    std::string strKey(key);
    auto got = cfg.configuration.find(strKey);
    if(got == cfg.configuration.end())
        return false;
    if(1 != got->second.size()){
        GUILog(strKey + std::string(" in the style configuration file should only contain one value!"), 255, 0, 0);
        return false;
    }
    if(std::string::npos != got->second[0].find_first_of("-")){
        GUILog(strKey + std::string(" in the style configuration file must contain an unsigned value!"), 255, 0, 0);
        return false;
    }
    int num;
    uint32_t u;
    int r = sscanf(got->second[0].c_str(),"%u%n",&u,&num);
    if((1 != r) || ((int)got->second[0].size() != num)){
        GUILog(strKey + std::string(" in the style configuration file has an invalid value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
        return false;
    }
    value = u;
    return true;
}

bool StyleConfiguration::FindColor(CFGParser& cfg, glm::dvec3& value, const char* key){
    std::string strKey(key);
    auto got = cfg.configuration.find(strKey);
    if(got == cfg.configuration.end())
        return false;
    if(1 != got->second.size()){
        GUILog(strKey + std::string(" in the style configuration file should only contain one value!"), 255, 0, 0);
        return false;
    }
    if(std::string::npos != got->second[0].find_first_of("-")){
        GUILog(strKey + std::string(" in the style configuration file must contain only unsigned values!"), 255, 0, 0);
        return false;
    }
    int num;
    uint32_t red, green, blue;
    int r = sscanf(got->second[0].c_str(),"%u,%u,%u%n",&red,&green,&blue,&num);
    if((3 != r) || ((int)got->second[0].size() != num) || (red > 255) || (green > 255) || (blue > 255)){
        GUILog(strKey + std::string(" in the style configuration file has an invalid value \"") + got->second[0] + std::string("\"!"), 255, 0, 0);
        return false;
    }
    value.x = double(red) / 255.0;
    value.y = double(green) / 255.0;
    value.z = double(blue) / 255.0;
    return true;
}

