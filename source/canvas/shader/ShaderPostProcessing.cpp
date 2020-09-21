#include <ShaderPostProcessing.hpp>
#include <FileManager.hpp>
#include <Common.hpp>


ShaderPostProcessing::ShaderPostProcessing(){
    locationInvGamma = 0;
}

ShaderPostProcessing::~ShaderPostProcessing(){}

bool ShaderPostProcessing::Generate(void){
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_POST_PROCESSING))){
        return false;
    }
    Use();
    Uniform1i("textureGUI", TEXTURENUM_GUI);
    Uniform1i("textureSceneAlbedo", TEXTURENUM_ENVIRONMENT_ALBEDO);
    locationInvGamma = GetUniformLocation("invGamma");
    return true;
}

GLfloat ShaderPostProcessing::SetGamma(GLfloat gamma){
    gamma = (gamma < GAMMA_MIN) ? GAMMA_MIN : ((gamma > GAMMA_MAX) ? GAMMA_MAX : gamma);
    Uniform1f(locationInvGamma, 1.0f / gamma);
    return gamma;
}

