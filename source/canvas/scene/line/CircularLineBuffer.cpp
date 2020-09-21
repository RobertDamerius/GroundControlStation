#include <CircularLineBuffer.hpp>


CircularLineBuffer::CircularLineBuffer(){
    this->width = 1.0f;
    this->vao = 0;
    this->vbo = 0;
    this->index = 0;
}

CircularLineBuffer::~CircularLineBuffer(){}

void CircularLineBuffer::Generate(uint32_t numLineSegments, glm::vec3 initialPosition){
    // Make sure that the circular line buffer is deleted
    Delete();

    // Create buffer
    CircularLineVertex initialVertex;
    initialVertex.positionPrev[0] = initialVertex.positionA[0] = initialVertex.positionB[0] = initialVertex.positionNext[0] = initialPosition.x;
    initialVertex.positionPrev[1] = initialVertex.positionA[1] = initialVertex.positionB[1] = initialVertex.positionNext[1] = initialPosition.y;
    initialVertex.positionPrev[2] = initialVertex.positionA[2] = initialVertex.positionB[2] = initialVertex.positionNext[2] = initialPosition.z;
    initialVertex.valueA = initialVertex.valueB = 0.0f;
    this->vertices.clear();
    this->vertices.shrink_to_fit();
    this->vertices.resize(numLineSegments ? numLineSegments : 1, initialVertex);

    // Generate buffer
    DEBUG_GLCHECK( glGenVertexArrays(1, &this->vao); );
    DEBUG_GLCHECK( glGenBuffers(1, &this->vbo); );
    DEBUG_GLCHECK( glBindVertexArray(this->vao); );
        DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, this->vbo); );
        DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(CircularLineVertex) * this->vertices.size(), &this->vertices[0], GL_DYNAMIC_DRAW); );
        DEBUG_GLCHECK( glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, valueA)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
        DEBUG_GLCHECK( glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, valueB)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(1); );
        DEBUG_GLCHECK( glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, positionPrev)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(2); );
        DEBUG_GLCHECK( glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, positionA)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(3); );
        DEBUG_GLCHECK( glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, positionB)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(4); );
        DEBUG_GLCHECK( glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, positionNext)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(5); );
    DEBUG_GLCHECK( glBindVertexArray(0); );
}

void CircularLineBuffer::Resize(uint32_t numLineSegments){
    // Nothing to do if size is equal
    uint32_t length = (uint32_t)this->vertices.size();
    if(!numLineSegments || !length || (numLineSegments == length)){
        return;
    }

    // Delete VBO/VAO
    if(this->vbo){
        DEBUG_GLCHECK( glDeleteBuffers(1, &this->vbo) );
        this->vbo = 0;
    }
    if(this->vao){
        DEBUG_GLCHECK( glDeleteVertexArrays(1, &this->vao) );
        this->vao = 0;
    }

    // Downsizing
    if(numLineSegments < length){
        std::vector<CircularLineVertex> newBuffer;
        uint32_t idx = this->index + 1 + length - numLineSegments;
        for(uint32_t n = 0; n < numLineSegments; n++, idx++){
            idx %= length;
            newBuffer.push_back(this->vertices[idx]);
        }
        this->vertices.swap(newBuffer);
        this->vertices.shrink_to_fit();
        this->index = numLineSegments - 1;
    }
    // Upsizing
    else{
        std::vector<CircularLineVertex> newBuffer;
        uint32_t idx = this->index + 1;
        for(uint32_t n = 0; n < length; n++){
            newBuffer.push_back(this->vertices[(idx + n) % length]);
        }
        newBuffer.resize(numLineSegments, newBuffer[0]);
        this->index = length - 1;
        this->vertices.swap(newBuffer);
        this->vertices.shrink_to_fit();
    }

    // Generate buffer
    DEBUG_GLCHECK( glGenVertexArrays(1, &this->vao); );
    DEBUG_GLCHECK( glGenBuffers(1, &this->vbo); );
    DEBUG_GLCHECK( glBindVertexArray(this->vao); );
        DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, this->vbo); );
        DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, sizeof(CircularLineVertex) * this->vertices.size(), &this->vertices[0], GL_DYNAMIC_DRAW); );
        DEBUG_GLCHECK( glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, valueA)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
        DEBUG_GLCHECK( glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, valueB)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(1); );
        DEBUG_GLCHECK( glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, positionPrev)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(2); );
        DEBUG_GLCHECK( glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, positionA)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(3); );
        DEBUG_GLCHECK( glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, positionB)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(4); );
        DEBUG_GLCHECK( glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(CircularLineVertex), (GLvoid*)offsetof(CircularLineVertex, positionNext)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(5); );
    DEBUG_GLCHECK( glBindVertexArray(0); );
}

