#include <JoystickInterface.hpp>
#include <Common.hpp>
#include <Configuration.hpp>


JoystickInterface::JoystickInterface(){}

JoystickInterface::~JoystickInterface(){}

bool JoystickInterface::Initialize(GLFWwindow* wnd){
    Terminate();

    // Create joystick database based on configuration
    for(auto&& joystickConf : Configuration::gcs.network.joystick){
        joysticks.insert({joystickConf.id, new Joystick(joystickConf.id, &joystickConf.ipDestination[0], joystickConf.port, &joystickConf.ipInterface[0])});
    }
    (void)wnd;
    return true;
}

void JoystickInterface::Terminate(void){
    for(auto&& entry : joysticks){
        delete entry.second;
    }
    joysticks.clear();
}

void JoystickInterface::CallbackJoystickStatus(int joy, int event, const char* name){
    if((GLFW_CONNECTED == event) && name){
        GUILog(std::string("Joystick ") + std::to_string(joy) + std::string(" (") + std::string(name) + std::string(") connected"),255,255,255);
    }
    else if(GLFW_DISCONNECTED == event){
        // GUILog(std::string("Joystick ") + std::to_string(joy) + std::string(" disconnected"),255,255,255);
    }
}

void JoystickInterface::CallbackJoystickData(int joy, const float* axes, int numAxes, const uint8_t* buttons, int numButtons){
    // Allow at most 255 values
    if((joy < 0) || (joy > 255))
        return;
    uint8_t joystickID = (uint8_t)joy;

    // Send message
    auto got = joysticks.find(joystickID);
    if(got != joysticks.end()){
        got->second->SendData(joystickID, axes, numAxes, buttons, numButtons);
    }
}

