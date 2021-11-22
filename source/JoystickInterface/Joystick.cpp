#include <Joystick.hpp>
#include <Common.hpp>


union f32 {
    float f;
    uint32_t u;
    uint8_t bytes[4];
};


Joystick::Joystick(uint8_t id, uint8_t* ipDestination, uint16_t port, uint8_t* ipInterface){
    // Set destination endpoint
    destination.SetIPv4(ipDestination[0], ipDestination[1], ipDestination[2], ipDestination[3]);
    destination.SetPort(port);

    // Open socket
    if(udpSocket.Open() < 0){
        std::ostringstream msg;
        msg << "Could not open UDP socket for joystick " << std::to_string(id) << "!";
        GUILog(msg.str(), 255, 0, 0);
        return;
    }

    // Bind port and set interface
    char ipIF[16];
    sprintf(ipIF,"%u.%u.%u.%u",ipInterface[0],ipInterface[1],ipInterface[2],ipInterface[3]);
    if(udpSocket.Bind(50000, 60000, (ipInterface[0] || ipInterface[1] || ipInterface[2] || ipInterface[3]) ? &ipIF[0] : nullptr) < 0){
        std::ostringstream msg;
        msg << "Could not bind a port for UDP socket for joystick " << std::to_string(id) << "!";
        GUILog(msg.str(), 255, 0, 0);
        udpSocket.Close();
        return;
    }
}

Joystick::~Joystick(){
    udpSocket.Close();
}

void Joystick::SendData(uint8_t joystickID, const float* axes, int numAxes, const uint8_t* buttons, int numButtons){
    // Allow at most 255 values
    if((numAxes < 0) || (numAxes > 255) || (numButtons < 0) || (numButtons > 255))
        return;
    uint8_t numberOfAxes = (uint8_t)numAxes;
    uint8_t numberOfButtons = (uint8_t)numButtons;

    // Create message
    std::vector<uint8_t> msg;
    msg.push_back(joystickID);
    msg.push_back(numberOfAxes);
    msg.push_back(numberOfButtons);
    for(int i = 0; i != numAxes; ++i){
        f32 value;
        value.f = axes[i];
        value.u = htonl(value.u);
        msg.push_back(value.bytes[0]);
        msg.push_back(value.bytes[1]);
        msg.push_back(value.bytes[2]);
        msg.push_back(value.bytes[3]);
    }
    for(int i = 0, r = 0; i != numButtons; ++i, (++r) %= 8){
        if(!r){
            msg.push_back(0x00);
        }
        msg.back() |= ((GLFW_PRESS == buttons[i]) ? 0x01 : 0x00) << (7 - r);
    }

    // Send message
    if(udpSocket.IsOpen()){
        udpSocket.SendTo(destination, &msg[0], (int)msg.size());
    }
}

