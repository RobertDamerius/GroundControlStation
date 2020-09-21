#pragma once


#include <RD.hpp>


class ShaderTransparentVehicle: protected RD::Engine::Shader {
    public:
        using RD::Engine::Shader::Use;
        using RD::Engine::Shader::Delete;

        /**
         *  @brief Create transparent vehicle shader.
         */
        ShaderTransparentVehicle();

        /**
         *  @brief Delete transparent vehicle shader.
         */
        ~ShaderTransparentVehicle();

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
         *  @brief Set color multiplier.
         *  @param [in] color The color multiplier (RGB).
         *  @return Final color value ensured to be in range [0, 1] for each component.
         */
        glm::vec3 SetColor(glm::vec3 color);

        /**
         *  @brief Set alpha value.
         *  @param [in] alpha Alpha value (>= 0).
         *  @return Final alpha value ensured to be greater than or equal to zero.
         */
        GLfloat SetAlpha(GLfloat alpha);

    private:
        GLint locationModelMatrix;
        GLint locationColorMultiplier;
        GLint locationAlpha;
};

