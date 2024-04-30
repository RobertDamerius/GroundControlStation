#include <VehicleManager.hpp>
#include <AppWindow.hpp>
#include <Configuration.hpp>
#include <FileManager.hpp>
#include <Common.hpp>
#include <nlohmann/json.hpp>


VehicleManager::VehicleManager(){}

VehicleManager::~VehicleManager(){}

bool VehicleManager::Initialize(void){
    Terminate();
    return true;
}

void VehicleManager::Terminate(void){
    mtxVehicles.lock();
    for(auto&& v : vehicles){
        v.second.DeleteGL();
        v.second.polygons.DeleteMutex();
    }
    vehicles.clear();
    mtxVehicles.unlock();
}

void VehicleManager::AddVehicle(VehicleID& id, Vehicle& v){
    v.UpdateAABB();
    mtxVehicles.lock();
    std::pair<std::unordered_map<VehicleID, Vehicle>::iterator, bool> result = vehicles.insert(std::make_pair(id, v));
    if(result.second){
        vehiclesToGenerate.push_back(id);
        result.first->second.polygons.CreateMutex();
        
        // read style from database
        Vehicle& v = result.first->second;
        bool success = true;
        std::string filename = FileManager::GetRootDirectory() + DIRECTORY_STYLES + id.name + ".json";
        nlohmann::json jsonData;
        try{
            std::ifstream configurationFile(filename);
            jsonData = nlohmann::json::parse(configurationFile);
        }
        catch(...){
            success = false;
        }
        if(success){
            std::array<uint8_t,3> c;
            try{ v.visible                        = jsonData.at("visible");                                                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.useMesh                        = jsonData.at("useMesh");                                                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.altitudeToZero                 = jsonData.at("altitudeToZero");                                          } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.positionHistory.enable         = jsonData.at("positionHistory").at("enable");                            } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.positionHistory.SetMaximumSOG(   jsonData.at("positionHistory").at("maximumSOG").get<double>());         } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.positionHistory.width          = jsonData.at("positionHistory").at("lineWidth");                         } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.positionHistory.SetTimePeriodMs( jsonData.at("positionHistory").at("updatePeriodMs").get<uint32_t>());   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.positionHistory.SetDefaultBufferSize(jsonData.at("positionHistory").at("bufferSize").get<uint32_t>());   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.waypoints.enable               = jsonData.at("waypoints").at("enable");                                  } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ c                                = jsonData.at("waypoints").at("color");
                 v.waypoints.color = glm::vec3(double(c[0])/255.0, double(c[1])/255.0, double(c[2])/255.0);                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ c                                = jsonData.at("waypoints").at("vehicleColor");
                 v.waypoints.vehicleColor = glm::vec3(double(c[0])/255.0, double(c[1])/255.0, double(c[2])/255.0);          } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.waypoints.SetVehicleAlpha(       jsonData.at("waypoints").at("vehicleAlpha").get<double>());             } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.waypoints.SetLineWidth(          jsonData.at("waypoints").at("lineWidth").get<double>());                } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.waypoints.SetSymbolSize(         jsonData.at("waypoints").at("symbolSize").get<double>());               } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.trajectory.enable              = jsonData.at("trajectory").at("enable");                                 } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ c                                = jsonData.at("trajectory").at("color");
                 v.trajectory.color = glm::vec3(double(c[0])/255.0, double(c[1])/255.0, double(c[2])/255.0);                } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ c                                = jsonData.at("trajectory").at("vehicleColor");
                 v.trajectory.vehicleColor = glm::vec3(double(c[0])/255.0, double(c[1])/255.0, double(c[2])/255.0);         } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.trajectory.SetVehicleAlpha(      jsonData.at("trajectory").at("vehicleAlpha").get<double>());            } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.trajectory.SetVehicleStride(     jsonData.at("trajectory").at("vehicleStride").get<double>());           } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.trajectory.SetLineWidth(         jsonData.at("trajectory").at("lineWidth").get<double>());               } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.compass.enable                 = jsonData.at("compass").at("enable");                                    } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.compass.SetRadius(               jsonData.at("compass").at("radius").get<double>());                     } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ c                                = jsonData.at("compass").at("color");
                 v.compass.color = glm::vec3(double(c[0])/255.0, double(c[1])/255.0, double(c[2])/255.0);                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.polygons.enable                = jsonData.at("polygons").at("enable");                                   } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.polygons.upperLimit            = jsonData.at("polygons").at("upperLimit");                               } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ v.polygons.lowerLimit            = jsonData.at("polygons").at("lowerLimit");                               } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
            try{ c                                = jsonData.at("polygons").at("color");
                 v.polygons.color = glm::vec3(double(c[0])/255.0, double(c[1])/255.0, double(c[2])/255.0);                  } catch(const std::exception& e){ GUILog(std::string(e.what()), 255, 0, 0); }
        }
    }
    mtxVehicles.unlock();
}

