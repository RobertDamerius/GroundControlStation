#include <RigidBodyState.hpp>


RigidBodyState::RigidBodyState(){
    Reset();
}

RigidBodyState::RigidBodyState(const RigidBodyState& rbs){
    this->latitude = rbs.latitude;
    this->longitude = rbs.longitude;
    this->altitude = rbs.altitude;
    this->position = rbs.position;
    this->quaternion = rbs.quaternion;
    this->u = rbs.u;
    this->v = rbs.v;
    this->w = rbs.w;
    this->p = rbs.p;
    this->q = rbs.q;
    this->r = rbs.r;
    this->velocityNorth = rbs.velocityNorth;
    this->velocityEast = rbs.velocityEast;
    this->velocityDown = rbs.velocityDown;
    this->roll = rbs.roll;
    this->pitch = rbs.pitch;
    this->yaw = rbs.yaw;
    this->sog = rbs.sog;
    this->overridePosition = rbs.overridePosition;
}

RigidBodyState::RigidBodyState(const IMP::State& state){
    CopyFromIMPState(state);
}

RigidBodyState::~RigidBodyState(){}

void RigidBodyState::Reset(void){
    this->latitude = 0.0;
    this->longitude = 0.0;
    this->altitude = 0.0;
    this->position = glm::dvec3(0.0);
    this->quaternion = glm::dvec4(0.0, 0.0, 0.0, 1.0);
    this->u = 0.0;
    this->v = 0.0;
    this->w = 0.0;
    this->p = 0.0;
    this->q = 0.0;
    this->r = 0.0;
    this->velocityNorth = 0.0;
    this->velocityEast = 0.0;
    this->velocityDown = 0.0;
    this->roll = 0.0;
    this->pitch = 0.0;
    this->yaw = 0.0;
    this->sog = 0.0;
    this->overridePosition = false;
}

void RigidBodyState::CalculateOptionals(void){
    double q0q0 = this->quaternion.w * this->quaternion.w;
    double q1q1 = this->quaternion.x * this->quaternion.x;
    double q2q2 = this->quaternion.y * this->quaternion.y;
    double q3q3 = this->quaternion.z * this->quaternion.z;
    double q1q2 = this->quaternion.x * this->quaternion.y;
    double q0q3 = this->quaternion.w * this->quaternion.z;
    double q1q3 = this->quaternion.x * this->quaternion.z;
    double q0q2 = this->quaternion.w * this->quaternion.y;
    double q2q3 = this->quaternion.y * this->quaternion.z;
    double q0q1 = this->quaternion.w * this->quaternion.x;
    double d = q1q3+q1q3-q0q2-q0q2;
    this->velocityNorth = (q0q0+q1q1-q2q2-q3q3)*u + (q1q2+q1q2-q0q3-q0q3)*v + (q1q3+q1q3+q0q2+q0q2)*w;
    this->velocityEast = (q1q2+q1q2+q0q3+q0q3)*u + (q0q0-q1q1+q2q2-q3q3)*v + (q2q3+q2q3-q0q1-q0q1)*w;
    this->velocityDown = (q1q3+q1q3-q0q2-q0q2)*u + (q2q3+q2q3+q0q1+q0q1)*v + (q0q0-q1q1-q2q2+q3q3)*w;
    this->roll = atan2(q2q3+q2q3+q0q1+q0q1, q0q0-q1q1-q2q2+q3q3);
    this->pitch = -asin((d < -1.0) ? -1.0 : ((d > 1.0) ? 1.0 : d));
    this->yaw = atan2(q1q2+q1q2+q0q3+q0q3, q0q0+q1q1-q2q2-q3q3);
    this->sog = sqrt(velocityNorth*velocityNorth + velocityEast*velocityEast);
}

RigidBodyState& RigidBodyState::operator=(const RigidBodyState& rhs){
    this->latitude = rhs.latitude;
    this->longitude = rhs.longitude;
    this->altitude = rhs.altitude;
    this->position = rhs.position;
    this->quaternion = rhs.quaternion;
    this->u = rhs.u;
    this->v = rhs.v;
    this->w = rhs.w;
    this->p = rhs.p;
    this->q = rhs.q;
    this->r = rhs.r;
    this->velocityNorth = rhs.velocityNorth;
    this->velocityEast = rhs.velocityEast;
    this->velocityDown = rhs.velocityDown;
    this->roll = rhs.roll;
    this->pitch = rhs.pitch;
    this->yaw = rhs.yaw;
    this->sog = rhs.sog;
    this->overridePosition = rhs.overridePosition;
    return *this;
}

RigidBodyState& RigidBodyState::operator=(const IMP::State& rhs){
    this->CopyFromIMPState(rhs);
    return *this;
}

