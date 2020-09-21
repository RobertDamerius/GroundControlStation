#include <ShaderSceneText.hpp>
#include <FileManager.hpp>
#include <Common.hpp>


ShaderSceneText::ShaderSceneText(){
    locationInvResolutionHalf = 0;
    locationOffsetClipSpace = 0;
    locationColor = 0;
}

ShaderSceneText::~ShaderSceneText(){}

bool ShaderSceneText::Generate(void){
    if(!RD::Engine::Shader::Generate(FileManager::GetName(FILE_SHADER_SCENE_TEXT))){
        return false;
    }
    Use();
    Uniform1i("textureFontSprite", TEXTURENUM_FONT_SPRITE);
    locationInvResolutionHalf = GetUniformLocation("invResolutionHalf");
    locationOffsetClipSpace = GetUniformLocation("offsetClipSpace");
    locationColor = GetUniformLocation("color");
    return true;
}

glm::vec4 ShaderSceneText::SetColor(glm::vec4 color){
    color.x = (color.x < 0.0) ? 0.0 : (color.x > 1.0) ? 1.0 : color.x;
    color.y = (color.y < 0.0) ? 0.0 : (color.y > 1.0) ? 1.0 : color.y;
    color.z = (color.z < 0.0) ? 0.0 : (color.z > 1.0) ? 1.0 : color.z;
    color.w = (color.w < 0.0) ? 0.0 : (color.w > 1.0) ? 1.0 : color.w;
    Uniform4f(locationColor, color);
    return color;
}

void ShaderSceneText::SetResolution(glm::vec2 size){
    size.x = (size.x < 1.0f) ? 1.0f : size.x * 0.5f;
    size.y = (size.y < 1.0f) ? 1.0f : size.y * 0.5f;
    glm::vec2 inv;
    inv.x = 1.0f / size.x;
    inv.y = 1.0f / size.y;
    Uniform2f(locationInvResolutionHalf, inv);
}

void ShaderSceneText::SetOffsetClipSpace(glm::vec2 offset){
    Uniform2f(locationOffsetClipSpace, offset);
}