void VehicleManager::UpdateVehicleNavigation(const VehicleID& id, RigidBodyState& stateNavigation){
    mtxVehicles.lock();
    std::unordered_map<VehicleID,Vehicle>::iterator got = vehicles.find(id);
    if(got != vehicles.end()){
        got->second.navigation = stateNavigation;
        got->second.timeOfLatestMessage.navigation = std::chrono::high_resolution_clock::now();
        if(got->second.navigation.overridePosition){
            appWindow.canvas.scene.origin.Update(got->second.navigation.latitude(), got->second.navigation.longitude(), got->second.navigation.altitude);
            appWindow.canvas.scene.origin.LLA2NED(got->second.navigation.position, glm::dvec3(got->second.navigation.latitude(), got->second.navigation.longitude(), got->second.navigation.altitude));
        }
    }
    mtxVehicles.unlock();
}

void VehicleManager::UpdateVehicleGuidance(const VehicleID& id, RigidBodyState& stateGuidance){
    mtxVehicles.lock();
    std::unordered_map<VehicleID,Vehicle>::iterator got = vehicles.find(id);
    if(got != vehicles.end()){
        got->second.guidance = stateGuidance;
        got->second.timeOfLatestMessage.guidance = std::chrono::high_resolution_clock::now();
        if(got->second.guidance.overridePosition){
            appWindow.canvas.scene.origin.Update(got->second.guidance.latitude(), got->second.guidance.longitude(), got->second.guidance.altitude);
            appWindow.canvas.scene.origin.LLA2NED(got->second.guidance.position, glm::dvec3(got->second.guidance.latitude(), got->second.guidance.longitude(), got->second.guidance.altitude));
        }
    }
    mtxVehicles.unlock();
}

void VehicleManager::SetWaypoints(const VehicleID& id, std::vector<RigidBodyState>& waypoints){
    mtxVehicles.lock();
    std::unordered_map<VehicleID,Vehicle>::iterator got = vehicles.find(id);
    if(got != vehicles.end()){
        got->second.waypoints.elements.swap(waypoints);
        got->second.waypoints.shouldGenerate = true;
        got->second.timeOfLatestMessage.guidanceWaypoint = std::chrono::high_resolution_clock::now();
    }
    mtxVehicles.unlock();
}

void VehicleManager::SetTrajectory(const VehicleID& id, std::vector<RigidBodyState>& trajectory, double dt){
    mtxVehicles.lock();
    std::unordered_map<VehicleID,Vehicle>::iterator got = vehicles.find(id);
    if(got != vehicles.end()){
        got->second.trajectory.SetSampletime(dt);
        got->second.trajectory.elements.swap(trajectory);
        got->second.trajectory.shouldGenerate = true;
        got->second.timeOfLatestMessage.guidanceTrajectory = std::chrono::high_resolution_clock::now();
    }
    mtxVehicles.unlock();
}

