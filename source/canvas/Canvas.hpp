#pragma once


#include <Renderer.hpp>
#include <Scene.hpp>
#include <GUI.hpp>
#include <GLCallback.hpp>


class Canvas: public GLCallback {
    public:
        Renderer renderer;   ///< The rendering manager.
        Scene scene;         ///< Scene content.
        GUI gui;             ///< GUI manager.

        /**
         *  @brief Create the canvas.
         */
        Canvas();

        /**
         *  @brief Delete the canvas.
         */
        ~Canvas();

        /**
         *  @brief Initialize the canvas.
         *  @param wnd The GLFW window.
         *  @return True if success, false otherwise.
         */
        bool Initialize(GLFWwindow* wnd);

        /**
         *  @brief Terminate the canvas.
         */
        void Terminate(void);

        /**
         *  @brief Render the canvas
         *  @param [in] wnd The GLFW window.
         *  @param [in] dt Elapsed time in seconds.
         *  @param [in] deltaCursor Elapsed cursor position if mouse cursor is captured.
         */
        void Render(GLFWwindow* wnd, double dt, glm::dvec2 deltaCursor);
};

