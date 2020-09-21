#pragma once


#include <FrameBufferObject.hpp>


class FrameBufferEnvironment: public FrameBufferObject {
    public:
        GLuint cbo;  ///< Colorbuffer (RGBA).
        GLuint fbo;  ///< The actual framebuffer object.

        /**
         *  @brief Create framebuffer for final scene rendering.
         */
        FrameBufferEnvironment();

        /**
         *  @brief Delete the final scene framebuffer.
         */
        ~FrameBufferEnvironment();

        /**
         *  @brief Generate the framebuffers.
         *  @param [in] width Width of the framebuffer in pixels.
         *  @param [in] height Height of the framebuffer in pixels.
         *  @param [in] rbo Render buffer object of the G-buffer.
         *  @param [in] textureUnitSceneColor Texture unit for scene albedo color texture.
         *  @return True if success, false otherwise.
         */
        bool Generate(GLint width, GLint height, GLuint rbo, GLenum textureUnitSceneColor);

        /**
         *  @brief Delete colorbuffers and framebuffer object.
         */
        void Delete(void);
};

