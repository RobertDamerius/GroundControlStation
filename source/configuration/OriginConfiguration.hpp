#pragma once


class OriginConfiguration {
    public:
        double latitude;  ///< Latitude in radians.
        double longitude; ///< Longitude in radians.
        double altitude;  ///< Altitude in radians.
        bool undefined;   ///< True if origin is undefined.

        /**
         *  @brief Create the origin configuration.
         */
        OriginConfiguration();

        /**
         *  @brief Read the origin configuration file.
         */
        void Read(void);
};

