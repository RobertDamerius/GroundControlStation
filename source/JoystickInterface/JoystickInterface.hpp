#pragma once


#include <GLCallback.hpp>
#include <Joystick.hpp>


class JoystickInterface: public GLCallback {
    public:
        std::map<uint8_t,Joystick*> joysticks;

        /**
         *  @brief Create the joystick interface.
         */
        JoystickInterface();

        /**
         *  @brief Delete the joystick interface.
         */
        ~JoystickInterface();

        /**
         *  @brief Initialize the joystick interface.
         *  @param [in] wnd The GLFW window.
         *  @return True if success, false otherwise.
         */
        bool Initialize(GLFWwindow* wnd);

        /**
         *  @brief Terminate the joystick interface.
         */
        void Terminate(void);

        /**
         *  @brief Joystick status callback function.
         *  @param [in] joy Joystick value: [GLFW_JOYSTICK_1; GLFW_JOYSTICK_16].
         *  @param [in] event Event value, either GLFW_CONNECTED or GLFW_DISCONNECTED.
         *  @param [in] name The name of the joystick or "(null)" if not connected.
         */
        void CallbackJoystickStatus(int joy, int event, const char* name);

        /**
         *  @brief Joystick data callback function.
         *  @param [in] joy Joystick value: [GLFW_JOYSTICK_1; GLFW_JOYSTICK_16].
         *  @param [in] axes Axes data in range [-1.0, +1.0].
         *  @param [in] numAxes Number of axes.
         *  @param [in] buttons Button data, either GLFW_PRESS or GLFW_RELEASE.
         *  @param [in] numButtons Number of buttons.
         *  @details This callback only occurs, if @ref PollEvents() is called.
         */
        void CallbackJoystickData(int joy, const float* axes, int numAxes, const uint8_t* buttons, int numButtons);
};

