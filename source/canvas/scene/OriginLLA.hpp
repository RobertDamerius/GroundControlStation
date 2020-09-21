#pragma once


#include <Angle.hpp>


class OriginLLA {
    public:
        /**
         *  @brief Create an LLA origin object.
         */
        OriginLLA();

        /**
         *  @brief Delete the LLA origin object.
         */
        ~OriginLLA();

        /**
         *  @brief Update and set the origin if it has not been set.
         *  @param [in] lat Latitude in radians.
         *  @param [in] lon Longitude in radians.
         *  @param [in] alt Altitude in meters.
         */
        void Update(double lat, double lon, double alt);

        /**
         *  @brief Get the current origin.
         *  @param [out] lat Latitude in radians.
         *  @param [out] lon Longitude in radians.
         *  @param [out] alt Altitude in meters (positive upwards).
         */
        void Get(double& lat, double& lon, double& alt);

        /**
         *  @brief Convert geographical position to NED position.
         *  @param [out] xyz NED position.
         *  @param [in] lla Geographical position.
         */
        void LLA2NED(glm::dvec3& xyz, glm::dvec3 lla);

    private:
        bool undefined;      ///< True if origin has not been defined.
        Angle latitude;      ///< Latitude in radians.
        Angle longitude;     ///< Longitude in radians.
        double altitude;     ///< Altitude in meters (positive upwards).
        double sinOriginLat; ///< Sine of @ref latitude.
        double cosOriginLat; ///< Cosine of @ref latitude.
};

