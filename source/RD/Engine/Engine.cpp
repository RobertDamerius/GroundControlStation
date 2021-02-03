/**
 *  @file Engine.cpp
 *  @details Version 20210203.
 */
#include <Engine.hpp>


std::string RD::Engine::GLErrorToString(GLenum error){
    if(GL_NO_ERROR == error)                        return std::string("GL_NO_ERROR");
    if(GL_INVALID_ENUM == error)                    return std::string("GL_INVALID_ENUM");
    if(GL_INVALID_VALUE == error)                   return std::string("GL_INVALID_VALUE");
    if(GL_INVALID_OPERATION == error)               return std::string("GL_INVALID_OPERATION");
    if(GL_INVALID_FRAMEBUFFER_OPERATION == error)   return std::string("GL_INVALID_FRAMEBUFFER_OPERATION");
    if(GL_OUT_OF_MEMORY == error)                   return std::string("GL_OUT_OF_MEMORY");
    if(GL_STACK_UNDERFLOW == error)                 return std::string("GL_STACK_UNDERFLOW");
    if(GL_STACK_OVERFLOW == error)                  return std::string("GL_STACK_OVERFLOW");
    return std::string("UNKNOWN GL ERROR");
}

void RD::Engine::GetGLInfo(std::string* versionGL, std::string* versionGLSL, std::string* vendor, std::string* renderer){
    GLint iMajor, iMinor;
    const GLubyte *strGLSL = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte *strVendor = glGetString(GL_VENDOR);
    const GLubyte *strRenderer = glGetString(GL_RENDERER);
    DEBUG_GLCHECK( glGetIntegerv(GL_MAJOR_VERSION, &iMajor); );
    DEBUG_GLCHECK( glGetIntegerv(GL_MINOR_VERSION, &iMinor); );
    try { if(versionGL) *versionGL = std::to_string(iMajor) + std::string(".") + std::to_string(iMinor);} catch(...){}
    try { if(versionGLSL) *versionGLSL = std::string((const char*)strGLSL);} catch(...){}
    try { if(vendor) *vendor = std::string((const char*)strVendor);} catch(...){}
    try { if(renderer) *renderer = std::string((const char*)strRenderer);} catch(...){}
}

