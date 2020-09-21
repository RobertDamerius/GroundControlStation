/**
 *  @file Camera.cpp
 *  @details Version 20200513.
 */
#include <Engine.hpp>


void RD::Engine::Camera::UpdateProjectionMatrix(void){
    if(CAMERA_MODE_PERSPECTIVE == this->mode)
        this->projectionMatrix = glm::dmat4(glm::perspective(fov, aspect, clipNear, clipFar));
    else
        this->projectionMatrix = glm::dmat4(glm::ortho(left, right, bottom, top, clipNear, clipFar));
}

RD::Engine::Camera::Camera(double left, double right, double bottom, double top, double clipNear, double clipFar, glm::dvec3 view, glm::dvec3 up): left(left), right(right), bottom(bottom), top(top), clipNear(clipNear), clipFar(clipFar), view(view), up(up){
    this->position = glm::dvec3(0.0);
    this->aspect = 8.0 / 5.0;
    this->fov = glm::radians(70.0);
    this->mode = CAMERA_MODE_ORTHOGRAPHIC;
    this->ubo = 0;
    UpdateProjectionMatrix();
}

RD::Engine::Camera::Camera(double clipNear, double clipFar, double aspect, double fov, glm::dvec3 position, glm::dvec3 view, glm::dvec3 up): clipNear(clipNear), clipFar(clipFar), aspect(aspect), fov(fov), position(position), view(view), up(up){
    this->left = -1.0;
    this->right = 1.0;
    this->bottom = -1.0;
    this->top = 1.0;
    this->mode = CAMERA_MODE_PERSPECTIVE;
    this->ubo = 0;
    UpdateProjectionMatrix();
}

RD::Engine::Camera::~Camera(){}

RD::Engine::Camera& RD::Engine::Camera::operator=(const RD::Engine::Camera& rhs){
    position = rhs.position;
    view = rhs.view;
    up = rhs.up;
    left = rhs.left;
    right = rhs.right;
    bottom = rhs.bottom;
    top = rhs.top;
    clipNear = rhs.clipNear;
    clipFar = rhs.clipFar;
    aspect = rhs.aspect;
    fov = rhs.fov;
    mode = rhs.mode;
    ubo = rhs.ubo;
    projectionMatrix = rhs.projectionMatrix;
    return *this;
}

void RD::Engine::Camera::Rotate(double angle, glm::dvec3 axis){
    axis = glm::normalize(axis);
    double angle_half = 0.5 * angle;
    double sin_angle_half = sin(angle_half);
    double q0 = cos(angle_half);
    double q1 = axis.x * sin_angle_half;
    double q2 = axis.y * sin_angle_half;
    double q3 = axis.z * sin_angle_half;
    double invNorm = 1.0 / sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 *= invNorm;
    q1 *= invNorm;
    q2 *= invNorm;
    q3 *= invNorm;
    double q0q0 = q0*q0;
    double q0q1 = q0*q1;
    double q0q2 = q0*q2;
    double q0q3 = q0*q3;
    double q1q1 = q1*q1;
    double q1q2 = q1*q2;
    double q1q3 = q1*q3;
    double q2q2 = q2*q2;
    double q2q3 = q2*q3;
    double q3q3 = q3*q3;
    double d[9];
    d[0] = q0q0+q1q1-q2q2-q3q3; d[3] = q1q2+q1q2-q0q3-q0q3; d[6] = q1q3+q1q3+q0q2+q0q2;
    d[1] = q1q2+q1q2+q0q3+q0q3; d[4] = q0q0-q1q1+q2q2-q3q3; d[7] = q2q3+q2q3-q0q1-q0q1;
    d[2] = q1q3+q1q3-q0q2-q0q2; d[5] = q2q3+q2q3+q0q1+q0q1; d[8] = q0q0-q1q1-q2q2+q3q3;
    glm::mat3 D = glm::make_mat3(d);
    view = glm::normalize(D * view);
    up = glm::normalize(D * up);
}

void RD::Engine::Camera::RollView(double angle){
    Rotate(angle, view);
}

void RD::Engine::Camera::PitchView(double angle){
    Rotate(angle, glm::cross(view, up));
}

void RD::Engine::Camera::YawView(double angle){
    Rotate(angle, -up);
}

void RD::Engine::Camera::MoveView(glm::dvec3 xyz){
    position += (xyz.x * glm::cross(view, up)) + (xyz.y * up) - (xyz.z * view);
}

glm::dmat4 RD::Engine::Camera::GetViewMatrix(void){
    return glm::lookAt(position, position + view, up);
}

glm::dmat4 RD::Engine::Camera::GetProjectionMatrix(void){
    return this->projectionMatrix;
}

glm::dmat4 RD::Engine::Camera::GetProjectionViewMatrix(void){
    return this->projectionMatrix * GetViewMatrix();
}

void RD::Engine::Camera::GenerateUniformBufferObject(GLuint bindingPoint){
    // Delete if ubo already exists
    DeleteUniformBufferObject();

    // Create UBO (40 floats = 160 bytes)
    DEBUG_GLCHECK( glGenBuffers(1, &ubo); );
    DEBUG_GLCHECK( glBindBuffer(GL_UNIFORM_BUFFER, ubo); );
    DEBUG_GLCHECK( glBufferData(GL_UNIFORM_BUFFER, 160, nullptr, GL_DYNAMIC_DRAW); );
    DEBUG_GLCHECK( glBindBuffer(GL_UNIFORM_BUFFER, 0); );

    // Set binding point
    DEBUG_GLCHECK( glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo); );

    // Initial buffer update
    UpdateUniformBufferObject();
}

void RD::Engine::Camera::DeleteUniformBufferObject(void){
    if(ubo){
        DEBUG_GLCHECK( glDeleteBuffers(1, &ubo); );
        ubo = 0;
    }
}

void RD::Engine::Camera::UpdateUniformBufferObject(void){
    // Buffer of 40 floats (160 bytes)
    GLfloat buffer[40];

    // Set position
    buffer[32] = (GLfloat)(this->position.x);
    buffer[33] = (GLfloat)(this->position.y);
    buffer[34] = (GLfloat)(this->position.z);
    buffer[35] = 0.0f;

    // Set view direction
    buffer[36] = (GLfloat)(this->view.x);
    buffer[37] = (GLfloat)(this->view.y);
    buffer[38] = (GLfloat)(this->view.z);
    buffer[39] = 0.0f;

    // Get view matrix
    glm::dmat4 V = GetViewMatrix();

    // Compute projection-view and projection-view (translation free)
    glm::mat4 PV = glm::mat4(this->projectionMatrix * V);
    glm::mat4 PVTFree = glm::mat4(this->projectionMatrix * glm::dmat4(glm::dmat3(V)));

    // Copy matrices to buffer
    GLfloat* p = glm::value_ptr(PV);
    memcpy(&buffer[0], p, 64); // 16 floats = 64 bytes
    p = glm::value_ptr(PVTFree);
    memcpy(&buffer[16], p, 64); // 16 floats = 64 bytes

    // Update buffer data (all 96 bytes)
    DEBUG_GLCHECK( glBindBuffer(GL_UNIFORM_BUFFER, ubo); );
    DEBUG_GLCHECK( glBufferSubData(GL_UNIFORM_BUFFER, 0, 160, &buffer[0]); );
    DEBUG_GLCHECK( glBindBuffer(GL_UNIFORM_BUFFER, 0); );
}

