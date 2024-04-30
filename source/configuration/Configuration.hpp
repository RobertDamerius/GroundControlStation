#pragma once


#include <GCSConfiguration.hpp>


class Configuration {
    public:
        static GCSConfiguration gcs;   ///< The common configuration file for the GCS.

        /**
         *  @brief Read all configuration files.
         */
        static void ReadAndSaveAll(void);
};

