#include <GLWindow.hpp>
#include <Core/Core.hpp>
#include <Engine/Engine.hpp>


GLFWwindow* GLWindow::glfwWindow = nullptr;
GLWindow* GLWindow::glWindow = nullptr;
GLuint GLWindow::quadVAO = 0;
GLuint GLWindow::quadVBO = 0;
GLuint GLWindow::cubeVAO = 0;
GLuint GLWindow::cubeVBO = 0;
GLuint GLWindow::cubeEBO = 0;
uint32_t GLWindow::fps = 0;


GLWindow::GLWindow(){}

GLWindow::~GLWindow(){}

bool GLWindow::Initialize(GLWindow* glWnd, uint32_t width, uint32_t height, std::string title, bool fullScreen){
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Make sure that the window is terminated
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Terminate();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Initialize GLFW and set some window hints
    // For MAC OS X also call: GLWindow::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        glWnd->SetCallbacks();
        if(GL_TRUE != glfwInit()){
            Terminate();
            LogError("Could not initialize GLFW!\n");
            return false;
        }
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
        // glfwWindowHint(GLFW_STENCIL_BITS, 0);
        // glfwWindowHint(GLFW_DEPTH_BITS, 0);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 0);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Create GLFW window, make context and set callbacks
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        GLFWmonitor* monitor = fullScreen ? glfwGetPrimaryMonitor() : nullptr;
        glfwWindow = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
        if(!glfwWindow){
            LogError("Could not create GLFW window!\n");
            Terminate();
            return false;
        }
        glfwMakeContextCurrent(glfwWindow);
        glWnd->SetCallbacks(glfwWindow);
        glWindow = glWnd;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Initiate GLEW
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        glewExperimental = GL_TRUE;
        if(GLEW_OK != glewInit()){
            LogError("Could not initialize GLEW!\n");
            Terminate();
            return false;
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Create a simple screen quad
    // Setup a vertex array object/vertex buffer object for the quad
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        const GLfloat quadVertices[] = {
            // Position   // TexCoord
            -1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 0.0f
        };
        DEBUG_GLCHECK( glGenVertexArrays(1, &quadVAO); );
        DEBUG_GLCHECK( glGenBuffers(1, &quadVBO); );
        DEBUG_GLCHECK( glBindVertexArray(quadVAO); );
            DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, quadVBO); );
            DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices[0], GL_STATIC_DRAW); );
            DEBUG_GLCHECK( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0); );
            DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
            DEBUG_GLCHECK( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); );
            DEBUG_GLCHECK( glEnableVertexAttribArray(1); );
        DEBUG_GLCHECK( glBindVertexArray(0); );

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Create a simple screen cube
    // Setup a vertex array object/vertex buffer object for the cube
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        static GLfloat skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f
        };
        static GLuint skyboxIndices[] = {0,1,2,3,4,5,6,7,0,1, 1,7,3,5, 5,4,4,6,2,0};
        DEBUG_GLCHECK( glGenVertexArrays(1, &cubeVAO); );
        DEBUG_GLCHECK( glGenBuffers(1, &cubeVBO); );
        DEBUG_GLCHECK( glGenBuffers(1, &cubeEBO); );
        DEBUG_GLCHECK( glBindVertexArray(cubeVAO); );
            DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); );
            DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW); );
            DEBUG_GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO); );
            DEBUG_GLCHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW); );
            DEBUG_GLCHECK( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); );
            DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
        DEBUG_GLCHECK( glBindVertexArray(0); );

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Set initial OpenGL states
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        DEBUG_GLCHECK( glDisable(GL_DEPTH_TEST); );
        DEBUG_GLCHECK( glDisable(GL_STENCIL_TEST); );
        DEBUG_GLCHECK( glDepthFunc(GL_LEQUAL); );
        DEBUG_GLCHECK( glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA); );
        DEBUG_GLCHECK( glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD); );
        DEBUG_GLCHECK( glEnable(GL_CULL_FACE); );
        DEBUG_GLCHECK( glCullFace(GL_BACK); );
        DEBUG_GLCHECK( glStencilMask(GL_FALSE); );
        DEBUG_GLCHECK( glClearColor(0.0f, 0.0f, 0.0f, 0.0f); );

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // User-specific initialization
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if(glWindow && !glWindow->CallbackInitialize(glfwWindow)){
            Terminate();
            return false;
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Clear both buffers with clear-color and set swap interval to avoid tearing
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        DEBUG_GLCHECK( glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); );
        glfwSwapBuffers(glfwWindow);
        DEBUG_GLCHECK( glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); );
        glfwSwapInterval(1);
        return true;
}

