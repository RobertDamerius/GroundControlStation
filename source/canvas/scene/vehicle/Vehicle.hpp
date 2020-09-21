#pragma once


#include <RigidBody.hpp>
#include <Polytope.hpp>
#include <VehicleID.hpp>
#include <SceneText.hpp>
#include <VehicleCompass.hpp>
#include <PositionHistory.hpp>
#include <WaypointList.hpp>
#include <Trajectory.hpp>
#include <PolygonContainer.hpp>
#include <PolyMesh.hpp>


class Vehicle: public Polytope {
    public:
        bool visible;                         ///< True if vehicle should be rendered, false otherwise.
        bool altitudeToZero;                  ///< True if vehicle should be renfered with altitude equal to zero, false otherwise.
        RigidBody navigation;                 ///< Navigation data of the vehicle.
        RigidBody guidance;                   ///< Guidance data of the vehicle.
        RigidBody renderingState;             ///< Current rendering state from latest @ref Update event.
        SceneText name;                       ///< The name of the vehicle.
        std::string description;              ///< The optional description string of the vehicle.
        VehicleCompass compass;               ///< The vehicle compass.
        PositionHistory positionHistory;      ///< The position history.
        WaypointList waypoints;               ///< The list of waypoints.
        Trajectory trajectory;                ///< The commanded trajectory.
        PolygonContainer polygons;            ///< The polygon container.
        struct TimeOfLatestMessage {
            std::chrono::steady_clock::time_point navigation;
            std::chrono::steady_clock::time_point guidance;
            std::chrono::steady_clock::time_point guidanceTrajectory;
            std::chrono::steady_clock::time_point guidanceWaypoint;
            std::chrono::steady_clock::time_point polygon;
        } timeOfLatestMessage;
        PolyMesh mesh;
        bool meshExists;
        bool useMesh;

        /**
         *  @brief Create a vehicle.
         */
        Vehicle();

        /**
         *  @brief Default copy constructor.
         */
        Vehicle(const Vehicle& v) = default;

        /**
         *  @brief Delete the vehicle.
         */
        ~Vehicle();

        /**
         *  @brief Regenerate the GL content for the polytope only.
         */
        void RegeneratePolytope(void);

        /**
         *  @brief Generate the GL content for the vehicle.
         *  @details Overwrites the Polytope::GenerateGL() member function but calls it internally.
         */
        void GenerateGL(void);

        /**
         *  @brief Delete the GL content for the vehicle.
         *  @details Overwrites the Polytope::DeleteGL() member function but calls it internally.
         */
        void DeleteGL(void);

        /**
         *  @brief Update event.
         *  @param [in] dt Elapsed time in seconds.
         */
        void Update(double dt);

        /**
         *  @brief Draw the default cuboid or the poly mesh if it exists and is selected.
         */
        void Draw(void);

        /**
         *  @brief Create a default cuboid polytope.
         *  @param [in] length The length of the default cuboid in meters.
         *  @param [in] width The width of the default cuboid in meters.
         *  @param [in] height The height of the default cuboid in meters.
         *  @param [in] offsetX The x-offset in meters (body-frame), defaults to 0.0.
         *  @param [in] offsetY The y-offset in meters (body-frame), defaults to 0.0.
         *  @param [in] offsetZ The z-offset in meters (body-frame), defaults to 0.0.
         *  @details Only the vertices and indices of the polytope will be set.
         */
        void CreateDefaultCuboid(double length, double width, double height, double offsetX = 0.0, double offsetY = 0.0, double offsetZ = 0.0);

        /* Operators */
        Vehicle& operator=(const Vehicle& rhs);
};

