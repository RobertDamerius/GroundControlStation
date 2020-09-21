#include <FrameBufferEnvironment.hpp>


FrameBufferEnvironment::FrameBufferEnvironment(){
    this->fbo = 0;
    this->cbo = 0;
}

FrameBufferEnvironment::~FrameBufferEnvironment(){}

bool FrameBufferEnvironment::Generate(GLint width, GLint height, GLuint rbo, GLenum textureUnitSceneColor){
    // Make sure that the buffer is deleted.
    Delete();

    // Environment framebuffer
    DEBUG_GLCHECK( glGenFramebuffers(1, &this->fbo); );
    DEBUG_GLCHECK( glBindFramebuffer(GL_FRAMEBUFFER, this->fbo); );
        // Colorbuffer
        DEBUG_GLCHECK( glGenTextures(1, &this->cbo); );
        DEBUG_GLCHECK( glActiveTexture(textureUnitSceneColor); );
        DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, this->cbo); );
        DEBUG_GLCHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); );
        DEBUG_GLCHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); );
        DEBUG_GLCHECK( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->cbo, 0); );
        // Bind render buffer object
        DEBUG_GLCHECK( glBindRenderbuffer(GL_RENDERBUFFER, rbo);  );
        DEBUG_GLCHECK( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); );
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

void FrameBufferEnvironment::Delete(void){
    // Delete colorbuffers
    if(this->cbo){
        glDeleteTextures(1, &this->cbo);
        this->cbo = 0;
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

