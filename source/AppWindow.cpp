#include <AppWindow.hpp>
#include <RD.hpp>
#include <Configuration.hpp>


AppWindow appWindow;


AppWindow::AppWindow(){}

AppWindow::~AppWindow(){}

glm::ivec2 AppWindow::MinSize(glm::ivec2 size){
    int minSize = 1;
    return glm::ivec2((size.x < minSize) ? minSize : size.x, (size.y < minSize) ? minSize : size.y);
}

void AppWindow::AddGLTask(std::function<void(void)> f){
    mtxGLTasks.lock();
    glTasks.push_back(f);
    mtxGLTasks.unlock();
}

bool AppWindow::CallbackInitialize(GLFWwindow* wnd){
    // Make sure that the window is not initialized
    CallbackTerminate(wnd);
    RegisterCallbacksForChild(canvas);
    RegisterCallbacksForChild(joystickInterface);

    // Load configuration
    Configuration::ReadAll();

    // Initialize joystick interface and canvas
    if(!joystickInterface.Initialize(wnd)){
        LogError("Could not initialize joystick interface!\n");
        return false;
    }
    if(!canvas.Initialize(wnd)){
        LogError("Could not initialize canvas!\n");
        return false;
    }
    if(!Configuration::origin.undefined){
        canvas.scene.origin.Update(Configuration::origin.latitude, Configuration::origin.longitude, Configuration::origin.altitude);
    }

    // Initialize text to speech manager
    textToSpeech.Initialize();

    // Success
    return true;
}

void AppWindow::CallbackTerminate(GLFWwindow* wnd){
    UnregisterCallbacksOfChild(canvas);
    UnregisterCallbacksOfChild(joystickInterface);
    textToSpeech.Terminate();
    canvas.Terminate();
    joystickInterface.Terminate();

    // Delete GL tasks
    mtxGLTasks.lock();
    glTasks.clear();
    glTasks.shrink_to_fit();
    mtxGLTasks.unlock();
    (void)wnd;
}

void AppWindow::CallbackRender(GLFWwindow* wnd, double dt, glm::dvec2 deltaCursor){
    // Call GL tasks
    mtxGLTasks.lock();
    for(auto&& task : glTasks){
        task();
    }
    glTasks.clear();
    glTasks.shrink_to_fit();
    mtxGLTasks.unlock();

    // Render canvas
    canvas.Render(wnd, dt, deltaCursor);
}

void AppWindow::CallbackClose(GLFWwindow* wnd){
    textToSpeech.Terminate();
    (void) wnd;
}

