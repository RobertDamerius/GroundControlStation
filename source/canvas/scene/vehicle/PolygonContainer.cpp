#include <PolygonContainer.hpp>
#include <AppWindow.hpp>
#include <Configuration.hpp>


PolygonContainer::PolygonContainer(){
    lowerLimit = Configuration::style.polygonsLowerLimit;
    upperLimit = Configuration::style.polygonsUpperLimit;
    color = Configuration::style.polygonsColor;
    enable = Configuration::style.polygonsEnable;
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

void PolygonContainer::Render(ShaderVehicle& shader){
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
        p.Draw();
    }
    mtx->unlock();
}

void PolygonContainer::AddPolygons(std::vector<std::vector<std::array<double, 2>>>& polygons, std::vector<std::array<double, 3>> velocities){
    if(polygons.size() != velocities.size())
        return;

    // Create container of new polygon obstacles (create all polyhedrons)
    if(!mtx) return;
    std::vector<PolygonObstacle> newPolygons;
    for(size_t n = 0; n < polygons.size(); n++){
        newPolygons.push_back(PolygonObstacle());
        newPolygons.back().CreatePolyhedron(polygons[n], lowerLimit, upperLimit);
        newPolygons.back().navigation.u = velocities[n][0];
        newPolygons.back().navigation.v = velocities[n][1];
        newPolygons.back().navigation.r = velocities[n][2];
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

