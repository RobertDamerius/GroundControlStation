#include <LinePrimitive.hpp>
#include <RD.hpp>


LinePrimitive::LinePrimitive(LineType type){
    this->width = 1.0;
    switch(type){
        case LINE_PRIMITIVE_TYPE_LINES:
            this->mode = GL_LINES;
            break;
        case LINE_PRIMITIVE_TYPE_LINES_ADJACENCY:
            this->mode = GL_LINES_ADJACENCY;
            break;
        case LINE_PRIMITIVE_TYPE_LINE_LOOP:
            this->mode = GL_LINE_LOOP;
            break;
        case LINE_PRIMITIVE_TYPE_LINE_STRIP:
            this->mode = GL_LINE_STRIP;
            break;
        case LINE_PRIMITIVE_TYPE_LINE_STRIP_ADJACENCY:
            this->mode = GL_LINE_STRIP_ADJACENCY;
            break;
        default:
            this->mode = GL_LINES;
    }
    this->vao = 0;
    this->vbo = 0;
}

LinePrimitive::~LinePrimitive(){}

void LinePrimitive::Generate(GLenum usage){
    Delete();
    DEBUG_GLCHECK( glGenVertexArrays(1, &this->vao); );
    DEBUG_GLCHECK( glGenBuffers(1, &this->vbo); );
    DEBUG_GLCHECK( glBindVertexArray(this->vao); );
        DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, this->vbo); );
        DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * this->vertices.size(), &this->vertices[0], usage); );
        DEBUG_GLCHECK( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (GLvoid*)offsetof(LineVertex, position)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
        DEBUG_GLCHECK( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (GLvoid*)offsetof(LineVertex, color)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(1); );
    DEBUG_GLCHECK( glBindVertexArray(0); );
}

void LinePrimitive::Delete(void){
    if(this->vbo){
        DEBUG_GLCHECK( glDeleteBuffers(1, &this->vbo) );
        this->vbo = 0;
    }
    if(this->vao){
        DEBUG_GLCHECK( glDeleteVertexArrays(1, &this->vao) );
        this->vao = 0;
    }
}

void LinePrimitive::Draw(void){
    DEBUG_GLCHECK( glBindVertexArray(this->vao); );
    DEBUG_GLCHECK( glDrawArrays(this->mode, 0, this->vertices.size()); );
}

LinePrimitive& LinePrimitive::operator=(const LinePrimitive& rhs){
    this->width = rhs.width;
    this->vertices = rhs.vertices;
    this->mode = rhs.mode;
    this->vao = rhs.vao;
    this->vbo = rhs.vbo;
    return *this;
}

