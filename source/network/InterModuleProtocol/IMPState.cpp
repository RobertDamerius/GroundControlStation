#include <IMPState.hpp>


#define CLAMP(x, lo, hi) ((x < lo) ? lo : ((x > hi) ? hi : x))


IMP::State::State(){
    Reset();
}

IMP::State::~State(){}

void IMP::State::Reset(void){
    configuration.Reset();
    timestamp = 0.0;
    positionLatitude = 0.0;
    positionLongitude = 0.0;
    positionAltitude = 0.0;
    positionNorth = 0.0;
    positionEast = 0.0;
    positionDown = 0.0;
    orientationQuaternionW = 1.0;
    orientationQuaternionX = 0.0;
    orientationQuaternionY = 0.0;
    orientationQuaternionZ = 0.0;
    orientationRoll = 0.0;
    orientationPitch = 0.0;
    orientationYaw = 0.0;
    velocityNorth = 0.0;
    velocityEast = 0.0;
    velocityDown = 0.0;
    velocityU = 0.0;
    velocityV = 0.0;
    velocityW = 0.0;
    velocityP = 0.0;
    velocityQ = 0.0;
    velocityR = 0.0;
    accelerationNorth = 0.0;
    accelerationEast = 0.0;
    accelerationDown = 0.0;
    accelerationX = 0.0;
    accelerationY = 0.0;
    accelerationZ = 0.0;
    courseOverGround = 0.0;
    speedOverGround = 0.0;
    angleOfAttack = 0.0;
    sideSlipAngle = 0.0;
}

