#include <Trajectory.hpp>
#include <Configuration.hpp>
#include <AppWindow.hpp>


Trajectory::Trajectory(){
    this->enable = Configuration::style.trajectoryEnable;
    this->color = Configuration::style.trajectoryColor;
    this->dt = 0.0;
    this->vehicleAlpha = Configuration::style.trajectoryVehicleAlpha;
    this->vehicleStride = Configuration::style.trajectoryVehicleStride;
    this->line.width = (GLfloat)Configuration::style.trajectoryLineWidth;
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