void GLWindow::Terminate(void){
    if(quadVBO){
        glDeleteBuffers(1, &quadVBO);
        quadVBO = 0;
    }
    if(quadVAO){
        glDeleteVertexArrays(1, &quadVAO);
        quadVAO = 0;
    }
    if(cubeVBO){
        glDeleteBuffers(1, &cubeVBO);
        cubeVBO = 0;
    }
    if(cubeVAO){
        glDeleteVertexArrays(1, &cubeVAO);
        cubeVAO = 0;
    }
    if(cubeEBO){
        glDeleteBuffers(1, &cubeEBO);
        cubeEBO = 0;
    }
    if(glfwWindow){
        if(glWindow){
            glWindow->CallbackTerminate(glfwWindow);
        }
        glfwDestroyWindow(glfwWindow);
        glfwWindow = nullptr;
    }
    glfwTerminate();
    fps = 0;
    lastCursorPos = glm::dvec2(0.0);
    deltaCursorPos = glm::dvec2(0.0);
    evtCapture = false;
    glWindow = nullptr;
}

void GLWindow::MainLoop(void){
    // Do nothing if window has not been created
    if(!glfwWindow)
        return;

    // Run initial resize callback and initial joystick connect/disconnect callbacks
    if(glWindow){
        int w = 0, h = 0;
        glfwGetFramebufferSize(glfwWindow, &w, &h);
        glWindow->__callback_framebuffer_resize(glfwWindow, glm::ivec2(w, h));
        for(int joy = GLFW_JOYSTICK_1; joy <= GLFW_JOYSTICK_LAST; joy++){
            int event = (GLFW_TRUE == glfwJoystickPresent(joy)) ? GLFW_CONNECTED : GLFW_DISCONNECTED;
            glWindow->__callback_joystick_status(joy, event, glfwGetJoystickName(joy));
        }
    }

    // The actual main loop
    uint32_t fpsFrames = 0;
    double fpsTime = 0;
    double previousTime = 0.0;
    glfwSetTime(0.0);
    while(!glfwWindowShouldClose(glfwWindow)){
        // Get elapsed time to last cycle and limit FPS
        double time = glfwGetTime();
        double dt = time - previousTime;
        if(dt < 0.01){
            std::this_thread::sleep_for(std::chrono::microseconds((int)((0.01 - dt)*1e6)));
            time = glfwGetTime();
            dt = time - previousTime;
        }
        previousTime = time;

        // Calculate and show FPS
        fpsFrames++;
        if((fpsTime += dt) >= 1.0){
            fpsTime = fmod(fpsTime, 1.0);
            fps = fpsFrames;
            fpsFrames = 0;
        }

        // Rendering + swap buffers
        if(glWindow) glWindow->CallbackRender(glfwWindow, dt, deltaCursorPos);
        glfwSwapBuffers(glfwWindow);

        // Poll all events
        glfwPollEvents();
        PollJoysticks();
    }
}

void GLWindow::DrawScreenQuad(void){
    DEBUG_GLCHECK( glBindVertexArray(quadVAO); );
    DEBUG_GLCHECK( glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); );
}

void GLWindow::DrawScreenCube(void){
    DEBUG_GLCHECK( glBindVertexArray(cubeVAO); );
    DEBUG_GLCHECK( glDrawElements(GL_TRIANGLE_STRIP, 20, GL_UNSIGNED_INT, (GLvoid*)0); );
}

glm::ivec2 GLWindow::GetSize(void){
    int w = 0, h = 0;
    glfwGetFramebufferSize(glfwWindow, &w, &h);
    return glm::ivec2(w, h);
}

uint32_t GLWindow::GetFPS(void){
    return fps;
}

double GLWindow::GetTime(void){
    return glfwGetTime();
}

void GLWindow::SetWindowTitle(const char* title){
    glfwSetWindowTitle(glfwWindow, title);
}

void GLWindow::Close(void){
    glfwSetWindowShouldClose(glfwWindow, GL_TRUE);
}

bool GLWindow::LoadCursorImage(std::vector<uint8_t>& imageDataRGBA, int width, int height){
    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = &imageDataRGBA[0];
    GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
    if(!cursor){
        LogError("Could not create cursor!\n");
        return false;
    }
    if(glfwWindow){
        glfwSetCursor(glfwWindow, cursor);
        return true;
    }
    LogWarning("Tried to set a cursor but window was not created!\n");
    return false;
}

void GLWindow::CaptureCursor(void){
    if(!GLWindow::glfwWindow)
        return;
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    double x = 0.0, y = 0.0;
    glfwGetCursorPos(glfwWindow, &x, &y);
    lastCursorPos = glm::dvec2(x, y);
    deltaCursorPos = glm::dvec2(0.0);
    evtCapture = true;
}

void GLWindow::ReleaseCursor(void){
    if(!GLWindow::glfwWindow)
        return;
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    lastCursorPos = glm::dvec2(0.0);
    deltaCursorPos = glm::dvec2(0.0);
    evtCapture = false;
}

bool GLWindow::CursorIsCaptured(void){
    return (GLFW_CURSOR_NORMAL != glfwGetInputMode(glfwWindow, GLFW_CURSOR));
}

bool GLWindow::CallbackInitialize(GLFWwindow* wnd){
    (void)wnd;
    return true;
}

void GLWindow::CallbackTerminate(GLFWwindow* wnd){
    (void)wnd;
}

void GLWindow::CallbackRender(GLFWwindow* wnd, double dt, glm::dvec2 deltaCursor){
    (void)wnd;
    (void)dt;
    (void)deltaCursor;
}

