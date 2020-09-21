#include <ShaderGroundPlane.hpp>
#include <FileManager.hpp>
#include <Common.hpp>


ShaderGroundPlane::ShaderGroundPlane(){}

ShaderGroundPlane::~ShaderGroundPlane(){}

bool ShaderGroundPlane::Generate(void){
    std::vector<std::pair<std::string, std::string>> replacement;
    replacement.push_back(std::pair<std::string, std::string>("$UBO_VIEW_CAMERA$",std::to_string(UBO_VIEW_CAMERA)));
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_GROUNDPLANE), Shader::GetShadingLanguageVersion(), replacement)){
        return false;
    }
    Use();
    locationColor = GetUniformLocation("planeColor");
    return true;
}

glm::vec3 ShaderGroundPlane::SetColor(glm::vec3& color){
    color.x = (color.x < 0.0) ? 0.0 : ((color.x > 1.0) ? 1.0 : color.x);
    color.y = (color.y < 0.0) ? 0.0 : ((color.y > 1.0) ? 1.0 : color.y);
    color.z = (color.z < 0.0) ? 0.0 : ((color.z > 1.0) ? 1.0 : color.z);
    Uniform3f(locationColor, color);
    return color;
}

