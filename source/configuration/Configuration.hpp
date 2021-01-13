#pragma once


#include <NetworkConfiguration.hpp>
#include <OriginConfiguration.hpp>
#include <StyleConfiguration.hpp>
#include <TTSConfiguration.hpp>
#include <DatabaseConfiguration.hpp>


class Configuration {
    public:
        static NetworkConfiguration network;
        static OriginConfiguration origin;
        static StyleConfiguration style;
        static TTSConfiguration tts;
        static DatabaseConfiguration database;

        /**
         *  @brief Read all configuration files.
         */
        static void ReadAll(void);
};

