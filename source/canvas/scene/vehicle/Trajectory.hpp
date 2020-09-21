#pragma once


#include <WaypointList.hpp>


class Trajectory: public WaypointList {
    public:
        /**
         *  @brief Create a trajectory object.
         */
        Trajectory();

        /**
         *  @brief Default copy constructor.
         */
        Trajectory(const Trajectory& t) = default;

        /**
         *  @brief Delete the trajectory object.
         */
        ~Trajectory();

        /* Operators */
        Trajectory& operator=(const Trajectory& rhs);

        /**
         *  @brief Set the sampletime for the trajectory.
         *  @param [in] dt Sampletime in seconds.
         */
        void SetSampletime(double dt);

        /**
         *  @brief Get the sampletime of the trajectory.
         *  @return The sampletime in seconds.
         */
        inline double GetSampletime(void){ return this->dt; }

        /**
         *  @brief Set the vehicle stride.
         *  @param [in] stride Vehicle stride value in seconds.
         */
        void SetVehicleStride(double stride);

        /**
         *  @brief Get the vehicle stride.
         *  @return The vehicle stride value in seconds.
         */
        inline double GetVehicleStride(void){ return this->vehicleStride; }

    protected:
        double dt;              ///< Sampletime in seconds.
        double vehicleStride;   ///< Vehicle stride value in seconds.

        /**
         *  @brief Generate the GL content for the line strip.
         */
        void Generate(void) override;
};

