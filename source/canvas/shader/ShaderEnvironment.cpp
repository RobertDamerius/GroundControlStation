#include <ShaderEnvironment.hpp>
#include <FileManager.hpp>
#include <Common.hpp>
#include <Configuration.hpp>


ShaderEnvironment::ShaderEnvironment(){
    locationInvResolution = 0;
    locationAmbient = 0;
    locationShininess = 0;
}

ShaderEnvironment::~ShaderEnvironment(){}

bool ShaderEnvironment::Generate(void){
    std::vector<std::pair<std::string, std::string>> replacement;
    replacement.push_back(std::pair<std::string, std::string>("$UBO_VIEW_CAMERA$",std::to_string(UBO_VIEW_CAMERA)));
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_ENVIRONMENT), Shader::GetShadingLanguageVersion(), replacement)){
        return false;
    }
    Use();
    locationInvResolution = GetUniformLocation("invResolution");
    locationAmbient = GetUniformLocation("ambientStrength");
    locationShininess = GetUniformLocation("shininess");
    Uniform1i("textureSkybox", TEXTURENUM_SKYBOX);
    Uniform1i("texturePositionDepth", TEXTURENUM_GBUFFER_POSITION_DEPTH);
    Uniform1i("textureNormalSpecular", TEXTURENUM_GBUFFER_NORMAL_SPECULAR);
    Uniform1i("textureAlbedoReflection", TEXTURENUM_GBUFFER_ALBEDO_REFLECTION);
    SetAmbient((float)Configuration::style.sceneAmbient);
    SetShininess((float)Configuration::style.sceneShininess);
    return true;
}

void ShaderEnvironment::SetResolution(glm::ivec2 size){
    size.x = (size.x) < 1 ? 1 : size.x;
    size.y = (size.y) < 1 ? 1 : size.y;
    glm::vec2 v((GLfloat)(1.0 / (double)size.x), (GLfloat)(1.0 / (double)size.y));
    Uniform2f(locationInvResolution, v);
}

void ShaderEnvironment::SetAmbient(float ambient){
    ambient = (ambient < 0.0f) ? 0.0f : ambient;
    Uniform1f(locationAmbient, ambient);
}

void ShaderEnvironment::SetShininess(float shininess){
    shininess = (shininess < 0.0f) ? 0.0f : shininess;
    Uniform1f(locationShininess, shininess);
}

