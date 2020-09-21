#include <Scene.hpp>
#include <Common.hpp>
#include <AppWindow.hpp>
#include <SceneText.hpp>
#include <Configuration.hpp>


Scene::Scene(){}

Scene::~Scene(){}

bool Scene::Initialize(void){
    // Make sure that the scene is not initialized
        Terminate();

    // Register callbacks for childs
        RegisterCallbacksForChild(humanInterface);

    // Initialize scene modules
        viewCamera.GenerateUniformBufferObject(UBO_VIEW_CAMERA);
        if(!humanInterface.Initialize()){
            goto error;
        }
        if(!vehicleManager.Initialize()){
            goto error;
        }

    // Camera settings
        viewCamera.fov = glm::radians(70.0);
        viewCamera.clipNear = CAMERA_CLIP_NEAR_DEFAULT;
        viewCamera.clipFar = CAMERA_CLIP_FAR_DEFAULT;
        viewCamera.position = glm::dvec3(0.0, 10.0, 0.0);
        viewCamera.up = glm::dvec3(0.0, 1.0, 0.0);
        viewCamera.view = glm::dvec3(1.0, 0.0, 0.0);
        SetCameraMode(Configuration::style.cameraMode ? CAMERA_MODE_PERSPECTIVE : CAMERA_MODE_ORTHOGRAPHIC);
        renderingOffset = glm::dvec3(0.0);

    // Load and generate content
        if(!SceneText::GenerateGL()){
            LogError("Could not generate scene text content!\n");
            goto error;
        }
        if(!skybox.ReadTextures()){
            LogError("Could not read skybox textures!\n");
            goto error;
        }
        if(!skybox.GenerateGL()){
            LogError("Could not generate skybox texture!\n");
            goto error;
        }
        groundPlane.Reset();
        groundPlane.color = appWindow.canvas.renderer.SetGroundPlaneColor(groundPlane.color);

    // Return success
        return true;

    // Error handling
        error:
        Terminate();
        return false;
}

void Scene::Terminate(void){
    SceneText::DeleteGL();
    skybox.DeleteGL();
    vehicleManager.Terminate();
    humanInterface.Terminate();
    viewCamera.DeleteUniformBufferObject();
    UnregisterCallbacksOfChild(humanInterface);
}

void Scene::Update(double dt){
    // Update vehicle manager
    vehicleManager.Update(dt);

    // Update user movement
    humanInterface.Update(dt);

    // Update the view camera
    glm::dvec3 truePosition = viewCamera.position;
    glm::ivec2 numWraps((int)(truePosition.x / CAMERA_HORIZONTAL_WRAP),(int)(truePosition.z / CAMERA_HORIZONTAL_WRAP));
    renderingOffset = -glm::dvec3((double)numWraps.x * CAMERA_HORIZONTAL_WRAP, 0.0, (double)numWraps.y * CAMERA_HORIZONTAL_WRAP);
    viewCamera.position += renderingOffset;
    viewCamera.UpdateUniformBufferObject();
    viewCamera.position = truePosition;
}

void Scene::SetCameraMode(CameraMode mode){
    if(mode == viewCamera.mode)
        return;
    viewCamera.mode = mode;
    switch(mode){
        case CAMERA_MODE_ORTHOGRAPHIC: {
            double vlen = sqrt(viewCamera.view.x*viewCamera.view.x + viewCamera.view.z*viewCamera.view.z);
            if(vlen > 1e-3){
                vlen = 1.0 / vlen;
                viewCamera.up.x = vlen * viewCamera.view.x;
                viewCamera.up.y = 0.0;
                viewCamera.up.z = vlen * viewCamera.view.z;
            }
            else{
                viewCamera.up = glm::dvec3(0.0, 0.0, -1.0);
            }
            viewCamera.view = glm::dvec3(0.0, -1.0, 0.0);
            viewCamera.position.y = 0.5 * (viewCamera.clipNear + viewCamera.clipFar);
            break;
        }
        case CAMERA_MODE_PERSPECTIVE: {
            viewCamera.view = glm::dvec3(viewCamera.up.x, 0.0, viewCamera.up.z);
            viewCamera.up = glm::dvec3(0.0, 1.0, 0.0);
            viewCamera.position.y = 10.0;
            break;
        }
    }
    viewCamera.UpdateProjectionMatrix();
}

void Scene::UpdateViewCamera(glm::ivec2 size){
    size = AppWindow::MinSize(size);
    double aspectRatio = (double)size.x / (double)size.y;
    viewCamera.aspect = (GLfloat)aspectRatio;
    viewCamera.left = -0.5 * humanInterface.GetZoomOrtho();
    viewCamera.right = 0.5 * humanInterface.GetZoomOrtho();
    viewCamera.bottom = -0.5 * humanInterface.GetZoomOrtho();
    viewCamera.top = 0.5 * humanInterface.GetZoomOrtho();
    if(aspectRatio >= 1.0){
        viewCamera.left *= aspectRatio;
        viewCamera.right *= aspectRatio;
    }
    else{
        viewCamera.bottom /= aspectRatio;
        viewCamera.top /= aspectRatio;
    }
    viewCamera.UpdateProjectionMatrix();
}

void Scene::CallbackFramebufferResize(GLFWwindow* wnd, glm::ivec2 size){
    UpdateViewCamera(size);
    (void)wnd;
}

