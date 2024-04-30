#include <Trajectory.hpp>
#include <Configuration.hpp>
#include <AppWindow.hpp>


Trajectory::Trajectory(){
    this->enable = Configuration::gcs.defaultVehicleStyle.trajectory.enable;
    this->color.r = static_cast<double>(Configuration::gcs.defaultVehicleStyle.trajectory.color[0]) / 255.0;
    this->color.g = static_cast<double>(Configuration::gcs.defaultVehicleStyle.trajectory.color[1]) / 255.0;
    this->color.b = static_cast<double>(Configuration::gcs.defaultVehicleStyle.trajectory.color[2]) / 255.0;
    this->dt = 0.0;
    this->vehicleAlpha = Configuration::gcs.defaultVehicleStyle.trajectory.vehicleAlpha;
    this->vehicleStride = Configuration::gcs.defaultVehicleStyle.trajectory.vehicleStride;
    this->line.width = (GLfloat)Configuration::gcs.defaultVehicleStyle.trajectory.lineWidth;
}

Trajectory::~Trajectory(){}

void Trajectory::SetSampletime(double dt){
    this->dt = (dt < 0.0) ? 0.0 : dt;
}

void Trajectory::SetVehicleStride(double stride){
    this->vehicleStride = (stride < 0.0) ? 0.0 : stride;
}

void Trajectory::Generate(void){
    // Make sure that the line is not generated
    Delete();

    // Create and set vertices
    std::vector<LineVertex> vertices;
    LineVertex v;
    v.color[0] = v.color[1] = v.color[2] = 1.0f;
    for(auto&& wp : elements){
        if(wp.overridePosition){
            appWindow.canvas.scene.origin.LLA2NED(wp.position, glm::dvec3(wp.latitude(), wp.longitude(), wp.altitude));
            wp.overridePosition = false;
        }
        v.position[0] = (GLfloat)wp.position.x;
        v.position[1] = -(GLfloat)wp.position.z;
        v.position[2] = (GLfloat)wp.position.y;
        vertices.push_back(v);
    }

    // Generate line strip
    if(vertices.size()){
        line.SetVertices(vertices);
        line.Generate();
    }
    shouldGenerate = false;
}

Trajectory& Trajectory::operator=(const Trajectory& rhs){
    WaypointList::operator=(rhs);
    return *this;
}

