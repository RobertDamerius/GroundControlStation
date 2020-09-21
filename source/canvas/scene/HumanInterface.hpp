#pragma once


#include <GLCallback.hpp>
#include <CameraLock.hpp>


#define HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MIN     (2.0)
#define HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MAX     (500.0)


class HumanInterface: public GLCallback {
    public:
        CameraLock cameraLock;

        /**
         *  @brief Create a human interface.
         */
        HumanInterface();

        /**
         *  @brief Delete the human interface.
         */
        ~HumanInterface();

        /**
         *  @brief Initialize the human interface.
         *  @return True if success, false otherwise.
         */
        bool Initialize(void);

        /**
         *  @brief Terminate the human interface.
         */
        void Terminate(void);

        /**
         *  @brief Update movement.
         *  @param [in] dt Elapsed time in seconds.
         */
        void Update(double dt);

        /**
         *  @brief Get the current ortographic zoom.
         *  @return Current orthographic zoom.
         */
        inline double GetZoomOrtho(void)const{ return this->zoomOrtho; }

    private:
        double zoomOrtho;           ///< Orthographic zoom.
        glm::dvec2 prevCursor;      ///< Previous cursor for camera move.
        bool buttonMouseLeft;       ///< True if left mouse button is pressed, false otherwise.
        bool buttonMouseRight;      ///< True if right mouse button is pressed, false otherwise.
        bool moveKeyState[6];       ///< Move key states (W, A, S, D, LShift, Space).

        /**
         *  @brief Orthographic zoom operation.
         *  @param [in] wnd The GLFW window.
         *  @param [in] zoom Zoom value.
         */
        void ZoomOrtho(GLFWwindow* wnd, double zoom);

        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Callback functions
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /**
         *  @brief Mouse button callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] button The GLFW mouse button that was pressed or released.
         *  @param [in] action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
         *  @param [in] mods Bit field describing which modifier keys were held down.
         */
        void CallbackMouseButton(GLFWwindow* wnd, int button, int action, int mods);

        /**
         *  @brief Mouse cursor callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] cursor Position of the mouse cursor.
         *  @param [in] deltaCursor Delta cursor position if cursor is captured, glm::dvec2(0.0) otherwise.
         */
        void CallbackCursorPosition(GLFWwindow* wnd, glm::dvec2 cursor, glm::dvec2 deltaCursor);

        /**
         *  @brief Mouse scroll callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] scroll The GLFW scroll offset.
         */
        void CallbackScroll(GLFWwindow* wnd, glm::dvec2 scroll);

        /**
         *  @brief The key input callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] key The keyboard key that was pressed or released.
         *  @param [in] scancode The system-specific scancode of the key.
         *  @param [in] action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
         *  @param [in] mods Bit field describing which modifier keys were held down.
         */
        void CallbackKey(GLFWwindow* wnd, int key, int scancode, int action, int mods);
};

