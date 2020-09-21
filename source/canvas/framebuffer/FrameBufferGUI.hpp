#pragma once


#include <FrameBufferObject.hpp>


class FrameBufferGUI: public FrameBufferObject {
    public:
        GLuint cbo;  ///< Colorbuffer (RGBA).
        GLuint rbo;  ///< Renderbuffer object.
        GLuint fbo;  ///< The actual framebuffer object.

        /**
         *  @brief Create a GUI framebuffer.
         */
        FrameBufferGUI();

        /**
         *  @brief Delete the GUI framebuffer.
         *  @details The destructor does NOT call the @ref Delete member function!
         */
        ~FrameBufferGUI();

        /**
         *  @brief Generate colorbuffer, renderbuffer and framebuffer.
         *  @param [in] width Width of the framebuffer in pixels.
         *  @param [in] height Height of the framebuffer in pixels.
         *  @param [in] textureUnitGUI Texture unit for GUI color buffer.
         *  @return True if success, false otherwise.
         *  @details @ref Delete is called first before actually generating the FBO.
         */
        bool Generate(GLint width, GLint height, GLenum textureUnitGUI);

        /**
         *  @brief Delete colorbuffers, renderbuffer and framebuffer object.
         */
        void Delete(void);
};

