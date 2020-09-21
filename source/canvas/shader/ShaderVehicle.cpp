#include <ShaderVehicle.hpp>
#include <FileManager.hpp>
#include <Common.hpp>


ShaderVehicle::ShaderVehicle(){
    locationModelMatrix = 0;
    locationColorMultiplier = 0;
    locationSpecular = 0;
    locationReflection = 0;
}

ShaderVehicle::~ShaderVehicle(){}

bool ShaderVehicle::Generate(void){
    std::vector<std::pair<std::string, std::string>> replacement;
    replacement.push_back(std::pair<std::string, std::string>("$UBO_VIEW_CAMERA$",std::to_string(UBO_VIEW_CAMERA)));
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_VEHICLE), Shader::GetShadingLanguageVersion(), replacement)){
        return false;
    }
    Use();
    locationModelMatrix = GetUniformLocation("modelMat");
    locationColorMultiplier = GetUniformLocation("colorMultiplier");
    locationSpecular = GetUniformLocation("specular");
    locationReflection = GetUniformLocation("reflection");
    (void) SetColor(glm::vec3(1.0f));
    (void) SetSpecular(0.5f);
    (void) SetReflection(0.0f);
    return true;
}

void ShaderVehicle::SetModelMatrix(glm::mat4& matrix){
    UniformMatrix4fv(locationModelMatrix, GL_FALSE, glm::value_ptr(matrix));
}

glm::vec3 ShaderVehicle::SetColor(glm::vec3 color){
    color.x = (color.x < 0.0) ? 0.0 : (color.x > 1.0) ? 1.0 : color.x;
    color.y = (color.y < 0.0) ? 0.0 : (color.y > 1.0) ? 1.0 : color.y;
    color.z = (color.z < 0.0) ? 0.0 : (color.z > 1.0) ? 1.0 : color.z;
    Uniform3f(locationColorMultiplier, color);
    return color;
}

GLfloat ShaderVehicle::SetSpecular(GLfloat specular){
    specular = (specular < 0.0) ? 0.0 : specular;
    Uniform1f(locationSpecular, specular);
    return specular;
}

GLfloat ShaderVehicle::SetReflection(GLfloat reflection){
    reflection = (reflection < 0.0) ? 0.0 : (reflection > 1.0) ? 1.0 : reflection;
    Uniform1f(locationReflection, reflection);
    return reflection;
}

