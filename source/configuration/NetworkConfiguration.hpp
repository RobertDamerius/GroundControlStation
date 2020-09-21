#pragma once


class NetworkConfiguration {
    public:
        struct unicast_configuration {
            uint16_t port;
            uint8_t ipInterface[4];
        };
        struct multicast_configuration {
            uint8_t ipGroup[4];
            uint16_t port;
            uint8_t ipInterface[4];
        };
        struct joystick_configuration {
            uint8_t id;
            uint8_t ipDestination[4];
            uint16_t port;
            uint8_t ipInterface[4];
        };
        std::vector<unicast_configuration> unicast;
        std::vector<multicast_configuration> multicast;
        std::vector<joystick_configuration> joystick;

        /**
         *  @brief Read the network configuration file.
         */
        void Read(void);
};

