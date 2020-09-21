#pragma once


#include <GLCallback.hpp>
#include <RD.hpp>
#include <OriginLLA.hpp>
#include <Skybox.hpp>
#include <GroundPlane.hpp>
#include <HumanInterface.hpp>
#include <VehicleManager.hpp>


class Scene: public GLCallback {
    public:
        glm::dvec3 renderingOffset;      ///< The rendering offset (OpenGL-frame) that is set by the scene update and has to be added to all objects before rendering with a relative camera position.

        OriginLLA origin;                ///< The geographical origin.
        Skybox skybox;                   ///< The Skybox.
        GroundPlane groundPlane;         ///< The ground plane.
        RD::Engine::Camera viewCamera;   ///< The view camera for the scene.
        HumanInterface humanInterface;   ///< The human interface.
        VehicleManager vehicleManager;   ///< The vehicle manager.

        /**
         *  @brief Create the scene.
         */
        Scene();

        /**
         *  @brief Delete the scene.
         */
        ~Scene();

        /**
         *  @brief Initialize the scene.
         *  @return True if success, false otherwise.
         */
        bool Initialize(void);

        /**
         *  @brief Terminate the scene.
         */
        void Terminate(void);

        /**
         *  @brief Update the scene.
         *  @param [in] dt Elapsed time in seconds.
         */
        void Update(double dt);

        /**
         *  @brief Set the camera mode, either perspective or orthographic.
         *  @param [in] mode The camera mode.
         *  @details This will also update the view and up directions of the camera.
         */
        void SetCameraMode(CameraMode mode);

        /**
         *  @brief Update the view camera.
         *  @param [in] size The framebuffer size.
         */
        void UpdateViewCamera(glm::ivec2 size);

    private:
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Callback functions
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Framebuffer resize callback.
         *  @param [in] wnd The GLFW window.
         *  @param [in] size Framebuffer size in pixels.
         */
        void CallbackFramebufferResize(GLFWwindow* wnd, glm::ivec2 size);
};

