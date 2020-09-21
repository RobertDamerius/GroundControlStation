#pragma once


#include <RD.hpp>


class ShaderSceneText: protected RD::Engine::Shader {
    public:
        using RD::Engine::Shader::Use;
        using RD::Engine::Shader::Delete;

        /**
         *  @brief Create scene text shader.
         */
        ShaderSceneText();

        /**
         *  @brief Delete scene text shader.
         */
        ~ShaderSceneText();

        /**
         *  @brief Generate the shader.
         *  @return True if success, false otherwise.
         */
        bool Generate(void);

        /**
         *  @brief Set text color.
         *  @param [in] color The linear text color (RGBA).
         *  @return Final color value ensured to be in range [0, 1] for each component.
         */
        glm::vec4 SetColor(glm::vec4 color);

        /**
         *  @brief Set the screen resolution.
         *  @param [in] size Resolution in pixels.
         */
        void SetResolution(glm::vec2 size);

        /**
         *  @brief Set the x-y-offset for the clip space position.
         *  @param [in] offset The offset position in clip space.
         */
        void SetOffsetClipSpace(glm::vec2 offset);

    private:
        GLint locationInvResolutionHalf;
        GLint locationOffsetClipSpace;
        GLint locationColor;
};