void VehicleManager::SetPolygons(const VehicleID& id, const uint8_t segmentIndex, const uint8_t maxSegmentIndex, std::vector<std::vector<std::array<double, 2>>>& polygons, std::vector<std::array<double, 3>> velocities, std::vector<uint8_t> classifications){
    if(polygons.size() != velocities.size())
        return;
    if(polygons.size() != classifications.size())
        return;
    mtxVehicles.lock();
    std::unordered_map<VehicleID,Vehicle>::iterator got = vehicles.find(id);
    if(got != vehicles.end()){
        // Clear all polygons if segment indices indicate a new polygon set
        int num = (1 + (int)maxSegmentIndex);
        int idx = (int)segmentIndex % num;
        if(((1 + (size_t)maxSegmentIndex) != got->second.polygons.segmentInfo.size()) || got->second.polygons.segmentInfo[idx]){
            // Number of segments changed or this segment index has already been added
            got->second.polygons.ClearPolygons();
            got->second.polygons.segmentInfo.clear();
            got->second.polygons.segmentInfo.resize(num, false);
        }

        // Add new polygon
        got->second.polygons.segmentInfo[idx] = true;
        got->second.polygons.AddPolygons(polygons, velocities, classifications);

        // Update time info
        got->second.timeOfLatestMessage.polygon = std::chrono::high_resolution_clock::now();
    }
    mtxVehicles.unlock();
}

void VehicleManager::Update(double dt){
    mtxVehicles.lock();
    for(auto&& v : vehicles){
        v.second.Update(dt);
    }
    mtxVehicles.unlock();
}

void VehicleManager::Render(ShaderVehicle& shaderVehicle, ShaderPolygon& shaderPolygon){
    mtxVehicles.lock();
    for(auto&& id : vehiclesToGenerate){
        std::unordered_map<VehicleID,Vehicle>::iterator got = vehicles.find(id);
        if(got != vehicles.end()){
            got->second.name.textToBeGenerated = got->first.name;
            got->second.GenerateGL();
        }
    }
    vehiclesToGenerate.clear();
    vehiclesToGenerate.shrink_to_fit();
    shaderVehicle.Use();
    shaderVehicle.SetReflection(Configuration::gcs.scene.reflection);
    shaderVehicle.SetSpecular(Configuration::gcs.scene.specularity);
    for(auto&& v : vehicles){
        if(!v.second.visible)
            continue;
        v.second.renderingState.position.x += appWindow.canvas.scene.renderingOffset.x;
        v.second.renderingState.position.y += appWindow.canvas.scene.renderingOffset.z;
        v.second.renderingState.position.z -= appWindow.canvas.scene.renderingOffset.y;
        if(v.second.altitudeToZero){
            v.second.renderingState.position.z = 0.0;
        }
        v.second.renderingState.UpdateModelMatrix();
        shaderVehicle.SetModelMatrix(v.second.renderingState.modelMatrix);
        shaderVehicle.SetColor(glm::vec3(1.0f));
        v.second.Draw();
    }
    shaderPolygon.Use();
    shaderPolygon.SetReflection(Configuration::gcs.scene.reflection);
    shaderPolygon.SetSpecular(Configuration::gcs.scene.specularity);
    for(auto&& v : vehicles){
        if(!v.second.visible)
            continue;
        shaderPolygon.SetModelMatrix(v.second.renderingState.modelMatrix);
        v.second.polygons.Render(shaderPolygon);
    }
    mtxVehicles.unlock();
}

void VehicleManager::RenderLabels(ShaderSceneText& shader){
    glm::dmat4 matPV = appWindow.canvas.scene.viewCamera.GetProjectionViewMatrix();
    mtxVehicles.lock();
    for(auto&& v : vehicles){
        if(!v.second.visible)
            continue;

        // Transform vehicle position to clip space coordinates
        glm::dvec4 positionClipSpace = matPV * glm::dvec4(v.second.renderingState.position.x - appWindow.canvas.scene.renderingOffset.x, -v.second.renderingState.position.z + appWindow.canvas.scene.renderingOffset.y, v.second.renderingState.position.y - appWindow.canvas.scene.renderingOffset.z, 1.0);
        positionClipSpace.z /= positionClipSpace.w;

        // If z-component is out of range, discard text
        if((positionClipSpace.z < -1.0) || (positionClipSpace.z > 1.0))
            continue;

        // Perform perspective division
        positionClipSpace.x /= positionClipSpace.w;
        positionClipSpace.y /= positionClipSpace.w;

        // Render text with shadow color (offset will be [1, -1] pixels)
        if(v.second.name.shadowColor.w > 0.0f){
            glm::ivec2 resolution = appWindow.GetSize();
            resolution.x = (resolution.x < 1) ? 1 : resolution.x;
            resolution.y = (resolution.y < 1) ? 1 : resolution.y;
            shader.SetOffsetClipSpace(glm::vec2(positionClipSpace.x + (2.0 / double(resolution.x)), positionClipSpace.y - (2.0 / double(resolution.y))));
            shader.SetColor(v.second.name.shadowColor);
            v.second.name.Draw();
        }

        // Render text with text color
        if(v.second.name.color.w > 0.0f){
            shader.SetOffsetClipSpace(glm::vec2(positionClipSpace.x, positionClipSpace.y));
            shader.SetColor(v.second.name.color);
            v.second.name.Draw();
        }
    }
    mtxVehicles.unlock();
}

