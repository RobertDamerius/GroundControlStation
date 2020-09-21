#include <Polytope.hpp>


Polytope::Polytope(){
    primitives = GL_POINTS;
    vao = 0;
    vbo = 0;
    ebo = 0;
}

Polytope::~Polytope(){}

void Polytope::UpdateAABB(void){
    aabb.position = aabb.dimension = glm::dvec3(0.0);
    if(vertices.size()){
        glm::dvec3 posMin(vertices[0].position[0], vertices[0].position[1], vertices[0].position[2]);
        glm::dvec3 posMax = posMin;
        for(size_t n = 1; n < vertices.size(); n++){
            posMin.x = (vertices[n].position[0] < posMin.x) ? vertices[n].position[0] : posMin.x;
            posMin.y = (vertices[n].position[1] < posMin.y) ? vertices[n].position[1] : posMin.y;
            posMin.z = (vertices[n].position[2] < posMin.z) ? vertices[n].position[2] : posMin.z;
            posMax.x = (vertices[n].position[0] > posMax.x) ? vertices[n].position[0] : posMax.x;
            posMax.y = (vertices[n].position[1] > posMax.y) ? vertices[n].position[1] : posMax.y;
            posMax.z = (vertices[n].position[2] > posMax.z) ? vertices[n].position[2] : posMax.z;
        }
        aabb.position = posMin;
        aabb.dimension = posMax - posMin;
    }
}

void Polytope::GenerateGL(void){
    // Do nothing if already generated
    if(vao) return;

    // First make sure that GL parts are deleted
    DeleteGL();

    // Generate VAO, VBO, EBO
    DEBUG_GLCHECK( glGenVertexArrays(1, &vao); );
    DEBUG_GLCHECK( glGenBuffers(1, &vbo); );
    DEBUG_GLCHECK( glGenBuffers(1, &ebo); );
    DEBUG_GLCHECK( glBindVertexArray(vao); );
        DEBUG_GLCHECK( glBindBuffer(GL_ARRAY_BUFFER, vbo); );
        DEBUG_GLCHECK( glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PolytopeVertex), &(vertices[0]), GL_STATIC_DRAW); );
        DEBUG_GLCHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); );
        DEBUG_GLCHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &(indices[0]), GL_STATIC_DRAW); );
        // Position attribute
        DEBUG_GLCHECK( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PolytopeVertex), (GLvoid*)0); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(0); );
        // Normal attribute
        DEBUG_GLCHECK( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PolytopeVertex), (GLvoid*)offsetof(PolytopeVertex, normal)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(1); );
        // Color attribute
        DEBUG_GLCHECK( glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PolytopeVertex), (GLvoid*)offsetof(PolytopeVertex, color)); );
        DEBUG_GLCHECK( glEnableVertexAttribArray(2); );
    DEBUG_GLCHECK( glBindVertexArray(0); );
}

void Polytope::DeleteGL(void){
    if(vao) glDeleteVertexArrays(1, &(vao));
    if(vbo) glDeleteBuffers(1, &(vbo));
    if(ebo) glDeleteBuffers(1, &(ebo));
    vao = 0;
    vbo = 0;
    ebo = 0;
}

void Polytope::Draw(void){
    DEBUG_GLCHECK( glBindVertexArray(vao); );
    DEBUG_GLCHECK( glDrawElements(primitives, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0); );
}

Polytope& Polytope::operator=(const Polytope& rhs){
    primitives = rhs.primitives;
    vao = rhs.vao;
    vbo = rhs.vbo;
    ebo = rhs.ebo;
    indices = rhs.indices;
    vertices = rhs.vertices;
    aabb = rhs.aabb;
    return *this;
}

