#include <FrameBufferGBuffer.hpp>


FrameBufferGBuffer::FrameBufferGBuffer(){
    this->cboPositionDepth = 0;
    this->cboNormalSpecular = 0;
    this->cboAlbedoReflection = 0;
    this->rbo = 0;
    this->fbo = 0;
}

FrameBufferGBuffer::~FrameBufferGBuffer(){}

bool FrameBufferGBuffer::Generate(GLint width, GLint height, GLenum textureUnitPositionDepth, GLenum textureUnitNormalSpecular, GLenum textureUnitAlbedo){
    // Make sure that the G-Buffer is deleted.
    Delete();

    // Let's go:
    DEBUG_GLCHECK( glGenFramebuffers(1, &this->fbo); );
    DEBUG_GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, this->fbo); );
        // Colorbuffer 1: position (world-space) + depth (view space)
        DEBUG_GLCHECK( glGenTextures(1, &this->cboPositionDepth); );
        DEBUG_GLCHECK( glActiveTexture(textureUnitPositionDepth); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, this->cboPositionDepth); );
        DEBUG_GLCHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
        DEBUG_GLCHECK( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->cboPositionDepth, 0); );
        // Colorbuffer 2: normal (RGB) + Specular (A)
        DEBUG_GLCHECK( glGenTextures(1, &this->cboNormalSpecular); );
        DEBUG_GLCHECK( glActiveTexture(textureUnitNormalSpecular); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, this->cboNormalSpecular); );
        DEBUG_GLCHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
        DEBUG_GLCHECK( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->cboNormalSpecular, 0); );
        // Colorbuffer 3: Diffuse color (RGB) + Reflection (A)
        DEBUG_GLCHECK( glGenTextures(1, &this->cboAlbedoReflection); );
        DEBUG_GLCHECK( glActiveTexture(textureUnitAlbedo); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, this->cboAlbedoReflection); );
        DEBUG_GLCHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
        DEBUG_GLCHECK( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->cboAlbedoReflection, 0); );
        // Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        const GLenum colorAttachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        DEBUG_GLCHECK( glDrawBuffers(3, &colorAttachments[0]); );
        // Rendering buffer for depth
        DEBUG_GLCHECK( glGenRenderbuffers(1, &this->rbo); );
        DEBUG_GLCHECK( glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);  );
        DEBUG_GLCHECK( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height); );
        DEBUG_GLCHECK( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->rbo); );
        // Check for completeness
        if(GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)){
            Delete();
            return false;
        }
    DEBUG_GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, 0); );

    // Success
    this->width = width;
    this->height = height;
    return true;
}

void FrameBufferGBuffer::Delete(void){
    // Delete colorbuffers
    if(this->cboPositionDepth){
        glDeleteTextures(1, &this->cboPositionDepth);
        this->cboPositionDepth = 0;
    }
    if(this->cboNormalSpecular){
        glDeleteTextures(1, &this->cboNormalSpecular);
        this->cboNormalSpecular = 0;
    }
    if(this->cboAlbedoReflection){
        glDeleteTextures(1, &this->cboAlbedoReflection);
        this->cboAlbedoReflection = 0;
    }

    // Delete renderbuffer
    if(this->rbo){
        glDeleteRenderbuffers(1, &this->rbo);
        this->rbo = 0;
    }

    // Delete framebuffer
    if(this->fbo){
        glDeleteFramebuffers(1, &this->fbo);
        this->fbo = 0;
    }

    // Reset size
    this->width = 0;
    this->height = 0;
}