void VehicleManager::RenderLines(ShaderTransparentVehicle& shaderTransparentVehicle, ShaderLine& shaderLine, ShaderLineStrip& shaderLineStrip, ShaderCircularLineBuffer& shaderCircularLineBuffer){
    glm::mat4 eye(1.0f);
    mtxVehicles.lock();
    for(auto&& v : vehicles){
        if(!v.second.visible)
            continue;

        // Position history
        shaderCircularLineBuffer.Use();
        shaderCircularLineBuffer.SetModelMatrix(eye);
        shaderCircularLineBuffer.SetPositionOffset(appWindow.canvas.scene.renderingOffset);
        shaderCircularLineBuffer.SetAltitudeGain(v.second.altitudeToZero ? 0.0f : 1.0f);
        v.second.positionHistory.Draw(shaderCircularLineBuffer);

        // Waypoints and trajectory
        shaderLineStrip.Use();
        shaderLineStrip.SetModelMatrix(eye);
        shaderLineStrip.SetPositionOffset(appWindow.canvas.scene.renderingOffset);
        shaderLineStrip.SetAltitudeGain(v.second.altitudeToZero ? 0.0f : 1.0f);
        v.second.waypoints.Draw(shaderLineStrip);
        v.second.trajectory.Draw(shaderLineStrip);
        shaderLineStrip.SetAltitudeGain(1.0f);

        // Compass
        v.second.compass.DrawLines(shaderLine, shaderLineStrip, v.second);

        // Transparent vehicles for waypoints
        shaderTransparentVehicle.Use();
        for(size_t n = 0; n < (size_t)v.second.waypoints.elements.size(); n++){
            if(!v.second.waypoints.enable){
                continue;
            }
            double posOffset = 0.01*(double(n) / double(v.second.waypoints.elements.size()) - 0.5);
            GLfloat alpha = (GLfloat)v.second.waypoints.GetVehicleAlpha();
            if(alpha){
                RigidBody rb;
                rb = v.second.waypoints.elements[n];
                rb.position.x += appWindow.canvas.scene.renderingOffset.x + posOffset;
                rb.position.y += appWindow.canvas.scene.renderingOffset.z + posOffset;
                rb.position.z -= appWindow.canvas.scene.renderingOffset.y + posOffset;
                if(v.second.altitudeToZero){
                    rb.position.z = 0.0;
                }
                rb.UpdateModelMatrix();
                shaderTransparentVehicle.SetModelMatrix(rb.modelMatrix);
                shaderTransparentVehicle.SetColor(v.second.waypoints.vehicleColor);
                shaderTransparentVehicle.SetAlpha(alpha);
                v.second.Draw();
            }
        }

        // Transparent vehicles for trajectory
        double dt = v.second.trajectory.GetVehicleStride();
        for(size_t n = 0; n < (size_t)v.second.trajectory.elements.size(); n++){
            if(!v.second.trajectory.enable){
                continue;
            }
            if(dt >= v.second.trajectory.GetVehicleStride()){
                double posOffset = 0.01*(double(n) / double(v.second.trajectory.elements.size()) - 0.5);
                GLfloat alpha = (GLfloat)v.second.trajectory.GetVehicleAlpha();
                if(alpha){
                    dt -= v.second.trajectory.GetVehicleStride();
                    RigidBody rb;
                    rb = v.second.trajectory.elements[n];
                    rb.position.x += appWindow.canvas.scene.renderingOffset.x + posOffset;
                    rb.position.y += appWindow.canvas.scene.renderingOffset.z + posOffset;
                    rb.position.z -= appWindow.canvas.scene.renderingOffset.y + posOffset;
                    if(v.second.altitudeToZero){
                        rb.position.z = 0.0;
                    }
                    rb.UpdateModelMatrix();
                    shaderTransparentVehicle.SetColor(v.second.trajectory.vehicleColor);
                    shaderTransparentVehicle.SetAlpha(alpha);
                    shaderTransparentVehicle.SetModelMatrix(rb.modelMatrix);
                    v.second.Draw();
                }
            }
            dt += v.second.trajectory.GetSampletime();
        }
    }
    mtxVehicles.unlock();
}

