#pragma once


#include <GLWindow.hpp>
#include <nanogui.h>
#include <WidgetToolbar.hpp>
#include <WidgetView.hpp>
#include <WidgetVehicleList.hpp>
#include <WidgetTime.hpp>
#include <WidgetLog.hpp>
#include <WidgetInfo.hpp>


#define GUI_COLOR_CLOSE_BUTTON      0.5f,0.1f,0.1f,0.5f
#define GUI_COLOR_CLEAR_BUTTON      0.5f,0.5f,0.5f,0.5f
#define GUI_FONT_SIZE               18.0f
#define GUI_TEXT_COLOR_GUIDANCE_R   (30)
#define GUI_TEXT_COLOR_GUIDANCE_G   (190)
#define GUI_TEXT_COLOR_GUIDANCE_B   (255)


/* Forward declaration */
class Renderer;
class Canvas;


class GUI: public GLCallback {
    public:
        /**
         *  @brief Create the graphical user interface.
         */
        GUI();

        /**
         *  @brief Delete the graphical user interface.
         */
        ~GUI();

        /**
         *  @brief Initialize the GUI.
         *  @param wnd The GLFW window.
         *  @param renderer The renderer that contains the framebuffers.
         *  @return True if success, false otherwise.
         */
        bool Initialize(GLFWwindow* wnd, Renderer& renderer);

        /**
         *  @brief Terminate the GUI.
         */
        void Terminate(void);

        /**
         *  @brief Render callback function.
         */
        void Render(void);

        /**
         *  @brief Check if mouse cursor is over GUI.
         *  @param [in] cursor Mouse cursor position.
         *  @return True if mouse cursor is over GUI, false otherwise.
         */
        bool CursorOverGUI(glm::dvec2 cursor);

        /**
         *  @brief Check if any GUI element is focused.
         *  @return True if at least one GUI element is focused.
         */
        bool IsFocused(void);

        /**
         *  @brief Set the state of the view widget.
         *  @param [in] visible True if widget should be visible, false otherwise.
         */
        void SetWidgetViewState(bool visible);

        /**
         *  @brief Set the state of the vehicle list widget.
         *  @param [in] visible True if widget should be visible, false otherwise.
         */
        void SetWidgetVehicleListState(bool visible);

        /**
         *  @brief Set the state of the time widget.
         *  @param [in] visible True if widget should be visible, false otherwise.
         */
        void SetWidgetTimeState(bool visible);

        /**
         *  @brief Set the state of the log widget.
         *  @param [in] visible True if widget should be visible, false otherwise.
         */
        void SetWidgetLogState(bool visible);

        /**
         *  @brief Set the state of the info widget.
         *  @param [in] visible True if widget should be visible, false otherwise.
         */
        void SetWidgetInfoState(bool visible);

        /**
         *  @brief Add a new log entry to the log widget.
         *  @param [in] log Log entry to be added.
         */
        void AddLogEntry(LogEntry log);

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

        /**
         *  @brief The key input callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] key The keyboard key that was pressed or released.
         *  @param [in] scancode The system-specific scancode of the key.
         *  @param [in] action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
         *  @param [in] mods Bit field describing which modifier keys were held down.
         */
        void CallbackKey(GLFWwindow* wnd, int key, int scancode, int action, int mods);

        /**
         *  @brief The char input callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] codepoint The Unicode code point of the character.
         */
        void CallbackChar(GLFWwindow* wnd, uint32_t codepoint);

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
         *  @brief Drop callback function.
         *  @param [in] wnd The GLFW window.
         *  @param [in] count Number of files.
         *  @param [in] filenames List of filenames.
         */
        void CallbackDrop(GLFWwindow* wnd, int count, const char **filenames);

    private:
        nanogui::Screen* screen;
        WidgetToolbar* widgetToolbar;
        WidgetView* widgetView;
        WidgetVehicleList* widgetVehicleList;
        WidgetTime* widgetTime;
        WidgetLog* widgetLog;
        WidgetInfo* widgetInfo;
        std::vector<LogEntry> startupLogs;
        std::mutex mtxStartupLogs;
};

