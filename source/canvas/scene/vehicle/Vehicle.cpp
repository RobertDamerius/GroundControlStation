#include <Vehicle.hpp>
#include <AppWindow.hpp>
#include <Configuration.hpp>
#include <FileManager.hpp>
#include <Common.hpp>


Vehicle::Vehicle(){
    visible = true;
    useMesh = true;
    meshExists = false;
    altitudeToZero = Configuration::style.altitudeToZero;
    timeOfLatestMessage.navigation = std::chrono::steady_clock::now();
    timeOfLatestMessage.guidance = std::chrono::steady_clock::now();
    timeOfLatestMessage.guidanceTrajectory = std::chrono::steady_clock::now();
    timeOfLatestMessage.guidanceWaypoint = std::chrono::steady_clock::now();
    timeOfLatestMessage.polygon = std::chrono::steady_clock::now();
}

Vehicle::~Vehicle(){}

void Vehicle::RegeneratePolytope(void){
    Polytope::DeleteGL();
    mesh.DeleteGL();
    Polytope::GenerateGL();
    mesh.GenerateGL();
}

void Vehicle::GenerateGL(void){
    Polytope::GenerateGL();
    mesh.GenerateGL();
    name.GenerateText();
    compass.GenerateGL();
    positionHistory.Generate(Configuration::style.positionHistoryBufferSize, this->navigation.position);
    // Note: Trajectory and waypoints are generated when their shouldGenerate flag has been set
}

void Vehicle::DeleteGL(void){
    Polytope::DeleteGL();
    name.DeleteText();
    compass.DeleteGL();
    positionHistory.Delete();
    waypoints.Delete();
    trajectory.Delete();
    polygons.DeleteGL();
    mesh.DeleteGL();
}

void Vehicle::Update(double dt){
    if(navigation.overridePosition){
        appWindow.canvas.scene.origin.Update(navigation.latitude(), navigation.longitude(), navigation.altitude);
        appWindow.canvas.scene.origin.LLA2NED(navigation.position, glm::dvec3(navigation.latitude(), navigation.longitude(), navigation.altitude));
    }
    if(guidance.overridePosition){
        appWindow.canvas.scene.origin.Update(guidance.latitude(), guidance.longitude(), guidance.altitude);
        appWindow.canvas.scene.origin.LLA2NED(guidance.position, glm::dvec3(guidance.latitude(), guidance.longitude(), guidance.altitude));
    }
    renderingState = navigation;
    positionHistory.Update(dt, renderingState.position, renderingState.sog);
    waypoints.Update(dt);
    trajectory.Update(dt);
}

void Vehicle::Draw(void){
    if(meshExists && useMesh){
        mesh.DrawTriangles();
    }
    else{
        Polytope::Draw();
    }
}

