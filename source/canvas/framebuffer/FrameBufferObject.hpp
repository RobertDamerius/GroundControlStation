#pragma once


#include <RD.hpp>


class FrameBufferObject {
    public:
        GLuint width;  ///< The width of the framebuffer in pixels.
        GLuint height; ///< The height of the framebuffer in pixels.

        /**
         *  @brief Create a framebuffer object.
         */
        FrameBufferObject();

        /**
         *  @brief Delete the framebuffer objects.
         */
        virtual ~FrameBufferObject();
};