int IMP::State::Encode(uint8_t* bytes, const uint32_t length, bool insertHeader){
    // Required packet length (timestamp (5) + configuration (4) + data (n))
    uint32_t requiredPacketLength = configuration.GetRequiredPacketLength(insertHeader);
    requiredPacketLength += (uint32_t)insertHeader * 5;

    // Error if invalid buffer or input buffer is too small
    if(!bytes || (length < requiredPacketLength)){
        return -1;
    }

    // Encode header: timestamp and configuration
    int index = 0;
    if(insertHeader){
        uint64_t nanosec100 = (uint64_t)CLAMP(10000000.0 * timestamp, 0.0, 1099511627775.0);
        bytes[0] = (uint8_t)(0xFF & (nanosec100 >> 32));
        bytes[1] = (uint8_t)(0xFF & (nanosec100 >> 24));
        bytes[2] = (uint8_t)(0xFF & (nanosec100 >> 16));
        bytes[3] = (uint8_t)(0xFF & (nanosec100 >> 8));
        bytes[4] = (uint8_t)(0xFF & nanosec100);
        if((index = configuration.Encode(&bytes[5], length - 5)) < 0){
            return -1;
        }
        index += 5; // 5 bytes for timestamp
    }

    // Lambda function for symmetrical range of angles
    auto lambdaSymmetricalRange = [](double d){
        int i = (int)(d * 0.159154943091895);
        d -= 6.28318530717959 * (double)i;
        return (d < -3.14159265358979) ? (d + 6.28318530717959) : ((d >= 3.14159265358979) ? (d - 6.28318530717959) : d);
    };

    // Lambda functions for data type encoding
    auto lambdaEncodeDouble = [&bytes, &index](double value){
        union {
            double d;
            uint8_t b[8];
        } un;
        un.d = value;
        if(1 != IMPSwapByteOrder(1)){
            uint8_t h = un.b[0]; un.b[0] = un.b[7]; un.b[7] = h;
            h = un.b[1]; un.b[1] = un.b[6]; un.b[6] = h;
            h = un.b[2]; un.b[2] = un.b[5]; un.b[5] = h;
            h = un.b[3]; un.b[3] = un.b[4]; un.b[4] = h;
        }
        memcpy(&bytes[index], &un.b[0], 8);
        index += 8;
    };
    auto lambdaEncodeFloat = [&bytes, &index](double value){
        union {
            float f;
            uint8_t b[4];
            uint32_t u;
        } un;
        un.f = (float)value;
        un.u = IMPSwapByteOrder(un.u);
        memcpy(&bytes[index], &un.b[0], 4);
        index += 4;
    };
    auto lambdaEncodeInt32 = [&bytes, &index](int32_t value){
        union {
            int32_t i;
            uint8_t b[4];
            uint32_t u;
        } un;
        un.i = value;
        un.u = IMPSwapByteOrder(un.u);
        memcpy(&bytes[index], &un.b[0], 4);
        index += 4;
    };

    // Encode position (LLA)
    switch(configuration.positionLLA.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.positionLLA.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeFloat(positionAltitude); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: {
                    lambdaEncodeInt32(int32_t(CLAMP(1367130550.51624 * lambdaSymmetricalRange(positionLatitude), -2147483647.0, 2147483647.0)));
                    lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(positionLongitude), -2147483648.0, 2147483647.0)));
                    break;
                }
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: {
                    lambdaEncodeInt32(int32_t(CLAMP(1367130550.51624 * lambdaSymmetricalRange(positionLatitude), -2147483647.0, 2147483647.0)));
                    lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(positionLongitude), -2147483648.0, 2147483647.0)));
                    lambdaEncodeFloat(positionAltitude);
                    break;
                }
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.positionLLA.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeDouble(positionAltitude); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: {
                    lambdaEncodeDouble(CLAMP(lambdaSymmetricalRange(positionLatitude), -1.5707963267949, 1.5707963267949));
                    lambdaEncodeDouble(lambdaSymmetricalRange(positionLongitude));
                    break;
                }
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: {
                    lambdaEncodeDouble(CLAMP(lambdaSymmetricalRange(positionLatitude), -1.5707963267949, 1.5707963267949));
                    lambdaEncodeDouble(lambdaSymmetricalRange(positionLongitude));
                    lambdaEncodeDouble(positionAltitude);
                    break;
                }
            }
            break;
        }
    }

    // Encode position (NED)
    switch(configuration.positionNED.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.positionNED.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeFloat(positionDown); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeFloat(positionNorth); lambdaEncodeFloat(positionEast); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeFloat(positionNorth); lambdaEncodeFloat(positionEast); lambdaEncodeFloat(positionDown); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.positionNED.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeDouble(positionDown); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeDouble(positionNorth); lambdaEncodeDouble(positionEast); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeDouble(positionNorth); lambdaEncodeDouble(positionEast); lambdaEncodeDouble(positionDown); break;
            }
            break;
        }
    }

    // Encode quaternion
    switch(configuration.orientationQuaternion.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.orientationQuaternion.available){
                lambdaEncodeInt32(int32_t(CLAMP(2147483647.0 * orientationQuaternionW, -2147483647.0, 2147483647.0)));
                lambdaEncodeInt32(int32_t(CLAMP(2147483647.0 * orientationQuaternionX, -2147483647.0, 2147483647.0)));
                lambdaEncodeInt32(int32_t(CLAMP(2147483647.0 * orientationQuaternionY, -2147483647.0, 2147483647.0)));
                lambdaEncodeInt32(int32_t(CLAMP(2147483647.0 * orientationQuaternionZ, -2147483647.0, 2147483647.0)));
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.orientationQuaternion.available){
                lambdaEncodeDouble(orientationQuaternionW);
                lambdaEncodeDouble(orientationQuaternionX);
                lambdaEncodeDouble(orientationQuaternionY);
                lambdaEncodeDouble(orientationQuaternionZ);
            }
            break;
        }
    }

    // Encode euler angles
    switch(configuration.orientationRollPitchYaw.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.orientationRollPitchYaw.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: {
                    lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(orientationYaw), -2147483648.0, 2147483647.0)));
                    break;
                }
                case STATE_CONFIGURATION_AVAILABILITY_XY: {
                    lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(orientationRoll), -2147483648.0, 2147483647.0)));
                    lambdaEncodeInt32(int32_t(CLAMP(1367130550.51624 * lambdaSymmetricalRange(orientationPitch), -2147483647.0, 2147483647.0)));
                    break;
                }
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: {
                    lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(orientationRoll), -2147483648.0, 2147483647.0)));
                    lambdaEncodeInt32(int32_t(CLAMP(1367130550.51624 * lambdaSymmetricalRange(orientationPitch), -2147483647.0, 2147483647.0)));
                    lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(orientationYaw), -2147483648.0, 2147483647.0)));
                    break;
                }
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.orientationRollPitchYaw.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeDouble(orientationYaw); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeDouble(orientationRoll); lambdaEncodeDouble(orientationPitch); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeDouble(orientationRoll); lambdaEncodeDouble(orientationPitch); lambdaEncodeDouble(orientationYaw); break;
            }
            break;
        }
    }

    // Encode velocity (NED, UVW)
    switch(configuration.velocityNEDUVW.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.velocityNEDUVW.availableE){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeFloat(velocityDown); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeFloat(velocityNorth); lambdaEncodeFloat(velocityEast); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeFloat(velocityNorth); lambdaEncodeFloat(velocityEast); lambdaEncodeFloat(velocityDown); break;
            }
            switch(configuration.velocityNEDUVW.availableB){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeFloat(velocityW); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeFloat(velocityU); lambdaEncodeFloat(velocityV); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeFloat(velocityU); lambdaEncodeFloat(velocityV); lambdaEncodeFloat(velocityW); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.velocityNEDUVW.availableE){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeDouble(velocityDown); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeDouble(velocityNorth); lambdaEncodeDouble(velocityEast); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeDouble(velocityNorth); lambdaEncodeDouble(velocityEast); lambdaEncodeDouble(velocityDown); break;
            }
            switch(configuration.velocityNEDUVW.availableB){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeDouble(velocityW); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeDouble(velocityU); lambdaEncodeDouble(velocityV); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeDouble(velocityU); lambdaEncodeDouble(velocityV); lambdaEncodeDouble(velocityW); break;
            }
            break;
        }
    }

    // Encode velocity (PQR)
    switch(configuration.velocityPQR.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.velocityPQR.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeFloat(velocityR); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeFloat(velocityP); lambdaEncodeFloat(velocityQ); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeFloat(velocityP); lambdaEncodeFloat(velocityQ); lambdaEncodeFloat(velocityR); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.velocityPQR.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeDouble(velocityR); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeDouble(velocityP); lambdaEncodeDouble(velocityQ); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeDouble(velocityP); lambdaEncodeDouble(velocityQ); lambdaEncodeDouble(velocityR); break;
            }
            break;
        }
    }

    // Encode acceleration (NED, XYZ)
    switch(configuration.accelerationNEDXYZ.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.accelerationNEDXYZ.availableE){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeFloat(accelerationDown); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeFloat(accelerationNorth); lambdaEncodeFloat(accelerationEast); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeFloat(accelerationNorth); lambdaEncodeFloat(accelerationEast); lambdaEncodeFloat(accelerationDown); break;
            }
            switch(configuration.accelerationNEDXYZ.availableB){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeFloat(accelerationZ); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeFloat(accelerationX); lambdaEncodeFloat(accelerationY); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeFloat(accelerationX); lambdaEncodeFloat(accelerationY); lambdaEncodeFloat(accelerationZ); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.accelerationNEDXYZ.availableE){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeDouble(accelerationDown); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeDouble(accelerationNorth); lambdaEncodeDouble(accelerationEast); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeDouble(accelerationNorth); lambdaEncodeDouble(accelerationEast); lambdaEncodeDouble(accelerationDown); break;
            }
            switch(configuration.accelerationNEDXYZ.availableB){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: lambdaEncodeDouble(accelerationZ); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: lambdaEncodeDouble(accelerationX); lambdaEncodeDouble(accelerationY); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: lambdaEncodeDouble(accelerationX); lambdaEncodeDouble(accelerationY); lambdaEncodeDouble(accelerationZ); break;
            }
            break;
        }
    }

    // Encode course over ground
    switch(configuration.courseOverGround.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.courseOverGround.available){
                lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(courseOverGround), -2147483648.0, 2147483647.0)));
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.courseOverGround.available){
                lambdaEncodeDouble(lambdaSymmetricalRange(courseOverGround));
            }
            break;
        }
    }

    // Encode speed over ground
    switch(configuration.speedOverGround.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.speedOverGround.available){
                lambdaEncodeFloat(speedOverGround);
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.speedOverGround.available){
                lambdaEncodeDouble(speedOverGround);
            }
            break;
        }
    }

    // Encode angle of attack
    switch(configuration.angleOfAttack.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.angleOfAttack.available){
                lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(angleOfAttack), -2147483648.0, 2147483647.0)));
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.angleOfAttack.available){
                lambdaEncodeDouble(lambdaSymmetricalRange(angleOfAttack));
            }
            break;
        }
    }

    // Encode side slip angle
    switch(configuration.sideSlipAngle.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.sideSlipAngle.available){
                lambdaEncodeInt32(int32_t(CLAMP(683565275.576432 * lambdaSymmetricalRange(sideSlipAngle), -2147483648.0, 2147483647.0)));
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.sideSlipAngle.available){
                lambdaEncodeDouble(lambdaSymmetricalRange(sideSlipAngle));
            }
            break;
        }
    }

    // Current index indicates length
    return index;
}

