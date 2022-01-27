#include <SceneText.hpp>
#include <RD.hpp>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Resources
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RESOURCE_EXTLD(source_gui_resource_roboto_bold_bin)


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Static attributes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FontSprite SceneText::fontSprites[SCENE_TEXT_FONT_MAX];


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Static member functions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool SceneText::GenerateGL(void){
    DeleteGL();
    if(!fontSprites[SCENE_TEXT_FONT_ROBOTO_BOLD_14].Generate((const uint8_t*)RESOURCE_LDVAR(source_gui_resource_roboto_bold_bin), (uint32_t)RESOURCE_LDLEN(source_gui_resource_roboto_bold_bin), 14, GL_TEXTURE0, GL_NEAREST, 2)) goto error;

    // Success
    return true;

    // Error
    error:
    DeleteGL();
    return false;
}

void SceneText::DeleteGL(void){
    for(uint32_t k = 0; k < (uint32_t)SCENE_TEXT_FONT_MAX; k++){
        fontSprites[k].Delete();
    }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Member functions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SceneText::SceneText(){
    color = glm::vec4(1.0f);
    shadowColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    textToBeGenerated = std::string("");
    font = (scene_text_font_t)0;
    vbo = 0;
    vao = 0;
    dimension.x = 0.0;
    dimension.y = 0.0;
    text = std::string("");
}

SceneText::~SceneText(){}

void SceneText::SetFont(scene_text_font_t font){
    uint32_t f = (uint32_t)font;
    this->font = (scene_text_font_t)((f < (uint32_t)SCENE_TEXT_FONT_MAX) ? f : (uint32_t)SCENE_TEXT_FONT_MAX);
}

void SceneText::GenerateText(GLenum usage, const uint8_t tabSize){
    // Generate vertices from text string
    this->text = this->textToBeGenerated;
    fontSprites[this->font].TextToVertices(this->vertices, this->dimension, this->text, tabSize);

    // Generate VAO/VBO
    DEBUG_GLCHECK( glGenVertexArrays(1, &this->vao); );
    DEBUG_GLCHECK( glGenBuffers(1, &this->vbo); );
    DEBUG_GLCHECK( glBindVertexArray(this->vao); );
        DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, this->vbo); );
        DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(font_sprite_vertex_t) * this->vertices.size(), &this->vertices[0], usage); );
        DEBUG_GLCHECK( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
        DEBUG_GLCHECK( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(1); );
    DEBUG_GLCHECK( glBindVertexArray(0); );
}

void SceneText::DeleteText(void){
    DEBUG_GLCHECK( glDeleteBuffers(1, &this->vbo) );
    DEBUG_GLCHECK( glDeleteVertexArrays(1, &this->vao) );
}

void SceneText::Draw(void){
    DEBUG_GLCHECK( glBindTexture(GL_TEXTURE_2D, SceneText::fontSprites[this->font].GetTexture()); );
    DEBUG_GLCHECK( glBindVertexArray(this->vao); );
    DEBUG_GLCHECK( glDrawArrays(GL_TRIANGLES, 0, this->vertices.size()); );
}

SceneText& SceneText::operator=(const SceneText& rhs){
    if(this != &rhs){
        this->color = rhs.color;
        this->shadowColor = rhs.shadowColor;
        this->textToBeGenerated = rhs.textToBeGenerated;
        this->font = rhs.font;
        this->vertices = rhs.vertices;
        this->dimension = rhs.dimension;
        this->text = rhs.text;
        this->vbo = rhs.vbo;
        this->vao = rhs.vao;
    }
    return *this;
}

std::string SceneText::GetString(void){
    return this->text;
}

// Only update current index buffer data
    // DEBUG_GLCHECK( glBindVertexArray(this->vao); );
    // DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vbo); );
    // DEBUG_GLCHECK( glBufferSubData(GL_ARRAY_BUFFER, 0, this->numVertices * sizeof(font_sprite_vertex_t), (const GLvoid*)&this->vertices[0]); );
    // DEBUG_GLCHECK( glBindVertexArray(0); );

