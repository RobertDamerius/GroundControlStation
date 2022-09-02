/**
 *  @file IMPStateConfiguration.hpp
 *  @brief Version 20220902.
 */
#pragma once


#include <IMPCommon.hpp>


/* Default namespace */
namespace IMP {


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Configuration data types
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* Configuration type for representation */
typedef enum {
    STATE_CONFIGURATION_REPRESENTATION_4BYTES = 0,         ///< Use 4-byte representation (int32_t or float).
    STATE_CONFIGURATION_REPRESENTATION_8BYTES              ///< Use 8-byte representation (double).
} state_configuration_representation;


/* Configuration type for availability indicator */
typedef enum {
    STATE_CONFIGURATION_AVAILABILITY_NONE = 0,             ///< Data not available.
    STATE_CONFIGURATION_AVAILABILITY_Z,                    ///< Data available for one dimendion: z.
    STATE_CONFIGURATION_AVAILABILITY_XY,                   ///< Data available for two dimensions: x and y.
    STATE_CONFIGURATION_AVAILABILITY_XYZ                   ///< Data available for three dimensions: x, y and z.
} state_configuration_availability_indicator;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Configuration structs for 1D, 3D and 2x3D
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* Configuration for 1-dimensional data */
struct state_configuration_1d {
    state_configuration_representation representation;     ///< Data representation.
    bool available;                                        ///< Data availability indicator: true if available, false otherwise.
};


/* Configuration for 3-dimensional data */
struct state_configuration_3d {
    state_configuration_representation representation;     ///< Data representation.
    state_configuration_availability_indicator available;  ///< Data availability indicator: none, z, xy or xyz.
};


/* Configuration for 2x3-dimensional data */
struct state_configuration_2x3d {
    state_configuration_representation representation;     ///< Data representation.
    state_configuration_availability_indicator availableE; ///< Data availability indicator for earth-fixed frame: none, z, xy or xyz.
    state_configuration_availability_indicator availableB; ///< Data availability indicator for body-fixed frame: none, z, xy or xyz.
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The GNC state configuration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class StateConfiguration {
    public:
        state_configuration_3d positionLLA;                ///< Configuration for geographic position (latitude, longitude, altitude).
        state_configuration_3d positionNED;                ///< Configuration for NED-position.
        state_configuration_1d orientationQuaternion;      ///< Configuration for unit quaternion (quaternion is either completely available or not: 1D)
        state_configuration_3d orientationRollPitchYaw;    ///< Configuration for euler angle representation.
        state_configuration_2x3d velocityNEDUVW;           ///< Configuration for translational velocity (earth-fixed: north, east, down; body-fixed: u, v, w).
        state_configuration_3d velocityPQR;                ///< Configuration for rotational velocity (p, q, r).
        state_configuration_2x3d accelerationNEDXYZ;       ///< Configuration for acceleration (earth-fixed: x, y, z; body-fixed: u, v, w).
        state_configuration_1d courseOverGround;           ///< Configuration for course over ground.
        state_configuration_1d speedOverGround;            ///< Configuration for speed over ground.
        state_configuration_1d angleOfAttack;              ///< Configuration for angle of attack.
        state_configuration_1d sideSlipAngle;              ///< Configuration for side slip angle.

        /**
         *  @brief Create a state configuration object.
         */
        StateConfiguration();

        /**
         *  @brief Delete the state configuration object.
         */
        ~StateConfiguration();

        /**
         *  @brief Reset the configuration. All configuration flags will be set to "not available". The byte representation will be set to a 4-byte representation.
         */
        void Reset(void);

        /**
         *  @brief Get the required packet length based on the configuration.
         *  @param [in] insertConfiguration True if configuration itself should be taken into account, false otherwise. The default value is true.
         *  @returns Number of bytes required to encode packet bytes.
         */
        uint32_t GetRequiredPacketLength(bool insertConfiguration = true);

        /**
         *  @brief Encode the state configuration.
         *  @param [out] bytes Output buffer where to store the encoded message.
         *  @param [in] length Length of the buffer (maximum number of bytes that can be written to the buffer).
         *  @returns The number of bytes that have been written to the buffer or -1 in case of failure.
         */
        int Encode(uint8_t* bytes, const uint32_t length);

        /**
         *  @brief Decode the state configuration.
         *  @param [in] bytes Input buffer that contains the state configuration.
         *  @param [in] length Length of the input buffer.
         *  @return The number of bytes that have been decoded or -1 in case of failure.
         */
        int Decode(const uint8_t* bytes, const uint32_t length);
};


} /* namespace: IMP */

