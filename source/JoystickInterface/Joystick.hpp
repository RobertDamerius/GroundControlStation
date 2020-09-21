#pragma once


#include <RD.hpp>


class Joystick {
    public:
        /**
         *  @brief Create the joystick.
         *  @param [in] id Joystick ID.
         *  @param [in] ipDestination 4-dimensional array containing the destination IP address.
         *  @param [in] port Destination port.
         *  @param [in] ipInterface 4-dimensional array containing the interface address.
         */
        Joystick(uint8_t id, uint8_t* ipDestination, uint16_t port, uint8_t* ipInterface);

        /**
         *  @brief Delete the joystick.
         */
        ~Joystick();

        /**
         *  @brief Send joystick data.
         *  @param [in] joystickID Joystick value: [GLFW_JOYSTICK_1; GLFW_JOYSTICK_16].
         *  @param [in] axes Axes data in range [-1.0, +1.0].
         *  @param [in] numAxes Number of axes.
         *  @param [in] buttons Button data, either GLFW_PRESS or GLFW_RELEASE.
         *  @param [in] numButtons Number of buttons.
         */
        void SendData(uint8_t joystickID, const float* axes, int numAxes, const uint8_t* buttons, int numButtons);

    protected:
        RD::Network::UDPSocket udpSocket;
        RD::Network::Endpoint destination;
};

