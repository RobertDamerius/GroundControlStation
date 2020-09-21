#pragma once


/* Forward declaration */
class GLWindow;


class GLCallback {
    public:
        /**
         *  @brief Create a GL callback handler.
         */
        GLCallback();

        /**
         *  @brief Delete the GL callback handler.
         */
        virtual ~GLCallback();

        /**
         *  @brief Register callback functions for a child callback handler.
         *  @param [in] child The child callback handler.
         */
        void RegisterCallbacksForChild(GLCallback& child);

        /**
         *  @brief Unregister the callback functions of a child callback handler.
         *  @param [in] child The child callback handler that should be removed from the child list.
         */
        void UnregisterCallbacksOfChild(GLCallback& child);

    protected:
        friend GLWindow;

        /**
         *  @brief Set callback functions with this object beeing the root node.
         *  @param [in] glfwWindow The GLFWwindow. If the pointer is null then at least the error callback will be set.
         */
        void SetCallbacks(GLFWwindow* glfwWindow = nullptr);

        /**
         *  @brief Poll joysticks.
         */
        static void PollJoysticks(void);

        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Overwritable Callbacks
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief GLFW error callback function.
         *  @param [in] error The GLFW error code.
         *  @param [in] description A UTF-8 encoded string describing the error.
         */
        virtual void CallbackError(int error, const char* description);

        /**
         *  @brief Window close callback function.
         *  @param [in] wnd The GLFW window.
         */
        virtual void CallbackClose(GLFWwindow* wnd);

        /**
         *  @brief The key input callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] key The keyboard key that was pressed or released.
         *  @param [in] scancode The system-specific scancode of the key.
         *  @param [in] action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
         *  @param [in] mods Bit field describing which modifier keys were held down.
         */
        virtual void CallbackKey(GLFWwindow* wnd, int key, int scancode, int action, int mods);

        /**
         *  @brief The char input callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] codepoint The Unicode code point of the character.
         */
        virtual void CallbackChar(GLFWwindow* wnd, uint32_t codepoint);

        /**
         *  @brief Mouse button callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] button The GLFW mouse button that was pressed or released.
         *  @param [in] action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
         *  @param [in] mods Bit field describing which modifier keys were held down.
         */
        virtual void CallbackMouseButton(GLFWwindow* wnd, int button, int action, int mods);

        /**
         *  @brief Mouse cursor callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] cursor Position of the mouse cursor.
         *  @param [in] deltaCursor Delta cursor position if cursor is captured, glm::dvec2(0.0) otherwise.
         */
        virtual void CallbackCursorPosition(GLFWwindow* wnd, glm::dvec2 cursor, glm::dvec2 deltaCursor);

        /**
         *  @brief Mouse scroll callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] scroll The GLFW scroll offset.
         */
        virtual void CallbackScroll(GLFWwindow* wnd, glm::dvec2 scroll);

        /**
         *  @brief Framebuffer resize callback.
         *  @param [in] wnd The GLFW window.
         *  @param [in] size Framebuffer size in pixels.
         */
        virtual void CallbackFramebufferResize(GLFWwindow* wnd, glm::ivec2 size);

        /**
         *  @brief Drop callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] count Number of dropped files.
         *  @param [in] paths UTF-8 encoded file and/or directory path names.
         */
        virtual void CallbackDrop(GLFWwindow* wnd, int count, const char** paths);

        /**
         *  @brief Joystick status callback function.
         *  @param [in] joy Joystick value: [GLFW_JOYSTICK_1; GLFW_JOYSTICK_16].
         *  @param [in] event Event value, either GLFW_CONNECTED or GLFW_DISCONNECTED.
         *  @param [in] name The name of the joystick or "(null)" if not connected.
         */
        virtual void CallbackJoystickStatus(int joy, int event, const char* name);

        /**
         *  @brief Joystick data callback function.
         *  @param [in] joy Joystick value: [GLFW_JOYSTICK_1; GLFW_JOYSTICK_16].
         *  @param [in] axes Axes data in range [-1.0, +1.0].
         *  @param [in] numAxes Number of axes.
         *  @param [in] buttons Button data, either GLFW_PRESS or GLFW_RELEASE.
         *  @param [in] numButtons Number of buttons.
         *  @details This callback only occurs, if @ref PollEvents() is called.
         */
        virtual void CallbackJoystickData(int joy, const float* axes, int numAxes, const uint8_t* buttons, int numButtons);

    private:
        static GLCallback* root;          ///< The root callback handler.
        static glm::dvec2 lastCursorPos;  ///< Last cursor position. Used to calculate delta position if cursor is captured.
        static glm::dvec2 deltaCursorPos; ///< The delta cursor position.
        static bool evtCapture;           ///< True if @ref CaptureCursor() was called.

        std::vector<GLCallback*> childs;  ///< List of child callback handlers.

        inline void __callback_error(int error, const char* description){
            this->CallbackError(error, description);
            for(auto&& p : this->childs) p->__callback_error(error, description);
        }
        inline void __callback_close(GLFWwindow* wnd){
            this->CallbackClose(wnd);
            for(auto&& p : this->childs) p->__callback_close(wnd);
        }
        inline void __callback_key(GLFWwindow* wnd, int key, int scancode, int action, int mods){
            this->CallbackKey(wnd, key, scancode, action, mods);
            for(auto&& p : this->childs) p->__callback_key(wnd, key, scancode, action, mods);
        }
        inline void __callback_char(GLFWwindow* wnd, uint32_t codepoint){
            this->CallbackChar(wnd, codepoint);
            for(auto&& p : this->childs) p->__callback_char(wnd, codepoint);
        }
        inline void __callback_mouse_button(GLFWwindow* wnd, int button, int action, int mods){
            this->CallbackMouseButton(wnd, button, action, mods);
            for(auto&& p : this->childs) p->__callback_mouse_button(wnd, button, action, mods);
        }
        inline void __callback_cursor_position(GLFWwindow* wnd, glm::dvec2 cursor, glm::dvec2 deltaCursor){
            this->CallbackCursorPosition(wnd, cursor, deltaCursor);
            for(auto&& p : this->childs) p->__callback_cursor_position(wnd, cursor, deltaCursor);
        }
        inline void __callback_scroll(GLFWwindow* wnd, glm::dvec2 scroll){
            this->CallbackScroll(wnd, scroll);
            for(auto&& p : this->childs) p->__callback_scroll(wnd, scroll);
        }
        inline void __callback_framebuffer_resize(GLFWwindow* wnd, glm::ivec2 size){
            this->CallbackFramebufferResize(wnd, size);
            for(auto&& p : this->childs) p->__callback_framebuffer_resize(wnd, size);
        }
        inline void __callback_drop(GLFWwindow* wnd, int count, const char** paths){
            this->CallbackDrop(wnd, count, paths);
            for(auto&& p : this->childs) p->__callback_drop(wnd, count, paths);
        }
        inline void __callback_joystick_status(int joy, int event, const char* name){
            this->CallbackJoystickStatus(joy, event, name);
            for(auto&& p : this->childs) p->__callback_joystick_status(joy, event, name);
        }
        inline void __callback_joystick_data(int joy, const float* axes, int numAxes, const uint8_t* buttons, int numButtons){
            this->CallbackJoystickData(joy, axes, numAxes, buttons, numButtons);
            for(auto&& p : this->childs) p->__callback_joystick_data(joy, axes, numAxes, buttons, numButtons);
        }
}; /* class: GLCallback */

