#pragma once


#include <GLCallback.hpp>


class GLWindow: public GLCallback {
    public:
        /**
         *  @brief Create a GL window.
         */
        GLWindow();

        /**
         *  @brief Delete the GL window.
         */
        virtual ~GLWindow();


        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // General
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Create and initialize the window.
         *  @param [in] glWnd Pointer to the GL window derived class.
         *  @return True if success, false otherwise.
         *  @details Calls the inialization callback.
         */
        static bool Initialize(GLWindow* glWnd);

        /**
         *  @brief Terminate and destroy the window.
         *  @details Calls the termination callback.
         */
        static void Terminate(void);

        /**
         *  @brief The main loop.
         */
        static void MainLoop(void);

        /**
         *  @brief Draw a template quad: [-1,-1] to [+1,+1].
         */
        static void DrawScreenQuad(void);

        /**
         *  @brief Draw a template cube ([-1,-1,-1] to [+1,+1,+1]) where the faces point to the center of the cube.
         */
        static void DrawScreenCube(void);

        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Window Management
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Get the framebuffer size in pixels.
         *  @return Framebuffer size in pixels.
         */
        static glm::ivec2 GetSize(void);

        /**
         *  @brief Get the latest FPS value.
         *  @return Latest FPS value.
         */
        static uint32_t GetFPS(void);

        /**
         *  @brief Get the time (runtime of the main loop).
         *  @return Time in seconds.
         */
        static double GetTime(void);

        /**
         *  @brief Set the window title.
         *  @param [in] title The window title.
         */
        static void SetWindowTitle(const char* title);

        /**
         *  @brief Close the window.
         */
        static void Close(void);


        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Mouse Management
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Load a mouse cursor image using RGBA pixel data.
         *  @param [in] imageDataRGBA RGBA pixel data, arraged from left to right, top to bottom.
         *  @param [in] width Image width in pixels.
         *  @param [in] height Image height in pixels.
         *  @return True if success, false otherwise.
         */
        static bool LoadCursorImage(std::vector<uint8_t>& imageDataRGBA, int width, int height);

        /**
         *  @brief Capture the mouse cursor.
         */
        static void CaptureCursor(void);

        /**
         *  @brief Release the mouse cursor.
         */
        static void ReleaseCursor(void);

        /**
         *  @brief Check if mouse cursor is captured.
         *  @return True if mouse cursor is captured, false otherwise.
         */
        static bool CursorIsCaptured(void);


    protected:
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Overwritable Callbacks
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Initialization callback function.
         *  @param [in] wnd The GLFW window.
         *  @return True if success, false otherwise.
         *  @details Overwrite this member function for user-specific initializations at the end of the window creation.
         */
        virtual bool CallbackInitialize(GLFWwindow* wnd);

        /**
         *  @brief Terminate callback function.
         *  @param [in] wnd The GLFW window.
         *  @details Overwrite this member function for user-specific termination before the actual window is destroyed.
         */
        virtual void CallbackTerminate(GLFWwindow* wnd);

        /**
         *  @brief Render callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] dt Elapsed time in seconds.
         *  @param [in] deltaCursor Elapsed cursor position if mouse cursor is captured.
         */
        virtual void CallbackRender(GLFWwindow* wnd, double dt, glm::dvec2 deltaCursor);

    private:
        static GLFWwindow* glfwWindow;    ///< The GLFW window or zero if not created.
        static GLWindow* glWindow;        ///< The GL window instance.
        static GLuint quadVAO;            ///< VAO for screen quad.
        static GLuint quadVBO;            ///< VBO for screen quad.
        static GLuint cubeVAO;            ///< VAO for screen cube.
        static GLuint cubeVBO;            ///< VBO for screen cube.
        static GLuint cubeEBO;            ///< EBO for screen cube.
        static uint32_t fps;              ///< The latest FPS value.

}; /* class: GLWindow */

