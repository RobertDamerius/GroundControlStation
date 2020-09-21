#pragma once


#include <RD.hpp>


class Skybox {
    public:
        /**
         *  @brief Create a skybox.
         */
        Skybox();

        /**
         *  @brief Delete the skybox.
         */
        ~Skybox();

        /**
         *  @brief Read texture arrays.
         *  @return True if success, false otherwise.
         *  @details The read data is stored in the private attribute @ref faces. If you want to generate
         *  the GL cubemap texture call the @ref GenerateGL member function.
         */
        bool ReadTextures(void);

        /**
         *  @brief Generate the cubemap texture for the skybox.
         *  @param [in] clearData Specifies whether the @ref faces attribute should be cleared or not.
         *  The face information will not be cleared. Only the data container.
         *  @return True if success, false otherwise.
         *  @note The data of the cubemap must be loaded using the @ref ReadTextureArrays member function.
         */
        bool GenerateGL(bool clearData = true);

        /**
         *  @brief Delete the cubemap that was generated using the @ref GenerateGL member function.
         *  @note This will not affect the @ref faces attribue.
         */
        void DeleteGL(void);

        /**
         *  @brief Bind texture, VAO and draw triangles.
         */
        void DrawCube(void);

    private:
        RD::Image::PNG faces[6];      ///< Raw image data for all faces
        bool dataAvailable;           ///< Specifies whether the image data is available or not.
        GLuint texture;               ///< The OpenGL cubemap texture.
        GLuint vao;                   ///< The cube VAO.
        GLuint vbo;                   ///< The cube VBO.
};