int IMP::State::Decode(const uint8_t* bytes, const uint32_t length, bool containsHeader){
    // Reset state attributes
    positionLatitude = 0.0;
    positionLongitude = 0.0;
    positionAltitude = 0.0;
    positionNorth = 0.0;
    positionEast = 0.0;
    positionDown = 0.0;
    orientationQuaternionW = 1.0;
    orientationQuaternionX = 0.0;
    orientationQuaternionY = 0.0;
    orientationQuaternionZ = 0.0;
    orientationRoll = 0.0;
    orientationPitch = 0.0;
    orientationYaw = 0.0;
    velocityNorth = 0.0;
    velocityEast = 0.0;
    velocityDown = 0.0;
    velocityU = 0.0;
    velocityV = 0.0;
    velocityW = 0.0;
    velocityP = 0.0;
    velocityQ = 0.0;
    velocityR = 0.0;
    accelerationNorth = 0.0;
    accelerationEast = 0.0;
    accelerationDown = 0.0;
    accelerationX = 0.0;
    accelerationY = 0.0;
    accelerationZ = 0.0;
    courseOverGround = 0.0;
    speedOverGround = 0.0;
    angleOfAttack = 0.0;
    sideSlipAngle = 0.0;

    // Decode header: timestamp and configuration
    int index = 0;
    if(containsHeader){
        if(length < 6){
            return -1;
        }
        timestamp = 0.0000001 * double((uint64_t(bytes[0]) << 32) | (uint64_t(bytes[1]) << 24) | (uint64_t(bytes[2]) << 16) | (uint64_t(bytes[3]) << 8) | uint64_t(bytes[4]));
        if((index = configuration.Decode(&bytes[5], length - 5)) < 0){
            return -1;
        }
        index += 5; // 5 bytes for timestamp
    }

    // Check for valid length
    uint32_t requiredPacketLength = configuration.GetRequiredPacketLength(containsHeader);
    requiredPacketLength += (int)containsHeader * 5;
    if(length < requiredPacketLength){
        return -1;
    }

    // Lambda function for symmetrical range of angles
    auto lambdaSymmetricalRange = [](double d){
        int i = (int)(d * 0.159154943091895);
        d -= 6.28318530717959 * (double)i;
        return (d < -3.14159265358979) ? (d + 6.28318530717959) : ((d >= 3.14159265358979) ? (d - 6.28318530717959) : d);
    };

    // Lambda functions for data type decoding
    auto lambdaDecodeDouble = [&bytes, &index](){
        union {
            double d;
            uint8_t b[8];
        } un;
        memcpy(&un.b[0], &bytes[index], 8);
        if(1 != IMPSwapByteOrder(1)){
            uint8_t h = un.b[0]; un.b[0] = un.b[7]; un.b[7] = h;
            h = un.b[1]; un.b[1] = un.b[6]; un.b[6] = h;
            h = un.b[2]; un.b[2] = un.b[5]; un.b[5] = h;
            h = un.b[3]; un.b[3] = un.b[4]; un.b[4] = h;
        }
        index += 8;
        return un.d;
    };
    auto lambdaDecodeFloat = [&bytes, &index](){
        union {
            float f;
            uint8_t b[4];
            uint32_t u;
        } un;
        memcpy(&un.b[0], &bytes[index], 4);
        un.u = IMPSwapByteOrder(un.u);
        index += 4;
        return (double)un.f;
    };
    auto lambdaDecodeInt32 = [&bytes, &index](){
        union {
            int32_t i;
            uint8_t b[4];
            uint32_t u;
        } un;
        memcpy(&un.b[0], &bytes[index], 4);
        un.u = IMPSwapByteOrder(un.u);
        index += 4;
        return (double)un.i;
    };

    // Decode position (LLA)
    switch(configuration.positionLLA.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.positionLLA.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: positionAltitude = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: positionLatitude = 7.31459039974192e-10 * lambdaDecodeInt32(); positionLongitude = 1.46291807926716e-09 * lambdaDecodeInt32(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: positionLatitude = 7.31459039974192e-10 * lambdaDecodeInt32(); positionLongitude = 1.46291807926716e-09 * lambdaDecodeInt32(); positionAltitude = lambdaDecodeFloat(); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.positionLLA.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: positionAltitude = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: positionLatitude = lambdaDecodeDouble(); positionLatitude = CLAMP(lambdaSymmetricalRange(positionLatitude), -1.5707963267949, 1.5707963267949); positionLongitude = lambdaSymmetricalRange(lambdaDecodeDouble()); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: positionLatitude = lambdaDecodeDouble(); positionLatitude = CLAMP(lambdaSymmetricalRange(positionLatitude), -1.5707963267949, 1.5707963267949); positionLongitude = lambdaSymmetricalRange(lambdaDecodeDouble()); positionAltitude = lambdaDecodeDouble(); break;
            }
            break;
        }
    }

    // Decode position (NED)
    switch(configuration.positionNED.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.positionNED.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: positionDown = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: positionNorth = lambdaDecodeFloat(); positionEast = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: positionNorth = lambdaDecodeFloat(); positionEast = lambdaDecodeFloat(); positionDown = lambdaDecodeFloat(); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.positionNED.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: positionDown = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: positionNorth = lambdaDecodeDouble(); positionEast = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: positionNorth = lambdaDecodeDouble(); positionEast = lambdaDecodeDouble(); positionDown = lambdaDecodeDouble(); break;
            }
            break;
        }
    }

    // Decode quaternion
    switch(configuration.orientationQuaternion.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.orientationQuaternion.available){
                orientationQuaternionW = 4.6566128752458e-10 * lambdaDecodeInt32();
                orientationQuaternionX = 4.6566128752458e-10 * lambdaDecodeInt32();
                orientationQuaternionY = 4.6566128752458e-10 * lambdaDecodeInt32();
                orientationQuaternionZ = 4.6566128752458e-10 * lambdaDecodeInt32();
                double d = sqrt(orientationQuaternionW*orientationQuaternionW + orientationQuaternionX*orientationQuaternionX + orientationQuaternionY*orientationQuaternionY + orientationQuaternionZ*orientationQuaternionZ);
                if(d < 1e-6){
                    orientationQuaternionW = 1.0;
                    orientationQuaternionX = 0.0;
                    orientationQuaternionY = 0.0;
                    orientationQuaternionZ = 0.0;
                }
                else{
                    d = 1.0 / d;
                    orientationQuaternionW *= d;
                    orientationQuaternionX *= d;
                    orientationQuaternionY *= d;
                    orientationQuaternionZ *= d;
                }
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.orientationQuaternion.available){
                orientationQuaternionW = lambdaDecodeDouble(); orientationQuaternionW = CLAMP(orientationQuaternionW, -1.0, 1.0);
                orientationQuaternionX = lambdaDecodeDouble(); orientationQuaternionX = CLAMP(orientationQuaternionX, -1.0, 1.0);
                orientationQuaternionY = lambdaDecodeDouble(); orientationQuaternionY = CLAMP(orientationQuaternionY, -1.0, 1.0);
                orientationQuaternionZ = lambdaDecodeDouble(); orientationQuaternionZ = CLAMP(orientationQuaternionZ, -1.0, 1.0);
                double d = sqrt(orientationQuaternionW*orientationQuaternionW + orientationQuaternionX*orientationQuaternionX + orientationQuaternionY*orientationQuaternionY + orientationQuaternionZ*orientationQuaternionZ);
                if(d < 1e-6){
                    orientationQuaternionW = 1.0;
                    orientationQuaternionX = 0.0;
                    orientationQuaternionY = 0.0;
                    orientationQuaternionZ = 0.0;
                }
                else{
                    d = 1.0 / d;
                    orientationQuaternionW *= d;
                    orientationQuaternionX *= d;
                    orientationQuaternionY *= d;
                    orientationQuaternionZ *= d;
                }
            }
            break;
        }
    }

    // Decode euler angles
    switch(configuration.orientationRollPitchYaw.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.orientationRollPitchYaw.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: orientationYaw = 1.46291807926716e-09 * lambdaDecodeInt32(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: orientationRoll = 1.46291807926716e-09 * lambdaDecodeInt32(); orientationPitch = 7.31459039974192e-10 * lambdaDecodeInt32(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: orientationRoll = 1.46291807926716e-09 * lambdaDecodeInt32(); orientationPitch = 7.31459039974192e-10 * lambdaDecodeInt32(); orientationYaw = 1.46291807926716e-09 * lambdaDecodeInt32(); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.orientationRollPitchYaw.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: orientationYaw = lambdaSymmetricalRange(lambdaDecodeDouble()); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: orientationRoll = lambdaSymmetricalRange(lambdaDecodeDouble()); orientationPitch = lambdaDecodeDouble(); orientationPitch = CLAMP(orientationPitch, -1.5707963267949, 1.5707963267949); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: orientationRoll = lambdaSymmetricalRange(lambdaDecodeDouble()); orientationPitch = lambdaDecodeDouble(); orientationPitch = CLAMP(orientationPitch, -1.5707963267949, 1.5707963267949); orientationYaw = lambdaSymmetricalRange(lambdaDecodeDouble()); break;
            }
            break;
        }
    }

    // Decode velocity (NED, UVW)
    switch(configuration.velocityNEDUVW.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.velocityNEDUVW.availableE){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: velocityDown = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: velocityNorth = lambdaDecodeFloat(); velocityEast = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: velocityNorth = lambdaDecodeFloat(); velocityEast = lambdaDecodeFloat(); velocityDown = lambdaDecodeFloat(); break;
            }
            switch(configuration.velocityNEDUVW.availableB){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: velocityW = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: velocityU = lambdaDecodeFloat(); velocityV = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: velocityU = lambdaDecodeFloat(); velocityV = lambdaDecodeFloat(); velocityW = lambdaDecodeFloat(); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.velocityNEDUVW.availableE){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: velocityDown = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: velocityNorth = lambdaDecodeDouble(); velocityEast = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: velocityNorth = lambdaDecodeDouble(); velocityEast = lambdaDecodeDouble(); velocityDown = lambdaDecodeDouble(); break;
            }
            switch(configuration.velocityNEDUVW.availableB){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: velocityW = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: velocityU = lambdaDecodeDouble(); velocityV = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: velocityU = lambdaDecodeDouble(); velocityV = lambdaDecodeDouble(); velocityW = lambdaDecodeDouble(); break;
            }
            break;
        }
    }

    // Decode velocity (PQR)
    switch(configuration.velocityPQR.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.velocityPQR.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: velocityR = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: velocityP = lambdaDecodeFloat(); velocityQ = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: velocityP = lambdaDecodeFloat(); velocityQ = lambdaDecodeFloat(); velocityR = lambdaDecodeFloat(); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.velocityPQR.available){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: velocityR = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: velocityP = lambdaDecodeDouble(); velocityQ = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: velocityP = lambdaDecodeDouble(); velocityQ = lambdaDecodeDouble(); velocityR = lambdaDecodeDouble(); break;
            }
            break;
        }
    }

    // Decode acceleration (NED, XYZ)
    switch(configuration.accelerationNEDXYZ.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            switch(configuration.accelerationNEDXYZ.availableE){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: accelerationDown = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: accelerationNorth = lambdaDecodeFloat(); accelerationEast = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: accelerationNorth = lambdaDecodeFloat(); accelerationEast = lambdaDecodeFloat(); accelerationDown = lambdaDecodeFloat(); break;
            }
            switch(configuration.accelerationNEDXYZ.availableB){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: accelerationZ = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: accelerationX = lambdaDecodeFloat(); accelerationY = lambdaDecodeFloat(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: accelerationX = lambdaDecodeFloat(); accelerationY = lambdaDecodeFloat(); accelerationZ = lambdaDecodeFloat(); break;
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            switch(configuration.accelerationNEDXYZ.availableE){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: accelerationDown = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: accelerationNorth = lambdaDecodeDouble(); accelerationEast = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: accelerationNorth = lambdaDecodeDouble(); accelerationEast = lambdaDecodeDouble(); accelerationDown = lambdaDecodeDouble(); break;
            }
            switch(configuration.accelerationNEDXYZ.availableB){
                case STATE_CONFIGURATION_AVAILABILITY_NONE: break;
                case STATE_CONFIGURATION_AVAILABILITY_Z: accelerationZ = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XY: accelerationX = lambdaDecodeDouble(); accelerationY = lambdaDecodeDouble(); break;
                case STATE_CONFIGURATION_AVAILABILITY_XYZ: accelerationX = lambdaDecodeDouble(); accelerationY = lambdaDecodeDouble(); accelerationZ = lambdaDecodeDouble(); break;
            }
            break;
        }
    }

    // Decode course over ground
    switch(configuration.courseOverGround.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.courseOverGround.available){
                courseOverGround = 1.46291807926716e-09 * lambdaDecodeInt32();
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.courseOverGround.available){
                courseOverGround = lambdaSymmetricalRange(lambdaDecodeDouble());
            }
            break;
        }
    }

    // Decode speed over ground
    switch(configuration.speedOverGround.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.speedOverGround.available){
                speedOverGround = lambdaDecodeFloat();
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.speedOverGround.available){
                speedOverGround = lambdaDecodeDouble();
            }
            break;
        }
    }

    // Decode angle of attack
    switch(configuration.angleOfAttack.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.angleOfAttack.available){
                angleOfAttack = 1.46291807926716e-09 * lambdaDecodeInt32();
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.angleOfAttack.available){
                angleOfAttack = lambdaSymmetricalRange(lambdaDecodeDouble());
            }
            break;
        }
    }

    // Decode side slip angle
    switch(configuration.sideSlipAngle.representation){
        case STATE_CONFIGURATION_REPRESENTATION_4BYTES: {
            if(configuration.sideSlipAngle.available){
                sideSlipAngle = 1.46291807926716e-09 * lambdaDecodeInt32();
            }
            break;
        }
        case STATE_CONFIGURATION_REPRESENTATION_8BYTES: {
            if(configuration.sideSlipAngle.available){
                sideSlipAngle = lambdaSymmetricalRange(lambdaDecodeDouble());
            }
            break;
        }
    }

    // Current index indicates length
    return index;
}

