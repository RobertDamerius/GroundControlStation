#include <PolygonObstacle.hpp>
#include <earcut.hpp>


#define POLYGON_OBSTACLE_RGB_BOTTOM   (0.5f)


PolygonObstacle::PolygonObstacle(): Polytope(){
    this->primitives = GL_TRIANGLES;
    this->generated = false;
    this->classification = 0x00;
}

PolygonObstacle::~PolygonObstacle(){}

void PolygonObstacle::CreatePolyhedron(std::vector<std::array<double, 2>>& vertices, GLfloat yMin, GLfloat yMax){
    // Do nothing if no input polygon (less than 3 vertices)
    this->vertices.clear();
    this->indices.clear();
    if(vertices.size() < 3){
        return;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Bottom of polyhedron
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Fill vertex data structure
    PolytopeVertex v;
    v.color[0] = v.color[1] = v.color[2] = POLYGON_OBSTACLE_RGB_BOTTOM;
    v.normal[0] = v.normal[2] = 0.0f;
    v.normal[1] = -1.0f;
    v.position[1] = yMin;
    for(size_t n = 0; n < vertices.size(); n++){
        v.position[0] = (GLfloat)vertices[n][0];
        v.position[2] = (GLfloat)vertices[n][1];
        this->vertices.push_back(v);
    }

    // Run tesselation
    std::vector<std::vector<std::array<double, 2>>> polygon;
    polygon.push_back(vertices);
    this->indices = mapbox::earcut<GLuint>(polygon);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Top of polyhedron
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    GLuint offsetIndices = (GLuint)vertices.size();
    v.color[0] = v.color[1] = v.color[2] = 1.0f;
    v.normal[0] = v.normal[2] = 0.0f;
    v.normal[1] = 1.0f;
    v.position[1] = yMax;
    for(size_t n = 0; n < vertices.size(); n++){
        v.position[0] = (GLfloat)vertices[n][0];
        v.position[2] = (GLfloat)vertices[n][1];
        this->vertices.push_back(v);
    }
    if(this->indices.size() % 3){
        LogError("Tesselation error: Number of indices should be a multiple of 3!\n");
        this->vertices.clear();
        this->indices.clear();
        return;
    }
    int32_t numIndexGroups = (int32_t)(this->indices.size() / 3);
    for(int32_t n = 0, idx = 0; n < numIndexGroups; n++, idx += 3){
        this->indices.push_back(offsetIndices + this->indices[idx]);
        this->indices.push_back(offsetIndices + this->indices[idx+2]);
        this->indices.push_back(offsetIndices + this->indices[idx+1]);
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Side of polyhedron
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    offsetIndices <<= 1;
    for(size_t idx = 0; idx < vertices.size(); idx++, offsetIndices += 4){
        size_t idxNext = (idx + 1) % vertices.size();
        glm::dvec3 p1((GLfloat)vertices[idx][0], yMin, (GLfloat)vertices[idx][1]);
        glm::dvec3 p2((GLfloat)vertices[idx][0], yMax, (GLfloat)vertices[idx][1]);
        glm::dvec3 p3((GLfloat)vertices[idxNext][0], yMin, (GLfloat)vertices[idxNext][1]);
        glm::dvec3 p4((GLfloat)vertices[idxNext][0], yMax, (GLfloat)vertices[idxNext][1]);
        glm::dvec3 n(p3.z-p1.z, 0.0, p1.x-p3.x);
        double dlen = sqrt(n.x*n.x + n.z*n.z);
        if(dlen < 1e-9){
            n = glm::dvec3(0.0, 1.0, 0.0);
        }
        else{
            dlen = 1.0 / dlen;
            n.x *= dlen;
            n.z *= dlen;
        }
        v.normal[0] = n.x; v.normal[1] = n.y; v.normal[2] = n.z;
        v.position[0] = p1.x; v.position[1] = p1.y; v.position[2] = p1.z; v.color[0] = v.color[1] = v.color[2] = POLYGON_OBSTACLE_RGB_BOTTOM; this->vertices.push_back(v);
        v.position[0] = p2.x; v.position[1] = p2.y; v.position[2] = p2.z; v.color[0] = v.color[1] = v.color[2] = 1.0f; this->vertices.push_back(v);
        v.position[0] = p3.x; v.position[1] = p3.y; v.position[2] = p3.z; v.color[0] = v.color[1] = v.color[2] = POLYGON_OBSTACLE_RGB_BOTTOM; this->vertices.push_back(v);
        v.position[0] = p4.x; v.position[1] = p4.y; v.position[2] = p4.z; v.color[0] = v.color[1] = v.color[2] = 1.0f; this->vertices.push_back(v);
        this->indices.push_back(offsetIndices);
        this->indices.push_back(offsetIndices + 1);
        this->indices.push_back(offsetIndices + 2);
        this->indices.push_back(offsetIndices + 3);
        this->indices.push_back(offsetIndices + 2);
        this->indices.push_back(offsetIndices + 1);
    }
}

void PolygonObstacle::GenerateGL(void){
    Polytope::GenerateGL();
    generated = true;
}

void PolygonObstacle::DeleteGL(void){
    Polytope::DeleteGL();
    generated = false;
}

PolygonObstacle& PolygonObstacle::operator=(const PolygonObstacle& rhs){
    Polytope::operator=(rhs);
    this->navigation = rhs.navigation;
    this->generated = rhs.generated;
    this->classification = rhs.classification;
    return *this;
}

