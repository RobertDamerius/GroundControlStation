#include <IMPStateConfiguration.hpp>


/* Protocol structure for state configuration bytes */
#pragma pack(push, 1)
union protocol_gnc_state_configuration {
    struct {
        uint32_t positionLLARepresentation:1;
        uint32_t positionLLAAvailability:2;
        uint32_t positionNEDRepresentation:1;
        uint32_t positionNEDAvailability:2;
        uint32_t orientationQuaternionRepresentation:1;
        uint32_t orientationQuaternionAvailability:1;
        uint32_t orientationRollPitchYawRepresentation:1;
        uint32_t orientationRollPitchYawAvailability:2;
        uint32_t velocityNEDUVWRepresentation:1;
        uint32_t velocityNEDUVWAvailabilityE:2;
        uint32_t velocityNEDUVWAvailabilityB:2;
        uint32_t velocityPQRRepresentation:1;
        uint32_t velocityPQRAvailability:2;
        uint32_t accelerationNEDXYZRepresentation:1;
        uint32_t accelerationNEDXYZAvailabilityE:2;
        uint32_t accelerationNEDXYZAvailabilityB:2;
        uint32_t courseOverGroundRepresentation:1;
        uint32_t courseOverGroundAvailability:1;
        uint32_t speedOverGroundRepresentation:1;
        uint32_t speedOverGroundAvailability:1;
        uint32_t angleOfAttackRepresentation:1;
        uint32_t angleOfAttackAvailability:1;
        uint32_t sideSlipAngleRepresentation:1;
        uint32_t sideSlipAngleAvailability:1;
    } values;
    uint8_t bytes[4];
};
#pragma pack(pop)


IMP::StateConfiguration::StateConfiguration(){
    Reset();
}

IMP::StateConfiguration::~StateConfiguration(){}

void IMP::StateConfiguration::Reset(void){
    positionLLA.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    positionLLA.available = STATE_CONFIGURATION_AVAILABILITY_NONE;
    positionNED.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    positionNED.available = STATE_CONFIGURATION_AVAILABILITY_NONE;
    orientationQuaternion.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    orientationQuaternion.available = false;
    orientationRollPitchYaw.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    orientationRollPitchYaw.available = STATE_CONFIGURATION_AVAILABILITY_NONE;
    velocityNEDUVW.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    velocityNEDUVW.availableE = STATE_CONFIGURATION_AVAILABILITY_NONE;
    velocityNEDUVW.availableB = STATE_CONFIGURATION_AVAILABILITY_NONE;
    velocityPQR.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    velocityPQR.available = STATE_CONFIGURATION_AVAILABILITY_NONE;
    accelerationNEDXYZ.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    accelerationNEDXYZ.availableE = STATE_CONFIGURATION_AVAILABILITY_NONE;
    accelerationNEDXYZ.availableB = STATE_CONFIGURATION_AVAILABILITY_NONE;
    courseOverGround.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    courseOverGround.available = false;
    speedOverGround.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    speedOverGround.available = false;
    angleOfAttack.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    angleOfAttack.available = false;
    sideSlipAngle.representation = STATE_CONFIGURATION_REPRESENTATION_4BYTES;
    sideSlipAngle.available = false;
}

uint32_t IMP::StateConfiguration::GetRequiredPacketLength(bool insertConfiguration){
    // Configuration has a fixed length of 4 bytes
    uint32_t length = insertConfiguration ? 4 : 0;

    // Data length is variable and depends on configuration
    auto lambdaLength1D = [](IMP::state_configuration_1d c){ return uint32_t(c.available) * (4 + (uint32_t(c.representation) << 2)); };
    auto lambdaLength3D = [](IMP::state_configuration_3d c){ return uint32_t(c.available) * (4 + (uint32_t(c.representation) << 2)); };
    auto lambdaLength2x3D = [](IMP::state_configuration_2x3d c){ return (uint32_t(c.availableE) * (4 + (uint32_t(c.representation) << 2))) + (uint32_t(c.availableB) * (4 + (uint32_t(c.representation) << 2))); };
    length += lambdaLength3D(positionLLA);
    length += lambdaLength3D(positionNED);
    length += (lambdaLength1D(orientationQuaternion) << 2);
    length += lambdaLength3D(orientationRollPitchYaw);
    length += lambdaLength2x3D(velocityNEDUVW);
    length += lambdaLength3D(velocityPQR);
    length += lambdaLength2x3D(accelerationNEDXYZ);
    length += lambdaLength1D(courseOverGround);
    length += lambdaLength1D(speedOverGround);
    length += lambdaLength1D(angleOfAttack);
    length += lambdaLength1D(sideSlipAngle);   
    return length;
}

