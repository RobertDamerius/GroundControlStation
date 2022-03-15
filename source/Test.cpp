#include <Test.hpp>
#include <VehicleManager.hpp>
#include <AppWindow.hpp>
#include <Common.hpp>


std::thread* Test::threadTest = nullptr;
std::atomic<bool> Test::terminate = false;


void Test::Start(void){
    Stop();
    threadTest = new std::thread(Test::Task);
}

void Test::Stop(void){
    terminate = true;
    if(threadTest){
        threadTest->join();
        delete threadTest;
        threadTest = nullptr;
    }
    terminate = false;
}

void Test::Task(void){
    VehicleID id("Test Vehicle");
    Vehicle v;
    v.name.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
    v.name.shadowColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);
    v.name.textToBeGenerated = std::string("Test Vehicle");
    v.description = "template";
    v.primitives = GL_TRIANGLES;
    v.CreateDefaultCuboid(2.0, 1.0, 0.5);
    appWindow.canvas.scene.vehicleManager.AddVehicle(id, v);

    std::vector<RigidBodyState> waypoints;
    RigidBodyState wp;
    wp.position = glm::vec3(0.0f, 0.0f, 1.0f); wp.quaternion.w = 1.0; wp.quaternion.x = 0.0; wp.quaternion.y = 0.0; wp.quaternion.z = 0.0; waypoints.push_back(wp);
    wp.position = glm::vec3(6.0f, 0.0f, 1.0f); wp.quaternion.w = 0.0; wp.quaternion.x = 1.0; wp.quaternion.y = 0.0; wp.quaternion.z = 0.0; waypoints.push_back(wp);
    wp.position = glm::vec3(9.0f, 3.0f, 1.0f); wp.quaternion.w = 0.0; wp.quaternion.x = 0.0; wp.quaternion.y = 1.0; wp.quaternion.z = 0.0; waypoints.push_back(wp);
    wp.position = glm::vec3(15.0f, 2.0f, 1.0f); wp.quaternion.w = 0.0; wp.quaternion.x = 0.0; wp.quaternion.y = 0.0; wp.quaternion.z = 1.0; waypoints.push_back(wp);
    appWindow.canvas.scene.vehicleManager.SetWaypoints(id, waypoints);

    std::vector<RigidBodyState> trajectory;
    wp.quaternion.w = 1.0; wp.quaternion.x = 0.0; wp.quaternion.y = 0.0; wp.quaternion.z = 0.0;
    wp.position = glm::vec3(-1.000f,    0.0f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(-0.700f, 0.0012f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(-0.400f, 0.0013f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(-0.100f, 0.0004f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(0.2000f,-0.0011f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(0.5000f,-0.0031f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(0.8000f,-0.0055f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(1.1000f,-0.0080f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(1.4000f,-0.0104f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(1.7000f,-0.0126f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(2.0000f,-0.0144f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(2.3000f,-0.0154f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(2.6000f,-0.0157f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(2.9000f,-0.0149f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(3.2000f,-0.0129f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(3.5000f,-0.0094f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(3.8000f,-0.0044f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(4.1000f, 0.0025f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(4.4000f, 0.0133f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(4.7000f, 0.0326f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(5.0000f, 0.0655f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(5.3000f, 0.1169f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(5.6000f, 0.1919f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(5.9000f, 0.2955f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(6.2000f, 0.4326f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(6.5000f, 0.6082f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(6.8000f, 0.8273f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(7.1000f, 1.0947f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(7.4000f, 1.4007f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(7.7000f, 1.7137f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(8.0000f, 2.0000f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(8.3000f, 2.2325f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(8.6000f, 2.4107f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(8.9000f, 2.5405f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(9.2000f, 2.6279f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(9.5000f, 2.6791f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(9.8000f, 2.6999f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(10.100f, 2.6963f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(10.400f, 2.6745f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(10.700f, 2.6404f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(11.000f, 2.6000f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(11.300f, 2.5583f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(11.600f, 2.5163f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(11.900f, 2.4739f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(12.200f, 2.4311f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(12.500f, 2.3877f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(12.800f, 2.3439f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(13.100f, 2.2994f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(13.400f, 2.2542f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(13.700f, 2.2084f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(14.000f, 2.1617f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(14.300f, 2.1143f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(14.600f, 2.0659f,1.0f); trajectory.push_back(wp);
    wp.position = glm::vec3(14.900f, 2.0166f,1.0f); trajectory.push_back(wp);
    appWindow.canvas.scene.vehicleManager.SetTrajectory(id, trajectory, 0.1);

    RigidBodyState state;
    state.position.x = 2.0f;
    appWindow.canvas.scene.vehicleManager.UpdateVehicleNavigation(id, state);

    PolygonObstacle obs;
    std::vector<std::array<double, 2>> vertices;
    std::vector<std::vector<std::array<double, 2>>> polygons;
    std::vector<std::array<double, 3>> velocities;
        vertices.push_back({-2.1965,-9.5342});
        vertices.push_back({14.9296,-9.7219});
        vertices.push_back({15.0704,-5.7336});
        vertices.push_back({-0.9765,-4.9829});
        polygons.push_back(vertices);
        velocities.push_back(std::array<double, 3>({0.0, 0.0, 0.0}));
    vertices.clear();
        vertices.push_back({-0.8546,11.9575});
        vertices.push_back({-1.5838, 8.6763});
        vertices.push_back({-1.0977, 7.1572});
        vertices.push_back({ 7.4092, 6.0635});
        vertices.push_back({ 8.4421,10.9245});
        polygons.push_back(vertices);
        velocities.push_back(std::array<double, 3>({0.0, 0.0, 0.0}));
    vertices.clear();
        vertices.push_back({15.5514, 9.8039});
        vertices.push_back({13.7285,10.6546});
        vertices.push_back({ 8.1991,10.4116});
        vertices.push_back({ 7.7737, 7.9203});
        vertices.push_back({16.2806, 6.2189});
        polygons.push_back(vertices);
        velocities.push_back(std::array<double, 3>({0.0, 0.0, 0.0}));
    vertices.clear();
        vertices.push_back({11.2348, 0.6816});
        vertices.push_back({10.1852, 0.3729});
        vertices.push_back({ 9.6295,-0.3063});
        vertices.push_back({10.3087,-1.1090});
        vertices.push_back({11.4200,-0.1211});
        polygons.push_back(vertices);
        velocities.push_back(std::array<double, 3>({0.0, 0.0, 0.0}));
    appWindow.canvas.scene.vehicleManager.SetPolygons(id, 0, 4, polygons, velocities);

    GUILog("Hello World!", 255, 255, 255);
    double t = 0.0;
    while(!Test::terminate){
        state.position.x = 10.0 * std::cos(t);
        state.position.y = 10.0 * std::sin(1.61803398874989 * t);
        state.sog = 10.0 * std::sqrt(std::sin(t)*std::sin(t) + 1.61803398874989*1.61803398874989*std::cos(1.61803398874989*t)*std::cos(1.61803398874989*t));
        appWindow.canvas.scene.vehicleManager.UpdateVehicleNavigation(id, state);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        t += 0.01;
    }
}

