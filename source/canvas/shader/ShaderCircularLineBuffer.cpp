#include <ShaderCircularLineBuffer.hpp>
#include <Common.hpp>
#include <FileManager.hpp>
#include <AppWindow.hpp>


ShaderCircularLineBuffer::ShaderCircularLineBuffer(){
    locationModelMatrix = 0;
    locationLineWidth = 0;
    locationInfoScreenSize = 0;
    locationValueScale = 0;
    locationPositionOffset = 0;
    locationAltitudeGain = 0;
}

ShaderCircularLineBuffer::~ShaderCircularLineBuffer(){}

bool ShaderCircularLineBuffer::Generate(void){
    std::vector<std::pair<std::string, std::string>> replacement;
    replacement.push_back(std::pair<std::string, std::string>("$UBO_VIEW_CAMERA$",std::to_string(UBO_VIEW_CAMERA)));
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_CIRCULAR_LINE_BUFFER), Shader::GetShadingLanguageVersion(), replacement)){
        return false;
    }
    Use();
    locationModelMatrix = GetUniformLocation("modelMat");
    locationLineWidth = GetUniformLocation("lineWidth");
    locationInfoScreenSize = GetUniformLocation("infoScreenSize");
    locationValueScale = GetUniformLocation("valueScale");
    locationPositionOffset = GetUniformLocation("positionOffset");
    locationAltitudeGain = GetUniformLocation("altitudeGain");
    SetLineWidth(1.0f);
    SetResolution(appWindow.GetSize());
    SetValueScale(1.0f);
    SetPositionOffset(glm::vec3(0.0f));
    SetAltitudeGain(1.0f);
    return true;
}

void ShaderCircularLineBuffer::SetModelMatrix(glm::mat4& matrix){
    UniformMatrix4fv(locationModelMatrix, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderCircularLineBuffer::SetLineWidth(GLfloat width){
    width = (width < 1.0f) ? 1.0f : width;
    Uniform1f(locationLineWidth, width);
}

void ShaderCircularLineBuffer::SetResolution(glm::ivec2 size){
    size.x = (size.x < 1) ? 1 : size.x;
    size.y = (size.y < 1) ? 1 : size.y;
    glm::vec3 info;
    info.x = (GLfloat)(double(size.x)/double(size.y));
    info.y = (GLfloat)(double(size.y)/double(size.x));
    info.z = (GLfloat)(1.0 / double(size.y));
    Uniform3f(locationInfoScreenSize, info);
}

void ShaderCircularLineBuffer::SetValueScale(GLfloat scale){
    Uniform1f(locationValueScale, scale);
}

void ShaderCircularLineBuffer::SetPositionOffset(glm::vec3 positionOffset){
    Uniform3f(locationPositionOffset, positionOffset);
}

void ShaderCircularLineBuffer::SetAltitudeGain(GLfloat gain){
    Uniform1f(locationAltitudeGain, gain);
}