void Vehicle::CreateDefaultCuboid(double length, double width, double height, double offsetX, double offsetY, double offsetZ){
    // Clear vertices/indices
    this->indices.clear();
    this->vertices.clear();

    // Transform to OpenGL coordinate frame
    GLfloat a = (GLfloat)(0.5 * length);
    GLfloat b = (GLfloat)(0.5 * height);
    GLfloat c = (GLfloat)(0.5 * width);
    double d = offsetY;
    offsetY = -offsetZ;
    offsetZ = d;
    this->primitives = GL_TRIANGLES;
    PolytopeVertex vertex;

    // Front
    vertex.normal[0] = 1.0f; vertex.normal[2] = vertex.normal[1] = 0.0f; vertex.position[0] = offsetX + a;
    vertex.color[1] = 1.0f;
    vertex.color[0] = vertex.color[2] = 0.0f;
    vertex.position[1] = offsetY + b; vertex.position[2] = offsetZ + c; this->vertices.push_back(vertex);
    vertex.position[1] = offsetY - b; vertex.position[2] = offsetZ + c; this->vertices.push_back(vertex);
    vertex.color[1] = 0.0f;
    vertex.color[0] = 1.0f;
    vertex.position[1] = offsetY - b; vertex.position[2] = offsetZ - c; this->vertices.push_back(vertex);
    vertex.position[1] = offsetY + b; vertex.position[2] = offsetZ - c; this->vertices.push_back(vertex);
    this->indices.push_back(0); this->indices.push_back(1); this->indices.push_back(3);
    this->indices.push_back(3); this->indices.push_back(1); this->indices.push_back(2);

    // Left
    vertex.normal[2] =-1.0f; vertex.normal[0] = vertex.normal[1] = 0.0f; vertex.position[2] = offsetZ - c;
    vertex.position[0] = offsetX + a; vertex.position[1] = offsetY + b; this->vertices.push_back(vertex);
    vertex.position[0] = offsetX + a; vertex.position[1] = offsetY - b; this->vertices.push_back(vertex);
    vertex.color[1] = vertex.color[2] = 1.0f;
    vertex.position[0] = offsetX - a; vertex.position[1] = offsetY - b; this->vertices.push_back(vertex);
    vertex.position[0] = offsetX - a; vertex.position[1] = offsetY + b; this->vertices.push_back(vertex);
    this->indices.push_back(4); this->indices.push_back(5); this->indices.push_back(7);
    this->indices.push_back(7); this->indices.push_back(5); this->indices.push_back(6);

    // Back
    vertex.normal[0] =-1.0f; vertex.normal[2] = vertex.normal[1] = 0.0f; vertex.position[0] = offsetX - a;
    vertex.position[1] = offsetY + b; vertex.position[2] = offsetZ - c; this->vertices.push_back(vertex);
    vertex.position[1] = offsetY - b; vertex.position[2] = offsetZ - c; this->vertices.push_back(vertex);
    vertex.position[1] = offsetY - b; vertex.position[2] = offsetZ + c; this->vertices.push_back(vertex);
    vertex.position[1] = offsetY + b; vertex.position[2] = offsetZ + c; this->vertices.push_back(vertex);
    this->indices.push_back(8); this->indices.push_back(9); this->indices.push_back(11);
    this->indices.push_back(11); this->indices.push_back(9); this->indices.push_back(10);

    // Right
    vertex.normal[2] = 1.0f; vertex.normal[0] = vertex.normal[1] = 0.0f; vertex.position[2] = offsetZ + c;
    vertex.position[0] = offsetX - a; vertex.position[1] = offsetY + b; this->vertices.push_back(vertex);
    vertex.position[0] = offsetX - a; vertex.position[1] = offsetY - b; this->vertices.push_back(vertex);
    vertex.color[0] = vertex.color[2] = 0.0f;
    vertex.position[0] = offsetX + a; vertex.position[1] = offsetY - b; this->vertices.push_back(vertex);
    vertex.position[0] = offsetX + a; vertex.position[1] = offsetY + b; this->vertices.push_back(vertex);
    this->indices.push_back(12); this->indices.push_back(13); this->indices.push_back(15);
    this->indices.push_back(15); this->indices.push_back(13); this->indices.push_back(14);

    // Top
    vertex.normal[1] = 1.0f; vertex.normal[0] = vertex.normal[2] = 0.0f; vertex.position[1] = offsetY + b;
    vertex.position[0] = offsetX + a; vertex.position[2] = offsetZ + c; this->vertices.push_back(vertex);
    vertex.color[0] = 1.0f;
    vertex.color[1] = 0.0f;
    vertex.position[0] = offsetX + a; vertex.position[2] = offsetZ - c; this->vertices.push_back(vertex);
    vertex.color[1] = vertex.color[2] = 1.0f;
    vertex.position[0] = offsetX - a; vertex.position[2] = offsetZ - c; this->vertices.push_back(vertex);
    vertex.position[0] = offsetX - a; vertex.position[2] = offsetZ + c; this->vertices.push_back(vertex);
    this->indices.push_back(16); this->indices.push_back(17); this->indices.push_back(19);
    this->indices.push_back(19); this->indices.push_back(17); this->indices.push_back(18);

    // Bottom
    vertex.normal[1] =-1.0f; vertex.normal[0] = vertex.normal[2] = 0.0f; vertex.position[1] = offsetY - b;
    vertex.position[0] = offsetX - a; vertex.position[2] = offsetZ + c; this->vertices.push_back(vertex);
    vertex.position[0] = offsetX - a; vertex.position[2] = offsetZ - c; this->vertices.push_back(vertex);
    vertex.color[1] = vertex.color[2] = 0.0f;
    vertex.position[0] = offsetX + a; vertex.position[2] = offsetZ - c; this->vertices.push_back(vertex);
    vertex.color[0] = 0.0f;
    vertex.color[1] = 1.0f;
    vertex.position[0] = offsetX + a; vertex.position[2] = offsetZ + c; this->vertices.push_back(vertex);
    this->indices.push_back(20); this->indices.push_back(21); this->indices.push_back(22);
    this->indices.push_back(20); this->indices.push_back(22); this->indices.push_back(23);

    // Update bounding box
    UpdateAABB();

    // Load optional poly mesh
    meshExists = false;
    if(!description.size()) return;
    std::string filename = FileManager::GetRootDirectory() + std::string(DIRECTORY_VEHICLE) + description + std::string(FILE_EXTENSION_PLY);
    std::string err;
    if(!mesh.Read(filename, err)){
        GUILog(err, 255, 0, 0);
        LogError("%s",err.c_str());
        return;
    }
    meshExists = true;
}

Vehicle& Vehicle::operator=(const Vehicle& rhs){
    Polytope::operator=(rhs);
    this->visible = rhs.visible;
    this->altitudeToZero = rhs.altitudeToZero;
    this->navigation = rhs.navigation;
    this->guidance = rhs.guidance;
    this->renderingState = rhs.renderingState;
    this->name = rhs.name;
    this->description = rhs.description;
    this->compass = rhs.compass;
    this->positionHistory = rhs.positionHistory;
    this->waypoints = rhs.waypoints;
    this->trajectory = rhs.trajectory;
    this->polygons = rhs.polygons;
    this->timeOfLatestMessage.navigation = rhs.timeOfLatestMessage.navigation;
    this->timeOfLatestMessage.guidance = rhs.timeOfLatestMessage.guidance;
    this->timeOfLatestMessage.guidanceTrajectory = rhs.timeOfLatestMessage.guidanceTrajectory;
    this->timeOfLatestMessage.guidanceWaypoint = rhs.timeOfLatestMessage.guidanceWaypoint;
    this->timeOfLatestMessage.polygon = rhs.timeOfLatestMessage.polygon;
    this->meshExists = rhs.meshExists;
    this->useMesh = rhs.useMesh;
    this->mesh = rhs.mesh;
    return *this;
}

