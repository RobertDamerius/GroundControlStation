#include <WaypointList.hpp>
#include <Configuration.hpp>
#include <RigidBody.hpp>
#include <AppWindow.hpp>


WaypointList::WaypointList(){
    this->enable = Configuration::style.waypointsEnable;
    this->color = glm::vec3(Configuration::style.waypointsColor);
    this->vehicleColor = glm::vec3(Configuration::style.waypointsVehicleColor);
    this->vehicleAlpha = Configuration::style.waypointsVehicleAlpha;
    this->symbolSize = Configuration::style.waypointsSymbolSize;
    this->line.width = (GLfloat)Configuration::style.waypointsLineWidth;
    this->shouldGenerate = false;
}

WaypointList::~WaypointList(){}

void WaypointList::Update(double dt){
    (void)dt;
}

void WaypointList::SetVehicleAlpha(double alpha){
    this->vehicleAlpha = (alpha < 0.0) ? 0.0 : ((alpha > 1.0) ? 1.0 : alpha);
}

void WaypointList::SetSymbolSize(double size){
    this->symbolSize = (size > 0.001) ? size : 0.001;
}

void WaypointList::SetLineWidth(double width){
    line.width = (GLfloat)((width > 0.001) ? width : 0.001);
}

void WaypointList::Generate(void){
    // Make sure that the line is not generated
    Delete();

    // Create and set vertices
    std::vector<LineVertex> vertices;
    LineVertex v;
    v.color[0] = v.color[1] = v.color[2] = 1.0f;
    GLfloat rmax = (GLfloat)(0.5 * this->symbolSize);
    GLfloat rmin = (GLfloat)(0.3 * this->symbolSize);
    for(auto&& wp : elements){
        if(wp.overridePosition){
            appWindow.canvas.scene.origin.LLA2NED(wp.position, glm::dvec3(wp.latitude(), wp.longitude(), wp.altitude));
            wp.overridePosition = false;
        }
        glm::vec3 pos((GLfloat)wp.position.x, -(GLfloat)wp.position.z, (GLfloat)wp.position.y);
        v.position[0] = pos.x;
        v.position[1] = pos.y;
        v.position[2] = pos.z;
        vertices.push_back(v);

        v.position[0] = pos.x + rmax;
        v.position[2] = pos.z;
        vertices.push_back(v);

        v.position[0] = pos.x;
        v.position[2] = pos.z - rmin;
        vertices.push_back(v);

        v.position[0] = pos.x - rmax;
        v.position[2] = pos.z;
        vertices.push_back(v);

        v.position[0] = pos.x;
        v.position[2] = pos.z + rmin;
        vertices.push_back(v);

        v.position[0] = pos.x + rmax;
        v.position[2] = pos.z;
        vertices.push_back(v);

        v.position[0] = pos.x;
        vertices.push_back(v);
    }

    // Generate line strip
    if(vertices.size()){
        line.SetVertices(vertices);
        line.Generate();
    }
    shouldGenerate = false;
}

void WaypointList::Delete(void){
    line.Delete();
}

void WaypointList::Draw(ShaderLineStrip& shader){
    if(shouldGenerate){
        Generate();
    }
    if(enable){
        shader.SetLineWidth(line.width);
        shader.SetColor(color);
        line.Draw();
    }
}

WaypointList& WaypointList::operator=(const WaypointList& rhs){
    this->enable = rhs.enable;
    this->elements = rhs.elements;
    this->line = rhs.line;
    this->symbolSize = rhs.symbolSize;
    this->color = rhs.color;
    this->shouldGenerate = rhs.shouldGenerate;
    return *this;
}

