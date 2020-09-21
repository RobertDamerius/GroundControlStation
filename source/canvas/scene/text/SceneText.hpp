#pragma once


#include <RD.hpp>
#include <FontSprite.hpp>


typedef enum {
    SCENE_TEXT_FONT_ROBOTO_BOLD_14 = 0,  ///< Font: Roboto Bold, Size: 14
    SCENE_TEXT_FONT_MAX                  ///< [!] ALWAYS USE THIS AS LAST ENUM [!]
} scene_text_font_t;


class SceneText {
    public:
        glm::vec4 color;
        glm::vec4 shadowColor;
        std::string textToBeGenerated;

        /**
         *  @brief Generate GL textures for the internal font sprites.
         *  @return True if success, false otherwise.
         */
        static bool GenerateGL(void);

        /**
         *  @brief Delete the GL textures for the internal font sprites.
         */
        static void DeleteGL(void);

        /**
         *  @brief Create a scene text.
         */
        SceneText();

        /**
         *  @brief Default copy constructor.
         */
        SceneText(const SceneText& text) = default;

        /**
         *  @brief Delete the scene text.
         */
        ~SceneText();

        /**
         *  @brief Set an available font for this scene text.
         *  @param [in] font The font to be set.
         */
        void SetFont(scene_text_font_t font);

        /**
         *  @brief Generate the text string. This will generate GL content (VAO, VBO).
         *  @param [in] usage Specifies the expected usage pattern of the data store (default is GL_STATIC_DRAW).
         *  @param [in] tabSize Number of SPACES that should be used to replace TAB (default is 4).
         *  @details This will generate @ref textToBeGenerated. Only characters in range 32...255 will be displayed. TAB will be replaced by SPACES.
         */
        void GenerateText(GLenum usage = GL_STATIC_DRAW, const uint8_t tabSize = 4);

        /**
         *  @brief Delete the text string. This will delete GL content.
         */
        void DeleteText(void);

        /**
         *  @brief Draw the text.
         */
        void Draw(void);

        /**
         *  @brief Get the text string.
         *  @return Text string.
         */
        std::string GetString(void);

        /* Operators */
        SceneText& operator=(const SceneText& rhs);

    private:
        static FontSprite fontSprites[SCENE_TEXT_FONT_MAX];

        scene_text_font_t font;
        std::vector<font_sprite_vertex_t> vertices;
        glm::vec2 dimension;
        std::string text;
        GLuint vbo;
        GLuint vao;
};

