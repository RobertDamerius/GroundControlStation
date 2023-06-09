#pragma once


#include <CFGParser.hpp>


class StyleConfiguration {
    public:
        bool cameraMode;                 ///< false: 2D, true: 3D
        double cameraClipNear;
        double cameraClipFar;
        double displayGamma;
        glm::dvec3 displayGroundColor;
        bool altitudeToZero;
        bool positionHistoryEnable;
        double positionHistoryMaximumSOG;
        double positionHistoryLineWidth;
        uint32_t positionHistoryUpdatePeriodMs;
        uint32_t positionHistoryBufferSize;
        bool waypointsEnable;
        glm::dvec3 waypointsColor;
        glm::dvec3 waypointsVehicleColor;
        double waypointsVehicleAlpha;
        double waypointsLineWidth;
        double waypointsSymbolSize;
        bool trajectoryEnable;
        glm::dvec3 trajectoryColor;
        glm::dvec3 trajectoryVehicleColor;
        double trajectoryVehicleAlpha;
        double trajectoryVehicleStride;
        double trajectoryLineWidth;
        bool compassEnable;
        double compassRadius;
        glm::dvec3 compassColor;
        bool polygonsEnable;
        double polygonsUpperLimit;
        double polygonsLowerLimit;
        glm::dvec3 polygonsColor;
        double sceneReflection;
        double sceneSpecularity;
        double sceneAmbient;
        double sceneShininess;

        /**
         *  @brief Create a style configuration.
         */
        StyleConfiguration();

        /**
         *  @brief Read the style configuration file.
         */
        void Read(void);

    private:
        /**
         *  @brief Reset to default values.
         */
        void Reset(void);

        /**
         *  @brief Find a camera mode value.
         *  @param [in] cfg Configuration parser.
         *  @param [out] value Value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if success, false otherwise.
         */
        bool FindCameraMode(CFGParser& cfg, bool& value, const char* key);

        /**
         *  @brief Find a boolean value.
         *  @param [in] cfg Configuration parser.
         *  @param [out] value Value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if success, false otherwise.
         */
        bool FindBool(CFGParser& cfg, bool& value, const char* key);

        /**
         *  @brief Find a positive double value.
         *  @param [in] cfg Configuration parser.
         *  @param [out] value Value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if success, false otherwise.
         */
        bool FindPositiveDouble(CFGParser& cfg, double& value, const char* key);

        /**
         *  @brief Find a double value.
         *  @param [in] cfg Configuration parser.
         *  @param [out] value Value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if success, false otherwise.
         */
        bool FindDouble(CFGParser& cfg, double& value, const char* key);

        /**
         *  @brief Find an unsigned value (32-bit).
         *  @param [in] cfg Configuration parser.
         *  @param [out] value Value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if success, false otherwise.
         */
        bool FindUint32(CFGParser& cfg, uint32_t& value, const char* key);

        /**
         *  @brief Find color values.
         *  @param [in] cfg Configuration parser.
         *  @param [out] value Value where to store the result.
         *  @param [in] key The key for which to search.
         *  @return True if success, false otherwise.
         */
        bool FindColor(CFGParser& cfg, glm::dvec3& value, const char* key);
};

