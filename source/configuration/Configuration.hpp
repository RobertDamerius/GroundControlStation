#pragma once


#include <NetworkConfiguration.hpp>
#include <OriginConfiguration.hpp>
#include <StyleConfiguration.hpp>


class Configuration {
    public:
        static NetworkConfiguration network;
        static OriginConfiguration origin;
        static StyleConfiguration style;

        /**
         *  @brief Read all configuration files.
         */
        static void ReadAll(void);
};

