#pragma once


#include <RD.hpp>


// Font sprite vertex type
#pragma pack(push, 1)
typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
} font_sprite_vertex_t;
#pragma pack(pop)


class FontSprite {
    public:
        /**
         *  @brief Create a font sprite.
         */
        FontSprite();

        /**
         *  @brief Delete the font sprite.
         *  @note This will NOT call the @ref Delete member function!
         */
        ~FontSprite();

        /**
         *  @brief Generate a sprite texture that will contain all extended ascii symbols.
         *  @param [in] filename Filename of the font file.
         *  @param [in] fontsize Size of the font that should be generated.
         *  @param [in] textureUnit The texture unit that should be used to generate the GL texture, defaults to GL_TEXTURE0.
         *  @param [in] textureFiler The texture filter enum that should be used as texture min and mag filter, defaults to GL_NEAREST.
         *  @param [in] spritePadding Padding between sprite symbols in pixels, defaults to 2.
         *  @return True if success, false otherwise.
         *  @details The generated texture will have a dimension of power two. Internally, the @ref Delete member function will be called first.
         *  @note The thread calling this function must use the GL context.
         */
        bool Generate(std::string filename, uint32_t fontsize, GLenum textureUnit = GL_TEXTURE0, const GLenum textureFilter = GL_NEAREST, const uint32_t spritePadding = 2);

        /**
         *  @brief Generate a sprite texture that will contain all extended ascii symbols.
         *  @param [in] bytes Array of bytes containing the font data.
         *  @param [in] length Number of bytes of the actual font data.
         *  @param [in] fontsize Size of the font that should be generated.
         *  @param [in] textureUnit The texture unit that should be used to generate the GL texture, defaults to GL_TEXTURE0.
         *  @param [in] textureFiler The texture filter enum that should be used as texture min and mag filter, defaults to GL_NEAREST.
         *  @param [in] spritePadding Padding between sprite symbols in pixels, defaults to 2.
         *  @return True if success, false otherwise.
         *  @details The generated texture will have a dimension of power two. Internally, the @ref Delete member function will be called first.
         *  @note The thread calling this function must use the GL context.
         */
        bool Generate(const uint8_t* bytes, const uint32_t length, uint32_t fontsize, GLenum textureUnit = GL_TEXTURE0, const GLenum textureFilter = GL_NEAREST, const uint32_t spritePadding = 2);

        /**
         *  @brief Delete the font sprite that has been generated using one of the @ref Generate member functions.
         */
        void Delete(void);

        /**
         *  @brief Get the generated OpenGL texture.
         *  @return The OpenGL texture or zero if no texture was generated.
         */
        GLuint GetTexture(void);

        /**
         *  @brief Get the width of the generated sprite font texture.
         *  @return Texture width in pixels or zero if no texture was generated.
         */
        uint32_t GetTextureWidth(void);

        /**
         *  @brief Get the height of the generated sprite font texture.
         *  @return Texture height in pixels or zero if no texture was generated.
         */
        uint32_t GetTextureHeight(void);

        /**
         *  @brief Get the line height.
         *  @return Line height in pixels.
         */
        double GetLineHeight(void);

        /**
         *  @brief Get the baseline offset.
         *  @return Offset in pixels from bottom of a symbol that marks the baseline.
         */
        double GetBaseline(void);

        /**
         *  @brief Convert a text (extended-ascii) to vertices. Note that the sprite image must be generated using the @ref Generate member function!
         *  @param [out] outVertices Output vertices.
         *  @param [out] outDimension Output dimension of the created text in pixels. This is the minimum rect size that contains the created text vertices.
         *  @param [in] inText Input text. Only characters in range 32...255 will be displayed. TAB will be replaced by SPACE.
         *  @param [in] tabSize Number of SPACES that should be used to replace TAB (default is 4).
         *  @details Texture coordinate range is [0, 1]. The unit of a vertex position is pixels.
         */
        void TextToVertices(std::vector<font_sprite_vertex_t>& outVertices, glm::vec2& outDimension, const std::string& inText, const uint8_t tabSize = 4);

    private:
        GLuint texture;                        ///< OpenGL generated texture.
        int32_t fontHeight;                    ///< Number of pixel rows.
        int32_t baseline;                      ///< Index of pixel row (from bottom) that marks the baseline.
        uint32_t textureWidth;                 ///< Width of generated sprite texture.
        uint32_t textureHeight;                ///< Height of generated sprite texture.
        glm::vec4 textureCoordinateTable[256]; ///< Table of texture coordinates. Order is s1, t1, s2, t2.
        int32_t spriteWidthTable[256];         ///< Table of sprite widths.
};

