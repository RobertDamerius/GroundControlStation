#include <ShaderPolygon.hpp>
#include <FileManager.hpp>
#include <Common.hpp>


ShaderPolygon::ShaderPolygon(){
    locationModelMatrix = 0;
    locationColorMultiplier = 0;
    locationSpecular = 0;
    locationReflection = 0;
    locationClassification = 0;
    locationVelocity = 0;
}

ShaderPolygon::~ShaderPolygon(){}

bool ShaderPolygon::Generate(void){
    std::vector<std::pair<std::string, std::string>> replacement;
    replacement.push_back(std::pair<std::string, std::string>("$UBO_VIEW_CAMERA$",std::to_string(UBO_VIEW_CAMERA)));
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_POLYGON), Shader::GetShadingLanguageVersion(), replacement)){
        return false;
    }
    Use();
    locationModelMatrix = GetUniformLocation("modelMat");
    locationColorMultiplier = GetUniformLocation("colorMultiplier");
    locationSpecular = GetUniformLocation("specular");
    locationReflection = GetUniformLocation("reflection");
    locationClassification = GetUniformLocation("classification");
    locationVelocity = GetUniformLocation("velocity");
    (void) SetColor(glm::vec3(1.0f));
    (void) SetSpecular(0.5f);
    (void) SetReflection(0.0f);
    SetClassification(0);
    SetVelocity(0.0f, 0.0f, 0.0f);
    return true;
}

void ShaderPolygon::SetModelMatrix(glm::mat4& matrix){
    UniformMatrix4fv(locationModelMatrix, GL_FALSE, glm::value_ptr(matrix));
}

glm::vec3 ShaderPolygon::SetColor(glm::vec3 color){
    color.x = (color.x < 0.0) ? 0.0 : (color.x > 1.0) ? 1.0 : color.x;
    color.y = (color.y < 0.0) ? 0.0 : (color.y > 1.0) ? 1.0 : color.y;
    color.z = (color.z < 0.0) ? 0.0 : (color.z > 1.0) ? 1.0 : color.z;
    Uniform3f(locationColorMultiplier, color);
    return color;
}

GLfloat ShaderPolygon::SetSpecular(GLfloat specular){
    specular = (specular < 0.0) ? 0.0 : specular;
    Uniform1f(locationSpecular, specular);
    return specular;
}

GLfloat ShaderPolygon::SetReflection(GLfloat reflection){
    reflection = (reflection < 0.0) ? 0.0 : (reflection > 1.0) ? 1.0 : reflection;
    Uniform1f(locationReflection, reflection);
    return reflection;
}

void ShaderPolygon::SetClassification(GLubyte classification){
    Uniform1ui(locationClassification, classification);
}

void ShaderPolygon::SetVelocity(GLfloat vN, GLfloat vE, GLfloat r){
    glm::vec3 velocity(vN, vE, r);
    Uniform3f(locationVelocity, velocity);
}