void CircularLineBuffer::Delete(void){
    if(this->vbo){
        DEBUG_GLCHECK( glDeleteBuffers(1, &this->vbo) );
        this->vbo = 0;
    }
    if(this->vao){
        DEBUG_GLCHECK( glDeleteVertexArrays(1, &this->vao) );
        this->vao = 0;
    }
    this->index = 0;
}

void CircularLineBuffer::Add(glm::vec3 position, GLfloat value){
    // Update indices
    uint32_t length = (uint32_t)this->vertices.size();
    if(!length) return;
    uint32_t indexThis = (this->index % length);
    uint32_t indexNext = (this->index + 1) % length;
    this->index = indexNext;

    // Update next vertex for current line segment
    this->vertices[indexThis].positionNext[0] = position.x;
    this->vertices[indexThis].positionNext[1] = position.y;
    this->vertices[indexThis].positionNext[2] = position.z;

    // New line segment
    this->vertices[indexNext].valueA = this->vertices[indexThis].valueB;
    this->vertices[indexNext].valueB = value;
    this->vertices[indexNext].positionPrev[0] = this->vertices[indexThis].positionA[0];
    this->vertices[indexNext].positionPrev[1] = this->vertices[indexThis].positionA[1];
    this->vertices[indexNext].positionPrev[2] = this->vertices[indexThis].positionA[2];
    this->vertices[indexNext].positionA[0] = this->vertices[indexThis].positionB[0];
    this->vertices[indexNext].positionA[1] = this->vertices[indexThis].positionB[1];
    this->vertices[indexNext].positionA[2] = this->vertices[indexThis].positionB[2];
    this->vertices[indexNext].positionB[0] = position.x;
    this->vertices[indexNext].positionB[1] = position.y;
    this->vertices[indexNext].positionB[2] = position.z;
    this->vertices[indexNext].positionNext[0] = position.x + 0.0001 * (position.x - this->vertices[indexThis].positionB[0]);
    this->vertices[indexNext].positionNext[1] = position.y + 0.0001 * (position.y - this->vertices[indexThis].positionB[1]);
    this->vertices[indexNext].positionNext[2] = position.z + 0.0001 * (position.z - this->vertices[indexThis].positionB[2]);

    // Copy new data to VBO
    DEBUG_GLCHECK( glBindVertexArray(this->vao); );
    DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, this->vbo); );
    if(indexNext){
        DEBUG_GLCHECK( glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)(indexNext * sizeof(CircularLineVertex) - 3*sizeof(GLfloat)), 3*sizeof(GLfloat) + sizeof(CircularLineVertex), (const GLvoid*)(&this->vertices[indexThis].positionNext[0])); );
    }
    else{
        DEBUG_GLCHECK( glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)(length * sizeof(CircularLineVertex) - 3*sizeof(GLfloat)), 3*sizeof(GLfloat), (const GLvoid*)(&this->vertices[indexThis].positionNext[0])); );
        DEBUG_GLCHECK( glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)(0), sizeof(CircularLineVertex), (const GLvoid*)(&this->vertices[0].valueA)); );
    }
}

void CircularLineBuffer::Draw(void){
    DEBUG_GLCHECK( glBindVertexArray(this->vao); );
    DEBUG_GLCHECK( glDrawArrays(GL_POINTS, 0, this->vertices.size()); );
}

CircularLineBuffer& CircularLineBuffer::operator=(const CircularLineBuffer& rhs){
    this->width = rhs.width;
    this->vao = rhs.vao;
    this->vbo = rhs.vbo;
    this->vertices = rhs.vertices;
    this->index = rhs.index;
    return *this;
}

