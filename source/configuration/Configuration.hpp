#pragma once


#include <NetworkConfiguration.hpp>
#include <OriginConfiguration.hpp>
#include <StyleConfiguration.hpp>
#include <TTSConfiguration.hpp>


class Configuration {
    public:
        static NetworkConfiguration network;
        static OriginConfiguration origin;
        static StyleConfiguration style;
        static TTSConfiguration tts;

        /**
         *  @brief Read all configuration files.
         */
        static void ReadAll(void);
};

