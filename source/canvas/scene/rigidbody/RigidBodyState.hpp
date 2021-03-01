#pragma once


#include <Angle.hpp>
#include <IMPState.hpp>


class RigidBodyState {
    public:
        IMP::StateConfiguration conf; ///< State configuration.
        Angle latitude;         ///< Geographic latitude in radians.
        Angle longitude;        ///< Geographic longitude in radians.
        double altitude;        ///< Altitude above WGS84 reference ellipsoid in meters.
        glm::dvec3 position;    ///< XYZ position (x: north, y: east, z:down).
        glm::dvec4 quaternion;  ///< NED Unit quaternion [x,y,z]: vector part; [w]: scalar part.
        double u;               ///< Translational body velocity in x-direction of body frame in meters per second.
        double v;               ///< Translational body velocity in y-direction of body frame in meters per second.
        double w;               ///< Translational body velocity in y-direction of body frame in meters per second.
        double p;               ///< Rotational body velocity around x-axis of the body frame in radians per second.
        double q;               ///< Rotational body velocity around y-axis of the body frame in radians per second.
        double r;               ///< Rotational body velocity around z-axis of the body frame in radians per second.

        /* Optional attributes that can be either set or calculated */
        double velocityNorth;   ///< Velocity in north direction in meters per second.
        double velocityEast;    ///< Velocity in east direction in meters per second.
        double velocityDown;    ///< Velocity in down direction in meters per second.
        Angle roll;             ///< Roll angle in radians.
        Angle pitch;            ///< Pitch angle in radians.
        Angle yaw;              ///< Yaw angle in radians.
        double sog;             ///< Speed over ground in meters per second.

        bool overridePosition;  ///< True if @ref position can be calculated from @ref latitude, @ref longitude and @ref altitude.

        /**
         *  @brief Create a rigid body state.
         */
        RigidBodyState();

        /**
         *  @brief Copy constructor.
         */
        RigidBodyState(const RigidBodyState& rbs);

        /**
         *  @brief Create a rigid body state from an IMP state.
         *  @param [in] state The IMP state from which to copy the state variables.
         */
        RigidBodyState(const IMP::State& state);

        /**
         *  @brief Delete the rigid body state.
         */
        virtual ~RigidBodyState();

        /**
         *  @brief Reset the rigid body state to default values (zero).
         */
        void Reset(void);

        /**
         *  @brief Calculate optional attributes based on basic attributes.
         */
        void CalculateOptionals(void);

        /* Operators */
        RigidBodyState& operator=(const RigidBodyState& rhs);
        RigidBodyState& operator=(const IMP::State& rhs);

    private:
        /**
         *  @brief Copy state values from an IMP state.
         *  @param [in] state The IMP state from which to copy the state variables.
         */
        void CopyFromIMPState(const IMP::State& rhs);
};

