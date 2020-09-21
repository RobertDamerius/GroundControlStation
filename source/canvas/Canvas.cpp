#include <Canvas.hpp>


Canvas::Canvas(){}

Canvas::~Canvas(){}

bool Canvas::Initialize(GLFWwindow* wnd){
    RegisterCallbacksForChild(renderer);
    RegisterCallbacksForChild(scene);
    RegisterCallbacksForChild(gui);
    if(!renderer.Initialize()){
        LogError("Could not initialize the renderer!\n");
        Terminate();
        return false;
    }
    if(!scene.Initialize()){
        LogError("Could not initialize the scene!\n");
        Terminate();
        return false;
    }
    if(!gui.Initialize(wnd, renderer)){
        LogError("Could not initialize the GUI!\n");
        Terminate();
        return false;
    }
    return true;
}

void Canvas::Terminate(void){
    UnregisterCallbacksOfChild(gui);
    UnregisterCallbacksOfChild(scene);
    UnregisterCallbacksOfChild(renderer);
    gui.Terminate();
    scene.Terminate();
    renderer.Terminate();
}

void Canvas::Render(GLFWwindow* wnd, double dt, glm::dvec2 deltaCursor){
    scene.Update(dt);
    renderer.Render(scene, gui);
    (void)wnd;
    (void)deltaCursor;
}

