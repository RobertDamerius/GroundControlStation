#ifndef APP_WINDOW_HPP
#define APP_WINDOW_HPP


#include <GLWindow.hpp>
#include <Canvas.hpp>
#include <JoystickInterface.hpp>


class AppWindow: public GLWindow {
    public:
        Canvas canvas;
        JoystickInterface joystickInterface;

        /**
         *  @brief Create an application window.
         */
        AppWindow();

        /**
         *  @brief Delete the application window.
         */
        ~AppWindow();

        /**
         *  @brief Get the minimum size of a framebuffer.
         *  @param [in] size The size that should be ensured to be at least the minimum size.
         *  @return The input size or the minimum size.
         */
        static glm::ivec2 MinSize(glm::ivec2 size);

        /**
         *  @brief Add a GL task that should be executed before the next canvas rendering.
         *  @param [in] f The function object of type void(void).
         */
        void AddGLTask(std::function<void(void)> f);

    protected:
        /**
         *  @brief Initialization callback function.
         *  @param [in] wnd The GLFW window.
         *  @return True if success, false otherwise.
         *  @details Overwrite this member function for user-specific initializations at the end of the window creation.
         */
        bool CallbackInitialize(GLFWwindow* wnd);

        /**
         *  @brief Terminate callback function.
         *  @param [in] wnd The GLFW window.
         *  @details Overwrite this member function for user-specific termination before the actual window is destroyed.
         */
        void CallbackTerminate(GLFWwindow* wnd);

        /**
         *  @brief Render callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] dt Elapsed time in seconds.
         *  @param [in] deltaCursor Elapsed cursor position if mouse cursor is captured.
         */
        void CallbackRender(GLFWwindow* wnd, double dt, glm::dvec2 deltaCursor);

    private:
        std::vector<std::function<void(void)>> glTasks;
        std::mutex mtxGLTasks;
};


extern AppWindow appWindow;


#endif /* APP_WINDOW_HPP */