AxisAlignedBoundingBox VehicleManager::GetAABBOfVehicles(void){
    AxisAlignedBoundingBox result;
    bool first = true;
    mtxVehicles.lock();
    for(auto&& v : vehicles){
        if(!v.second.visible)
            continue;

        // Add AABB of polytope
        if(first) result = v.second.navigation.modelMatrix * v.second.aabb;
        else result += v.second.navigation.modelMatrix * v.second.aabb;
        first = false;

        // Add AABB of compass (height = 1.0)
        glm::vec3 position(v.second.navigation.position.x, -v.second.navigation.position.z, v.second.navigation.position.y);
        GLfloat radius = (GLfloat)v.second.compass.GetRadius();
        position -= glm::vec3(radius, 0.5f, radius);
        result += AxisAlignedBoundingBox(position, glm::vec3(radius+radius, 1.0f, radius+radius));
    }
    mtxVehicles.unlock();
    return result;
}

void VehicleManager::GetVehiclesList(std::vector<std::tuple<VehicleID, RigidBodyState, RigidBodyState>>& out){
    out.clear();
    mtxVehicles.lock();
    for(auto&& v : vehicles){
        out.push_back(std::tuple<VehicleID, RigidBodyState, RigidBodyState>(v.first, v.second.guidance, v.second.navigation));
    }
    mtxVehicles.unlock();
}

std::string VehicleManager::GetVehicleName(const VehicleID& id){
    std::string result("");
    mtxVehicles.lock();
    auto got = vehicles.find(id);
    if(got != vehicles.end()){
        result = got->first.name;
    }
    mtxVehicles.unlock();
    return result;
}

std::string VehicleManager::GetVehicleDescription(const VehicleID& id){
    std::string result("");
    mtxVehicles.lock();
    auto got = vehicles.find(id);
    if(got != vehicles.end()){
        result = got->second.description;
    }
    mtxVehicles.unlock();
    return result;
}

void VehicleManager::GetVehicleState(const VehicleID& id, RigidBodyState& guidance, RigidBodyState& navigation, Vehicle::TimeOfLatestMessage& timeOfLatestMessage){
    mtxVehicles.lock();
    auto got = vehicles.find(id);
    if(got != vehicles.end()){
        guidance = got->second.guidance;
        navigation = got->second.navigation;
        timeOfLatestMessage.navigation = got->second.timeOfLatestMessage.navigation;
        timeOfLatestMessage.guidance = got->second.timeOfLatestMessage.guidance;
        timeOfLatestMessage.guidanceTrajectory = got->second.timeOfLatestMessage.guidanceTrajectory;
        timeOfLatestMessage.guidanceWaypoint = got->second.timeOfLatestMessage.guidanceWaypoint;
        timeOfLatestMessage.polygon = got->second.timeOfLatestMessage.polygon;
    }
    mtxVehicles.unlock();
}

Vehicle* VehicleManager::LockVehicle(const VehicleID& id){
    mtxVehicles.lock();
    auto got = vehicles.find(id);
    if(got != vehicles.end()){
        return &(got->second);
    }
    mtxVehicles.unlock();
    return nullptr;
}

void VehicleManager::UnlockVehicle(void){
    mtxVehicles.unlock();
}

