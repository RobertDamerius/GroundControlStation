#include <GLCallback.hpp>
#include <Core/Core.hpp>


GLCallback* GLCallback::root = nullptr;
glm::dvec2 GLCallback::lastCursorPos;
glm::dvec2 GLCallback::deltaCursorPos;
bool GLCallback::evtCapture = false;


GLCallback::GLCallback(){}

GLCallback::~GLCallback(){}

void GLCallback::RegisterCallbacksForChild(GLCallback& child){
    bool alreadyInList = false;
    for(size_t n = 0; n < childs.size(); n++){
        if((alreadyInList |= (&child == childs[n])))
            break;
    }
    if(!alreadyInList){
        childs.push_back(&child);
    }
}

void GLCallback::UnregisterCallbacksOfChild(GLCallback& child){
    for(size_t n = 0; n < childs.size(); n++){
        if(&child == childs[n]){
            childs.erase(childs.begin() + n);
            break;
        }
    }
    childs.shrink_to_fit();
}

void GLCallback::SetCallbacks(GLFWwindow* glfwWindow){
    // Set root
    GLCallback::root = this;

    // Register error callback
    glfwSetErrorCallback([](int error, const char* description){
        if(GLCallback::root){
            GLCallback::root->__callback_error(error, description);
        }
    });
    if(!glfwWindow) return;

    // Close
    glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* wnd){
        if(GLCallback::root){
            GLCallback::root->__callback_close(wnd);
        }
    });

    // Key
    glfwSetKeyCallback(glfwWindow, [](GLFWwindow* wnd, int key, int scancode, int action, int mods){
        if(GLCallback::root){
            GLCallback::root->__callback_key(wnd, key, scancode, action, mods);
        }
    });

    // Char
    glfwSetCharCallback(glfwWindow, [](GLFWwindow* wnd, unsigned codepoint){
        if(GLCallback::root){
            GLCallback::root->__callback_char(wnd, (uint32_t)codepoint);
        }
    });

    // Mouse button
    glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* wnd, int button, int action, int mods){
        if(GLCallback::root){
            GLCallback::root->__callback_mouse_button(wnd, button, action, mods);
        }
    });

    // Cursor position
    glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* wnd, double xPos, double yPos){
        // Update delta if captured
        glm::dvec2 newPos(xPos, yPos);
        GLCallback::deltaCursorPos = (GLFW_CURSOR_NORMAL != glfwGetInputMode(wnd, GLFW_CURSOR)) ? (newPos - GLCallback::lastCursorPos) : glm::dvec2(0.0);
        GLCallback::lastCursorPos = newPos;

        // Prevent cursor glitches when capturing
        if(GLCallback::evtCapture){
            GLCallback::evtCapture = false;
            GLCallback::deltaCursorPos = glm::dvec2(0.0);
            double x = 0.0, y = 0.0;
            glfwGetCursorPos(wnd, &x, &y);
            lastCursorPos = glm::dvec2(x, y);
        }

        // Run actual callback
        if(GLCallback::root){
            GLCallback::root->__callback_cursor_position(wnd, newPos, GLCallback::deltaCursorPos);
        }
    });

    // Scroll
    glfwSetScrollCallback(glfwWindow, [](GLFWwindow* wnd, double xoffset, double yoffset){
        if(GLCallback::root){
            GLCallback::root->__callback_scroll(wnd, glm::dvec2(xoffset, yoffset));
        }
    });

    // Framebuffer size
    glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow* wnd, int width, int height){
        if(GLCallback::root){
            GLCallback::root->__callback_framebuffer_resize(wnd, glm::ivec2(width, height));
        }
    });

    // Drop
    glfwSetDropCallback(glfwWindow, [](GLFWwindow* wnd, int count, const char** paths){
        if(GLCallback::root){
            GLCallback::root->__callback_drop(wnd, count, paths);
        }
    });

    // Joystick
    glfwSetJoystickCallback([](int joy, int event){
        if(GLCallback::root){
            GLCallback::root->__callback_joystick_status(joy, event, glfwGetJoystickName(joy));
        }
    });
}

void GLCallback::PollJoysticks(void){
    for(int joy = GLFW_JOYSTICK_1; joy <= GLFW_JOYSTICK_LAST; joy++){
        if(GLFW_FALSE == glfwJoystickPresent(joy))
            continue;
        int numAxes;
        const float* axes = glfwGetJoystickAxes(joy, &numAxes);
        int numButtons;
        const uint8_t* buttons = (const uint8_t*)glfwGetJoystickButtons(joy, &numButtons);
        if(GLCallback::root){
            GLCallback::root->__callback_joystick_data(joy, axes, numAxes, buttons, numButtons);
        }
    }
}

void GLCallback::CallbackError(int error, const char* description){ LogError("GLFW error 0x%08X (%d): %s\n", error, error, description); }

void GLCallback::CallbackClose(GLFWwindow* wnd){ (void)wnd; }

void GLCallback::CallbackKey(GLFWwindow* wnd, int key, int scancode, int action, int mods){ (void)wnd; (void)key; (void)scancode; (void)action; (void)mods; }

void GLCallback::CallbackChar(GLFWwindow* wnd, uint32_t codepoint){ (void)wnd; (void)codepoint; }

void GLCallback::CallbackMouseButton(GLFWwindow* wnd, int button, int action, int mods){ (void)wnd; (void)button; (void)action; (void)mods; }

void GLCallback::CallbackCursorPosition(GLFWwindow* wnd, glm::dvec2 cursor, glm::dvec2 deltaCursor){ (void)wnd; (void)cursor; (void)deltaCursor; }

void GLCallback::CallbackScroll(GLFWwindow* wnd, glm::dvec2 scroll){ (void)wnd; (void)scroll; }

void GLCallback::CallbackFramebufferResize(GLFWwindow* wnd, glm::ivec2 size){ (void)wnd; (void)size; }

void GLCallback::CallbackDrop(GLFWwindow* wnd, int count, const char** paths){ (void)wnd; (void)count; (void)paths; }

void GLCallback::CallbackJoystickStatus(int joy, int event, const char* name){ (void)joy; (void)event; (void)name; }

void GLCallback::CallbackJoystickData(int joy, const float* axes, int numAxes, const uint8_t* buttons, int numButtons){ (void)joy; (void)axes; (void)numAxes; (void)buttons; (void)numButtons; }

