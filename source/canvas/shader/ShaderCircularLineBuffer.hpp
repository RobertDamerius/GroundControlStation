#pragma once


#include <RD.hpp>


class ShaderCircularLineBuffer: protected RD::Engine::Shader {
    public:
        using RD::Engine::Shader::Use;
        using RD::Engine::Shader::Delete;

        /**
         *  @brief Create circular line buffer shader.
         */
        ShaderCircularLineBuffer();

        /**
         *  @brief Delete circular line buffer shader.
         */
        ~ShaderCircularLineBuffer();

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
         *  @brief Set scale for the vertex color value.
         *  @param [in] scale Scale value.
         */
        void SetValueScale(GLfloat scale);

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
        GLint locationValueScale;
        GLint locationPositionOffset;
        GLint locationAltitudeGain;
};