void VehicleManager::SaveVehicleStyle(const VehicleID& id){
    const std::lock_guard<std::mutex> lock(mtxVehicles);
    auto got = vehicles.find(id);
    if(got != vehicles.end()){
        Vehicle& v = got->second;

        // generate json file
        nlohmann::json jsonData;
        jsonData["visible"]                                    = v.visible;
        jsonData["useMesh"]                                    = v.useMesh;
        jsonData["altitudeToZero"]                             = v.altitudeToZero;
        jsonData["positionHistory"]["enable"]                  = v.positionHistory.enable;
        jsonData["positionHistory"]["maximumSOG"]              = v.positionHistory.GetMaximumSOG();
        jsonData["positionHistory"]["lineWidth"]               = v.positionHistory.width;
        jsonData["positionHistory"]["updatePeriodMs"]          = v.positionHistory.GetTimePeriodMs();
        jsonData["positionHistory"]["bufferSize"]              = v.positionHistory.GetBufferSize();
        jsonData["waypoints"]["enable"]                        = v.waypoints.enable;
        jsonData["waypoints"]["color"]                         = {static_cast<uint8_t>(v.waypoints.color.r * 255.0), static_cast<uint8_t>(v.waypoints.color.g * 255.0), static_cast<uint8_t>(v.waypoints.color.b * 255.0)};
        jsonData["waypoints"]["vehicleColor"]                  = {static_cast<uint8_t>(v.waypoints.vehicleColor.r * 255.0), static_cast<uint8_t>(v.waypoints.vehicleColor.g * 255.0), static_cast<uint8_t>(v.waypoints.vehicleColor.b * 255.0)};
        jsonData["waypoints"]["vehicleAlpha"]                  = v.waypoints.GetVehicleAlpha();
        jsonData["waypoints"]["lineWidth"]                     = v.waypoints.GetLineWidth();
        jsonData["waypoints"]["symbolSize"]                    = v.waypoints.GetSymbolSize();
        jsonData["trajectory"]["enable"]                       = v.trajectory.enable;
        jsonData["trajectory"]["color"]                        = {static_cast<uint8_t>(v.trajectory.color.r * 255.0), static_cast<uint8_t>(v.trajectory.color.g * 255.0), static_cast<uint8_t>(v.trajectory.color.b * 255.0)};
        jsonData["trajectory"]["vehicleColor"]                 = {static_cast<uint8_t>(v.trajectory.vehicleColor.r * 255.0), static_cast<uint8_t>(v.trajectory.vehicleColor.g * 255.0), static_cast<uint8_t>(v.trajectory.vehicleColor.b * 255.0)};
        jsonData["trajectory"]["vehicleAlpha"]                 = v.trajectory.GetVehicleAlpha();
        jsonData["trajectory"]["vehicleStride"]                = v.trajectory.GetVehicleStride();
        jsonData["trajectory"]["lineWidth"]                    = v.trajectory.GetLineWidth();
        jsonData["compass"]["enable"]                          = v.compass.enable;
        jsonData["compass"]["radius"]                          = v.compass.GetRadius();
        jsonData["compass"]["color"]                           = {static_cast<uint8_t>(v.compass.color.r * 255.0), static_cast<uint8_t>(v.compass.color.g * 255.0), static_cast<uint8_t>(v.compass.color.b * 255.0)};
        jsonData["polygons"]["enable"]                         = v.polygons.enable;
        jsonData["polygons"]["upperLimit"]                     = v.polygons.upperLimit;
        jsonData["polygons"]["lowerLimit"]                     = v.polygons.lowerLimit;
        jsonData["polygons"]["color"]                          = {static_cast<uint8_t>(v.polygons.color.r * 255.0), static_cast<uint8_t>(v.polygons.color.g * 255.0), static_cast<uint8_t>(v.polygons.color.b * 255.0)};

        // write to file
        FileManager::MakeStylesDirectory();
        std::string filename = FileManager::GetRootDirectory() + DIRECTORY_STYLES + got->first.name + ".json";
        try{
            std::ofstream outstream(filename);
            outstream << std::setw(4) << jsonData << std::endl;
        }
        catch(...){
            GUILog(std::string("Could not save \"") + filename + std::string("\"!"), 255, 0, 0);
        }
    }
}

