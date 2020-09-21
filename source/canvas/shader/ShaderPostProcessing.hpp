#pragma once


#include <RD.hpp>


class ShaderPostProcessing: protected RD::Engine::Shader {
    public:
        using RD::Engine::Shader::Use;
        using RD::Engine::Shader::Delete;

        /**
         *  @brief Create post processing shader.
         */
        ShaderPostProcessing();

        /**
         *  @brief Delete post processing shader.
         */
        ~ShaderPostProcessing();

        /**
         *  @brief Generate the shader.
         *  @return True if success, false otherwise.
         */
        bool Generate(void);

        /**
         *  @brief Set gamma value.
         *  @param [in] gamma Gamma, will be clamped to range [GAMMA_MIN,...,GAMMA_MAX].
         *  @return The gamma that was set.
         */
        GLfloat SetGamma(GLfloat gamma);

    private:
        GLint locationInvGamma;
};

