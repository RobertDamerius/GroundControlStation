#pragma once


#include <FrameBufferObject.hpp>


class FrameBufferGBuffer: public FrameBufferObject {
    public:
        GLuint cboPositionDepth;    ///< Colorbuffer (World-space Position and View Depth).
        GLuint cboNormalSpecular;   ///< Colorbuffer (Normal + Specular).
        GLuint cboAlbedoReflection; ///< Colorbuffer (Albedo + Reflection).
        GLuint rbo;                 ///< Renderbuffer object.
        GLuint fbo;                 ///< The actual framebuffer object.

        /**
         *  @brief Create a G-buffer.
         */
        FrameBufferGBuffer();

        /**
         *  @brief Delete the G-buffer.
         *  @details The destructor does NOT call the @ref Delete member function!
         */
        ~FrameBufferGBuffer();

        /**
         *  @brief Generate colorbuffer, renderbuffer and framebuffer.
         *  @param [in] width Width of the framebuffer in pixels.
         *  @param [in] height Height of the framebuffer in pixels.
         *  @param [in] textureUnitPositionDepth Texture unit for position/depth color buffer.
         *  @param [in] textureUnitNormalSpecular Texture unit for normal/specular color buffer.
         *  @param [in] textureUnitAlbedoReflection Texture unit for albedo/reflection color buffer.
         *  @return True if success, false otherwise.
         *  @details @ref Delete is called first before actually generating the FBO.
         */
        bool Generate(GLint width, GLint height, GLenum textureUnitPositionDepth, GLenum textureUnitNormalSpecular, GLenum textureUnitAlbedoReflection);

        /**
         *  @brief Delete colorbuffers, renderbuffer and framebuffer object.
         */
        void Delete(void);
};

