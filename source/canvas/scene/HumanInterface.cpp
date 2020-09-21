#include <HumanInterface.hpp>
#include <AppWindow.hpp>
#include <Common.hpp>


// Lookup table for camera movement
#define HALF_SQRT_2             (0.7071067811865475244)
static const glm::dvec2 __movement_table[3][3] = {glm::vec2(-HALF_SQRT_2, -HALF_SQRT_2), glm::vec2(-1.0f, 0.0f), glm::vec2(-HALF_SQRT_2, HALF_SQRT_2), glm::vec2(0.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(HALF_SQRT_2, -HALF_SQRT_2), glm::vec2(1.0f, 0.0f), glm::vec2(HALF_SQRT_2, HALF_SQRT_2)};


HumanInterface::HumanInterface(){
    (void)Initialize();
}

HumanInterface::~HumanInterface(){}

bool HumanInterface::Initialize(void){
    Terminate();
    zoomOrtho = 10.0;
    prevCursor.x = prevCursor.y = 0.0;
    buttonMouseLeft = false;
    buttonMouseRight = false;
    for(int i = 0; i < 6; i++){
        moveKeyState[i] = false;
    }
    cameraLock.Reset();
    return true;
}

void HumanInterface::Terminate(void){}

void HumanInterface::Update(double dt){
    if(cameraLock.lock){
        Vehicle* v = appWindow.canvas.scene.vehicleManager.LockVehicle(cameraLock.id);
        if(v){
            glm::dvec3 vehiclePosition(v->renderingState.position.x, -v->renderingState.position.z, v->renderingState.position.y);
            Angle yaw(v->renderingState.yaw);
            appWindow.canvas.scene.vehicleManager.UnlockVehicle();
            if(cameraLock.lockRotation){
                Angle deltaYaw = yaw - cameraLock.yaw;
                cameraLock.yaw = yaw;
                appWindow.canvas.scene.viewCamera.Rotate(deltaYaw(), glm::dvec3(0.0, -1.0, 0.0));
            }
            if(CAMERA_MODE_PERSPECTIVE == appWindow.canvas.scene.viewCamera.mode){
                appWindow.canvas.scene.viewCamera.position = vehiclePosition - cameraLock.zoom*appWindow.canvas.scene.viewCamera.view;
            }
            else{
                appWindow.canvas.scene.viewCamera.position.x = vehiclePosition.x;
                appWindow.canvas.scene.viewCamera.position.z = vehiclePosition.z;
            }
        }
        else{
            cameraLock.Reset();
        }
    }
    else{
        if(CAMERA_MODE_PERSPECTIVE == appWindow.canvas.scene.viewCamera.mode){
            int xIndex = 1, yIndex = 1;
            if(moveKeyState[0]) xIndex += 1;
            if(moveKeyState[2]) xIndex -= 1;
            if(moveKeyState[3]) yIndex += 1;
            if(moveKeyState[1]) yIndex -= 1;
            glm::dvec3 dirMovement(__movement_table[xIndex][yIndex].x, __movement_table[xIndex][yIndex].y, 0.0);
            if(moveKeyState[4]) dirMovement.z += 1.0;
            if(moveKeyState[5]) dirMovement.z -= 1.0;
            dirMovement *= (50.0 * dt);
            appWindow.canvas.scene.viewCamera.MoveView(glm::vec3(dirMovement.y, -dirMovement.z, -dirMovement.x));
        }
    }
}

void HumanInterface::CallbackMouseButton(GLFWwindow* wnd, int button, int action, int mods){
    // Left mouse button
    if(GLFW_MOUSE_BUTTON_LEFT == button){
        glfwGetCursorPos(wnd, &prevCursor.x, &prevCursor.y);
        buttonMouseLeft = ((GLFW_PRESS == action) && !appWindow.canvas.gui.CursorOverGUI(prevCursor));
    }

    // Right mouse button
    if(GLFW_MOUSE_BUTTON_RIGHT == button){
        glfwGetCursorPos(wnd, &prevCursor.x, &prevCursor.y);
        buttonMouseRight = ((GLFW_PRESS == action) && !appWindow.canvas.gui.CursorOverGUI(prevCursor));
    }
    (void)mods;
}

void HumanInterface::CallbackCursorPosition(GLFWwindow* wnd, glm::dvec2 cursor, glm::dvec2 deltaCursor){
    // Calculate delta cursor
    deltaCursor = cursor - prevCursor;
    prevCursor = cursor;

    // Get framebuffer size
    int width, height;
    glfwGetFramebufferSize(wnd, &width, &height);
    width = width < 1 ? 1 : width;
    height = height < 1 ? 1 : height;

    if(cameraLock.lock){
        // Left and right mouse button
        if(buttonMouseLeft && buttonMouseRight){
            ZoomOrtho(wnd, -0.1*deltaCursor.y);
            if(CAMERA_MODE_PERSPECTIVE == appWindow.canvas.scene.viewCamera.mode){
                double zoom = -0.1*deltaCursor.y;
                zoom = (zoom < -1.0) ? -1.0 : ((zoom > 1.0) ? 1.0 : zoom);
                cameraLock.zoom *= (1.0 - 0.1 * zoom);
                double zoomMax = CAMERA_CLIP_FAR_DEFAULT * 0.5;
                cameraLock.zoom = (cameraLock.zoom < 1.0) ? 1.0 : ((cameraLock.zoom > zoomMax) ? zoomMax : cameraLock.zoom);
            }
        }
        else{
            if(buttonMouseLeft || buttonMouseRight){
                if(CAMERA_MODE_ORTHOGRAPHIC == appWindow.canvas.scene.viewCamera.mode){
                    glm::dvec2 b = cursor - glm::dvec2(width >> 1, height >> 1);
                    glm::dvec2 a = b - deltaCursor;
                    double d = atan2(b.y,b.x) - atan2(a.y,a.x);
                    appWindow.canvas.scene.viewCamera.Rotate(d, glm::dvec3(0.0, 1.0, 0.0));
                }
                else{
                    appWindow.canvas.scene.viewCamera.Rotate(-0.003*deltaCursor.x, glm::dvec3(0.0, 1.0, 0.0));
                    appWindow.canvas.scene.viewCamera.PitchView(-0.003*deltaCursor.y);
                }
            }
        }
    }
    else{
        // Left and right mouse button
        if(buttonMouseLeft && buttonMouseRight){
            // Zoom in orthographic mode
            ZoomOrtho(wnd, -0.1*deltaCursor.y);

            // In perspective mode: move along view axis
            if(CAMERA_MODE_PERSPECTIVE == appWindow.canvas.scene.viewCamera.mode){
                appWindow.canvas.scene.viewCamera.position -= appWindow.canvas.scene.viewCamera.view * (0.6 * deltaCursor.y);
            }
        }
        else{
            // Left mouse button
            if(buttonMouseLeft){
                // Move or rotate camera
                if(CAMERA_MODE_ORTHOGRAPHIC == appWindow.canvas.scene.viewCamera.mode){
                    double d = zoomOrtho / double(width < height ? width : height);
                    appWindow.canvas.scene.viewCamera.MoveView(glm::vec3(-d * deltaCursor.x, d * deltaCursor.y, 0.0));
                }
                else{
                    glm::dvec3 left = glm::cross(appWindow.canvas.scene.viewCamera.up, appWindow.canvas.scene.viewCamera.view);
                    left.y = 0.0;
                    left = glm::normalize(left);
                    glm::dvec3 forward = appWindow.canvas.scene.viewCamera.view;
                    if(abs(appWindow.canvas.scene.viewCamera.up.y) <= 1e-3){
                        forward = appWindow.canvas.scene.viewCamera.up;
                    }
                    forward.y = 0.0;
                    forward = glm::normalize(forward);
                    appWindow.canvas.scene.viewCamera.position += left * (deltaCursor.x * 0.086) + forward * (deltaCursor.y * 0.086);
                }
            }
            // Right mouse button
            if(buttonMouseRight){
                if(CAMERA_MODE_ORTHOGRAPHIC == appWindow.canvas.scene.viewCamera.mode){
                    glm::dvec2 b = cursor - glm::dvec2(width >> 1, height >> 1);
                    glm::dvec2 a = b - deltaCursor;
                    double d = atan2(b.y,b.x) - atan2(a.y,a.x);
                    appWindow.canvas.scene.viewCamera.Rotate(d, glm::dvec3(0.0, 1.0, 0.0));
                }
                else{
                    appWindow.canvas.scene.viewCamera.Rotate(-0.003*deltaCursor.x, glm::dvec3(0.0, 1.0, 0.0));
                    appWindow.canvas.scene.viewCamera.PitchView(-0.003*deltaCursor.y);
                }
            }
        }
    }
}

void HumanInterface::CallbackScroll(GLFWwindow* wnd, glm::dvec2 scroll){
    glm::dvec2 cursor;
    glfwGetCursorPos(wnd, &cursor.x, &cursor.y);
    if(cameraLock.lock){
        ZoomOrtho(wnd, scroll.y);
        if(CAMERA_MODE_PERSPECTIVE == appWindow.canvas.scene.viewCamera.mode){
            double zoom = 3.0*scroll.y;
            zoom = (zoom < -1.0) ? -1.0 : ((zoom > 1.0) ? 1.0 : zoom);
            cameraLock.zoom *= (1.0 - 0.1 * zoom);
            double zoomMax = CAMERA_CLIP_FAR_DEFAULT * 0.5;
            cameraLock.zoom = (cameraLock.zoom < 1.0) ? 1.0 : ((cameraLock.zoom > zoomMax) ? zoomMax : cameraLock.zoom);
        }
    }
    else{
        if(!appWindow.canvas.gui.CursorOverGUI(cursor)){
            // Zoom in orthographic mode
            ZoomOrtho(wnd, scroll.y);

            // In perspective mode: move along view axis
            if(CAMERA_MODE_PERSPECTIVE == appWindow.canvas.scene.viewCamera.mode){
                appWindow.canvas.scene.viewCamera.position += appWindow.canvas.scene.viewCamera.view * (3.0 * scroll.y);
            }
        }
    }
}

void HumanInterface::CallbackKey(GLFWwindow* wnd, int key, int scancode, int action, int mods){
    if(appWindow.canvas.gui.IsFocused()){
        moveKeyState[0] = false;
        moveKeyState[1] = false;
        moveKeyState[2] = false;
        moveKeyState[3] = false;
        moveKeyState[4] = false;
        moveKeyState[5] = false;
        return;
    }
    if(GLFW_KEY_W == key){
        moveKeyState[0] = (GLFW_RELEASE != action);
    }
    if(GLFW_KEY_A == key){
        moveKeyState[1] = (GLFW_RELEASE != action);
    }
    if(GLFW_KEY_S == key){
        moveKeyState[2] = (GLFW_RELEASE != action);
    }
    if(GLFW_KEY_D == key){
        moveKeyState[3] = (GLFW_RELEASE != action);
    }
    if(GLFW_KEY_LEFT_SHIFT == key){
        moveKeyState[4] = (GLFW_RELEASE != action);
    }
    if(GLFW_KEY_SPACE == key){
        moveKeyState[5] = (GLFW_RELEASE != action);
    }
    if(CAMERA_MODE_ORTHOGRAPHIC == appWindow.canvas.scene.viewCamera.mode){
        if((GLFW_KEY_SPACE == key) && (GLFW_PRESS == action)){
            AxisAlignedBoundingBox aabb = appWindow.canvas.scene.vehicleManager.GetAABBOfVehicles();
            if((aabb.dimension.x > 0.0) && (aabb.dimension.z > 0.0)){
                appWindow.canvas.scene.viewCamera.position.x = aabb.position.x + 0.5*aabb.dimension.x;
                appWindow.canvas.scene.viewCamera.position.z = aabb.position.z + 0.5*aabb.dimension.z;

                // Reset AABB to center
                aabb.position = -0.5*aabb.dimension;

                // Rotate to camera frame
                glm::dvec3 right = glm::cross(appWindow.canvas.scene.viewCamera.view, appWindow.canvas.scene.viewCamera.up);
                double mat[16];
                mat[0] = right.x; mat[4] = -appWindow.canvas.scene.viewCamera.view.x; mat[8]  = -appWindow.canvas.scene.viewCamera.up.x; mat[12] = 0.0;
                mat[1] = right.y; mat[5] = -appWindow.canvas.scene.viewCamera.view.y; mat[9]  = -appWindow.canvas.scene.viewCamera.up.y; mat[13] = 0.0;
                mat[2] = right.z; mat[6] = -appWindow.canvas.scene.viewCamera.view.z; mat[10] = -appWindow.canvas.scene.viewCamera.up.z; mat[14] = 0.0;
                mat[3] = 0.0;     mat[7] = 0.0;                                       mat[11] = 0.0;                                     mat[15] = 1.0;
                glm::mat4 M = glm::make_mat4(mat);
                AxisAlignedBoundingBox aabbView = M * aabb;

                // Calculate scene zoom
                if((aabbView.dimension.x > 0.0) && (aabbView.dimension.z > 0.0)){
                    double a = aabbView.dimension.x / aabbView.dimension.z;
                    double aspect = double((appWindow.canvas.scene.viewCamera.aspect > 0.001f) ? appWindow.canvas.scene.viewCamera.aspect : 0.001f);
                    if(aspect < 1.0){
                        if(a < aspect){
                            zoomOrtho = aspect * aabbView.dimension.z;
                        }
                        else{
                            zoomOrtho = aabbView.dimension.x;
                        }
                    }
                    else{
                        if(a > aspect){
                            zoomOrtho = aabbView.dimension.x / aspect;
                        }
                        else{
                            zoomOrtho = aabbView.dimension.z;
                        }
                    }
                }

                // Set orthographic zoom
                zoomOrtho = (zoomOrtho < HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MIN) ? HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MIN : ((zoomOrtho > HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MAX) ? HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MAX : zoomOrtho);
                int width, height;
                glfwGetFramebufferSize(wnd, &width, &height);
                appWindow.canvas.scene.UpdateViewCamera(AppWindow::MinSize(glm::ivec2(width, height)));
            }
        }
    }
    (void)wnd;
    (void)scancode;
    (void)mods;
}

void HumanInterface::ZoomOrtho(GLFWwindow* wnd, double zoom){
    if(CAMERA_MODE_ORTHOGRAPHIC == appWindow.canvas.scene.viewCamera.mode){
        zoomOrtho *= (1.0 - 0.1 * zoom);
        zoomOrtho = (zoomOrtho < HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MIN) ? HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MIN : ((zoomOrtho > HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MAX) ? HUMAN_INTERFACE_ZOOM_ORTHOGRAPHIC_MAX : zoomOrtho);
        int width, height;
        glfwGetFramebufferSize(wnd, &width, &height);
        appWindow.canvas.scene.UpdateViewCamera(AppWindow::MinSize(glm::ivec2(width, height)));
    }
}

