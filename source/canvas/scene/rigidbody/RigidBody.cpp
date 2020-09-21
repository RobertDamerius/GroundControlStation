#include <RigidBody.hpp>


RigidBody::RigidBody(){
    UpdateModelMatrix();
}

RigidBody::~RigidBody(){}

void RigidBody::UpdateModelMatrix(void){
    double q0q0 = this->quaternion.w * this->quaternion.w;
    double q1q1 = this->quaternion.x * this->quaternion.x;
    double q2q2 = -this->quaternion.z * -this->quaternion.z;
    double q3q3 = this->quaternion.y * this->quaternion.y;
    double q1q2 = this->quaternion.x * -this->quaternion.z;
    double q0q3 = this->quaternion.w * this->quaternion.y;
    double q1q3 = this->quaternion.x * this->quaternion.y;
    double q0q2 = this->quaternion.w * -this->quaternion.z;
    double q2q3 = -this->quaternion.z * this->quaternion.y;
    double q0q1 = this->quaternion.w * this->quaternion.x;
    GLfloat m[16];
    m[0] = (GLfloat)(q0q0+q1q1-q2q2-q3q3);  m[4] = (GLfloat)(q1q2+q1q2-q0q3-q0q3);  m[8]  = (GLfloat)(q1q3+q1q3+q0q2+q0q2);  m[12] = (GLfloat)(this->position.x);
    m[1] = (GLfloat)(q1q2+q1q2+q0q3+q0q3);  m[5] = (GLfloat)(q0q0-q1q1+q2q2-q3q3);  m[9]  = (GLfloat)(q2q3+q2q3-q0q1-q0q1);  m[13] = (GLfloat)(-this->position.z);
    m[2] = (GLfloat)(q1q3+q1q3-q0q2-q0q2);  m[6] = (GLfloat)(q2q3+q2q3+q0q1+q0q1);  m[10] = (GLfloat)(q0q0-q1q1-q2q2+q3q3);  m[14] = (GLfloat)(this->position.y);
    m[3] = 0.0f;                            m[7] = 0.0f;                            m[11] = 0.0f;                            m[15] = 1.0f;
    this->modelMatrix = glm::make_mat4(m);
}

RigidBody& RigidBody::operator=(const RigidBody& rhs){
    RigidBodyState::operator=(rhs);
    modelMatrix = rhs.modelMatrix;
    return *this;
}

RigidBody& RigidBody::operator=(const RigidBodyState& rhs){
    RigidBodyState::operator=(rhs);
    return *this;
}

