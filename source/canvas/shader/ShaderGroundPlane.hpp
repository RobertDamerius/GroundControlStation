#pragma once


#include <RD.hpp>


class ShaderGroundPlane: protected RD::Engine::Shader {
    public:
        using RD::Engine::Shader::Use;
        using RD::Engine::Shader::Delete;

        /**
         *  @brief Create ground plane shader.
         */
        ShaderGroundPlane();

        /**
         *  @brief Delete ground plane shader.
         */
        ~ShaderGroundPlane();

        /**
         *  @brief Generate the shader.
         *  @return True if success, false otherwise.
         */
        bool Generate(void);

        /**
         *  @brief Set color for ground plane.
         *  @param [in] color Linear color value.
         *  @return Final color value that was set.
         */
        glm::vec3 SetColor(glm::vec3& color);

    private:
        GLint locationColor;
};

