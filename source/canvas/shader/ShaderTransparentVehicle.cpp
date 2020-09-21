#include <ShaderTransparentVehicle.hpp>
#include <FileManager.hpp>
#include <Common.hpp>


ShaderTransparentVehicle::ShaderTransparentVehicle(){
    locationModelMatrix = 0;
    locationColorMultiplier = 0;
    locationAlpha = 0;
}

ShaderTransparentVehicle::~ShaderTransparentVehicle(){}

bool ShaderTransparentVehicle::Generate(void){
    std::vector<std::pair<std::string, std::string>> replacement;
    replacement.push_back(std::pair<std::string, std::string>("$UBO_VIEW_CAMERA$",std::to_string(UBO_VIEW_CAMERA)));
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_TRANSPARENT_VEHICLE), Shader::GetShadingLanguageVersion(), replacement)){
        return false;
    }
    Use();
    locationModelMatrix = GetUniformLocation("modelMat");
    locationColorMultiplier = GetUniformLocation("colorMultiplier");
    locationAlpha = GetUniformLocation("alpha");
    (void) SetColor(glm::vec3(1.0f));
    (void) SetAlpha(1.0f);
    return true;
}

void ShaderTransparentVehicle::SetModelMatrix(glm::mat4& matrix){
    UniformMatrix4fv(locationModelMatrix, GL_FALSE, glm::value_ptr(matrix));
}

glm::vec3 ShaderTransparentVehicle::SetColor(glm::vec3 color){
    color.x = (color.x < 0.0) ? 0.0 : (color.x > 1.0) ? 1.0 : color.x;
    color.y = (color.y < 0.0) ? 0.0 : (color.y > 1.0) ? 1.0 : color.y;
    color.z = (color.z < 0.0) ? 0.0 : (color.z > 1.0) ? 1.0 : color.z;
    Uniform3f(locationColorMultiplier, color);
    return color;
}

GLfloat ShaderTransparentVehicle::SetAlpha(GLfloat alpha){
    alpha = (alpha < 0.0) ? 0.0 : alpha;
    Uniform1f(locationAlpha, alpha);
    return alpha;
}

