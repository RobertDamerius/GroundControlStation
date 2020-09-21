#pragma once


#include <VehicleID.hpp>
#include <Angle.hpp>


class CameraLock {
    public:
        bool lock;
        bool lockRotation;
        VehicleID id;
        double zoom;
        Angle yaw;

        /**
         *  @brief Create a camera lock.
         */
        CameraLock();

        /**
         *  @brief Delete the camera lock.
         */
        ~CameraLock();

        /**
         *  @brief Reset to default values.
         */
        void Reset(void);
};

