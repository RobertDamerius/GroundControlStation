#pragma once


#include <Line.hpp>
#include <LineStrip.hpp>
#include <ShaderLineStrip.hpp>
#include <ShaderLine.hpp>


// Forward declaration
class Vehicle;


class VehicleCompass {
    public:
        bool enable;
        glm::vec3 color;

        /**
         *  @brief Create a vehicle compass.
         */
        VehicleCompass();

        /**
         *  @brief Default copy constructor.
         */
        VehicleCompass(const VehicleCompass& v) = default;

        /**
         *  @brief Delete the vehicle compass.
         */
        ~VehicleCompass();

        /**
         *  @brief Generate the GL content.
         */
        void GenerateGL(void);

        /**
         *  @brief Delete the GL content.
         */
        void DeleteGL(void);

        /**
         *  @brief Draw the lines of the compass.
         *  @param [in] shaderLine The line shader.
         *  @param [in] shaderLineStrip The line strip shader.
         *  @param [in] parent The parent vehicle reference.
         */
        void DrawLines(ShaderLine& shaderLine, ShaderLineStrip& shaderLineStrip, Vehicle& parent);

        /**
         *  @brief Delete the generate the GL content with a new radius.
         *  @param [in] r Radius in meters.
         *  @return The radius that has been set.
         */
        double Resize(double r);

        /**
         *  @brief Set the Radius without re-generating the GL content.
         *  @param [in] r Radius in meters.
         *  @return The radius that has been set.
         */
        double SetRadius(double r);

        /**
         *  @brief Get the radius.
         *  @return Radius in world space units.
         */
        double GetRadius(void);

        /* Operators */
        VehicleCompass& operator=(const VehicleCompass& rhs);

    private:
        double radius;
        Line lineRulerMajor;
        Line lineRulerMinor;
        Line lineRulerMinorMinor;
        Line lineHeading;
        LineStrip lineCircle;
        LineStrip arrowNorth;
        LineStrip headingCommand;
};

