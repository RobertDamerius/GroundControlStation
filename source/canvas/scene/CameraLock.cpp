#include <CameraLock.hpp>


CameraLock::CameraLock(){
    Reset();
}

CameraLock::~CameraLock(){}

void CameraLock::Reset(void){
    this->lock = false;
    this->lockRotation = false;
    this->zoom = 10.0;
    this->yaw = 0.0;
}

