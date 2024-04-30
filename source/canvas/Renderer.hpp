#pragma once


#include <RD.hpp>
#include <GLCallback.hpp>
#include <Scene.hpp>
#include <GUI.hpp>
#include <FrameBufferGUI.hpp>
#include <FrameBufferGBuffer.hpp>
#include <FrameBufferEnvironment.hpp>
#include <ShaderVehicle.hpp>
#include <ShaderPolygon.hpp>
#include <ShaderTransparentVehicle.hpp>
#include <ShaderGroundPlane.hpp>
#include <ShaderEnvironment.hpp>
#include <ShaderPostProcessing.hpp>
#include <ShaderSceneText.hpp>
#include <ShaderLine.hpp>
#include <ShaderLineStrip.hpp>
#include <ShaderCircularLineBuffer.hpp>


class Renderer: public GLCallback {
    public:
        /**
         *  @brief Create the renderer.
         */
        Renderer();

        /**
         *  @brief Delete the renderer.
         */
        ~Renderer();

        /**
         *  @brief Initialize the renderer.
         *  @return True if success, false otherwise.
         */
        bool Initialize(void);

        /**
         *  @brief Terminate the renderer.
         */
        void Terminate(void);

        /**
         *  @brief The rendering event.
         *  @param [in] scene The scene that should be rendered.
         *  @param [in] gui The GUI that should be rendered.
         */
        void Render(Scene& scene, GUI& gui);

        /**
         *  @brief Set viewport and bind the GUI framebuffer.
         */
        void BindGUIFramebuffer(void);

        /**
         *  @brief Set new gamma value.
         *  @param [in] gamma Gamma value.
         *  @details The thread calling this function must have the GL context.
         */
        void SetGamma(GLfloat gamma);
        inline GLfloat GetGamma(void){ return this->gamma; }

        /**
         *  @brief Set the ground plane color for the shader.
         *  @param [in] color Linear color.
         *  @return Final resulting ground plane color.
         */
        inline glm::vec3 SetGroundPlaneColor(glm::vec3& color){ this->shaderGroundPlane.Use(); return this->shaderGroundPlane.SetColor(color); }

    private:
        GLfloat gamma;                                      ///< Latest set gamma value.

        /* Shaders */
        ShaderPolygon shaderPolygon;                        ///< The polygon shader.
        ShaderVehicle shaderVehicle;                        ///< The vehicle shader.
        ShaderTransparentVehicle shaderTransparentVehicle;  ///< The transparent vehicle shader.
        ShaderGroundPlane shaderGroundPlane;                ///< The ground plane shader.
        ShaderEnvironment shaderEnvironment;                ///< The environment shader.
        ShaderPostProcessing shaderPostProcessing;          ///< The post processing shader.
        ShaderSceneText shaderSceneText;                    ///< The scene text shader.
        ShaderLine shaderLine;                              ///< The line shader.
        ShaderLineStrip shaderLineStrip;                    ///< The line strip shader.
        ShaderCircularLineBuffer shaderCircularLineBuffer;  ///< The circular line buffer shader.

        /* Framebuffers */
        FrameBufferGUI fbGUI;                               ///< The GUI framebuffer.
        FrameBufferGBuffer fbGBuffer;                       ///< The G-buffer.
        FrameBufferEnvironment fbEnvironment;               ///< The environment framebuffer.

        /**
         *  @brief Generate all framebuffers.
         *  @param [in] width Framebuffer width in pixels.
         *  @param [in] height Framebuffer height in pixels.
         *  @return True if success, false otherwise.
         */
        bool GenerateFrameBuffers(int width, int height);

        /**
         *  @brief Delete all framebuffers.
         */
        void DeleteFrameBuffers(void);

        /**
         *  @brief Generate shaders.
         *  @return True if success, false otherwise.
         */
        bool GenerateShaders(void);

        /**
         *  @brief Delete all shaders.
         */
        void DeleteShaders(void);

        /**
         *  @brief Render scene to G-buffer.
         *  @param [in] The scene that should be rendered.
         */
        void RenderToGBuffer(Scene& scene);

        /**
         *  @brief Render transparency parts.
         *  @param [in] The scene object.
         */
        void RenderTransparencyParts(Scene& scene);

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

