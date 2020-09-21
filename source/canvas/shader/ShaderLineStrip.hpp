#pragma once


#include <RD.hpp>


class ShaderLineStrip: protected RD::Engine::Shader {
    public:
        using RD::Engine::Shader::Use;
        using RD::Engine::Shader::Delete;

        /**
         *  @brief Create line strip shader.
         */
        ShaderLineStrip();

        /**
         *  @brief Delete line strip shader.
         */
        ~ShaderLineStrip();

        /**
         *  @brief Generate the shader.
         *  @return True if success, false otherwise.
         */
        bool Generate(void);

        /**
         *  @brief Set model matrix.
         *  @param [in] matrix The model matrix.
         */
        void SetModelMatrix(glm::mat4& matrix);

        /**
         *  @brief Set the line width.
         *  @param [in] width Line width (will be ensured to be greater than or equal to 1.0f).
         */
        void SetLineWidth(GLfloat width);

        /**
         *  @brief Set the screen resolution.
         *  @param [in] size The screen size in pixels (will be ensured to be greater than or equal to [1, 1]).
         */
        void SetResolution(glm::ivec2 size);

        /**
         *  @brief Set color multiplier.
         *  @param [in] color Color multiplier.
         */
        void SetColor(glm::vec3 color);

        /**
         *  @brief Set position offset.
         *  @param [in] positionOffset The position offset (OpenGL-frame).
         */
        void SetPositionOffset(glm::vec3 positionOffset);

        /**
         *  @brief Set altitude gain.
         *  @param [in] gain Altitude gain.
         */
        void SetAltitudeGain(GLfloat gain);

    private:
        GLint locationModelMatrix;
        GLint locationLineWidth;
        GLint locationInfoScreenSize;
        GLint locationColor;
        GLint locationPositionOffset;
        GLint locationAltitudeGain;
};

