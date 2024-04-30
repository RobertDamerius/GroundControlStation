#include <PolygonContainer.hpp>
#include <AppWindow.hpp>
#include <Configuration.hpp>


PolygonContainer::PolygonContainer(){
    lowerLimit = Configuration::gcs.defaultVehicleStyle.polygons.lowerLimit;
    upperLimit = Configuration::gcs.defaultVehicleStyle.polygons.upperLimit;
    color.r = static_cast<double>(Configuration::gcs.defaultVehicleStyle.polygons.color[0]) / 255.0;
    color.g = static_cast<double>(Configuration::gcs.defaultVehicleStyle.polygons.color[1]) / 255.0;
    color.b = static_cast<double>(Configuration::gcs.defaultVehicleStyle.polygons.color[2]) / 255.0;
    enable = Configuration::gcs.defaultVehicleStyle.polygons.enable;
    mtx = nullptr;
}

PolygonContainer::~PolygonContainer(){
    DeleteMutex();
}

void PolygonContainer::CreateMutex(void){
    DeleteMutex();
    mtx = new std::mutex;
}

void PolygonContainer::DeleteMutex(void){
    if(mtx){
        delete mtx;
        mtx = nullptr;
    }
}

void PolygonContainer::DeleteGL(void){
    if(!mtx) return;
    mtx->lock();
    for(auto&& p : polytopesToDelete){
        p.DeleteGL();
    }
    polytopesToDelete.clear();
    for(auto&& p : polygons){
        p.DeleteGL();
    }
    polygons.clear();
    mtx->unlock();
}

void PolygonContainer::Render(ShaderPolygon& shader){
    // Generate polytopes that have not been generated
    if(!mtx || !enable) return;
    mtx->lock();
    for(auto&& p : polygons){
        if(!p.IsGenerated()){
            p.GenerateGL();
        }
    }

    // Delete those polytopes that are no longer needed
    for(auto&& p : polytopesToDelete){
        p.DeleteGL();
    }
    polytopesToDelete.clear();

    // Draw all polygons
    shader.SetColor(color);
    for(auto&& p : polygons){
        p.navigation.position.x += appWindow.canvas.scene.renderingOffset.x;
        p.navigation.position.y += appWindow.canvas.scene.renderingOffset.z;
        p.navigation.position.z -= appWindow.canvas.scene.renderingOffset.y;
        p.navigation.UpdateModelMatrix();
        p.navigation.position.x -= appWindow.canvas.scene.renderingOffset.x;
        p.navigation.position.y -= appWindow.canvas.scene.renderingOffset.z;
        p.navigation.position.z += appWindow.canvas.scene.renderingOffset.y;
        shader.SetModelMatrix(p.navigation.modelMatrix);
        shader.SetClassification(p.classification);
        shader.SetVelocity(p.navigation.velocityNorth, p.navigation.velocityEast, p.navigation.r);
        p.Draw();
    }
    mtx->unlock();
}

void PolygonContainer::AddPolygons(std::vector<std::vector<std::array<double, 2>>>& polygons, std::vector<std::array<double, 3>> velocities, std::vector<uint8_t> classifications){
    if(polygons.size() != velocities.size())
        return;
    if(polygons.size() != classifications.size())
        return;

    // Create container of new polygon obstacles (create all polyhedrons)
    if(!mtx) return;
    std::vector<PolygonObstacle> newPolygons;
    for(size_t n = 0; n < polygons.size(); n++){
        newPolygons.push_back(PolygonObstacle());
        newPolygons.back().CreatePolyhedron(polygons[n], lowerLimit, upperLimit);
        newPolygons.back().navigation.velocityNorth = velocities[n][0];
        newPolygons.back().navigation.velocityEast = velocities[n][1];
        newPolygons.back().navigation.r = velocities[n][2];
        newPolygons.back().classification = classifications[n];
    }

    // Add new polygon obstacles to internal container
    mtx->lock();
    this->polygons.insert(this->polygons.end(), std::make_move_iterator(newPolygons.begin()), std::make_move_iterator(newPolygons.end()));
    mtx->unlock();
}

void PolygonContainer::ClearPolygons(void){
    if(!mtx) return;
    mtx->lock();
    for(auto&& p : polygons){
        if(p.IsGenerated()){
            polytopesToDelete.push_back(p);
        }
    }
    polygons.clear();
    mtx->unlock();
}

PolygonContainer& PolygonContainer::operator=(const PolygonContainer& rhs){
    this->polygons = rhs.polygons;
    this->polytopesToDelete = rhs.polytopesToDelete;
    this->upperLimit = rhs.upperLimit;
    this->lowerLimit = rhs.lowerLimit;
    this->color = rhs.color;
    this->enable = rhs.enable;
    this->segmentInfo = rhs.segmentInfo;
    return *this;
}