void RigidBodyState::CopyFromIMPState(const IMP::State& state){
    this->Reset();

    // Position LLA
    switch(state.configuration.positionLLA.available){
        case IMP::STATE_CONFIGURATION_AVAILABILITY_NONE: break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_Z: this->altitude = state.positionAltitude; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XY: this->latitude = state.positionLatitude; this->longitude = state.positionLongitude; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ: this->latitude = state.positionLatitude; this->longitude = state.positionLongitude; this->altitude = state.positionAltitude; break;
    }

    // Position NED
    switch(state.configuration.positionNED.available){
        case IMP::STATE_CONFIGURATION_AVAILABILITY_NONE: break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_Z: this->position.z = state.positionDown; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XY: this->position.x = state.positionNorth; this->position.y = state.positionEast; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ: this->position.x = state.positionNorth; this->position.y = state.positionEast; this->position.z = state.positionDown; break;
    }

    // Quaternion
    if(state.configuration.orientationQuaternion.available){
        this->quaternion.w = state.orientationQuaternionW;
        this->quaternion.x = state.orientationQuaternionX;
        this->quaternion.y = state.orientationQuaternionY;
        this->quaternion.z = state.orientationQuaternionZ;
    }

    // Velocity UVW
    switch(state.configuration.velocityNEDUVW.availableB){
        case IMP::STATE_CONFIGURATION_AVAILABILITY_NONE: break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_Z: this->w = state.velocityW; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XY: this->u = state.velocityU; this->v = state.velocityV; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ: this->u = state.velocityU; this->v = state.velocityV; this->w = state.velocityW; break;
    }

    // Velocity PQR
    switch(state.configuration.velocityPQR.available){
        case IMP::STATE_CONFIGURATION_AVAILABILITY_NONE: break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_Z: this->r = state.velocityR; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XY: this->p = state.velocityP; this->q = state.velocityQ; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ: this->p = state.velocityP; this->q = state.velocityQ; this->r = state.velocityR; break;
    }

    // Velocity NED
    switch(state.configuration.velocityNEDUVW.availableE){
        case IMP::STATE_CONFIGURATION_AVAILABILITY_NONE: break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_Z: this->velocityDown = state.velocityDown; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XY: this->velocityNorth = state.velocityNorth; this->velocityEast = state.velocityEast; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ: this->velocityNorth = state.velocityNorth; this->velocityEast = state.velocityEast; this->velocityDown = state.velocityDown; break;
    }

    // Roll, pitch, yaw
    switch(state.configuration.orientationRollPitchYaw.available){
        case IMP::STATE_CONFIGURATION_AVAILABILITY_NONE: break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_Z: this->yaw = state.orientationYaw; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XY: this->roll = state.orientationRoll; this->pitch = state.orientationPitch; break;
        case IMP::STATE_CONFIGURATION_AVAILABILITY_XYZ: this->roll = state.orientationRoll; this->pitch = state.orientationPitch; this->yaw = state.orientationYaw; break;
    }

    // SOG
    if(state.configuration.speedOverGround.available){
        this->sog = state.speedOverGround;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Optional settings
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Position (NED) can be overwritten if it's not available but (LLA) is available
    this->overridePosition = (IMP::STATE_CONFIGURATION_AVAILABILITY_NONE == state.configuration.positionNED.available) && (IMP::STATE_CONFIGURATION_AVAILABILITY_NONE != state.configuration.positionLLA.available);

    // If quaternion is not available, calculate quaternion from euler angles
    if(!state.configuration.orientationQuaternion.available){
        double phi = 0.5 * this->roll(), theta = 0.5 * this->pitch(), psi = 0.5 * this->yaw();
        double c1 = cos(phi), c2 = cos(theta), c3 = cos(psi), s1 = sin(phi), s2 = sin(theta), s3 = sin(psi);
        this->quaternion.w = c1*c2*c3 + s1*s2*s3;
        this->quaternion.x = s1*c2*c3 - c1*s2*s3;
        this->quaternion.y = c1*s2*c3 + s1*c2*s3;
        this->quaternion.z = c1*c2*s3 - s1*s2*c3;
    }
    double q0q0 = this->quaternion.w * this->quaternion.w;
    double q1q1 = this->quaternion.x * this->quaternion.x;
    double q2q2 = this->quaternion.y * this->quaternion.y;
    double q3q3 = this->quaternion.z * this->quaternion.z;
    double q1q2 = this->quaternion.x * this->quaternion.y;
    double q0q3 = this->quaternion.w * this->quaternion.z;
    double q1q3 = this->quaternion.x * this->quaternion.z;
    double q0q2 = this->quaternion.w * this->quaternion.y;
    double q2q3 = this->quaternion.y * this->quaternion.z;
    double q0q1 = this->quaternion.w * this->quaternion.x;

    // If euler angles are not available, calculate from quaternion
    if(IMP::STATE_CONFIGURATION_AVAILABILITY_NONE == state.configuration.orientationRollPitchYaw.available){
        this->roll = atan2(q2q3+q2q3+q0q1+q0q1, q0q0-q1q1-q2q2+q3q3);
        this->pitch = -(asin(std::clamp(q1q3+q1q3-q0q2-q0q2,-1.0,1.0)));
        this->yaw = atan2(q1q2+q1q2+q0q3+q0q3, q0q0+q1q1-q2q2-q3q3);
    }

    // If vNED is not available, calculate from u,v,r and orientation
    if(IMP::STATE_CONFIGURATION_AVAILABILITY_NONE == state.configuration.velocityNEDUVW.availableE){
        this->velocityNorth = (q0q0+q1q1-q2q2-q3q3)*this->u + (q1q2+q1q2-q0q3-q0q3)*this->v + (q1q3+q1q3+q0q2+q0q2)*this->w;
        this->velocityEast = (q1q2+q1q2+q0q3+q0q3)*this->u + (q0q0-q1q1+q2q2-q3q3)*this->v + (q2q3+q2q3-q0q1-q0q1)*this->w;
        this->velocityDown = (q1q3+q1q3-q0q2-q0q2)*this->u + (q2q3+q2q3+q0q1+q0q1)*this->v + (q0q0-q1q1-q2q2+q3q3)*this->w;
    }

    // If SOG is not available, calculate from vNED
    if(!state.configuration.speedOverGround.available){
        this->sog = sqrt(velocityNorth*velocityNorth + velocityEast*velocityEast);
    }
}

