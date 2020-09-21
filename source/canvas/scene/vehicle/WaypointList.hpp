#pragma once


#include <LineStrip.hpp>
#include <RigidBodyState.hpp>
#include <ShaderLineStrip.hpp>


class WaypointList {
    public:
        bool enable;                           ///< True if waypoints should be rendered, false otherwise.
        glm::vec3 color;                       ///< Color for the waypoint line.
        glm::vec3 vehicleColor;                ///< Vehicle color multiplier.
        std::vector<RigidBodyState> elements;  ///< Internal waypoint elements.
        bool shouldGenerate;                   ///< True if line strip should be generated.

        /**
         *  @brief Create a waypoint list object.
         */
        WaypointList();

        /**
         *  @brief Default copy constructor.
         */
        WaypointList(const WaypointList& w) = default;

        /**
         *  @brief Delete the waypoint list object.
         */
        virtual ~WaypointList();

        /**
         *  @brief Update the waypoint list.
         *  @param [in] dt Elapsed time in seconds.
         */
        void Update(double dt);

        /**
         *  @brief Set vehicle alpha.
         *  @param [in] alpha Vehicle alpha value (normalized).
         */
        void SetVehicleAlpha(double alpha);

        /**
         *  @brief Get vehicle alpha.
         *  @return Vehicle alpha value in range [0,1].
         */
        inline double GetVehicleAlpha(void){ return this->vehicleAlpha; }

        /**
         *  @brief Set line width.
         *  @param [in] width Line width in pixels.
         */
        void SetLineWidth(double width);

        /**
         *  @brief Get line width.
         *  @return Line width in pixels.
         */
        inline double GetLineWidth(void){ return (double)this->line.width; }

        /**
         *  @brief Set the symbol size.
         *  @param [in] size Symbol size in meters.
         */
        void SetSymbolSize(double size);

        /**
         *  @brief Get the symbol size.
         *  @return Symbol size in meters.
         */
        inline double GetSymbolSize(void){ return this->symbolSize; }

        /**
         *  @brief Delete the GL content.
         */
        void Delete(void);

        /**
         *  @brief Draw the waypoints.
         *  @param [in] shader The line strip shader.
         */
        void Draw(ShaderLineStrip& shader);

        /* Operators */
        WaypointList& operator=(const WaypointList& rhs);

    protected:
        double vehicleAlpha;                   ///< Vehicle alpha value.
        double symbolSize;                     ///< Symbol size in meters.
        LineStrip line;                        ///< The internal line strip.

        /**
         *  @brief Generate the GL content for the line strip.
         */
        virtual void Generate(void);
};

