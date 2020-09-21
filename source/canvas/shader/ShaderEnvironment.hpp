#pragma once


#include <RD.hpp>


class ShaderEnvironment: protected RD::Engine::Shader {
    public:
        using RD::Engine::Shader::Use;
        using RD::Engine::Shader::Delete;

        /**
         *  @brief Create environment shader.
         */
        ShaderEnvironment();

        /**
         *  @brief Delete environment shader.
         */
        ~ShaderEnvironment();

        /**
         *  @brief Generate the shader.
         *  @return True if success, false otherwise.
         */
        bool Generate(void);

        /**
         *  @brief Set the size of the framebuffer.
         *  @param [in] size Size in pixels.
         */
        void SetResolution(glm::ivec2 size);

        /**
         *  @brief Set ambient strength.
         *  @param [in] ambient Ambient strength greater than zero.
         */
        void SetAmbient(float ambient);

        /**
         *  @brief Set shininess for specular light.
         *  @param [in] shininess Shininess value greater than zero.
         */
        void SetShininess(float shininess);

    private:
        GLint locationInvResolution;
        GLint locationAmbient;
        GLint locationShininess;
};

