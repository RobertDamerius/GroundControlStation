#pragma once


#include <Vehicle.hpp>
#include <ShaderVehicle.hpp>
#include <ShaderTransparentVehicle.hpp>
#include <ShaderSceneText.hpp>
#include <ShaderLineStrip.hpp>
#include <ShaderCircularLineBuffer.hpp>
#include <RigidBodyState.hpp>


class VehicleManager {
    public:
        /**
         * @brief Create a vehicle manager.
         */
        VehicleManager();

        /**
         * @brief Delete the vehicle manager.
         */
        ~VehicleManager();

        /**
         * @brief Initialize the vehicle manager.
         * @return True if success, false otherwise.
         */
        bool Initialize(void);

        /**
         * @brief Terminate the vehicle manager.
         * @details This will delete the GL content for all vehicles and clear the vehicle list.
         */
        void Terminate(void);

        /**
         * @brief Add a new vehicle (thread-safe).
         * @param [in] id The vehicle ID that identifies the vehicle. The AABB will be updated.
         * @param [in] v The vehicle that should be added.
         * @details If this vehicle (ID) was already added, nothing will happen.
         * @note The name of the vehicle is overwritten by the name of the vehicle ID.
         */
        void AddVehicle(VehicleID& id, Vehicle& v);

        /**
         * @brief Update the navigation state of a vehicle.
         * @param [in] id ID of the vehicle that should be updated.
         * @param [in] stateNavigation New vehicle state (navigation).
         */
        void UpdateVehicleNavigation(const VehicleID& id, RigidBodyState& stateNavigation);

        /**
         * @brief Update the guidance state of a vehicle.
         * @param [in] id ID of the vehicle that should be updated.
         * @param [in] stateGuidance New vehicle state (guidance).
         */
        void UpdateVehicleGuidance(const VehicleID& id, RigidBodyState& stateGuidance);

        /**
         * @brief Set waypoints for a vehicle.
         * @param [in] id ID of the vehicle.
         * @param [inout] waypoints List of waypoints (is swapped internally).
         */
        void SetWaypoints(const VehicleID& id, std::vector<RigidBodyState>& waypoints);

        /**
         * @brief Set trajectory for a vehicle.
         * @param [in] id ID of the vehicle.
         * @param [inout] trajectory Trajectory that should be set (is swapped internally).
         * @param [in] dt Trajectory sampletime in seconds.
         */
        void SetTrajectory(const VehicleID& id, std::vector<RigidBodyState>& trajectory, double dt);

        /**
         * @brief Set a set of polygons for a vehicle.
         * @param [in] id ID of the vehicle.
         * @param [in] segmentIndex The segment index for the polygon.
         * @param [in] maxSegmentIndex The maximum segment index for the polygon.
         * @param [in] polygons Set of polygons to be added.
         * @param [in] velocities Set of velocities (u,v,r) for all polygons. Size must be equal to size of polygons.
         * @param [in] classifications Set of classifications for all polygons. Size must be equal to size of polygons.
         */
        void SetPolygons(const VehicleID& id, const uint8_t segmentIndex, const uint8_t maxSegmentIndex, std::vector<std::vector<std::array<double, 2>>>& polygons, std::vector<std::array<double, 3>> velocities, std::vector<uint8_t> classifications);

        /**
         * @brief Update the vehicle manager.
         * @param [in] dt Elapsed time in seconds.
         */
        void Update(double dt);

        /**
         * @brief Render vehicles.
         * @param [in] shaderVehicle The vehicle shader.
         * @param [in] shaderPolygon The polygon shader.
         */
        void Render(ShaderVehicle& shaderVehicle, ShaderPolygon& shaderPolygon);

        /**
         * @brief Render the labels of the vehicles.
         * @param [in] shader The scene text shader.
         */
        void RenderLabels(ShaderSceneText& shader);

        /**
         * @brief Render lines.
         * @param [in] shaderTransparentVehicle The transparent vehicle shader.
         * @param [in] shaderLine The line shader.
         * @param [in] shaderLineStrip The line strip shader.
         * @param [in] shaderCircularLineBuffer The circular line buffer shader.
         */
        void RenderLines(ShaderTransparentVehicle& shaderTransparentVehicle, ShaderLine& shaderLine, ShaderLineStrip& shaderLineStrip, ShaderCircularLineBuffer& shaderCircularLineBuffer);

        /**
         * @brief Get the AABB that includes all vehicles.
         * @return AABB that includes all vehicles.
         */
        AxisAlignedBoundingBox GetAABBOfVehicles(void);

        /**
         * @brief Get list of vehicles.
         * @param [out] out The list where to store the vehicles.
         */
        void GetVehiclesList(std::vector<std::tuple<VehicleID, RigidBodyState, RigidBodyState>>& out);

        /**
         * @brief Get the vehicle name based on the vehicle ID.
         * @param [in] id The vehicle ID.
         * @return Name of the vehicle with that ID or empty string.
         */
        std::string GetVehicleName(const VehicleID& id);

        /**
         * @brief Get the vehicle description based on the vehicle ID.
         * @param [in] id The vehicle ID.
         * @return Description of the vehicle with that ID or empty string.
         */
        std::string GetVehicleDescription(const VehicleID& id);

        /**
         * @brief Get the vehicle state for a vehicle ID.
         * @param [in] id The vehicle ID.
         * @param [out] guidance The guidance state of the vehicle.
         * @param [out] navigation The guidance state of the vehicle.
         * @param [out] timeToLatestMessage Structure containing the time to the latest messages.
         */
        void GetVehicleState(const VehicleID& id, RigidBodyState& guidance, RigidBodyState& navigation, Vehicle::TimeOfLatestMessage& timeOfLatestMessage);

        /**
         * @brief Lock a vehicle.
         * @param [in] id The ID of the vehicle.
         * @return Pointer to the vehicle object or nullptr if the vehicle ID does not exist.
         * @details If the returned pointer is valid, the @ref UnlockVehicle member function must be called!
         * @note This will lock the whole vehicle manager. The locked section should be as small as possible.
         */
        Vehicle* LockVehicle(const VehicleID& id);

        /**
         * @brief Unlock a vehicle that has been locked by the @ref LockVehicle member function.
         */
        void UnlockVehicle(void);

        /**
         * @brief Save the style for a vehicle to a JSON file.
         * @param [in] id The ID of the vehicle to be saved.
         */
        void SaveVehicleStyle(const VehicleID& id);

    private:
        std::mutex mtxVehicles;
        std::unordered_map<VehicleID, Vehicle> vehicles;
        std::vector<VehicleID> vehiclesToGenerate;
};

