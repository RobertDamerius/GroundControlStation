#include <ShaderLine.hpp>
#include <Common.hpp>
#include <FileManager.hpp>
#include <AppWindow.hpp>


ShaderLine::ShaderLine(){
    locationModelMatrix = 0;
    locationLineWidth = 0;
    locationInfoScreenSize = 0;
    locationColor = 0;
}

ShaderLine::~ShaderLine(){}

bool ShaderLine::Generate(void){
    std::vector<std::pair<std::string, std::string>> replacement;
    replacement.push_back(std::pair<std::string, std::string>("$UBO_VIEW_CAMERA$",std::to_string(UBO_VIEW_CAMERA)));
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_LINE), Shader::GetShadingLanguageVersion(), replacement)){
        return false;
    }
    Use();
    locationModelMatrix = GetUniformLocation("modelMat");
    locationLineWidth = GetUniformLocation("lineWidth");
    locationInfoScreenSize = GetUniformLocation("infoScreenSize");
    locationColor = GetUniformLocation("colorMultiplier");
    SetLineWidth(1.0f);
    SetResolution(appWindow.GetSize());
    SetColor(glm::vec3(1.0f));
    return true;
}

void ShaderLine::SetModelMatrix(glm::mat4& matrix){
    UniformMatrix4fv(locationModelMatrix, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderLine::SetLineWidth(GLfloat width){
    width = (width < 1.0f) ? 1.0f : width;
    Uniform1f(locationLineWidth, width);
}

void ShaderLine::SetResolution(glm::ivec2 size){
    size.x = (size.x < 1) ? 1 : size.x;
    size.y = (size.y < 1) ? 1 : size.y;
    glm::vec3 info;
    info.x = (GLfloat)(double(size.x)/double(size.y));
    info.y = (GLfloat)(double(size.y)/double(size.x));
    info.z = (GLfloat)(1.0 / double(size.y));
    Uniform3f(locationInfoScreenSize, info);
}

void ShaderLine::SetColor(glm::vec3 color){
    Uniform3f(locationColor, color);
}

