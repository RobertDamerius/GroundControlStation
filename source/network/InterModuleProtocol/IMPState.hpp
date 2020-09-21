/**
 *  @file IMPState.hpp
 *  @brief Version 20200919.
 */
#pragma once


#include <IMPCommon.hpp>
#include <IMPStateConfiguration.hpp>


/* Default namespace */
namespace IMP {


class State {
    public:
        /* Header */
        double timestamp;                    ///< UTC Timestamp: seconds of the day.
        StateConfiguration configuration;    ///< The configuration for the state data.

        /* Data */
        double positionLatitude;             ///< Geographic position (latitude in radians) according to WGS84, range [-pi/2, +pi/2].
        double positionLongitude;            ///< Geographic position (latitude in radians) according to WGS84, range [-pi, +pi).
        double positionAltitude;             ///< Geographic position (altitude in meters) according to WGS84, positive upwards.
        double positionNorth;                ///< Position in north direction of earth-fixed frame in meters.
        double positionEast;                 ///< Position in east direction of earth-fixed frame in meters.
        double positionDown;                 ///< Position in down direction of earth-fixed frame in meters, positive downwards.
        double orientationQuaternionW;       ///< Scalar part for the unit quaternion that describes the rotation from body to navigation frame, range [-1, +1].
        double orientationQuaternionX;       ///< X component of the vector part for the unit quaternion that describes the rotation from body to navigation frame, range [-1, +1].
        double orientationQuaternionY;       ///< Y component of the vector part for the unit quaternion that describes the rotation from body to navigation frame, range [-1, +1].
        double orientationQuaternionZ;       ///< Z component of the vector part for the unit quaternion that describes the rotation from body to navigation frame, range [-1, +1].
        double orientationRoll;              ///< Roll angle (euler angle around X axis) in radians according to the ZYX-convention, range [-pi, +pi).
        double orientationPitch;             ///< Pitch angle (euler angle around Y axis) in radians according to the ZYX-convention, range [-pi/2, +pi/2].
        double orientationYaw;               ///< Yaw angle (euler angle around Z axis) in radians according to the ZYX-convention, range [-pi, +pi).
        double velocityNorth;                ///< Velocity in north direction in meters per second.
        double velocityEast;                 ///< Velocity in east direction in meters per second.
        double velocityDown;                 ///< Velocity in down direction in meters per second.
        double velocityU;                    ///< Velocity in X direction of the body frame in meters per second.
        double velocityV;                    ///< Velocity in Y direction of the body frame in meters per second.
        double velocityW;                    ///< Velocity in Z direction of the body frame in meters per second.
        double velocityP;                    ///< Rotational velocity around X axis of the body frame in radians per second.
        double velocityQ;                    ///< Rotational velocity around Y axis of the body frame in radians per second.
        double velocityR;                    ///< Rotational velocity around Z axis of the body frame in radians per second.
        double accelerationNorth;            ///< Acceleration in north direction in meters per square-second.
        double accelerationEast;             ///< Acceleration in east direction in meters per square-second.
        double accelerationDown;             ///< Acceleration in down direction in meters per square-second.
        double accelerationX;                ///< Acceleration in X direction of the body frame in meters per square-second.
        double accelerationY;                ///< Acceleration in Y direction of the body frame in meters per square-second.
        double accelerationZ;                ///< Acceleration in Z direction of the body frame in meters per square-second.
        double courseOverGround;             ///< Course over ground in radians, range [-pi, +pi).
        double speedOverGround;              ///< Speed over ground in meters per second, always >= 0.0.
        double angleOfAttack;                ///< Angle of attack in radians, range [-pi, +pi).
        double sideSlipAngle;                ///< Side slip angle in radians, range [-pi, +pi).

        /**
         *  @brief Create a state object.
         */
        State();

        /**
         *  @brief Delete the state object.
         */
        virtual ~State();

        /**
         *  @brief Reset the @ref timestamp and all state attributes to zero. @ref quaternionW will be set to 1.0. The @ref configuration will be reset, too.
         */
        virtual void Reset(void);

        /**
         *  @brief Encode the state to a packet. The packet length depends on the @ref configuration.
         *  @param [out] bytes Output buffer where to store the message bytes.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @param [in] insertHeader True if header (timestamp and configuration) should be inserted to the beginning of the message, false otherwise.
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         *  @details The maximum number of bytes is 265.
         */
        virtual int Encode(uint8_t* bytes, const uint32_t length, bool insertHeader);

        /**
         *  @brief Decode a packet.
         *  @param [in] bytes Input buffer containing the packet to be decoded.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be read from the buffer).
         *  @param [in] containsHeader True if the packet contains header (timestamp and configuration) at the beginning.
         *  @return Number of bytes that have been decoded or -1 in case of failure.
         */
        virtual int Decode(const uint8_t* bytes, const uint32_t length, bool containsHeader);
};


} /* namespace: IMP */