int IMP::StateConfiguration::Encode(uint8_t* bytes, const uint32_t length){
    // Pointer to the buffer must be valid and at least 4 bytes are required
    if(!bytes || length < 4){
        return -1;
    }

    // Create configuration data
    union protocol_gnc_state_configuration data;
    data.values.positionLLARepresentation = (uint32_t)positionLLA.representation;
    data.values.positionLLAAvailability = (uint32_t)positionLLA.available;
    data.values.positionNEDRepresentation = (uint32_t)positionNED.representation;
    data.values.positionNEDAvailability = (uint32_t)positionNED.available;
    data.values.orientationQuaternionRepresentation = (uint32_t)orientationQuaternion.representation;
    data.values.orientationQuaternionAvailability = (uint32_t)orientationQuaternion.available;
    data.values.orientationRollPitchYawRepresentation = (uint32_t)orientationRollPitchYaw.representation;
    data.values.orientationRollPitchYawAvailability = (uint32_t)orientationRollPitchYaw.available;
    data.values.velocityNEDUVWRepresentation = (uint32_t)velocityNEDUVW.representation;
    data.values.velocityNEDUVWAvailabilityE = (uint32_t)velocityNEDUVW.availableE;
    data.values.velocityNEDUVWAvailabilityB = (uint32_t)velocityNEDUVW.availableB;
    data.values.velocityPQRRepresentation = (uint32_t)velocityPQR.representation;
    data.values.velocityPQRAvailability = (uint32_t)velocityPQR.available;
    data.values.accelerationNEDXYZRepresentation = (uint32_t)accelerationNEDXYZ.representation;
    data.values.accelerationNEDXYZAvailabilityE = (uint32_t)accelerationNEDXYZ.availableE;
    data.values.accelerationNEDXYZAvailabilityB = (uint32_t)accelerationNEDXYZ.availableB;
    data.values.courseOverGroundRepresentation = (uint32_t)courseOverGround.representation;
    data.values.courseOverGroundAvailability = (uint32_t)courseOverGround.available;
    data.values.speedOverGroundRepresentation = (uint32_t)speedOverGround.representation;
    data.values.speedOverGroundAvailability = (uint32_t)speedOverGround.available;
    data.values.angleOfAttackRepresentation = (uint32_t)angleOfAttack.representation;
    data.values.angleOfAttackAvailability = (uint32_t)angleOfAttack.available;
    data.values.sideSlipAngleRepresentation = (uint32_t)sideSlipAngle.representation;
    data.values.sideSlipAngleAvailability = (uint32_t)sideSlipAngle.available;

    // Copy to output buffer
    memcpy(&bytes[0], &data.bytes[0], 4);
    return (int)4;
}

int IMP::StateConfiguration::Decode(const uint8_t* bytes, const uint32_t length){
    // Pointer to the buffer must be valid and at least 4 bytes are required
    if(!bytes || length < 4){
        return -1;
    }

    // Copy bytes from input buffer
    union protocol_gnc_state_configuration data;
    memcpy(&data.bytes[0], &bytes[0], 4);

    // Set attributes
    positionLLA.representation = (IMP::state_configuration_representation)data.values.positionLLARepresentation;
    positionLLA.available = (IMP::state_configuration_availability_indicator)data.values.positionLLAAvailability;
    positionNED.representation = (IMP::state_configuration_representation)data.values.positionNEDRepresentation;
    positionNED.available = (IMP::state_configuration_availability_indicator)data.values.positionNEDAvailability;
    orientationQuaternion.representation = (IMP::state_configuration_representation)data.values.orientationQuaternionRepresentation;
    orientationQuaternion.available = (bool)data.values.orientationQuaternionAvailability;
    orientationRollPitchYaw.representation = (IMP::state_configuration_representation)data.values.orientationRollPitchYawRepresentation;
    orientationRollPitchYaw.available = (IMP::state_configuration_availability_indicator)data.values.orientationRollPitchYawAvailability;
    velocityNEDUVW.representation = (IMP::state_configuration_representation)data.values.velocityNEDUVWRepresentation;
    velocityNEDUVW.availableE = (IMP::state_configuration_availability_indicator)data.values.velocityNEDUVWAvailabilityE;
    velocityNEDUVW.availableB = (IMP::state_configuration_availability_indicator)data.values.velocityNEDUVWAvailabilityB;
    velocityPQR.representation = (IMP::state_configuration_representation)data.values.velocityPQRRepresentation;
    velocityPQR.available = (IMP::state_configuration_availability_indicator)data.values.velocityPQRAvailability;
    accelerationNEDXYZ.representation = (IMP::state_configuration_representation)data.values.accelerationNEDXYZRepresentation;
    accelerationNEDXYZ.availableE = (IMP::state_configuration_availability_indicator)data.values.accelerationNEDXYZAvailabilityE;
    accelerationNEDXYZ.availableB = (IMP::state_configuration_availability_indicator)data.values.accelerationNEDXYZAvailabilityB;
    courseOverGround.representation = (IMP::state_configuration_representation)data.values.courseOverGroundRepresentation;
    courseOverGround.available = (bool)data.values.courseOverGroundAvailability;
    speedOverGround.representation = (IMP::state_configuration_representation)data.values.speedOverGroundRepresentation;
    speedOverGround.available = (bool)data.values.speedOverGroundAvailability;
    angleOfAttack.representation = (IMP::state_configuration_representation)data.values.angleOfAttackRepresentation;
    angleOfAttack.available = (bool)data.values.angleOfAttackAvailability;
    sideSlipAngle.representation = (IMP::state_configuration_representation)data.values.sideSlipAngleRepresentation;
    sideSlipAngle.available = (bool)data.values.sideSlipAngleAvailability;
    return 4;
}

